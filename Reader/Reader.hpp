#pragma once

#include <PhysicsObjects.hpp>
#include <Systematics.hpp>
#include <CSVReweighter.hpp>

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>
#include <list>
#include <memory>


/**
 * \class Reader
 * \brief Reads the requested tree(s) from the source file
 * 
 * Reads all events in the tree(s) and provides an access to basic properties of each event with the
 * help of dedicated getters. Allows to perform systematical variations, which can be requested via
 * the SetSystematics method. When the requested systematical variation is changed, it affects
 * results of all relevant getters.
 */
class Reader
{
public:
    /**
     * \brief Constructor from a source file and names of trees to be read from it
     * 
     * The trees will be read one by one, in the specified order. An exception is thrown if the
     * source file does not exist or is corrupted. The flag isMC indicates if the sampe is a
     * simulation.
     */
    Reader(std::shared_ptr<TFile> &srcFile, std::list<std::string> const &treeNames,
     bool isMC = true);
    
    /**
     * \brief Constructor from a source file and name of a single tree
     * 
     * Internally calls the first constructor.
     */
    Reader(std::shared_ptr<TFile> &srcFile, std::string const &treeName, bool isMC = true);
    
    /// There is no construction without parameters
    Reader() = delete;
    
    /// Copy constructor is disabled
    Reader(Reader const &) = delete;
    
    /// Assignment operator is disabled
    Reader &operator=(Reader const &) = delete;
    
public:
    /**
     * \brief Reads next event from the source trees
     * 
     * Reads the next event from the source trees. Returns true in case of success and false if
     * there are no more events to be read.
     */
    bool ReadNextEvent();
    
    /// Rewinds the reader to the first event in the first tree
    void Rewind() noexcept;
    
    /**
     * \brief Sets desired systematical variation
     * 
     * Depending on the type, the requested variation can affect results of many getters. The method
     * can be called before as well as after an event has been read. It affects all subsequent calls
     * to getters, in the current and following events.
     */
    void SetSystematics(SystType systType, SystDirection systDirection);
    
    /**
     * \brief Returns the collection of leptons in the current event
     * 
     * The collection is ordered in pt, in the decreasing order.
     */
    std::vector<Lepton> const &GetLeptons() const noexcept;
    
    /**
     * \brief Returns the collection of jets in the current event
     * 
     * The collection is ordered in pt, in the decreasing order. If the JEC systematical variations
     * have been requested, the appropriate jet collection is returned insted of the nominal one.
     */
    std::vector<Jet> const &GetJets() const noexcept;
    
    /**
     * \brief Returns MET of the current event
     * 
     * If the JEC systematical variations have been requested, the MET is altered accordingly.
     */
    MET const &GetMET() const noexcept;
    
    /**
     * \brief Returns weight of the current event
     * 
     * Needed for simulation only, while it always equals 1. in case of data. Incorporates
     * reweighting for cross section and target integrated luminosity, lepton ID scale factors,
     * pile-up, and b-tagging. The weight can be affected by systematics.
     * 
     * When the method is called for the first time for an event, the weight is calculated and
     * cached. Thus, the weight is not recalculated in subsequent calls for the same event, unless
     * the user calls the SetSystematics method, which destroys the cache.
     */
    double GetWeight() noexcept;
    
    /// Returns the number of reconstructed primary vertices in the current event
    unsigned GetNumPV() const noexcept;
    
    /**
     * \brief Switches reweighting for b-tagging on or off
     * 
     * If the reweighting is switched off, the b-tagging weights are not accounted in the result of
     * GetWeight. The method is expected to be used for debugging only. By default the b-tagging
     * reweighting is enabled.
     */
    void SwitchBTagReweighting(bool on = true);
    
private:
    /**
     * \brief Gets a new tree from the source file and sets up buffers to read it
     * 
     * The method is called before the class starts reading each new tree. Throws an exception if
     * the tree does not exist.
     */
    void GetTree(std::string const &name);
    
private:
    /// Pointer to the source file
    std::shared_ptr<TFile> srcFile;
    
    /// Names of trees to be read from the source file
    std::list<std::string> treeNames;
    
    /// An object to perform CSV reweighting
    CSVReweighter csvReweighter;
    
    /// Iterator that points to the name of the current tree
    decltype(treeNames)::iterator curTreeNameIt;
    
    /// Pointer to the current tree
    std::unique_ptr<TTree> curTree;
    
    /// Number of events in the current tree
    unsigned long nEntries;
    
    /// Index of the current event in the current tree
    unsigned long curEntry;
    
    /// Flag that indicates if the current sample is simulation
    bool isMC;
    
    /**
     * \brief Type of systematical variation that is in effect currently
     * 
     * The data member is used for simulation only.
     */
    SystType curSystType;
    
    /**
     * \brief Direction of systematical variation that is in effect currently
     * 
     * The data member is used for simulation only.
     */
    SystDirection curSystDirection;
    
    /// Size of buffers to read the source tree
    static unsigned const maxSize = 64;
    
    /// Leptons in the current events
    std::vector<Lepton> leptons;
    
    /**
     * \brief Jets in the current event
     * 
     * Nominal collection and variations due to JEC uncertainty.
     */
    std::vector<Jet> jets, jetsJECUp, jetsJECDown;
    
    /**
     * \brief METs in the current event
     * 
     * Nominal value and systematical variations due to JEC uncertainty.
     */
    MET met, metJECUp, metJECDown;
    
    /// Total weight of the event
    double weight;
    
    /// Indicates if the weight is up-to-date and should not be recalculated
    bool weightCached;
    
    /**
     * \brief Flag showing if the reweighting for b-tagging should be applied
     * 
     * Needed for debug purposes only.
     */
    bool applyBTagReweighting;
    
    
    // Buffers to read the trees
    Int_t lepSize;
    Float_t lepPt[maxSize], lepEta[maxSize], lepPhi[maxSize], lepIso[maxSize];
    Int_t lepFlavour[maxSize];
    
    Int_t jetSize;
    Float_t jetPt[maxSize], jetEta[maxSize], jetPhi[maxSize], jetBTag[maxSize];
    Int_t jetFlavour[maxSize];
    
    Int_t jetJECUpSize;
    Float_t jetJECUpPt[maxSize], jetJECUpEta[maxSize], jetJECUpPhi[maxSize], jetJECUpBTag[maxSize];
    Int_t jetJECUpFlavour[maxSize];
    
    Int_t jetJECDownSize;
    Float_t jetJECDownPt[maxSize], jetJECDownEta[maxSize], jetJECDownPhi[maxSize],
     jetJECDownBTag[maxSize];
    Int_t jetJECDownFlavour[maxSize];
    
    Float_t metPt, metPhi;
    Float_t metJECUpPt, metJECUpPhi;
    Float_t metJECDownPt, metJECDownPhi;
    
    Int_t nPV;
    Float_t rawWeight;
};
