import numpy as np
import pandas as pd

# Number of samples
num_samples = 100000

# Link lengths (meters)
L1 = 0.02
L2 = 0.062
L3 = 0.063
L4 = 0.067

data = []

for _ in range(num_samples):

    # Random joint angles
    J1 = np.random.uniform(0, 180)
    J2 = np.random.uniform(20, 160)
    J3 = np.random.uniform(20, 160)
    J4 = np.random.uniform(0, 180)

    # Convert to radians
    t1 = np.radians(J1)
    t2 = np.radians(J2)
    t3 = np.radians(J3)
    t4 = np.radians(J4)

    # Effective reach
    effective_length = (
        L2*np.cos(t2) +
        L3*np.cos(t2+t3) +
        L4*np.cos(t2+t3+t4)
    )

    effective_length = abs(effective_length)

    # End effector position
    X = effective_length*np.cos(t1)
    Y = effective_length*np.sin(t1)

    Z = (
        L1 +
        L2*np.sin(t2) +
        L3*np.sin(t2+t3) +
        L4*np.sin(t2+t3+t4)
    )

    # Payload estimation
    payload = 70 * (0.08 / (effective_length + 0.02))

    payload = max(5, min(payload, 70))

    data.append([
        J1,
        J2,
        J3,
        J4,
        X,
        Y,
        Z,
        effective_length,
        payload
    ])

columns = [
    "J1",
    "J2",
    "J3",
    "J4",
    "X",
    "Y",
    "Z",
    "Effective_Length",
    "Payload_g"
]

df = pd.DataFrame(data, columns=columns)

df.to_csv("robotic_arm_payload_dataset.csv", index=False)

print(df.head())
print("\nDataset saved as robotic_arm_payload_dataset.csv")