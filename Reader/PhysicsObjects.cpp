#include <PhysicsObjects.hpp>


Candidate::Candidate() noexcept:
    p4()
{}


Candidate::Candidate(TLorentzVector const &p4_) noexcept:
    p4(p4_)
{}


Candidate::Candidate(double pt, double eta, double phi, double mass /*= 0.*/) noexcept
{
    p4.SetPtEtaPhiM(pt, eta, phi, mass);
}


TLorentzVector const &Candidate::P4() const noexcept
{
    return p4;
}


double Candidate::Pt() const noexcept
{
    return p4.Pt();
}


double Candidate::Eta() const noexcept
{
    return p4.Eta();
}


double Candidate::Phi() const noexcept
{
    return p4.Phi();
}


double Candidate::M() const noexcept
{
    return p4.M();
}


double Candidate::DeltaR(Candidate const &rhs) const noexcept
{
    return p4.DeltaR(rhs.p4);
}


bool Candidate::operator<(Candidate const &rhs) const noexcept
{
    return (p4.Pt() < rhs.p4.Pt());
}



Lepton::Lepton() noexcept:
    Candidate(),
    flavour(0), isolation(0.)
{}


Lepton::Lepton(int flavour_, double pt, double eta, double phi, double isolation_) noexcept:
    flavour(flavour_), isolation(isolation_)
{
    // Deduce the mass
    double mass = 0.;
    
    switch (abs(flavour))
    {
        case 11:  // electron
            mass = 0.511e-3;
            break;
        
        case 13:  // muon
            mass = 105.7e-3;
            break;
        
        case 15:  // tau-lepton
            mass = 1776.8e-3;
            break;
    }
    
    
    // Set the four-momentum
    Candidate::p4.SetPtEtaPhiM(pt, eta, phi, mass);
}


int Lepton::Flavour() const noexcept
{
    return flavour;
}


double Lepton::Isolation() const noexcept
{
    return isolation;
}



Jet::Jet() noexcept:
    Candidate(),
    flavour(0), bTag(0.)
{}


Jet::Jet(double pt, double eta, double phi, double bTag_, int flavour_ /* = 0*/) noexcept:
    Candidate(pt, eta, phi, 0.),
    flavour(flavour_), bTag(bTag_)
{}


int Jet::Flavour() const noexcept
{
    return flavour;
}


double Jet::BTag() const noexcept
{
    return bTag;
}



MET::MET() noexcept:
    Candidate()
{}


MET::MET(double pt, double phi) noexcept:
    Candidate(pt, 0., phi, 0.)
{}


void MET::Set(double pt, double phi) noexcept
{
    Candidate::p4.SetPtEtaPhiM(pt, 0., phi, 0.);
}
