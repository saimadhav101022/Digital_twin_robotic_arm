#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// =====================================================
// WiFi Details
// =====================================================
const char* ssid = "ESP8266_Robot";
const char* password = "12345678";

ESP8266WebServer server(80);

// =====================================================
// Servo Objects
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
// Servo Positions
// =====================================================
int basePos = 90;
int shoulderPos = 90;
int elbowPos = 90;
int gripPos = 120;

// =====================================================
// Smooth Servo Movement (LOW POWER)
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
// HTML PAGE
// =====================================================
String webpage = R"=====(

<!DOCTYPE html>
<html>

<head>

<title>ESP8266 Robot Arm</title>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
    text-align:center;
    font-family:Arial;
    background:#f2f2f2;
}

.container{
    background:white;
    padding:20px;
    border-radius:10px;
    width:90%;
    margin:auto;
}

.slider{
    width:90%;
}

button{
    padding:15px;
    width:80%;
    background:green;
    color:white;
    border:none;
    border-radius:10px;
    font-size:18px;
}

</style>

</head>

<body>

<div class="container">

<h2>ESP8266 Robot Arm</h2>

<p>Base</p>
<input type="range"
min="0"
max="180"
value="90"
class="slider"
oninput="sendData('base',this.value)">

<p>Shoulder</p>
<input type="range"
min="0"
max="180"
value="90"
class="slider"
oninput="sendData('shoulder',this.value)">

<p>Elbow</p>
<input type="range"
min="0"
max="180"
value="90"
class="slider"
oninput="sendData('elbow',this.value)">

<p>Gripper</p>
<input type="range"
min="0"
max="180"
value="120"
class="slider"
oninput="sendData('grip',this.value)">

<br><br>

<button onclick="autoPickPlace()">
AUTO PICK & PLACE
</button>

</div>

<script>

function sendData(servo,value)
{
    var xhttp = new XMLHttpRequest();

    xhttp.open("GET","/"+servo+"?value="+value,true);

    xhttp.send();
}

function autoPickPlace()
{
    var xhttp = new XMLHttpRequest();

    xhttp.open("GET","/auto",true);

    xhttp.send();
}

</script>

</body>
</html>

)=====";

// =====================================================
// HOMEPAGE
// =====================================================
void handleRoot()
{
  server.send(200, "text/html", webpage);
}

// =====================================================
// MANUAL CONTROLS
// =====================================================
void handleBase()
{
  int value = server.arg("value").toInt();

  slowMove(baseServo, basePos, value, 15);

  server.send(200, "text/plain", "OK");
}

void handleShoulder()
{
  int value = server.arg("value").toInt();

  slowMove(shoulderServo, shoulderPos, value, 15);

  server.send(200, "text/plain", "OK");
}

void handleElbow()
{
  int value = server.arg("value").toInt();

  slowMove(elbowServo, elbowPos, value, 15);

  server.send(200, "text/plain", "OK");
}

void handleGrip()
{
  int value = server.arg("value").toInt();

  slowMove(gripServo, gripPos, value, 15);

  server.send(200, "text/plain", "OK");
}

// =====================================================
// AUTO PICK & PLACE
// =====================================================
void handleAuto()
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

  server.send(200, "text/plain", "AUTO DONE");
}

// =====================================================
// SETUP
// =====================================================
void setup()
{
  Serial.begin(115200);

  // Attach Servos
  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  gripServo.attach(gripPin);

  // Initial Positions
  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);
  elbowServo.write(elbowPos);
  gripServo.write(gripPos);

  delay(1000);

  // Start WiFi Access Point
  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("WiFi Started");

  Serial.print("IP Address: ");

  Serial.println(WiFi.softAPIP());

  // Web Routes
  server.on("/", handleRoot);

  server.on("/base", handleBase);
  server.on("/shoulder", handleShoulder);
  server.on("/elbow", handleElbow);
  server.on("/grip", handleGrip);

  server.on("/auto", handleAuto);

  server.begin();

  Serial.println("Server Started");
}

// =====================================================
// LOOP
// =====================================================
void loop()
{
  server.handleClient();

  yield();
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
