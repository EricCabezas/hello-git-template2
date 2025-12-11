
/*
LESSON 3 - Motors & TRIM
=====================================================
PURPOSE:
Find your robot's TRIM value to make it drive straight.
BUTTONS:
A (Left) → Decrease TRIM by 5
B (Center) → Run motor test (drive forward)
C (Right) → Increase TRIM by 5
Hold A+C together → Reset TRIM to 0
PROCEDURE:
1. Press B to test drive
2. If robot curves LEFT, press C to increase TRIM
3. If robot curves RIGHT, press A to decrease TRIM
4. Repeat until robot drives straight
5. Write down your TRIM value!
HARDWARE:
- Zumo 32U4 with A-Star 32U4 board
- Motors, OLED display, buttons
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
// ===== CONFIGURATION =====
const int BASE_SPEED = 200; // Base motor speed for testing
const int TEST_DURATION = 2000; // How long to run motors (ms)
const int TRIM_STEP = 5; // How much TRIM changes per press
// ===== STATE VARIABLES =====
int trimValue = 0; // Current TRIM (-50 to +50 typical)
int testCount = 0; // How many tests we've run
// ===== HELPER FUNCTIONS =====
/*
updateDisplay()
---------------
Shows current TRIM value, battery voltage, and instructions.
Called whenever settings change.
*/
void updateDisplay() {
display.clear();
display.setLayout21x8();
// Title
display.gotoXY(0, 0);
display.print(F("=== TRIM FINDER ==="));
// Current TRIM value (big and obvious)
display.gotoXY(0, 2);
display.print(F("TRIM: "));
if (trimValue >= 0) display.print(F(" ")); // Align positive
display.print(trimValue);
// Battery voltage
display.gotoXY(0, 4);
display.print(F("Battery: "));
int mv = readBatteryMillivolts();
display.print(mv / 1000); // Whole volts
display.print(F("."));
display.print((mv % 1000) / 100); // Tenths
display.print(F("V"));
// Test count
display.gotoXY(0, 5);
display.print(F("Tests run: "));
display.print(testCount);
// Button hints
display.gotoXY(0, 7);
display.print(F("A:- B:TEST C:+"));
}
/*
runMotorTest()
--------------
Runs motors for TEST_DURATION milliseconds using current TRIM.
Displays countdown on OLED and logs to Serial.
*/
void runMotorTest() {
// Calculate actual motor speeds with TRIM
int leftSpeed = BASE_SPEED;
int rightSpeed = BASE_SPEED + trimValue;
// Keep speeds in valid range
leftSpeed = constrain(leftSpeed, -400, 400);
rightSpeed = constrain(rightSpeed, -400, 400);
// Show what we're about to do
display.clear();
display.setLayout21x8();
display.gotoXY(0, 0); display.print(F("MOTOR TEST"));
display.gotoXY(0, 2); display.print(F("L: "));
display.print(leftSpeed);
display.gotoXY(0, 3); display.print(F("R: "));
display.print(rightSpeed);
display.gotoXY(0, 5); display.print(F("TRIM: "));
display.print(trimValue);
// Log to Serial
Serial.println(F(""));
Serial.println(F("===== MOTOR TEST ====="));
Serial.print(F("Left speed: ")); Serial.println(leftSpeed);
Serial.print(F("Right speed: ")); Serial.println(rightSpeed);
Serial.print(F("TRIM value: ")); Serial.println(trimValue);
Serial.print(F("Duration: ")); Serial.print(TEST_DURATION);
Serial.println(F(" ms"));
// Countdown before starting
display.gotoXY(0, 7); display.print(F("Starting in 3..."));
delay(1000);
display.gotoXY(0, 7); display.print(F("Starting in 2..."));
delay(1000);
display.gotoXY(0, 7); display.print(F("Starting in 1..."));
delay(1000);
// START MOTORS
display.gotoXY(0, 7); display.print(F(">>> RUNNING <<< "));
Serial.println(F("Motors ON"));
motors.setSpeeds(leftSpeed, rightSpeed);
delay(TEST_DURATION);
motors.setSpeeds(0, 0); // STOP!
Serial.println(F("Motors OFF"));
Serial.println(F("======================"));
// Update test counter
testCount++;
// Show results prompt
display.gotoXY(0, 7); display.print(F("Done! L/R curve?"));
delay(1500);
// Return to main display
updateDisplay();
}
/*
blinkConfirm()
--------------
Quick LED blink to confirm button press.
*/
void blinkConfirm() {
ledYellow(1);
delay(50);
ledYellow(0);
}
/*
printTrimAdvice()
-----------------
Prints helpful advice based on current TRIM value.
*/
void printTrimAdvice() {
Serial.print(F("Current TRIM: "));
Serial.println(trimValue);
if (trimValue > 0) {
Serial.println(F(" (Boosting RIGHT motor - use if curves LEFT)"));
} else if (trimValue < 0) {
Serial.println(F(" (Boosting LEFT motor - use if curves RIGHT)"));
} else {
Serial.println(F(" (No correction - motors balanced)"));
}
}
// ===== SETUP =====
void setup() {
Serial.begin(115200);
delay(500);
Serial.println(F(""));
Serial.println(F("========================================"));
Serial.println(F(" LESSON 3 - TRIM FINDER"));
Serial.println(F("========================================"));
Serial.println(F(""));
Serial.println(F("INSTRUCTIONS:"));
Serial.println(F("1. Press B to run a motor test"));
Serial.println(F("2. Watch which way the robot curves"));
Serial.println(F("3. Curves LEFT? Press C to increase TRIM"));
Serial.println(F(" Curves RIGHT? Press A to decrease TRIM"));
Serial.println(F("4. Repeat until robot drives straight"));
Serial.println(F("5. WRITE DOWN your final TRIM value!"));
Serial.println(F(""));
Serial.println(F("Hold A+C together to reset TRIM to 0"));
Serial.println(F(""));
// Show initial battery voltage
int mv = readBatteryMillivolts();
Serial.print(F("Battery: "));
Serial.print(mv);
Serial.print(F(" mV ("));
Serial.print(mv / 1000);
Serial.print(F("."));
Serial.print((mv % 1000) / 100);
Serial.println(F(" V)"));
Serial.println(F(""));
printTrimAdvice();
Serial.println(F(""));
Serial.println(F("Ready! Press B to begin testing..."));
updateDisplay();
}
// ===== LOOP =====
void loop() {
// Check for A+C held together (reset TRIM)
if (buttonA.isPressed() && buttonC.isPressed()) {
trimValue = 0;
blinkConfirm();
delay(100);
blinkConfirm(); // Double blink for reset
Serial.println(F("[RESET] TRIM set to 0"));
printTrimAdvice();
updateDisplay();
delay(500); // Debounce
}
// Button A: Decrease TRIM
else if (buttonA.getSingleDebouncedPress()) {
trimValue -= TRIM_STEP;
trimValue = constrain(trimValue, -50, 50);
blinkConfirm();
Serial.print(F("[BTN A] TRIM decreased to "));
Serial.println(trimValue);
printTrimAdvice();
updateDisplay();
}
// Button B: Run motor test
else if (buttonB.getSingleDebouncedPress()) {
blinkConfirm();
Serial.println(F("[BTN B] Starting motor test..."));
runMotorTest();
}
// Button C: Increase TRIM
else if (buttonC.getSingleDebouncedPress()) {
trimValue += TRIM_STEP;
trimValue = constrain(trimValue, -50, 50);
blinkConfirm();
Serial.print(F("[BTN C] TRIM increased to "));
Serial.println(trimValue);
printTrimAdvice();
updateDisplay();
}
delay(10);
}
