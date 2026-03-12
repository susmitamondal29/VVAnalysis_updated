import numpy as np

# Function to create a Lorentz Vector from pt, eta, phi, and mass
def create_lorentz_vector(pt, eta, phi, mass):
    # Calculate components of the 4-momentum vector
    px = pt * np.cos(phi)
    py = pt * np.sin(phi)
    pz = pt * np.sinh(eta)
    energy = np.sqrt(px**2 + py**2 + pz**2 + mass**2)
    
    # Return as a numpy array for convenience
    return np.array([px, py, pz, energy])

# Function to calculate CosTheta
def cos_theta_calculation():
    # Given values for lepton1, Z1, Z2
    lepton1_pt = 57.4077
    lepton1_eta = -0.358898
    lepton1_phi = -2.97834
    lepton1_mass = 0.00995027

    Z1_pt = 74.8886
    Z1_eta = 0.524061
    Z1_phi = -2.94723
    Z1_mass = 92.2344

    Z2_pt = 28.6794
    Z2_eta = 0.290343
    Z2_phi = 0.570786
    Z2_mass = 63.4505

    # Create Lorentz Vectors for lepton1, Z1, and Z2
    lepton1_p4 = create_lorentz_vector(lepton1_pt, lepton1_eta, lepton1_phi, lepton1_mass)
    Z1_p4 = create_lorentz_vector(Z1_pt, Z1_eta, Z1_phi, Z1_mass)
    Z2_p4 = create_lorentz_vector(Z2_pt, Z2_eta, Z2_phi, Z2_mass)

    # ZZ system (Z1 + Z2)
    ZZ_p4 = Z1_p4 + Z2_p4

    # Boost lepton1 to Z1 rest frame
    boost_vector_z1 = Z1_p4[:3] / np.linalg.norm(Z1_p4[:3])  # 3-vector boost direction for Z1
    lepton1_p4_boosted = lepton1_p4.copy()  # Make a copy to preserve original
    lepton1_p4_boosted[:3] -= np.dot(lepton1_p4[:3], boost_vector_z1) * boost_vector_z1  # Subtract boost from 3-momentum
    lepton1_p4_boosted[3] = np.sqrt(np.sum(lepton1_p4_boosted[:3]**2) + lepton1_mass**2)  # Recalculate energy

    # Boost Z1 to ZZ rest frame
    boost_vector_ZZ = ZZ_p4[:3] / np.linalg.norm(ZZ_p4[:3])  # 3-vector boost direction for ZZ
    Z1_p4_boosted = Z1_p4.copy()  # Make a copy to preserve original
    Z1_p4_boosted[:3] -= np.dot(Z1_p4[:3], boost_vector_ZZ) * boost_vector_ZZ  # Subtract boost from 3-momentum
    Z1_p4_boosted[3] = np.sqrt(np.sum(Z1_p4_boosted[:3]**2) + Z1_mass**2)  # Recalculate energy

    # Calculate CosTheta: Cosine of the angle between the two vectors
    dot_product = np.dot(lepton1_p4_boosted[:3], Z1_p4_boosted[:3])  # Dot product of spatial parts
    norm_product = np.linalg.norm(lepton1_p4_boosted[:3]) * np.linalg.norm(Z1_p4_boosted[:3])  # Magnitudes of spatial parts
    cos_theta = dot_product / norm_product  # Cosine of the angle

    # Output the result
    print("CosTheta: ", cos_theta)

# Call the function to calculate CosTheta
cos_theta_calculation()

