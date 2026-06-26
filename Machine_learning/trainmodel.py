import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.callbacks import EarlyStopping
import joblib

# ==========================================
# LOAD DATASET
# ==========================================

df = pd.read_csv("robotic_arm_payload_dataset.csv")

print(df.head())

# ==========================================
# INPUTS AND OUTPUT
# ==========================================

X = df[['J1','J2','J3','J4','X','Y','Z','Effective_Length']]
y = df['Payload_g']

# ==========================================
# SCALING
# ==========================================

x_scaler = StandardScaler()
X_scaled = x_scaler.fit_transform(X)

joblib.dump(x_scaler, "x_scaler.pkl")

# ==========================================
# TRAIN TEST SPLIT
# ==========================================

X_train, X_test, y_train, y_test = train_test_split(
    X_scaled,
    y,
    test_size=0.2,
    random_state=42
)

# ==========================================
# MODEL
# ==========================================

model = Sequential([
    Dense(128, activation='relu', input_shape=(8,)),
    Dense(128, activation='relu'),
    Dense(64, activation='relu'),
    Dense(32, activation='relu'),
    Dense(1)
])

model.compile(
    optimizer='adam',
    loss='mse',
    metrics=['mae']
)

# ==========================================
# EARLY STOPPING
# ==========================================

early_stop = EarlyStopping(
    monitor='val_loss',
    patience=10,
    restore_best_weights=True
)

# ==========================================
# TRAIN
# ==========================================

history = model.fit(
    X_train,
    y_train,
    validation_split=0.2,
    epochs=100,
    batch_size=256,
    callbacks=[early_stop],
    verbose=1
)

# ==========================================
# EVALUATE
# ==========================================

loss, mae = model.evaluate(X_test, y_test)

print("\nTest MAE =", mae)

# ==========================================
# SAVE MODEL
# ==========================================

model.save("payload_model.h5")

print("\nModel Saved Successfully!")