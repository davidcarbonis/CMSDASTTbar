#include "TString.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "THStack.h"


#include <iostream>



bool useDDTemplates = false;
bool applyDD = false;




bool PlotStack(TString varname, TString namechan, TString selection, bool setlogy,
     std::vector<TString> dataSample_list, 
     std::vector<TString> channel_list, std::vector<TString> mcSample_list, std::vector<TString > signalSample_list, std::vector<int> colorVector, std::vector< TString > dataDrivenTemplates_list, bool sumChannels){
  

  
  
  
  TString channel = "";
  if(!sumChannels ) channel = namechan;
  
  
  
  TString filename;
  
  filename = "../Output/histofile_merged.root";
              
  
  Int_t stati=0;
  Bool_t  fit=1;
  
  
  //-----------------------------------
  //define the canvas 
  //-----------------------------------
  
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0); // must be kWhite but I dunno how to do that in PyROOT
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);
  
  
  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // ROOT . gStyle . SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(0); // kWhite
  gStyle->SetPadGridX(0); //false
  gStyle->SetPadGridY(0); //false
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  
  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // ROOT . gStyle . SetHistFillColor(1);
  // ROOT . gStyle . SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  // ROOT . gStyle . SetLegoInnerR(Float_t rad = 0.5);
  // ROOT . gStyle . SetNumberContours(Int_t number = 20);
  
  gStyle->SetEndErrorSize(2);
  //ROOT . gStyle . SetErrorMarker(20);   /// I COMMENTED THIS OUT
  //ROOT . gStyle . SetErrorX(0.);
  
  //ROOT . gStyle . SetMarkerStyle(20);
  
  
  //For the fit/function:
  gStyle->SetOptFit(1011);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);
  
  //For the date:
  gStyle->SetOptDate(0);
  // ROOT . gStyle . SetDateX(Float_t x = 0.01);
  // ROOT . gStyle . SetDateY(Float_t y = 0.01);
  
  // For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(0); // kWhite
  gStyle->SetStatFont(42);
  //ROOT . gStyle . SetStatFontSize(0.025);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  
  // Margins:
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  //ROOT . gStyle . SetPadRightMargin(0.12);
  gStyle->SetPadRightMargin(0.03);
  
  // For the Global title:
  
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  // ROOT . gStyle . SetTitleH(0); // Set the height of the title box
  // ROOT . gStyle . SetTitleW(0); // Set the width of the title box
  // ROOT . gStyle . SetTitleX(0); // Set the position of the title box
  // ROOT . gStyle . SetTitleY(0.985); // Set the position of the title box
  // ROOT . gStyle . SetTitleStyle(Style_t style = 1001);
  // ROOT . gStyle . SetTitleBorderSize(2);
  
  // For the axis titles:
  
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.06, "XYZ");
  // ROOT . gStyle . SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // ROOT . gStyle . SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(0.9);
  gStyle->SetTitleYOffset(1.25);
  // ROOT . gStyle . SetTitleOffset(1.1, "Y"); // Another way to set the Offset
  
  // For the axis labels:
  
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");
  
  // For the axis:
  
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(1); // kTRUE
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  
  // Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);
  
  // Postscript options:
  gStyle->SetPaperSize(20.,20.);
  
  
  
  //TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
  TCanvas *c1 = new TCanvas("c1","c1", 1000, 800);
  c1->SetBottomMargin(0.3);
  c1->SetLogy(setlogy);
  c1->cd();
  
  
  
  TFile * filechannel = new TFile(filename);
  
  
  
  TH1F *histo_data = 0;
  std::vector<TH1F *> histo_mcSamples;  
  std::vector<TH1F *> histo_ddSamples;  
  std::vector<TH1F *> histo_mcSignal;  
  
  //---------------------------------------------
  //retrieve and add histograms
  //---------------------------------------------
  int niter_data = 0;
  int niter_chan  = 0;
  cout << "channel " << channel << endl;
  for(unsigned int ichan = 0; ichan < channel_list.size() ; ichan++){
  
  
    if( !sumChannels  &&  channel_list[ichan] != channel ) continue;
    
  
    //--------------------
    //loop over datasamples
    //--------------------
    for(unsigned int idatasampl = 0; idatasampl< dataSample_list.size() ; idatasampl++){
      
      //TString histo_Data_name   = varname+"_"+channel+"_"+selection+ "_"+dataSample_list[idatasampl];
      TString histo_Data_name   = varname+"_"+channel_list[ichan]+"_"+selection+ "__"+dataSample_list[idatasampl];
      cout << histo_Data_name << endl;
      if( niter_data == 0 && niter_chan == 0){
        histo_data = (TH1F*)filechannel->Get(histo_Data_name);
        //if(varname == "mWT") histo_data->Rebin(5);
        if( histo_data== 0)  cout << "  no existing histo data with name  " << histo_Data_name  << endl;
      }
      else if(niter_data!=0) {
        TH1F * histo_tmp = (TH1F*)filechannel->Get(histo_Data_name);
        if(histo_tmp == 0)  cout << "  no existing histo data with name  " << histo_Data_name  << endl;
        //if(varname == "mWT") histo_tmp->Rebin(5);
        histo_data->Add(histo_data, histo_tmp);
      }
      
      
      niter_data++;
    }
    
    
    
    //--------------------
    //loop over MC samples
    //--------------------
    for(unsigned int imc = 0; imc < mcSample_list.size(); imc++){
    
      TString histo_mc_name   = varname+"_"+channel_list[ichan]+"_"+selection+ "__"+ mcSample_list[imc];
      TH1F * histo_tmp = (TH1F*)filechannel->Get(histo_mc_name);
      //if(varname == "mWT") histo_tmp->Rebin(5);
      
      int numchan = -1;
      
      if(channel_list[ichan] == "mujets" ) numchan = 0;
      
      if(histo_tmp == 0)  cout << "  no existing histo with name  " << histo_mc_name << endl;
      if(sumChannels){ 
        if(niter_chan == 0){
	    histo_mcSamples.push_back(histo_tmp);
        }else{
	     histo_mcSamples[imc]->Add(histo_mcSamples[imc], histo_tmp);
        }
      }else{
          histo_mcSamples.push_back(histo_tmp); 
      }
        
    }
    
    
    //-----------------------------
    //loop over data driven samples
    //-----------------------------
    for(unsigned int idd = 0; idd < dataDrivenTemplates_list.size(); idd++){
    
      TString histo_dd_name   = varname+"_"+channel_list[ichan]+"_"+selection+ "__"+ dataDrivenTemplates_list[idd];
      TH1F * histo_tmp = (TH1F*)filechannel->Get(histo_dd_name);
      //if(varname == "mWT") histo_tmp->Rebin(5);
      if(histo_tmp == 0)  cout << "  no existing histo with name  " << histo_dd_name << endl;
      if(niter_chan == 0){
        histo_ddSamples.push_back(histo_tmp);
      }else{
        histo_ddSamples[idd]->Add(histo_ddSamples[idd], histo_tmp);
      }
        
    }
    
    
    
    //------------------------
    //loop over signal samples
    //------------------------
  
    for(unsigned int isign = 0; isign < signalSample_list.size(); isign++){
      TString histo_mc_name   = varname+"_"+channel_list[ichan]+"_"+selection+ "_"+ signalSample_list[isign];
      TH1F * histo_tmp = (TH1F*)filechannel->Get(histo_mc_name);
      if(histo_tmp == 0)  cout << "  no existing histo with name  " << histo_mc_name << endl;
      if(niter_chan == 0){
        histo_mcSignal.push_back(histo_tmp);
      }else{
        histo_mcSignal[isign]->Add(histo_mcSignal[isign], histo_tmp);
      }
    }
  
  
  
    
    niter_chan++;
  }
  
  
  double nDY = 0;
  
  THStack *the_stack_histo= new THStack();
  for(unsigned int imc = 0; imc< histo_mcSamples.size(); imc++){
    
    histo_mcSamples[imc]->SetFillStyle(1001);
    histo_mcSamples[imc]->SetFillColor(colorVector[imc]);
    histo_mcSamples[imc]->SetLineColor(colorVector[imc]);
    if(imc < histo_mcSamples.size() && colorVector[imc] != colorVector[imc+1] )  histo_mcSamples[imc]->SetLineColor(1);
    if(imc ==  histo_mcSamples.size()) histo_mcSamples[imc]->SetLineColor(1);
    if(mcSample_list[imc] == "DYToLL_M10-50" || mcSample_list[imc] == "Zjets" ) {
      nDY += histo_mcSamples[imc]->Integral();
      if(useDDTemplates) continue;
    }
    the_stack_histo->Add(histo_mcSamples[imc]);
  }  
  
  //---------------------------
  //include data driven templates
  //---------------------------
  TH1F* histo_dd =0 ;
  if(useDDTemplates){
    int ndd = 0;
    for(unsigned int idd = 0; idd < histo_ddSamples.size(); idd++){
      if(ndd==0){
        histo_dd = (TH1F*) histo_ddSamples[idd]->Clone();
      }else{
        histo_dd->Add(histo_ddSamples[idd]);
      }
      ndd++;
    }
  
    histo_dd->SetFillStyle(1001);
    histo_dd->SetFillColor(kAzure-2);
    cout << "nDY " << nDY << endl;
    cout << "histo_dd->Integral() " << histo_dd->Integral() << endl;
    if(histo_dd->Integral() > 0.0001) histo_dd->Scale(nDY/histo_dd->Integral());
    the_stack_histo->Add(histo_dd);
  }
  
  the_stack_histo->Draw("h");
  the_stack_histo->GetXaxis()->SetLabelSize(0.0);
  if(histo_data->GetMaximum() > the_stack_histo->GetMaximum() ) the_stack_histo->SetMaximum(histo_data->GetMaximum()+0.3*histo_data->GetMaximum());
  else the_stack_histo->SetMaximum(the_stack_histo->GetMaximum()+0.3*the_stack_histo->GetMaximum());
  
  if(setlogy) the_stack_histo->SetMinimum(1000);
  
  histo_data->SetMarkerStyle(20);
  histo_data->SetMarkerSize(1.2);
  histo_data->SetLineColor(1);
  histo_data->Draw("epsame");
  
  //--------------------------
  //MC systematic plot
  //--------------------------
  TH1F * histo_syst_MC   = (TH1F*)(the_stack_histo->GetHistogram() )->Clone();
  for(unsigned int imc=0; imc< histo_mcSamples.size(); imc++){
    if(useDDTemplates){
      if  (mcSample_list[imc] != "DYToLL_M10-50" && mcSample_list[imc] != "Zjets" ) histo_syst_MC->Add(histo_mcSamples[imc]);
    }else histo_syst_MC->Add(histo_mcSamples[imc]);
  }
  
  
  if(useDDTemplates)  histo_syst_MC->Add(  histo_dd );
  //histo_syst_MC->Draw();
  //add here systematic uncertainties
  for (int ibin=0; ibin<histo_syst_MC->GetNbinsX(); ibin++) {
    //histo_syst_MC->SetBinError(ibin, histo_syst_MC->GetBinError(ibin)+histo_syst_MC->GetBinError(ibin)*0.025);
    
  }
  TGraphErrors *thegraph = new TGraphErrors(histo_syst_MC);
  thegraph->SetFillStyle(3005);
  thegraph->SetFillColor(1);
  thegraph->Draw("e2same");
  
  
  
  //-------------------
  //legend and captions
  //-------------------
  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.04);
  latex->SetTextAlign(31); 
  //latex->DrawLatex(0.45, 0.95, "CMS Preliminary");
  
  
  
  TLatex *latex2 = new TLatex();
  latex2->SetNDC();
  latex2->SetTextSize(0.04);
  latex2->SetTextAlign(31); 
  latex2->DrawLatex(0.87, 0.95, "19.7 fb^{-1} at #sqrt{s} = 8 TeV");
  
  TString info_data; 
  if (namechan=="eee")    info_data = "eee channel";
  if (namechan=="emumu")  info_data = "e#mu#mu channel";
  if (namechan=="mumue")  info_data = "#mu#mu e channel";
  if (namechan=="mumumu") info_data = "#mu#mu #mu channel";
  if (sumChannels )    info_data = "eee, #mu#mu#mu, e#mu#mu, ee#mu channels";
  
  
  TLatex * text2 = new TLatex(0.45,0.98, info_data);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.18);
  text2->SetY(0.92);
  //text2->SetLineWidth(2);
  text2->SetTextFont(42);
  text2->SetTextSize(0.0610687);
  //    text2->SetTextSizePixels(24);// dflt=28
  text2->Draw();
  
  
  TLegend* qw = 0;
  qw = new TLegend(.80,.60,.95,.90);

  qw->SetShadowColor(0);
  qw->SetFillColor(0);
  qw->SetLineColor(0);
  
  qw->AddEntry(histo_data,         "Data" ,                "ep");
  for(unsigned int i=0; i<mcSample_list.size(); i++){
    if(mcSample_list[i] == "NTuple_53X_TTJetsMadgraphZ2"        ) qw->AddEntry( histo_mcSamples[0],	"t#bar{t}"	 ,"f");
    if(mcSample_list[i] == "NTuple_53X_T_s-channel" ) qw->AddEntry( histo_mcSamples[6],	"single top"	 ,"f");
    if(mcSample_list[i] == "NTuple_53X_TTWJets_8TeVmadgraph"    ) qw->AddEntry( histo_mcSamples[1],	"t#bar{t}V"	 ,"f");
    if(mcSample_list[i] == "NTuple_53X_DYJetsToLL_M-50"         ) qw->AddEntry( histo_mcSamples[4],	"DY"		 ,"f");
    if(mcSample_list[i] == "NTuple_53X_WJetsToLNu"              ) qw->AddEntry( histo_mcSamples[3],	"W+jets"	 ,"f");
    if(mcSample_list[i] == "NTuple_53X_WWJetsIncl"              ) qw->AddEntry( histo_mcSamples[11],	"VV"	         ,"f");
  }
  
  qw->Draw();
  
  //--------------------------
  //Data over background ratio
  //--------------------------
  
   //TPad *canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.34);
  TPad *canvas_2 = new TPad("canvas_2", "canvas_2", 0.0, 0.0, 1.0, 1.0);
  canvas_2->SetTopMargin(0.7);
  canvas_2->SetFillColor(0);
  canvas_2->SetFillStyle(0);
  canvas_2->SetGridy(1);
  canvas_2->Draw();
  canvas_2->cd(0);
  
  
  
  TH1F * histo_ratio_data = (TH1F*)histo_data->Clone();
  histo_ratio_data->Divide(histo_syst_MC);
  
  //double theX[thegraph->GetN()] = thegraph->GetX();
  TGraphErrors *thegraph_tmp = (TGraphErrors*) thegraph->Clone();
  
  double *theErrorX  = thegraph_tmp->GetEX();
  double *theErrorY  = thegraph_tmp->GetEY();
  double *theY       = thegraph_tmp->GetY() ;
  for(int i=0; i<thegraph_tmp->GetN(); i++){
    theErrorY[i] = theErrorY[i]/theY[i];
    theY[i]=1;
  }
  
  
  TGraphErrors *thegraph_ratio = new TGraphErrors(thegraph_tmp->GetN(), thegraph_tmp->GetX() , theY ,  thegraph_tmp->GetEX(),     thegraph_tmp->GetEY() );
  thegraph_ratio->SetFillStyle(3005);
  thegraph_ratio->SetFillColor(1);
  if(varname == "InvM_ll") 	  histo_ratio_data->GetXaxis()->SetTitle("M_{ll} [GeV/c^{-1}]");
  else if(varname == "Njets")     histo_ratio_data->GetXaxis()->SetTitle("jet mult.");
  else if(varname == "NBjets")    histo_ratio_data->GetXaxis()->SetTitle("b-tagged jet mult.");
  else if(varname == "MET")	  histo_ratio_data->GetXaxis()->SetTitle("missing E_{T} [GeV]");
  else if(varname == "mWT")	  histo_ratio_data->GetXaxis()->SetTitle("m(W)_{T} [GeV]");
  else if(varname == "mW")	  histo_ratio_data->GetXaxis()->SetTitle("m(W) [GeV]");
  else if(varname == "BJetCSV")   histo_ratio_data->GetXaxis()->SetTitle("CSV discriminator");
  
  histo_ratio_data->SetMinimum(0.0);
  histo_ratio_data->SetMaximum(2.0);
  histo_ratio_data->GetXaxis()->SetTitleOffset(1.2);
  histo_ratio_data->GetXaxis()->SetLabelSize(0.04);
  histo_ratio_data->GetYaxis()->SetLabelSize(0.04);
  histo_ratio_data->GetYaxis()->SetNdivisions(6);
  histo_ratio_data->GetYaxis()->SetTitleSize(0.03);
  histo_ratio_data->Draw("E1X0");
  
  thegraph_ratio->Draw("e2same");
  
  TString end_name;
  if(setlogy) end_name="_Logy.gif";
  else end_name=".gif"; 
  //if(setlogy) end_name="_Logy.eps";
  //else end_name=".eps"; 

  TString outputname = "plots/"+varname+"_"+namechan+"_"+selection+end_name;
  if(sumChannels) outputname = "plots/"+varname+"_"+"all"+"_"+selection+end_name;
 
  c1->SaveAs(outputname.Data());
  
  
}
