#pragma once

#include <PhysicsObjects.hpp>

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>
#include <list>
#include <initializer_list>
#include <memory>


/**
 * \class Reader
 * \brief Reads the requested tree from the source file
 * 
 * Read all events in the tree and provides an access to basic properties of each event with the
 * help of dedicated getters.
 */
class Reader
{
public:
    /**
     * \brief Constructor from a source file and names of trees to be read from it
     * 
     * The trees will be read one by one, in the specified order. An exception is thrown if the
     * source file does not exist or is corrupted.
     */
    Reader(std::shared_ptr<TFile> &srcFile, std::initializer_list<std::string> const &treeNames);
    
    /**
     * \brief Constructor from a source file and name of a single tree
     * 
     * Internally calls the first constructor.
     */
    Reader(std::shared_ptr<TFile> &srcFile, std::string const &treeName);
    
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
     * \brief Returns the collection of leptons in the current event
     * 
     * The collection is ordered in pt, in the decreasing order.
     */
    std::vector<Lepton> const &GetLeptons() const noexcept;
    
    /**
     * \brief Returns the collection of jets in the current event
     * 
     * The collection is ordered in pt, in the decreasing order.
     */
    std::vector<Jet> const &GetJets() const noexcept;
    
    /// Returns MET of the current event
    MET const &GetMET() const noexcept;
    
    /**
     * \brief Returns weight of the current event
     * 
     * Needed for simulation only. Always 1. for data.
     */
    double GetWeight() const noexcept;
    
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
    
    /// Iterator that points to the name of the current tree
    decltype(treeNames)::iterator curTreeNameIt;
    
    /// Pointer to the current tree
    std::unique_ptr<TTree> curTree;
    
    /// Number of events in the current tree
    unsigned long nEntries;
    
    /// Index of the current event in the current tree
    unsigned long curEntry;
    
    /// Size of buffers to read the source tree
    static unsigned const maxSize = 64;
    
    /// Leptons in the current events
    std::vector<Lepton> leptons;
    
    /// Jets in the current event
    std::vector<Jet> jets;
    
    /// MET in the current event
    MET met;
    
    /// Total weight of the event
    double weight;
    
    
    // Buffers to read the trees
    Int_t lepSize;
    Float_t lepPt[maxSize], lepEta[maxSize], lepPhi[maxSize], lepIso[maxSize];
    Int_t lepFlavour[maxSize];
    
    Int_t jetSize;
    Float_t jetPt[maxSize], jetEta[maxSize], jetPhi[maxSize], jetBTag[maxSize];
    Int_t jetFlavour[maxSize];
    
    Float_t metPt, metPhi;
    
    Float_t rawWeight;
};
