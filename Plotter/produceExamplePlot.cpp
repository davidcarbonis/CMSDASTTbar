#include <Plotter.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv)
{
    std::stringstream lSStr; 
    lSStr << argv[1];
//    std::cout << "argv:" << argv[1] << std::endl;

    // Create a plotter
    Plotter plotter("MtW.root");
    
    // Specify the data histogram, along with a label to be used in the legend
    plotter.AddDataHist(("Data_hTopMass"+lSStr.str()).c_str(), " Data ");
    
    // Specify MC histograms, along with their colours and legend labels
    plotter.AddMCHist(("ttbar_hTopMass"+lSStr.str()).c_str(), kOrange + 1, " t#bar{t} ");
    plotter.AddMCHist(("SingleTop_hTopMass"+lSStr.str()).c_str(), kRed + 1, " t ");
    plotter.AddMCHist(("Wjets_hTopMass"+lSStr.str()).c_str(), kGreen + 1, " W+jets ");
    plotter.AddMCHist(("VV_hTopMass"+lSStr.str()).c_str(), kCyan, " VV ");
    plotter.AddMCHist(("DrellYan_hTopMass"+lSStr.str()).c_str(), kAzure, " Z/#gamma* ");
    plotter.AddMCHist(("QCD_hTopMass"+lSStr.str()).c_str(), kGray, " QCD ");
    
    // Produce files with pictures
    plotter.Plot( "Top mass;M(t), GeV;Events", "TopMass"+lSStr.str() );
    
    
    return EXIT_SUCCESS;
}
