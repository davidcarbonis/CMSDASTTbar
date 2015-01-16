using namespace RooFit;
using namespace RooStats;
using namespace std;
#include <vector>
#include <iostream>


void TemplateFit(){

    //The variable 
    double lower = -1.5; //adjust accordingly
    double upper = 1.5; //adjust accordingly
    RooRealVar var("charge","lepton charge", lower, upper);

    //Get histograms for data, signal and backgrounds
    //The current modeling discriminates between W+jets and other backgrounds. 
    //It could be different :)
    
    //Remember to adjust all the names for histograms and input files
	TFile * f = TFile::Open("Charge.root");
	
	TH1D * dataHist =  (TH1D*)f->Get("Data");
	
    TH1D * signalHist =  (TH1D*)f->Get("ttbar");
    double nSignal = signalHist->Integral();
    
    TH1D * wjetHist =  (TH1D*)f->Get("Wjets");
    double nWjets = wjetHist->Integral();
        
    TH1D * bkgHist =  (TH1D*)f->Get("SingleTop");
    TList * list = f->GetListOfKeys();
    for(int i  = 0; i < list->GetSize(); i++){
    	if(list->At(i)->GetName() == TString("Data") || list->At(i)->GetName() == TString("ttbar") || list->At(i)->GetName() == TString("Wjets") || list->At(i)->GetName() == TString("SingleTop"))
    		continue;
    	cout << i <<endl;
    	bkgHist->Add((TH1D*)f->Get(list->At(i)->GetName()));
    }
    double nBkg = bkgHist->Integral();	
    

	//Declaration of variables
    RooRealVar nbkg("nbkg","nbkg", nBkg, 0,2*nBkg);
    RooRealVar nsig("nsig","nsig", nSignal, 0,2*nSignal);  
    RooRealVar nwjets("nwjets","nwjets", nWjets, 0,2*nWjets); 
    
    //Data declaration
    RooDataHist data("data","data",var, dataHist);
    
    //Model construction
    RooDataHist sig("sig","sig",var, signalHist);
    RooHistPdf sigPdf("sigPdf","sigPdf",var, sig);
    
    RooDataHist wjets("wjets","wjets",var, wjetHist);
    RooHistPdf wjetsPdf("wjetsPdf","wjetsPdf",var, wjets);
    
	RooDataHist bkg("bkg","bkg",var, bkgHist);
    RooHistPdf bkgPdf("bkgPdf","bkgPdf",var, bkg);
    
    RooAddPdf model("model","model", RooArgList(sigPdf,wjetsPdf,bkgPdf),RooArgList(nsig,nwjets,nbkg));    
    
    //Fitting 
    model.fitTo(data);    
    
    //Visualizing
    RooPlot * fr = var.frame();
    data.plotOn(fr);
    model.plotOn(fr);
    model.plotOn(fr,RooFit::Components(sigPdf), 
        RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen)); 
    model.plotOn(fr,RooFit::Components(wjetsPdf), 
        RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));      
    model.plotOn(fr,RooFit::Components(bkgPdf), 
        RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen+3));               
    TCanvas c;
    c.cd();
    fr->Draw(); 
    c.SaveAs("charge.C");
    
    //Print out
    //You can find more clever method to have the same on your screen :)
	cout<<"Signal yield: "<<nsig.getVal() <<" +/- "<<nsig.getError() <<endl;
    cout<<"Bkg yield: "<<nbkg.getVal() <<" +/- "<<nbkg.getError() <<endl;
    cout<<"Wjets yield: "<<nwjets.getVal() <<" +/- "<<nwjets.getError() <<endl;
	
}

