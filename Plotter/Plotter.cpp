#include <Plotter.hpp>

#include <THStack.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGaxis.h>

#include <stdexcept>
#include <sstream>


using namespace std;


Plotter::Plotter(string const &srcFileName):
    srcFile(TFile::Open(srcFileName.c_str()))
{}


void Plotter::AddDataHist(string const &name, string const &legendLabel)
{
    // Get the histogram from the file
    TH1 *hist = reinterpret_cast<TH1 *>(srcFile->Get(name.c_str()));
    // TODO: Change to dynamic_cast
    
    
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
    TH1 *hist = reinterpret_cast<TH1 *>(srcFile->Get(name.c_str()));
    // TODO: Change to dynamic_cast
    
    
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
    if (not dataHist)
        throw runtime_error("Data histogram has not been provided.");
    
    if (mcHists.size() == 0)
        throw runtime_error("No MC histograms have beed provided.");
    
    
    // Set decoration for all histograms
    dataHist->SetMarkerStyle(20);
    
    for (auto &h: mcHists)
    {
        h->SetLineColor(kBlack);
        h->SetLineWidth(1);
    }
    
    
    // Global decoration settings
    gStyle->SetErrorX(0.);
    gStyle->SetHistMinimumZero(kTRUE);
    TGaxis::SetMaxDigits(3);
    
    
    // Put MC histogramss into a stack
    THStack mcStack("mcStack", figureTitle.c_str());
    
    for (auto h = mcHists.crbegin(); h != mcHists.crend(); ++h)
        mcStack.Add(h->get(), "hist");
    
    
    // Create a canvas
    TCanvas canvas("canvas", "", 1500, 1000);
    canvas.SetTicks();
    
    
    // Draw the MC stack
    mcStack.Draw();
    
    
    // Draw the data histogram
    dataHist->Draw("p0 e1 same");
    
    
    // Update the maximum
    double const histMax = 1.1 * max(mcStack.GetMaximum(), dataHist->GetMaximum());
    mcStack.SetMaximum(histMax);
    dataHist->SetMaximum(histMax);
    
    
    // Save the picture
    for (auto const &fileType: {"png", "pdf", "C"})
        canvas.Print((outFileName + "." + fileType).c_str());
}
