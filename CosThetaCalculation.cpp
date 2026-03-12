#include <iostream>
#include <cmath>
#include <TLorentzVector.h>

// Function to calculate Lorentz vector components from pt, eta, phi, and mass
TLorentzVector createLorentzVector(double pt, double eta, double phi, double mass) {
    // Calculate the components of the four-vector
    double px = pt * cos(phi);
    double py = pt * sin(phi);
    double pz = pt * sinh(eta);
    double energy = sqrt(px * px + py * py + pz * pz + mass * mass);

    // Return the four-vector
    return TLorentzVector(px, py, pz, energy);
}

// Function to calculate and print CosTheta
void CosThetaCalculation() {
    // Given values for lepton1, Z1, Z2
    double lepton1Pt = 57.4077;
    double lepton1Eta = -0.358898;
    double lepton1Phi = -2.97834;
    double lepton1Mass = 0.00995027;

    double Z1Pt = 74.8886;
    double Z1Eta = 0.524061;
    double Z1Phi = -2.94723;
    double Z1Mass = 92.2344;

    double Z2Pt = 28.6794;
    double Z2Eta = 0.290343;
    double Z2Phi = 0.570786;
    double Z2Mass = 63.4505;

    // Create Lorentz Vectors for lepton1, Z1, and Z2
    TLorentzVector l1P4 = createLorentzVector(lepton1Pt, lepton1Eta, lepton1Phi, lepton1Mass);
    TLorentzVector z1P4 = createLorentzVector(Z1Pt, Z1Eta, Z1Phi, Z1Mass);
    TLorentzVector z2P4 = createLorentzVector(Z2Pt, Z2Eta, Z2Phi, Z2Mass);

    // ZZ system (z1 + z2)
    TLorentzVector zzP4 = z1P4 + z2P4;

    // Boost lepton1 to Z1 rest frame
    TLorentzVector l1P4_boosted = l1P4;
    l1P4_boosted.Boost(-z1P4.BoostVector()); // Boost lepton1 to the rest frame of Z1

    // Boost z1 to ZZ rest frame
    TLorentzVector z1P4_boosted = z1P4;
    z1P4_boosted.Boost(-zzP4.BoostVector()); // Boost Z1 to the rest frame of ZZ

    // Calculate CosTheta: Cosine of the angle between the two vectors
    double ct = l1P4_boosted.Vect().Dot(z1P4_boosted.Vect()) / (l1P4_boosted.Vect().Mag() * z1P4_boosted.Vect().Mag());

    // Output the result
    std::cout << "CosTheta: " << ct << std::endl;
}

int main() {
    // Call the function to calculate CosTheta
    CosThetaCalculation();
    return 0;
}

