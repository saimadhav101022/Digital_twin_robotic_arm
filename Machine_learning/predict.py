import numpy as np
import joblib
from tensorflow.keras.models import load_model

# Load model
model = load_model("payload_model.h5", compile=False)

# Load scaler
x_scaler = joblib.load("x_scaler.pkl")

# Example input
sample = np.array([[92.2697969, 76.21597794, 66.36239924,
                    61.15362043, -0.003825681, 0.09651995,
                    0.091533252, 0.096595738]])
# Scale input
sample_scaled = x_scaler.transform(sample)

# Predict
prediction = model.predict(sample_scaled)

print("Predicted Payload =", prediction[0][0], "grams")