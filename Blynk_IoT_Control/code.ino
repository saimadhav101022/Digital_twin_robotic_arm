#define BLYNK_TEMPLATE_ID "TMPL30qlIyd6x"
#define BLYNK_TEMPLATE_NAME "robotic Arm"
#define BLYNK_AUTH_TOKEN "FUbLSyN5AdXA04vv0WvKMaS2ahq6YF7C"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// =====================================================
// WIFI DETAILS
// =====================================================

char ssid[] = "vivo t2 5g";
char pass[] = "11111111";

// =====================================================
// SERVO OBJECTS
// =====================================================

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripServo;

// =====================================================
// SAFE SERVO PINS
// =====================================================

const int basePin      = D1;
const int shoulderPin  = D2;
const int elbowPin     = D5;
const int gripPin      = D6;

// =====================================================
// SERVO POSITIONS
// =====================================================

int basePos = 90;
int shoulderPos = 90;
int elbowPos = 90;
int gripPos = 120;

// =====================================================
// SMOOTH SERVO MOVEMENT
// =====================================================

void slowMove(Servo &servo, int &currentPos, int targetPos, int speedDelay)
{
  if (currentPos < targetPos)
  {
    for (int pos = currentPos; pos <= targetPos; pos++)
    {
      servo.write(pos);

      delay(speedDelay + 5);

      yield();
    }
  }
  else
  {
    for (int pos = currentPos; pos >= targetPos; pos--)
    {
      servo.write(pos);

      delay(speedDelay + 5);

      yield();
    }
  }

  currentPos = targetPos;
}

// =====================================================
// BLYNK SLIDER CONTROLS
// =====================================================

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  slowMove(baseServo, basePos, value, 15);
}

BLYNK_WRITE(V1)
{
  int value = param.asInt();

  slowMove(shoulderServo, shoulderPos, value, 15);
}

BLYNK_WRITE(V2)
{
  int value = param.asInt();

  slowMove(elbowServo, elbowPos, value, 15);
}

BLYNK_WRITE(V3)
{
  int value = param.asInt();

  slowMove(gripServo, gripPos, value, 15);
}

// =====================================================
// AUTO PICK & PLACE BUTTON
// =====================================================

BLYNK_WRITE(V4)
{
  int buttonState = param.asInt();

  if(buttonState == 1)
  {
    // Open Gripper
    slowMove(gripServo, gripPos, 140, 20);

    delay(500);

    // Bend Forward
    slowMove(shoulderServo, shoulderPos, 150, 20);

    slowMove(elbowServo, elbowPos, 125, 20);

    delay(500);

    // Close Gripper
    slowMove(gripServo, gripPos, 60, 20);

    delay(800);

    // Lift Object
    slowMove(elbowServo, elbowPos, 90, 20);

    slowMove(shoulderServo, shoulderPos, 90, 20);

    delay(500);

    // Rotate Base
    slowMove(baseServo, basePos, 150, 20);

    delay(500);

    // Bend Again
    slowMove(shoulderServo, shoulderPos, 150, 20);

    slowMove(elbowServo, elbowPos, 125, 20);

    delay(500);

    // Release Object
    slowMove(gripServo, gripPos, 140, 20);

    delay(800);

    // Return Back
    slowMove(elbowServo, elbowPos, 90, 20);

    slowMove(shoulderServo, shoulderPos, 90, 20);

    delay(500);

    // Home Position
    slowMove(baseServo, basePos, 90, 20);
  }
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // CONNECT TO BLYNK
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // ATTACH SERVOS
  baseServo.attach(basePin);

  shoulderServo.attach(shoulderPin);

  elbowServo.attach(elbowPin);

  gripServo.attach(gripPin);

  // INITIAL POSITIONS
  baseServo.write(basePos);

  shoulderServo.write(shoulderPos);

  elbowServo.write(elbowPos);

  gripServo.write(gripPos);

  delay(1000);

  Serial.println("Blynk Connected");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  Blynk.run();

  yield();
}
