

{

 gROOT->ProcessLine(".L PlotStack.C+");
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 gStyle->SetOptStat(0);
 
 //----------------
 //list of channels
 std::vector<TString> channel_list;
 channel_list.push_back("mujets");
 
  
  //---------------------------
  //define list of data samples 
  std::vector<TString> dataSample_list;
  dataSample_list.push_back("NTuple_53X_SingleMuRun2012A");	
  dataSample_list.push_back("NTuple_53X_SingleMuRun2012B");	
  dataSample_list.push_back("NTuple_53X_SingleMuRun2012C");	
  dataSample_list.push_back("NTuple_53X_SingleMuRun2012D");	
  
  //------------------------------------
  //define list of data-driven templates
  std::vector< TString > dataDrivenTemplates_list; 
  
 //-------------------------
 //define list of MC samples 
  std::vector<TString> mcSample_list;
  std::vector<int> colorVector;
  
  mcSample_list.push_back("NTuple_53X_TTJetsMadgraphZ2");     colorVector.push_back(kRed+1);
  //mcSample_list.push_back("NTuple_53X_TTWJets_8TeVmadgraph");     colorVector.push_back(kRed+3);
  //mcSample_list.push_back("NTuple_53X_TTZJets_8TeVmadgraph_v2");     colorVector.push_back(kRed+3);
  //mcSample_list.push_back("NTuple_53X_TZJetsTo3LNuB");     colorVector.push_back(kMagenta);
  mcSample_list.push_back("NTuple_53X_WJetsToLNu");     colorVector.push_back(kGreen+2);
  mcSample_list.push_back("NTuple_53X_DYJetsToLL_M-10To50");     colorVector.push_back(kAzure-2);
  mcSample_list.push_back("NTuple_53X_DYJetsToLL_M-50");     colorVector.push_back(kAzure-2);
  mcSample_list.push_back("NTuple_53X_T_s-channel");     colorVector.push_back(kRed+2);
  mcSample_list.push_back("NTuple_53X_T_t-channel");     colorVector.push_back(kRed+2);
  mcSample_list.push_back("NTuple_53X_T_tW-channel");     colorVector.push_back(kRed+2);
  mcSample_list.push_back("NTuple_53X_Tbar_t-channel");     colorVector.push_back(kRed+2);
  mcSample_list.push_back("NTuple_53X_Tbar_tW-channel");     colorVector.push_back(kRed+2);
  //mcSample_list.push_back("NTuple_53X_Tbar_s-channel");     colorVector.push_back();
  mcSample_list.push_back("NTuple_53X_WZJetsIncl");     colorVector.push_back(13);
  mcSample_list.push_back("NTuple_53X_WWJetsIncl");     colorVector.push_back(13);
  mcSample_list.push_back("NTuple_53X_ZZJetsIncl");     colorVector.push_back(13);
  mcSample_list.push_back("QCD_Pt-20to30_MuEnrichedPt5");     colorVector.push_back(kYellow+1);
  mcSample_list.push_back("QCD_Pt-30to50_MuEnrichedPt5");     colorVector.push_back(kYellow+1);
  mcSample_list.push_back("QCD_Pt-50to80_MuEnrichedPt5");     colorVector.push_back(kYellow+1);
  mcSample_list.push_back("QCD_Pt-80to120_MuEnrichedPt5");     colorVector.push_back(kYellow+1);
  mcSample_list.push_back("QCD_Pt-120to170_MuEnrichedPt5");     colorVector.push_back(kYellow+1);
  //mcSample_list.push_back("QCD_Pt-170to300_MuEnrichedPt");     colorVector.push_back(kYellow+1);
  mcSample_list.push_back("QCD_Pt-300to470_MuEnrichedPt5");     colorVector.push_back(kYellow+1);

  
  
  
  /*mcSample_list.push_back("TbarsChan");     colorVector.push_back(kMagenta);
  mcSample_list.push_back("TsChan");        colorVector.push_back(kMagenta);
  mcSample_list.push_back("TtChan");        colorVector.push_back(kMagenta);
  mcSample_list.push_back("TbartChan");     colorVector.push_back(kMagenta);
  mcSample_list.push_back("TtW");           colorVector.push_back(kMagenta);
  mcSample_list.push_back("TbartW");        colorVector.push_back(kMagenta);
  mcSample_list.push_back("TT");            colorVector.push_back(kRed-7);
  mcSample_list.push_back("TTZ");           colorVector.push_back(kRed+1);
  mcSample_list.push_back("TTW");           colorVector.push_back(kRed+1);
  mcSample_list.push_back("WZ");            colorVector.push_back(13);
  mcSample_list.push_back("ZZ");            colorVector.push_back(13);
  mcSample_list.push_back("WZHF");            colorVector.push_back(12);
  mcSample_list.push_back("DYToLL_M10-50"); colorVector.push_back(kAzure-2);
  mcSample_list.push_back("Zjets");         colorVector.push_back(kAzure-2);
  mcSample_list.push_back("tZq");           colorVector.push_back(kGreen+2);*/
  //mcSample_list.push_back("WW");            colorVector.push_back(13);
  
  
  
 //-----------------------------
 //define list of signal samples 
  std::vector<TString> signalSample_list;
  
  
  
  
 //--------------------------
 //define list of systematics
 std::vector<TString> syst_list;
 /*syst_list.push_back("leptup");
 syst_list.push_back("leptdown");
 //syst_list.push_back("trigup");
 //syst_list.push_back("trigdown");
 //syst_list.push_back("PDFup");
 //syst_list.push_back("PDFdown");
 syst_list.push_back("jesup");
 syst_list.push_back("jesdown");
 syst_list.push_back("jerup");
 syst_list.push_back("jerdown");
 syst_list.push_back("metunclsup");
 syst_list.push_back("metunclsdown");*/
  
  
 
 //--------------------------
 //define list of systematics
 std::vector<TString> selectionStep_list;
 selectionStep_list.push_back("ttbarregion");
 
 //------------------------
 //define list of variables
 std::vector<TString> variables_list;
 //variables_list.push_back("NJet");
 //variables_list.push_back("JetPt");
 //variables_list.push_back("JetEta");
 variables_list.push_back("mWT");
 //variables_list.push_back("mW");
 //variables_list.push_back("MET");
 //variables_list.push_back("LeptPt");
 //variables_list.push_back("LeptEta");
 /*variables_list.push_back("JetPt");
 variables_list.push_back("JetEta");
 variables_list.push_back("JetPt1");
 variables_list.push_back("JetEta1");
 variables_list.push_back("JetPt2");
 variables_list.push_back("JetEta2");
 variables_list.push_back("JetPt3");
 variables_list.push_back("JetEta3");
 variables_list.push_back("JetPt4");
 variables_list.push_back("JetEta4");*/
 
 bool dology = false;

  for(int iselstep=0; iselstep < selectionStep_list.size(); iselstep++){
      //PlotStack("InvM_ll_",channel_list[ichan],"_afterleptsel", i, k);
    for(int ivar=0; ivar < variables_list.size(); ivar++){
        
      for (int ichan=0; ichan<channel_list.size(); ichan++) {
	  //PlotStack(variables_list[ivar],  channel_list[ichan],  selectionStep_list[iselstep] , 0,  1);
	  PlotStack(variables_list[ivar],  channel_list[ichan],  selectionStep_list[iselstep] , dology, dataSample_list,  channel_list, mcSample_list, signalSample_list, colorVector, dataDrivenTemplates_list, false);
      }
       //PlotStack(variables_list[ivar],  "",  selectionStep_list[iselstep] , 0, dataSample_list,  channel_list, mcSample_list, signalSample_list, colorVector, dataDrivenTemplates_list, true);
    }
   } // end loop jchan
   
   
   
   
   
 }
