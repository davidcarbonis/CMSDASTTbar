#include <Plotter.hpp>

#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGaxis.h>

#include <stdexcept>
#include <sstream>
/**/#include <iostream>


using namespace std;


Plotter::Plotter(string const &srcFileName):
    srcFile(TFile::Open(srcFileName.c_str())),
    plotResiduals(true)
{}


void Plotter::AddDataHist(string const &name, string const &legendLabel)
{
    // Get the histogram from the file
    //TH1 *hist = dynamic_cast<TH1 *>(srcFile->Get(name.c_str()));
    TH1 *hist = (TH1 *)(srcFile->Get(name.c_str()));
    
    
    // Check if such histogram exists
    if (not hist)
    {
        ostringstream ost;
        ost << "File \"" << srcFile->GetName() << "\" does not contain a histogram called \"" <<
         name << "\".";
        throw runtime_error(ost.str());
    }
    
    
    // Disentangle the histogram from the source file
    hist->SetDirectory(nullptr);
    
    
    // Save the legend label as the title of this histogram
    hist->SetTitle(legendLabel.c_str());
    
    
    // Update the pointer
    dataHist.reset(hist);
}


void Plotter::AddMCHist(string const &name, Color_t colour, string const &legendLabel)
{
    // Get the histogram from the file
    //TH1 *hist = dynamic_cast<TH1 *>(srcFile->Get(name.c_str()));
    TH1 *hist = (TH1 *)(srcFile->Get(name.c_str()));
    
    
    // Check if such histogram exists
    if (not hist)
    {
        ostringstream ost;
        ost << "File \"" << srcFile->GetName() << "\" does not contain a histogram called \"" <<
         name << "\".";
        throw runtime_error(ost.str());
    }
    
    
    // Disentangle the histogram from the source file
    hist->SetDirectory(nullptr);
    
    
    // Set the fill colour
    hist->SetFillColor(colour);
    
    
    // Save the legend label as the title of this histogram
    hist->SetTitle(legendLabel.c_str());
    
    
    // Add the histogram to the collection of simulated processes
    mcHists.emplace_back(hist);
}


