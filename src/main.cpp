/*
=====================================================
LESSON 4 - Line Sensors
=====================================================
PURPOSE:
Test and understand the five line sensors.
Learn calibration and position detection.
BUTTONS:
A (Left) → Show raw sensor values (uncalibrated)
B (Center) → Run calibration routine
C (Right) → Show calibrated values and position
SENSOR VALUES:
Raw: 0-2000+ (varies by environment)
Calibrated: 0-1000 (0=light, 1000=dark)
Position: 0-4000 (0=far left, 2000=center, 4000=far right)
HARDWARE:
- Zumo 32U4 line sensors, motors, OLED, buttons
AUTHOR: [Your Name]
DATE: [Today's Date]
=====================================================
*/
#include <Zumo32U4.h>
// ===== HARDWARE OBJECTS =====
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4OLED display;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
// ===== CONFIGURATION =====
const int CALIBRATION_SPEED = 100; // Motor speed during auto-calibration
const int CALIBRATION_TIME = 5000; // Total calibration duration (ms)
// ===== STATE VARIABLES =====
unsigned int sensorValues[5]; // Array to hold sensor readings
int displayMode = 0; // 0 = raw, 1 = calibrated
bool isCalibrated = false; // Has calibration been done?
void showRawValues() {
// Read raw values
lineSensors.read(sensorValues);
display.clear();
display.setLayout21x8();
display.gotoXY(0, 0);
display.print(F("=== RAW VALUES ==="));
// Show each sensor value
display.gotoXY(0, 2);
display.print(F("0:"));
display.print(sensorValues[0]);
display.gotoXY(10, 2);
display.print(F("1:"));
display.print(sensorValues[1]);
// ... (continues for all 5 sensors)
}
void showCalibratedValues() {
if (!isCalibrated) {
display.clear();
display.print(F("NOT CALIBRATED!"));
return;
}
// Read calibrated values and get position
int position = lineSensors.readLine(sensorValues);
display.clear();
display.setLayout21x8();
display.gotoXY(0, 0);
display.print(F("=== CALIBRATED ==="));
// Show position
display.gotoXY(0, 4);
display.print(F("Position: "));
display.print(position);
}
void runCalibration() {
display.clear();
display.print(F("Place robot on line boundary."));
display.print(F("Press B to start."));
while (!buttonB.getSingleDebouncedPress()) {
delay(10);
}
// Spin and calibrate
unsigned long startTime = millis();
bool spinRight = true;
while (millis() - startTime < CALIBRATION_TIME) {
if (spinRight) {
motors.setSpeeds(CALIBRATION_SPEED, -CALIBRATION_SPEED);
} else {
motors.setSpeeds(-CALIBRATION_SPEED, CALIBRATION_SPEED);
}
lineSensors.calibrate();
// Switch direction every 1500ms for wider sweep
if ((millis() - startTime) % 3000 < 1500) {
spinRight = true;
} else {
spinRight = false;
}
}
motors.setSpeeds(0, 0);
isCalibrated = true;
}
void setup() {
Serial.begin(115200);
delay(500);
Serial.println(F("LESSON 4 - LINE SENSORS"));
// Initialize line sensors
lineSensors.initFiveSensors();
Serial.println(F("Line sensors initialized."));
// Show welcome
display.clear();
display.setLayout21x8();
display.print(F("LESSON 4"));
display.print(F("Line Sensors"));
}
void loop() {
// Handle button presses
if (buttonA.getSingleDebouncedPress()) {
displayMode = 0;
Serial.println(F("[MODE] Raw values"));
}
if (buttonB.getSingleDebouncedPress()) {
runCalibration();
displayMode = 1;
}
if (buttonC.getSingleDebouncedPress()) {
displayMode = 1;
Serial.println(F("[MODE] Calibrated values"));
}
// Update display based on mode
if (displayMode == 0) {
showRawValues();
} else {
showCalibratedValues();
}
delay(100);
}
