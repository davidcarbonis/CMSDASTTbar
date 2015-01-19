#pragma once

#include <PhysicsObjects.hpp>
#include <Systematics.hpp>

#include <TH1D.h>

#include <memory>
#include <map>


/**
 * \class CSVReweighter
 * \brief Calculates per-jet CSV weight
 * 
 * The performs CSV reweighting following the recipe in [1]. It calculates only per-jet weights, and
 * the user should use them to calculate the per-event weight. A number of sources of systematics
 * are supported. In addition to sources that are related to b-tagging only (their names in the
 * SystType enumeration start with BTag prefix), the CSV weights are also affected by the JEC
 * systematics. More details about the reweighting method and its systematics can be found in the
 * supporting AN [2].
 * [1] https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagShapeCalibration
 * [2] http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2013/130
 */
class CSVReweighter
{
private:
    /**
     * \brief A compact representation of a systematical variation
     * 
     * It combines the type and the direction. The representation is used in the class internally.
     */
    typedef unsigned SystCode;
    
public:
    /**
     * \brief Constructor
     * 
     * Reads histograms with the weights from data files. Throws exceptions if the files are not
     * found or do not contain required histograms.
     */
    CSVReweighter();
    
public:
    /**
     * \brief Calculates per-jet CSV weight
     * 
     * A systematical variation is applied if requested. It is described by the type of systematics
     * and the direction of the variation. If a mismatched variation is requested (e.g. a variation
     * that makes sense for b-quark jets only while calculating the weight for a light-flavour jet)
     * or a variation that is not relevant for b-tagging, the nominal weight is calculated. If the
     * type of systematics is Nominal, the direction is ignored.
     */
    double CalculateJetWeight(Jet const &jet, SystType systType, SystDirection systDirection) const;
    
    /// A short-cut to calculate nominal per-jet CSV weight
    double CalculateJetWeight(Jet const &jet) const;
    
private:
    /// Combines type of systematics and direction of the variation into a single code
    static SystCode EncodeSyst(SystType systType, SystDirection systDirection);
    
private:
    /// Number of bins in pt in histograms for heavy-flavour jets
    static unsigned const nPtBinsHF = 6;
    
    /// Number of bins in pt in histograms for light-flavour jets
    static unsigned const nPtBinsLF = 4;
    
    /// Number of bins in absolute pseudorapidity in histograms for light-flavour jets
    static unsigned const nEtaBinsLF = 3;
    
    /// Histograms with weights for b-quark jets
    mutable std::map<SystCode, std::unique_ptr<TH1D>[nPtBinsHF]> weightsBottom;
    
    /// Histograms with weights for c-quark jets
    mutable std::map<SystCode, std::unique_ptr<TH1D>[nPtBinsHF]> weightsCharm;
    
    /// Histograms with weights for light-flavour jets
    mutable std::map<SystCode, std::unique_ptr<TH1D>[nPtBinsLF][nEtaBinsLF]> weightsLight;
};