void Plotter::Plot(string const &figureTitle, string const &outFileName)
{
    // Make sure the histograms exist
    // if (not dataHist)
    //     throw runtime_error("Data histogram has not been provided.");
    
    if (mcHists.size() == 0)
        throw runtime_error("No MC histograms have beed provided.");
    
    
    // Set decoration for all histograms
    if (dataHist)
        dataHist->SetMarkerStyle(20);
    
    for (auto &h: mcHists)
    {
        h->SetLineColor(kBlack);
        h->SetLineWidth(1);
    }
    
    
    // Global decoration settings
    gStyle->SetErrorX(0.);
    gStyle->SetHistMinimumZero(kTRUE);
    gStyle->SetOptStat(0);
    gStyle->SetStripDecimals(kFALSE);
    TGaxis::SetMaxDigits(3);
    
    
    // Setup layout of pads within the canvas
    // Allow space for the residuals plot if requested
    double const bottomSpacing = (plotResiduals) ? 0.2 : 0.;
    
    // Margin for axis labels
    double const margin = 0.1;
    
    // Width of the main pad
    double const mainPadWidth = 0.85;
    
    
    // Create a canvas and pads to draw in
    TCanvas canvas("canvas", "", 1500, 1000 / (1. - bottomSpacing));
    
    TPad mainPad("mainPad", "", 0., bottomSpacing, mainPadWidth + margin, 1.);
    mainPad.SetTicks();
    
    // Adjust margins to host axis labels (otherwise they would be cropped)
    mainPad.SetLeftMargin(margin / mainPad.GetWNDC());
    mainPad.SetRightMargin(margin / mainPad.GetWNDC());
    mainPad.SetBottomMargin(margin / mainPad.GetHNDC());
    mainPad.SetTopMargin(margin / mainPad.GetHNDC());
    
    mainPad.Draw();
    
    
    // Put MC histogramss into a stack
    THStack mcStack("mcStack", figureTitle.c_str());
    
    for (auto h = mcHists.crbegin(); h != mcHists.crend(); ++h)
        mcStack.Add(h->get(), "hist");
    
    
    // Create a legend
    TLegend legend(0.86, 0.9 - 0.04 * (mcHists.size() + ((dataHist) ? 1 : 0)), 0.99, 0.9);
    legend.SetFillColor(kWhite);
    legend.SetTextFont(42);
    legend.SetTextSize(0.03);
    
    if (dataHist)
        legend.AddEntry(dataHist.get(), dataHist->GetTitle(), "p");
    
    for (auto const &h: mcHists)
        legend.AddEntry(h.get(), h->GetTitle(), "f");
    
    
    // Draw the MC stack and the data histogram
    mainPad.cd();
    mcStack.Draw();
    
    if (dataHist)
        dataHist->Draw("p0 e1 same");
    
    
    // Draw the legend
    canvas.cd();
    legend.Draw();
    
    
    // Update the maximum
    if (dataHist)
    {
        double const histMax = 1.1 * max(mcStack.GetMaximum(), dataHist->GetMaximum());
        mcStack.SetMaximum(histMax);
        dataHist->SetMaximum(histMax);
    }
    
    
    // Plot residuals histogram if needed
    unique_ptr<TPad> residualsPad;
    unique_ptr<TH1> residualsHist;
    
    if (plotResiduals)
    {
        // Create a histogram with total MC expectation. Use a pointer rather than an object in
        //order to infer in terms of the base class
        unique_ptr<TH1> mcTotalHist((TH1 *)(mcHists.front()->Clone("mcTotalHist")));
        
        for (unsigned i = 1; i < mcHists.size(); ++i)
            mcTotalHist->Add(mcHists.at(i).get());
        
        
        // Create a histogram with residuals. Again avoid referring to a concrete histogram class
        residualsHist.reset(((TH1 *)(dataHist->Clone("residualsHist"))));
        residualsHist->Add(mcTotalHist.get(), -1);
        residualsHist->Divide(mcTotalHist.get());
                
        
        // Create a pad to draw residuals
        residualsPad.reset(new TPad("residualsPad", "", 0., 0., mainPadWidth + margin,
         bottomSpacing + margin));
        
        
        // Adjust the pad's margins so that axis labels are not cropped
        residualsPad->SetLeftMargin(margin / residualsPad->GetWNDC());
        residualsPad->SetRightMargin(margin / residualsPad->GetWNDC());
        residualsPad->SetBottomMargin(margin / residualsPad->GetHNDC());
        
        
        // Decoration of the pad
        residualsPad->SetTicks();
        residualsPad->SetGrid(0, 1);
        
        residualsPad->SetFillStyle(0);
        //^ Needed not to oscure the lower half of the zero label in the mainPad
        
        
        // Draw the pad
        canvas.cd();
        residualsPad->Draw();
        
        
        // Extract the label of the x axis of the MC stacked plot
        auto const pos1 = figureTitle.find_first_of(';');
        auto const pos2 = figureTitle.find_first_of(';', pos1 + 1);
        string const xAxisTitle(figureTitle.substr(pos1 + 1, pos2 - pos1 - 1));
        
        
        // Reset the title of the residuals histogram. It gets axis titles only
        residualsHist->SetTitle((string(";") + xAxisTitle + ";#frac{Data-MC}{MC}").c_str());
        
        
        // Decoration of the residuals histogram
        residualsHist->SetMinimum(-0.2);
        residualsHist->SetMaximum(0.2);
        
        residualsHist->SetMarkerStyle(20);
        residualsHist->SetLineColor(kBlack);
        
        TAxis *xAxis = residualsHist->GetXaxis();
        TAxis *yAxis = residualsHist->GetYaxis();
        
        // Make axis label and titles of same size as in the main pad (the actual text size for the
        //default font is linked up with the current pad's smallest dimension)
        xAxis->SetTitleSize(mcStack.GetXaxis()->GetTitleSize() *
         mainPad.GetHNDC() / residualsPad->GetHNDC());
        xAxis->SetLabelSize(mcStack.GetXaxis()->GetLabelSize() *
         mainPad.GetHNDC() / residualsPad->GetHNDC());
        yAxis->SetTitleSize(mcStack.GetXaxis()->GetTitleSize() *
         mainPad.GetHNDC() / residualsPad->GetHNDC());
        yAxis->SetLabelSize(mcStack.GetXaxis()->GetLabelSize() *
         mainPad.GetHNDC() / residualsPad->GetHNDC());
        
        yAxis->SetNdivisions(405);
        yAxis->CenterTitle();
        yAxis->SetTitleOffset(0.5);
        
        
        // Draw the residuals histogram
        residualsPad->cd();
        residualsHist->Draw("p0 e1");
        
        
        // Remove the labels from x axis of the main histogram
        mcStack.GetXaxis()->SetLabelOffset(999.);
    }
    
    
    // Save the picture
    for (auto const &fileType: {"png", "pdf", "C"})
        canvas.Print((outFileName + "." + fileType).c_str());
}
