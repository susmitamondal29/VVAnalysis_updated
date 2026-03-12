#include <iostream>
#include <cmath>
#include "TLorentzVector.h"

int main() {
    // Input values from your data
    double lepton1Pt = 57.4077, lepton1Eta = -0.358898, lepton1Phi = -2.97834, lepton1Mass = 0.00995027;
    double Z1Pt = 74.8886, Z1Eta = 0.524061, Z1Phi = -2.94723, Z1Mass = 92.2344;
    double Z2Pt = 28.6794, Z2Eta = 0.290343, Z2Phi = 0.570786, Z2Mass = 63.4505;

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

    return 0;
}

