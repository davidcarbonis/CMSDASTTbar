#include <Reader.hpp>

#include <stdexcept>
#include <sstream>
#include <algorithm>


using namespace std;


// A static data member
unsigned const Reader::maxSize;


Reader::Reader(shared_ptr<TFile> &srcFile_, list<string> const &treeNames_):
    srcFile(srcFile_), treeNames(treeNames_), curTreeNameIt(treeNames.begin())
{
    // Make sure the source file is a valid one
    if (not srcFile or srcFile->IsZombie())
        throw runtime_error("The source file does not exist or is corrupted.");
    
    
    // Get the first tree
    GetTree(*curTreeNameIt);
}


Reader::Reader(shared_ptr<TFile> &srcFile_, string const &treeName):
    Reader(srcFile_, list<string>{treeName})
{}


bool Reader::ReadNextEvent()
{
    // Check if there are events left in the current source tree
    if (curEntry == nEntries)  // no more events in the current tree
    {
        ++curTreeNameIt;
        
        if (curTreeNameIt == treeNames.end())  // no more source trees
            return false;
        
        GetTree(*curTreeNameIt);
    }
    
    
    // Either there were events in the current source file or a new file has been opened
    curTree->GetEntry(curEntry);
    ++curEntry;
    
    
    // Copy properies of objects in the event from read buffers
    leptons.clear();
    
    for (int i = 0; i < lepSize; ++i)
        leptons.emplace_back(lepFlavour[i], lepPt[i], lepEta[i], lepPhi[i], lepIso[i]);
    
    jets.clear();
    
    for (int i = 0; i < jetSize; ++i)
        jets.emplace_back(jetPt[i], jetEta[i], jetPhi[i], jetBTag[i], jetFlavour[i]);
    
    met.Set(metPt, metPhi);
    weight = rawWeight;
    
    
    // Make sure vector of leptons and jets are ordered in pt
    sort(leptons.rbegin(), leptons.rend());
    sort(jets.rbegin(), jets.rend());
    
    
    return true;
}


void Reader::Rewind() noexcept
{
    curTreeNameIt = treeNames.begin();
    GetTree(*curTreeNameIt);
}


vector<Lepton> const &Reader::GetLeptons() const noexcept
{
    return leptons;
}


vector<Jet> const &Reader::GetJets() const noexcept
{
    return jets;
}


MET const &Reader::GetMET() const noexcept
{
    return met;
}


double Reader::GetWeight() const noexcept
{
    return weight;
}


void Reader::GetTree(string const &name)
{
    // Get the tree from the source file
    //curTree.reset(dynamic_cast<TTree *>(srcFile->Get(name.c_str())));
    curTree.reset(static_cast<TTree *>(srcFile->Get(name.c_str())));
    //^ Apparently, my copy of ROOT is compiled w/o RTTI support, so dynamic_cast fails. Opt for the
    //unsafe cast for the time being
    
    
    // Make sure the tree exists
    if (not curTree)
    {
        ostringstream ost;
        ost << "Cannot find tree \"" << name << "\" in file \"" << srcFile->GetTitle() << "\".";
        throw runtime_error(ost.str());
    }
    
    
    // Set event counters
    nEntries = curTree->GetEntries();
    curEntry = 0;
    
    
    // Set buffers to read the tree
    curTree->SetBranchAddress("smalltree_nlepton", &lepSize);
    curTree->SetBranchAddress("smalltree_lept_pt", lepPt);
    curTree->SetBranchAddress("smalltree_lept_eta", lepEta);
    curTree->SetBranchAddress("smalltree_lept_phi", lepPhi);
    curTree->SetBranchAddress("smalltree_lept_iso", lepIso);
    curTree->SetBranchAddress("smalltree_lept_flav", lepFlavour);
    
    curTree->SetBranchAddress("smalltree_njets", &jetSize);
    curTree->SetBranchAddress("smalltree_jet_pt", jetPt);
    curTree->SetBranchAddress("smalltree_jet_eta", jetEta);
    curTree->SetBranchAddress("smalltree_jet_phi", jetPhi);
    curTree->SetBranchAddress("smalltree_jet_btagdiscri", jetBTag);
    
    curTree->SetBranchAddress("smalltree_met_pt", &metPt);
    curTree->SetBranchAddress("smalltree_met_phi", &metPhi);
    
    curTree->SetBranchAddress("smalltree_evtweight", &rawWeight);
}
