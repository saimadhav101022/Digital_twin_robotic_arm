import pandas as pd
import joblib

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.callbacks import EarlyStopping

# ==========================================
# LOAD DATASET
# ==========================================

df = pd.read_csv("robotic_arm_torque_dataset.csv")

print(df.head())

# ==========================================
# INPUTS
# ==========================================

X = df[['J1','J2','J3','J4','X','Y','Z','Payload_g']]

# OUTPUTS
y = df[['Torque_J2_Nm',
        'Torque_J3_Nm',
        'Torque_J4_Nm']]

# ==========================================
# SCALE INPUTS
# ==========================================

x_scaler = StandardScaler()
X_scaled = x_scaler.fit_transform(X)

joblib.dump(x_scaler, "torque_x_scaler.pkl")

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
    Dense(3)
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

model.save("torque_model.keras")

print("\nTorque Model Saved Successfully!")