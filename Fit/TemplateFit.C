using namespace RooFit;
using namespace RooStats;
using namespace std;


std::vactor<TString> dataSampleProvider(){
    //Data files
    std::vector<TString > datalist_longnames;
    datalist_longnames.push_back("NTuple_53X_SingleMuRun2012A");
    datalist_longnames.push_back("NTuple_53X_SingleMuRun2012B");
    datalist_longnames.push_back("NTuple_53X_SingleMuRun2012C");
    datalist_longnames.push_back("NTuple_53X_SingleMuRun2012D");
    return datalist_longnames;
}

//std::vactor<TString> signalSampleProvider(){
//    std::vector<TString > signallist_longnames;
//    signallist_longnames.push_back("NTuple_53X_TTJetsMadgraphZ2"            );
//    return signallist_longnames;
//}

//std::vactor<TString> bkg1SampleProvider(){
//    std::vector<TString > bkg1list_longnames;
//    bkg1list_longnames.push_back("NTuple_53X_WJetsToLNu"                  );
//    return bkg1list_longnames;
//}

//std::vactor<TString> bkg2SampleProvider(){
//    std::vector<TString > bkg2list_longnames;
//    bkg2list_longnames.push_back("NTuple_53X_DYJetsToLL_M-10To50"         );
//    bkg2list_longnames.push_back("NTuple_53X_DYJetsToLL_M-50"             );
//    bkg2list_longnames.push_back("NTuple_53X_T_s-channel"                 );
//   bkg2list_longnames.push_back("NTuple_53X_T_t-channel"                 );
//    bkg2list_longnames.push_back("NTuple_53X_T_tW-channel"                );
//    bkg2list_longnames.push_back("NTuple_53X_Tbar_t-channel"              );
//    bkg2list_longnames.push_back("NTuple_53X_Tbar_tW-channel"             );
//    bkg2list_longnames.push_back("NTuple_53X_WZJetsIncl"                  );
//    bkg2list_longnames.push_back("NTuple_53X_WWJetsIncl"                  );
//    bkg2list_longnames.push_back("NTuple_53X_ZZJetsIncl"                  );
    /*bkg2list_longnames.push_back("QCD_Pt-20to30_MuEnrichedPt5"            );
    bkg2list_longnames.push_back("QCD_Pt-30to50_MuEnrichedPt5"            );
    bkg2list_longnames.push_back("QCD_Pt-50to80_MuEnrichedPt5"            );
    bkg2list_longnames.push_back("QCD_Pt-80to120_MuEnrichedPt5"           );
    bkg2list_longnames.push_back("QCD_Pt-120to170_MuEnrichedPt5"          );
    bkg2list_longnames.push_back("QCD_Pt-170to300_MuEnrichedPt5"          );
    bkg2list_longnames.push_back("QCD_Pt-300to470_MuEnrichedPt5"          );*/
//    return bkg12ist_longnames;
//}

TH1D * GetHistogram(TString prefix, std::vector<TString> names, TString histname){
    TFile * f = TFile::Open(prefix+names[0]+".root");
    TH1D * ret = (TH1D*)f->Get(histname);
    for(unsigned int i = 1; i < names.size(); i++){
	f = new TFile(prefix+names[0]+".root","read");
	ret->Add((TH1D*)f->Get(histname));
    }
    return ret;
}

void TemplateFit(){

    //The variable 
    double lower = -999.; //adjust accordingly
    double upper = 999.; //adjust accordingly
    RooRealVar var("var","var", lower, upper);

    //Provide the approporiate prefix
    TString prefix = "histofile_";

    //Provide the approporiate histogram's name
    TString histname = "your_hist";

    //Get histograms for data, signal and backgrounds
    std::vactor<TString> datanames = dataSampleProvider();
    TH1D * dataHist =  GetHistogram(prefix,datanames,histname);
    dataHist->SetName("dataHist");

//    std::vactor<TString> signalnames = signalSampleProvider();
//   TH1D * signalHist =  GetHistogram(prefix,signalnames,histname);
//   signalHist->SetName("signalHist");    

//    std::vactor<TString> bkg1names = bkg1SampleProvider();
//    TH1D * wjetHist =  GetHistogram(prefix,bkg1names,histname);
//    wjetHist->SetName("wjetHist");

//    std::vactor<TString> bkg2names = bkg2SampleProvider();
//    TH1D * bkgHist =  GetHistogram(prefix,bkg2names,histname);
//    bkgHist->SetName("bkgHist");

    //Prepare RooDataHist for data
    RooDataHist data("data", "data", var, dataHist);

    //Prepare the model
//    RooRealVar sigSt("sigSt","sigSt",0.5, 0.,1.); //addjust the mean, lower bound and upper bound 
//    RooRealVar wjetSt("wjetSt","wjetSt",0.5, 0.,1.); //addjust the mean, lower bound and upper bound 
//    RooRealVar bkgSt("bkgSt","bkgSt",0.5, 0.,1.); //addjust the mean, lower bound and upper bound 

//    RooDataHist signal("signal", "signal", var, signalHist);    
//    RooDataHist wjet("wjet", "wjet", var, wjetHist);
//    RooDataHist bkg("bkg", "bkg", var, bkgHist);

//    RooHistPdf sigPdf("sigPdf","sigPdf", var, signal);
//    RooHistPdf wjetPdf("wjetPdf","wjetPdf", var, wjet);
//    RooHistPdf bkgPdf("bkgPdf","bkgPdf", var, bkg);

//    RooAddPdf model("mymodel","my model", RooArgList(sigPdf,wjetPdf,bkgPdf), RooArgList(sigSt,wjetSt,bkgSt));

    //Fitting
    //If the integrated luminosity is not multiplied to the histograms, the RooFit::Extended() option should be added.
//    model.fitTo(data, RooFit::Save(kTRUE));


    //Reading values and calculating the cross section
    //getVal() & getError() returns the value and its statistical uncertainty.

}
