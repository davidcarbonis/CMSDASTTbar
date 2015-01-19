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
    
    if (isMC)
    {
        jetsJECUp.clear();
        jetsJECDown.clear();
        
        for (int i = 0; i < jetJECUpSize; ++i)
            jetsJECUp.emplace_back(jetJECUpPt[i], jetJECUpEta[i], jetJECUpPhi[i],
             jetJECUpBTag[i], jetJECUpFlavour[i]);
        
        for (int i = 0; i < jetJECDownSize; ++i)
            jetsJECDown.emplace_back(jetJECDownPt[i], jetJECDownEta[i], jetJECDownPhi[i],
             jetJECDownBTag[i], jetJECDownFlavour[i]);
        
        metJECUp.Set(metJECUpPt, metJECUpPhi);
        metJECDown.Set(metJECDownPt, metJECDownPhi);
    }
    
    
    // Make sure vector of leptons and jets are ordered in pt
    sort(leptons.rbegin(), leptons.rend());
    sort(jets.rbegin(), jets.rend());
    
    if (isMC)
    {
        sort(jetsJECUp.rbegin(), jetsJECUp.rend());
        sort(jetsJECDown.rbegin(), jetsJECDown.rend());
    }
    
    
    // Calculate event weight
    weight = rawWeight;
    
    if (isMC)
        for (auto const &j: jets)
        {
            double const perJetBTagWeight = csvReweighter.CalculateJetWeight(j);
            
            if (perJetBTagWeight != 0.)
                weight *= perJetBTagWeight;
        }
    
    
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
    
    
    // FIXME: For the time being treat all samples as simulation. With the new file it will be
    //possible to guess from the tree structure
    isMC = true;
    
    
    // Set event counters
    nEntries = curTree->GetEntries();
    curEntry = 0;
    
    
    // Set buffers to read the tree
    curTree->SetBranchAddress("nlepton", &lepSize);
    curTree->SetBranchAddress("lept_pt", lepPt);
    curTree->SetBranchAddress("lept_eta", lepEta);
    curTree->SetBranchAddress("lept_phi", lepPhi);
    curTree->SetBranchAddress("lept_iso", lepIso);
    curTree->SetBranchAddress("lept_flav", lepFlavour);
    
    curTree->SetBranchAddress("njets", &jetSize);
    curTree->SetBranchAddress("jet_pt", jetPt);
    curTree->SetBranchAddress("jet_eta", jetEta);
    curTree->SetBranchAddress("jet_phi", jetPhi);
    curTree->SetBranchAddress("jet_btagdiscri", jetBTag);
    curTree->SetBranchAddress("jet_flav", jetFlavour);
    
    curTree->SetBranchAddress("met_pt", &metPt);
    curTree->SetBranchAddress("met_phi", &metPhi);
    
    if (isMC)
    {
        curTree->SetBranchAddress("jesup_njets", &jetJECUpSize);
        curTree->SetBranchAddress("jet_jesup_pt", jetJECUpPt);
        curTree->SetBranchAddress("jet_jesup_eta", jetJECUpEta);
        curTree->SetBranchAddress("jet_jesup_phi", jetJECUpPhi);
        curTree->SetBranchAddress("jet_jesup_btagdiscri", jetJECUpBTag);
        curTree->SetBranchAddress("jet_jesup_flav", jetJECUpFlavour);
        
        curTree->SetBranchAddress("jesdown_njets", &jetJECDownSize);
        curTree->SetBranchAddress("jet_jesdown_pt", jetJECDownPt);
        curTree->SetBranchAddress("jet_jesdown_eta", jetJECDownEta);
        curTree->SetBranchAddress("jet_jesdown_phi", jetJECDownPhi);
        curTree->SetBranchAddress("jet_jesdown_btagdiscri", jetJECDownBTag);
        curTree->SetBranchAddress("jet_jesdown_flav", jetJECDownFlavour);
        
        curTree->SetBranchAddress("met_jesup_pt", &metJECUpPt);
        curTree->SetBranchAddress("met_jesup_phi", &metJECUpPhi);
        
        curTree->SetBranchAddress("met_jesdown_pt", &metJECDownPt);
        curTree->SetBranchAddress("met_jesdown_phi", &metJECDownPhi);
        
        curTree->SetBranchAddress("evtweight", &rawWeight);
    }
}
