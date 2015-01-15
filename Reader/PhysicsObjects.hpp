#pragma once

#include <TLorentzVector.h>


/**
 * \class Candidate
 * \brief A wrapper around a Lorentz vector
 * 
 * The class is used as a base class for leptons and jets
 */
class Candidate
{
public:
    /// Constructor with no parameters
    Candidate() noexcept;
    
    /// Constructor from a Lorentz vector
    Candidate(TLorentzVector const &p4) noexcept;
    
    /// Constructor with explicit initialisation
    Candidate(double pt, double eta, double phi, double mass = 0.) noexcept;
    
public:
    /// Accessor to the four-momentum
    TLorentzVector const &P4() const noexcept;
    
    /// A short-cut for transverse momentum
    double Pt() const noexcept;
    
    /// A short-cut for pseudorapidity
    double Eta() const noexcept;
    
    /// A short-cut for azimuthal angle
    double Phi() const noexcept;
    
    /// A short-cut for mass
    double M() const noexcept;
    
    /// A short-cut to calculate dR distance
    double DeltaR(Candidate const &rhs) const noexcept;
    
    /// Comparison operator for pt ordering
    bool operator<(Candidate const &rhs) const noexcept;
    
protected:
    /// Four-momentum
    TLorentzVector p4;
};


/**
 * \class Lepton
 * \brief Describes a reconstructed lepton
 */
class Lepton: public Candidate
{
public:
    /// Constructor without parameters
    Lepton() noexcept;
    
    /**
     * \brief Constructor explicit initialisation
     * 
     * The mass is set according to the given flavour.
     */
    Lepton(int flavour, double pt, double eta, double phi, double isolation) noexcept;
    
public:
    /**
     * \brief Returns lepton flavour
     * 
     * Encoded with PDG ID codes [1].
     * [1] https://twiki.cern.ch/twiki/bin/view/Main/PdgId
     */
    int Flavour() const noexcept;
    
    /**
     * \brief Returns lepton isolation
     * 
     * The smaller the value, the smaller are energy depositions around the lepton.
     */
    double Isolation() const noexcept;
    
private:
    /**
     * \brief Flavour
     * 
     * See documentation for the method 'Flavour'.
     */
    int flavour;
    
    /// Isolation
    double isolation;
};


/**
 * \class Jet
 * \brief Describes a reconstructed jet
 */
class Jet: public Candidate
{
public:
    /// Constructor without parameters
    Jet() noexcept;
    
    /**
     * \brief Constructor explicit initialisation
     * 
     * The mass is set to zero.
     */
    Jet(double pt, double eta, double phi, double bTag, int flavour = 0) noexcept;
    
public:
    /**
     * \brief Returns jet flavour
     * 
     * Encoded with PDG ID codes [1]; zero indicates that the flavour is not known. The flavour is
     * available only in simulation.
     * [1] https://twiki.cern.ch/twiki/bin/view/Main/PdgId
     */
    int Flavour() const noexcept;
    
    /**
     * \brief Returns value of the b-tagging discriminator
     * 
     * The larger the value, the more likely it is that the jet stems from fragmentation of a
     * B hadron.
     */
    double BTag() const noexcept;
    
private:
    /**
     * \brief Flavour
     * 
     * See documentation for the method 'Flavour'.
     */
    int flavour;
    
    /**
     * \brief Value of the b-tagging discriminator
     * 
     * See documentation for the method 'BTag'.
     */
    double bTag;
};


/**
 * \class MET
 * \brief Represents reconstructed missing transverse energy
 */
class MET: public Candidate
{
public:
    /// Constructor without parameters
    MET() noexcept;
    
    /**
     * \brief Constructor with complete initialisation
     * 
     * Pseudorapidity and mass are always set to zero.
     */
    MET(double pt, double phi) noexcept;
    
public:
    /// Updates the object
    void Set(double pt, double phi) noexcept;
};
