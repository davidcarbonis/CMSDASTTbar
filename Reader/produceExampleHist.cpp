#include <Reader.hpp>
#include <CalculatePzNu.hpp>
#include <TFile.h>
#include <TH1D.h>
#include <TLatex.h>

#include <list>
#include <iostream>
#include <memory>

#include <TLorentzVector.h>

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
    Group(string const &name, initializer_list<string> const &treeNames, bool isMC = true);
    
    /// Move constructor
    Group(Group &&) = default;
    
    /// A name to refer to the group
    string name;
    
    /// Names of trees that contribute to this group
    list<string> treeNames;
    
    /// Flag to indicate MC simulation as opposed to data
    bool isMC;
};


Group::Group(string const &name_, initializer_list<string> const &treeNames_,
 bool isMC_ /*= true*/):
    name(name_), treeNames(treeNames_), isMC(isMC_)
{}


int main()
{
    // ROOT manages memory in a very funny way. By default, it will assign every histogram to the
    //file accessed lastly. This behaviour is not desirable and is disabled by the following command
    TH1::AddDirectory(kFALSE);
    
    
    // Open the source ROOT file
    //shared_ptr<TFile> srcFile(TFile::Open("/data/shared/Long_Exercise_TTbar/mujets_v3.root"));
    shared_ptr<TFile> srcFile(TFile::Open("/afs/cern.ch/work/j/jandrea/public/proof_merged.root"));
    //^ There are copies at CMS DAS machines and AFS
    
    
    // There are trees for many processes in the source file. The processes will be combined into
    //several groups, and an independent histogram will be produced for all processes in each group.
    //Define here what processes (what trees) are grouped together and assign some meaningful name
    //to each group
    list<Group> groups;
    //    groups.emplace_back(Group("Data", {"SingleMuRun2012A", "SingleMuRun2012B", "SingleMuRun2012C", "SingleMuRun2012D"}, false));
    groups.emplace_back(Group("ttbar", {"TTJets"}));
//    groups.emplace_back(Group("SingleTop", {"T_t-channel", "Tbar_t-channel", "T_tW-channel", "Tbar_tW-channel"}));
//    groups.emplace_back(Group("Wjets", {"W1JetToLNu", "W2JetsToLNu", "W3JetsToLNu", "W4JetsToLNu"}));
//    groups.emplace_back(Group("VV", {"WWJetsIncl", "WZJetsIncl", "ZZJetsIncl"}));
//    groups.emplace_back(Group("DrellYan", {"DYJetsToLL_M-10To50", "DYJetsToLL_M-50"}));
//    groups.emplace_back(Group("QCD", {"QCD_Pt-20to30_MuEnrichedPt5", "QCD_Pt-30to50_MuEnrichedPt5", "QCD_Pt-50to80_MuEnrichedPt5", "QCD_Pt-80to120_MuEnrichedPt5", "QCD_Pt-120to170_MuEnrichedPt5", "QCD_Pt-170to300_MuEnrichedPt5", "QCD_Pt-300to470_MuEnrichedPt5"}));
    
    
    // Create an output file to store the histograms that will be created
    TFile outFile("MtW.root", "recreate");
    
    vector<Jet const *>bTaggedJets, untaggedJets;  // move to class definition
    vector<Jet const *> WHadronicCandidate;
    //vector<Jet> WHad;
    
    // Loop over the groups
    for (auto const &group: groups)
    {
        // A bit of verbosity
        cout << "Processing group \"" << group.name << "\"..." << endl;
        
        
        // Create a reader for the current group
        Reader reader(srcFile, group.treeNames, group.isMC);
        
        
        // Create a histogram to be filled. It is named after the group
        TH1D histMtW((group.name+"_histMtW").c_str(), "Transverse W mass;M_{T}(W), GeV;Events", 100., 0., 200.);
        TH1D histInv3Jet((group.name+"_histInv3Jet").c_str(), "Invariant mass of 3 leading jet; M(jjj), GeV; Events", 300., 0., 600.);
	TH1D hTopMass1((group.name+"_hTopMass1").c_str(), "Top mass Hadronic; M(top), GeV; Events", 300., 0., 600.);
	TH1D hTopMass2((group.name+"_hTopMass2").c_str(), "Top mass Leptonic; M(top), GeV; Events", 300., 0., 600.);
	TH1D hWmass1((group.name+"_hWmass1").c_str(), "W mass from Hadronic Decay; M(W), GeV; Events", 300.0, 0., 600.0);
	TH1D hWmass2((group.name+"_hWmass2").c_str(), "W mass from Leptonic Decay; M(W), GeV; Events", 300.0, 0.0, 600.0);
	TH1D hLeptonMass((group.name+"_hLeptonMass").c_str(), "Lepton Mass; M(l), GeV; Events", 300.0, 0.0, 600.0);
	TH1D hNuMass((group.name+"_hNuMass").c_str(), "Neutrino Mass; M(#nu), GeV; Events", 300.0, 0.0, 600.0);

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
	    float mass;
	    int nSelJet = 0;

	    //vector<Jet const *> bTaggedJets, untaggedJets;  // move to class definition
	    bTaggedJets.clear();
	    untaggedJets.clear();
            
            for (Jet const &j: jets)
            {
                if (j.Pt() < 30.)  // jets are ordered in pt
                    break;
                ++nSelJet;

                //if (fabs(j.Eta()) < 2.4)
		if (fabs(j.Eta()) > 2.4) continue;
		++nGoodJets;

		if (j.BTag() > 0.679)
		  bTaggedJets.push_back(&j);
		else
		  untaggedJets.push_back(&j);
            }

	    if (bTaggedJets.size() != 2) continue;


      	    if (nSelJet > 3) {
	      mass = (jets.at(0).P4()+ jets.at(1).P4()+ jets.at(2).P4()).M();
	      histInv3Jet.Fill(mass, reader.GetWeight());
	    }

            if (nGoodJets < 4)
                continue;
            
            // Calculate the variable of interest
            MET const &met = reader.GetMET();
            double const MtW = sqrt(pow(l.Pt() + met.Pt(), 2) -
             pow(l.P4().Px() + met.P4().Px(), 2) - pow(l.P4().Py() + met.P4().Py(), 2));
            if (MtW < 50.) continue; // MtW cut from group 1

            // Fill the histogram. Note that simulated events are weighted
            histMtW.Fill(MtW, reader.GetWeight());

	    //loop to choose 2 jets from W candidate
	    const int nUnTagJet = untaggedJets.size();
	    double Mass_W = 80.4;
	    double massW;
	    double minimiser = 1000.;
	    //vector <Jet> WHadronicCandidate;

	    for (int i =0; i < nUnTagJet; ++i) {
	      for (int j = i+1; j < nUnTagJet; ++j) {
		massW = (untaggedJets.at(i)->P4() + untaggedJets.at(j)->P4()).M();

		if ( fabs(massW-Mass_W) < minimiser) {

		  minimiser = fabs(massW-Mass_W);
		  WHadronicCandidate.clear();
		  WHadronicCandidate.push_back(untaggedJets.at(i));
		  WHadronicCandidate.push_back(untaggedJets.at(j));
		}
	      }
	    }

	    if (WHadronicCandidate.size() != 2) continue;

	    //W from lepton channel
	    TLorentzVector WLepton;
	    WLepton = Nu4Momentum(l.P4(), met.Pt(), met.Phi()) + l.P4(); //Nu 4mom + lepton 4mon

	    double massTop1, massTop2;
	    double mtWHad1, mtWHad2;
	    double mtWLep1, mtWLep2;

	    //mtWHad1 = (bTaggedJets.at(0).P4 + WHad.P4()).M();
	    //mtWHad2 = (bTaggedJets.at(1).P4 + WHad.P4()).M();

	    mtWHad1 = (bTaggedJets.at(0)->P4() + WHadronicCandidate.at(0)->P4() + WHadronicCandidate.at(1)->P4()).M();
	    mtWHad2 = (bTaggedJets.at(1)->P4() + WHadronicCandidate.at(0)->P4() + WHadronicCandidate.at(1)->P4()).M();

	    mtWLep1 = (bTaggedJets.at(0)->P4() + WLepton).M();
	    mtWLep2 = (bTaggedJets.at(1)->P4() + WLepton).M();

	    if (fabs(mtWHad1 - mtWLep2) < fabs(mtWHad2 - mtWLep1)) {
	      massTop1 = mtWHad1;
	      massTop2 = mtWLep2;
	    }
	    else {
	      massTop1 = mtWHad2;
	      massTop2 = mtWLep1;
	    }

	    hLeptonMass.Fill(l.M());
	    hNuMass.Fill( Nu4Momentum(l.P4(), met.Pt(), met.Phi() ).M() );
	    hWmass1.Fill(massW);
	    hWmass2.Fill(WLepton.M());
	    hTopMass1.Fill(massTop1);
	    hTopMass2.Fill(massTop2);

        }
        
        
       // The histogram for the current group has been filled. Save it in the output file
        outFile.cd();
        histMtW.Write();
	histInv3Jet.Write();
	hLeptonMass.Write();
	hNuMass.Write();
	hWmass1.Write();
	hWmass2.Write();
	hTopMass1.Write();
	hTopMass2.Write();

    }
    
    
    cout << "Done. Results are saved in the file \"" << outFile.GetName() << "\".\n";
    
    
    return EXIT_SUCCESS;
}
