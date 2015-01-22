#include <Plotter.hpp>


int main()
{
    // Create a plotter
    Plotter plotter("MtW.root");
    
    // Specify the data histogram, along with a label to be used in the legend
    plotter.AddDataHist("Data", " Data ");
    
    // Specify MC histograms, along with their colours and legend labels
    plotter.AddMCHist("ttbar", kOrange + 1, " t#bar{t} ");
    plotter.AddMCHist("SingleTop", kRed + 1, " t ");
    plotter.AddMCHist("Wjets", kGreen + 1, " W+jets ");
    plotter.AddMCHist("VV", kCyan, " VV ");
    plotter.AddMCHist("DrellYan", kAzure, " Z/#gamma* ");
    plotter.AddMCHist("QCD", kGray, " QCD ");
    
    // Produce files with pictures
    plotter.Plot("Transverse W mass;M_{T}(W), GeV;Events", "MtW");
    
    
    return EXIT_SUCCESS;
}
