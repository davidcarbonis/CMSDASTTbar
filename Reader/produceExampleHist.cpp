#include <Reader.hpp>

#include <TFile.h>
#include <TH1D.h>

#include <list>
#include <iostream>
#include <memory>


using namespace std;


/**
 * \struct Group
 * \brief An auxiliary structure to group several trees together
 * 
 * Each tree in the source file corresponds to a different physics process. It is useful to consider
 * several processes together. This structure defines what trees should be considered with a group,
 * and gives the group a name.
 */
struct Group
{
    /// Constructor without paramters
    Group() = default;
    
    /// Constructor with explicit initialisation
    Group(string const &name, initializer_list<string> const &treeNames);
    
    /// Move constructor
    Group(Group &&) = default;
    
    /// A name to refer to the group
    string name;
    
    /// Names of trees that contribute to this group
    list<string> treeNames;
};


Group::Group(string const &name_, initializer_list<string> const &treeNames_):
    name(name_), treeNames(treeNames_)
{}


int main()
{
    // ROOT manages memory in a very funny way. By default, it will assign every histogram to the
    //file accessed lastly. This behaviour is not desirable and is disabled by the following command
    TH1::AddDirectory(kFALSE);
    
    
    // Open the source ROOT file
    shared_ptr<TFile> srcFile(TFile::Open("../../proof_merged_mujets.root"));
    
    
    // There are trees for many processes in the source file. The processes will be combined into
    //several groups, and an independent histogram will be produced for all processes in each group.
    //Define here what processes (what trees) are grouped together and assign some meaningful name
    //to each group
    list<Group> groups;
    groups.emplace_back(Group("Data", {"SmallTree_NTuple_53X_SingleMuRun2012A",
     "SmallTree_NTuple_53X_SingleMuRun2012B", "SmallTree_NTuple_53X_SingleMuRun2012C",
     "SmallTree_NTuple_53X_SingleMuRun2012D"}));
    groups.emplace_back(Group("ttbar", {"SmallTree_NTuple_53X_TTJetsMadgraphZ2"}));
    groups.emplace_back(Group("SingleTop", {"SmallTree_NTuple_53X_T_t-channel",
     "SmallTree_NTuple_53X_Tbar_t-channel", "SmallTree_NTuple_53X_T_tW-channel",
     "SmallTree_NTuple_53X_Tbar_tW-channel"}));
    groups.emplace_back(Group("Wjets", {"SmallTree_NTuple_53X_WJetsToLNu"}));
    groups.emplace_back(Group("VV", {"SmallTree_NTuple_53X_WWJetsIncl", "SmallTree_NTuple_53X_WZJetsIncl",
     "SmallTree_NTuple_53X_ZZJetsIncl"}));
    groups.emplace_back(Group("DrellYan", {"SmallTree_NTuple_53X_DYJetsToLL_M-10To50",
     "SmallTree_NTuple_53X_DYJetsToLL_M-50"}));
    groups.emplace_back(Group("QCD", {"SmallTree_QCD_Pt-20to30_MuEnrichedPt5",
     "SmallTree_QCD_Pt-30to50_MuEnrichedPt5", "SmallTree_QCD_Pt-50to80_MuEnrichedPt5",
     "SmallTree_QCD_Pt-80to120_MuEnrichedPt5", "SmallTree_QCD_Pt-120to170_MuEnrichedPt5",
     "SmallTree_QCD_Pt-170to300_MuEnrichedPt5", "SmallTree_QCD_Pt-300to470_MuEnrichedPt5"}));
    
    
    // Create an output file to store the histograms that will be created
    TFile outFile("MtW.root", "recreate");
    
    
    // Loop over the groups
    for (auto const &group: groups)
    {
        // Create a reader for the current group
        Reader reader(srcFile, group.treeNames);
        
        
        // Create a histogram to be filled. It is named after the group
        TH1D histMtW(group.name.c_str(), "Transverse W mass;M_{T}(W), GeV;Events", 60, 0., 120.);
        
        
        // Loop over all events in the current group of processes
        while (reader.ReadNextEvent())
        {
            // Perform some event selection
            // Event should contain exactly one charged lepton (muon in this case)
            if (reader.GetLeptons().size() != 1)
                continue;
            
            
            // The muon should have sufficient transverse momentum and should not be too forward
            Lepton const &l = reader.GetLeptons().front();
            
            if (l.Pt() < 26. or fabs(l.Eta()) > 2.1)
                continue;
            
            
            // Require that there are at least four central jets with pt > 30 GeV
            auto const &jets = reader.GetJets();
            unsigned nGoodJets = 0;
            
            for (auto const &j: jets)
            {
                if (j.Pt() < 30.)  // jets are ordered in pt
                    break;
                
                if (fabs(j.Eta()) < 2.4)
                    ++nGoodJets;
            }
            
            if (nGoodJets < 4)
                continue;
            
            
            // Calculate the variable of interest
            MET const &met = reader.GetMET();
            double const MtW = sqrt(pow(l.Pt() + met.Pt(), 2) -
             pow(l.P4().Px() + met.P4().Px(), 2) - pow(l.P4().Py() + met.P4().Py(), 2));
            
            
            // Fill the histogram. Note that simulated events are weighted
            histMtW.Fill(MtW, reader.GetWeight());
        }
        
        
        // The histogram for the current group has been filled. Save it in the output file
        outFile.cd();
        histMtW.Write();
    }
    
    
    return EXIT_SUCCESS;
}
