#pragma once

#include <PhysicsObjects.hpp>

#include <TH1D.h>

#include <memory>
#include <map>


/**
 * \class CSVReweighter
 */
class CSVReweighter
{
public:
    /// Supported systematical variations
    enum class Systematics
    {
        Nominal,
        JECUp,
        JECDown,
        PurityUp,
        PurityDown,
        StatUp,
        StatDown
    };
    
public:
    /**
     */
    CSVReweighter();
    
public:
    /**
     * \brief Calculates per-jet CSV weight
     * 
     * A systematical variation is applied if requested.
     */
    double GetJetWeight(Jet const &jet, Systematics syst = Systematics::Nominal) const;
    
private:
    static unsigned const nPtBinsHF = 6;
    static unsigned const nPtBinsLF = 4;
    static unsigned const nEtaBinsLF = 3;
    
    mutable std::map<Systematics, std::unique_ptr<TH1D>[nPtBinsHF]> weightsBottom;
    mutable std::map<Systematics, std::unique_ptr<TH1D>[nPtBinsHF]> weightsCharm;
    mutable std::map<Systematics, std::unique_ptr<TH1D>[nPtBinsLF][nEtaBinsLF]> weightsLight;
};
