#include <iostream>
#include <cmath>
#include "TLorentzVector.h"

void CosThetaCalculation() {
    // Input values from your data
    double lepton1Pt = 28.1061, lepton1Eta = -1.35736, lepton1Phi= -1.18865, lepton1Mass = 0.0235788;
    double Z1Pt =23.1725, Z1Eta = -1.00877, Z1Phi = 0.285264, Z1Mass = 93.3671;
    double Z2Pt = 24.5963, Z2Eta = 2.82625, Z2Phi = -2.98964, Z2Mass = 117.539;

    // Create TLorentzVectors for lepton1, Z1, and Z2 using pt, eta, phi, and mass
    TLorentzVector lepton1P4, Z1P4, Z2P4;

    // Set the 4-vectors using pt, eta, phi, and mass for each particle
    lepton1P4.SetPtEtaPhiM(lepton1Pt, lepton1Eta, lepton1Phi, lepton1Mass);
    Z1P4.SetPtEtaPhiM(Z1Pt, Z1Eta, Z1Phi, Z1Mass);
    Z2P4.SetPtEtaPhiM(Z2Pt, Z2Eta, Z2Phi, Z2Mass);

    // Boost Z1 and Z2 into the ZZ rest frame
    TLorentzVector zzP4 = Z1P4 + Z2P4; // The combined ZZ system (Z1 + Z2)
    lepton1P4.Boost(-Z1P4.BoostVector());  // Boost lepton1 into Z1 rest frame
    Z1P4.Boost(-zzP4.BoostVector());  // Boost Z1 into ZZ rest frame

    // Now calculate CosTheta
    // Compute the 3-momentum vectors of lepton1 and Z1 in the boosted frames
    double dotProduct = lepton1P4.Vect().Dot(Z1P4.Vect());
    double magLepton1 = lepton1P4.Vect().Mag();
    double magZ1 = Z1P4.Vect().Mag();

    // Cosine of the angle between the two 3-momentum vectors
    double cosTheta = dotProduct / (magLepton1 * magZ1);
    
    // Output the result
    std::cout << "CosTheta: " << cosTheta << std::endl;
}

