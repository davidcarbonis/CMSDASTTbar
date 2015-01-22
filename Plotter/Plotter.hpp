#pragma once

#include <TFile.h>
#include <TH1.h>

#include <string>
#include <memory>
#include <vector>


/**
 * \class Plotter
 * \brief Plots data and MC expectations using histograms produced by the Reader
 * 
 * Simulated processes are drawn in a stacked plot. The data histogram is optional.
 */
class Plotter
{
public:
    /**
     * \brief Constructor
     * 
     * Takes as the argument the name of a ROOT file with a set of histograms for different
     * processes.
     */
    Plotter(std::string const &srcFileName);
    
public:
    /**
     * \brief Updates the data histogram
     * 
     * The name is used to get the histogram from the source file. The legend label is put into the
     * legend. If this method is called several times, each subsequent call replaces the previous
     * data histogram.
     */
    void AddDataHist(std::string const &name, std::string const &legendLabel);
    
    /**
     * \brief Adds an MC histogram
     * 
     * The name is used to get the histogram from the source file. The legend label is put into the
     * legend. Each call to this method adds a new MC histogram to the list.
     */
    void AddMCHist(std::string const &name, Color_t colour, std::string const &legendLabel);
    
    /**
     * \brief Produces plots with data and MC
     * 
     * The given title can be in the same format as for TH1::SetTitle, i.e. labels for x and y
     * axes can be provided, separated by semicolons. The output file name should not include the
     * file extension, which is added automatically. If the name contains directories, they must
     * exist before calling this method.
     */
    void Plot(std::string const &figureTitle, std::string const &outFileName);
    
    /**
     * \brief Adds or removes the residuals plot
     * 
     * The plot is not produced by default. It cannot be created if the data histogram is not
     * available.
     */
    void SwitchResiduals(bool on = true);
    
private:
    /// ROOT file that contains histograms to be plotted
    std::unique_ptr<TFile> srcFile;
    
    /// Histogram for data
    std::unique_ptr<TH1> dataHist;
    
    /// Histograms for simulation
    std::vector<std::unique_ptr<TH1>> mcHists;
    
    /// Indicates if the data/MC residuals should be plotted
    bool plotResiduals;
};
