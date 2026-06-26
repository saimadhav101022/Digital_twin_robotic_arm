import numpy as np
import joblib
from tensorflow.keras.models import load_model

# Load model
model = load_model("torque_model.keras")

# Load scaler
x_scaler = joblib.load("torque_x_scaler.pkl")

# ==========================
# USER INPUTS
# ==========================

J1 = float(input("Enter J1: "))
J2 = float(input("Enter J2: "))
J3 = float(input("Enter J3: "))
J4 = float(input("Enter J4: "))

X = float(input("Enter X (m): "))
Y = float(input("Enter Y (m): "))
Z = float(input("Enter Z (m): "))

Payload = float(input("Enter Payload (g): "))

# Create input array
sample = np.array([[J1, J2, J3, J4,
                    X, Y, Z,
                    Payload]])

# Scale
sample_scaled = x_scaler.transform(sample)

# Predict
prediction = model.predict(sample_scaled, verbose=0)

print("\n========== RESULTS ==========")
print("Torque J2 (Shoulder) =", prediction[0][0], "Nm")
print("Torque J3 (Elbow)    =", prediction[0][1], "Nm")
print("Torque J4 (Wrist)    =", prediction[0][2], "Nm")