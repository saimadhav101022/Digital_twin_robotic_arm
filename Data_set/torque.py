import numpy as np
import pandas as pd

# ==========================================
# NUMBER OF SAMPLES
# ==========================================

num_samples = 100000

# ==========================================
# LINK LENGTHS (meters)
# ==========================================

L1 = 0.02
L2 = 0.062
L3 = 0.063
L4 = 0.067

g = 9.81

data = []

for _ in range(num_samples):

    # Random Joint Angles
    J1 = np.random.uniform(0, 180)
    J2 = np.random.uniform(20, 160)
    J3 = np.random.uniform(20, 160)
    J4 = np.random.uniform(0, 180)

    # Random Payload (grams)
    payload_g = np.random.uniform(5, 70)

    # Convert grams to kg
    payload_kg = payload_g / 1000.0

    # Effective Reach
    t2 = np.radians(J2)
    t3 = np.radians(J3)
    t4 = np.radians(J4)

    effective_length = abs(
        L2*np.cos(t2) +
        L3*np.cos(t2+t3) +
        L4*np.cos(t2+t3+t4)
    )

    # End Effector Position
    X = effective_length * np.cos(np.radians(J1))
    Y = effective_length * np.sin(np.radians(J1))

    Z = (
        L1 +
        L2*np.sin(t2) +
        L3*np.sin(t2+t3) +
        L4*np.sin(t2+t3+t4)
    )

    # ==========================================
    # TORQUE CALCULATIONS
    # ==========================================

    torque_j4 = payload_kg * g * L4

    torque_j3 = payload_kg * g * (L3 + L4)

    torque_j2 = payload_kg * g * (L2 + L3 + L4)

    torque_j1 = 0

    data.append([
        J1,
        J2,
        J3,
        J4,
        X,
        Y,
        Z,
        payload_g,
        torque_j1,
        torque_j2,
        torque_j3,
        torque_j4
    ])

columns = [
    "J1",
    "J2",
    "J3",
    "J4",
    "X",
    "Y",
    "Z",
    "Payload_g",
    "Torque_J1_Nm",
    "Torque_J2_Nm",
    "Torque_J3_Nm",
    "Torque_J4_Nm"
]

df = pd.DataFrame(data, columns=columns)

df.to_csv("robotic_arm_torque_dataset.csv", index=False)

print(df.head())
print("\nDataset Saved Successfully!")