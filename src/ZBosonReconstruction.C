#include "TLorentzVector.h"
#include <iostream>

void ZBosonReconstruction() {
    // Define lepton 1 properties
    double pt1 = 45.0;     // GeV
    double eta1 = 0.5;
    double phi1 = 1.0;     // radians
    double mass1 = 0.000511; // Electron mass in GeV (for example)

    // Define lepton 2 properties
    double pt2 = 40.0;     // GeV
    double eta2 = -0.6;
    double phi2 = -1.2;    // radians
    double mass2 = 0.000511; // Electron mass in GeV

    // Create TLorentzVectors for leptons
    TLorentzVector lepton1, lepton2;
    lepton1.SetPtEtaPhiM(pt1, eta1, phi1, mass1);
    lepton2.SetPtEtaPhiM(pt2, eta2, phi2, mass2);

    // Reconstruct Z boson as the sum of the two leptons
    TLorentzVector zBoson = lepton1 + lepton2;

    // Extract and print Z boson properties
    std::cout << "Z Boson Candidate Properties:" << std::endl;
    std::cout << "Pt   = " << zBoson.Pt()   << " GeV" << std::endl;
    std::cout << "Eta  = " << zBoson.Eta()  << std::endl;
    std::cout << "Phi  = " << zBoson.Phi()  << " rad" << std::endl;
    std::cout << "Mass = " << zBoson.M()    << " GeV" << std::endl;
}
