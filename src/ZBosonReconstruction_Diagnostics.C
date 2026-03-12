#include "TLorentzVector.h"
#include <iostream>

void ZBosonReconstruction_Diagnostics() {
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

    // Print out the individual lepton 4-vectors to verify
    std::cout << "Lepton 1 4-vector: " << std::endl;
    std::cout << "Pt   = " << lepton1.Pt()   << " GeV" << std::endl;
    std::cout << "Eta  = " << lepton1.Eta()  << std::endl;
    std::cout << "Phi  = " << lepton1.Phi()  << " rad" << std::endl;
    std::cout << "Mass = " << lepton1.M()    << " GeV" << std::endl;

    std::cout << std::endl;

    std::cout << "Lepton 2 4-vector: " << std::endl;
    std::cout << "Pt   = " << lepton2.Pt()   << " GeV" << std::endl;
    std::cout << "Eta  = " << lepton2.Eta()  << std::endl;
    std::cout << "Phi  = " << lepton2.Phi()  << " rad" << std::endl;
    std::cout << "Mass = " << lepton2.M()    << " GeV" << std::endl;

    std::cout << std::endl;

    // Reconstruct Z boson as the sum of the two leptons
    TLorentzVector zBoson = lepton1 + lepton2;
    std::cout << "Pt_test   = " << (lepton1+lepton2).Pt()   << " GeV" << std::endl;
    std::cout << "Eta_test  = " <<  (lepton1+lepton2).Eta()  << std::endl;
    std::cout << "Phi_test  = " <<  (lepton1+lepton2).Phi()  << " rad" << std::endl;
    std::cout << "Mass_test = " << (lepton1+lepton2).M()    << " GeV" << std::endl;
    // Print out the Z boson 4-vector to verify
    std::cout << "Z Boson Candidate 4-vector: " << std::endl;
    std::cout << "Pt   = " << zBoson.Pt()   << " GeV" << std::endl;
    std::cout << "Eta  = " << zBoson.Eta()  << std::endl;
    std::cout << "Phi  = " << zBoson.Phi()  << " rad" << std::endl;
    std::cout << "Mass = " << zBoson.M()    << " GeV" << std::endl;
std::cout << "Lepton 1 Longitudinal Momentum (Pz) = " << lepton1.Pz() << " GeV" << std::endl;
std::cout << "Lepton 2 Longitudinal Momentum (Pz) = " << lepton2.Pz() << " GeV" << std::endl;
std::cout << "Z Boson Longitudinal Momentum (Pz) = " << zBoson.Pz() << " GeV" << std::endl;

 std::cout << "Lepton 1 Energy = " << lepton1.E() << " GeV" << std::endl;
std::cout << "Lepton 2 Energy = " << lepton2.E() << " GeV" << std::endl;
std::cout << "Z Boson Energy = " << zBoson.E() << " GeV" << std::endl;

}
