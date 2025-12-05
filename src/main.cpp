/*
Lesson 1 - Hello, Robot! (PlatformIO)
Goal: Verify that PlatformIO, libraries, and hardware all work.
This program:
1) Starts Serial communication at 115200 baud
2) Shows a startup message on the OLED
3) Waits for you to press Button A
4) Blinks the yellow LED 3 times
5) Nudges the motors forward then backward (gently!)
6) Prints status messages to Serial Monitor
*/
#include <Zumo32U4.h>
// ===== HARDWARE OBJECTS =====
// These objects give us control over physical parts of the robot
Zumo32U4ButtonA buttonA; // Left button on top
Zumo32U4OLED display; // Small screen
Zumo32U4Motors motors; // Left and right drive motors
Zumo32U4Buzzer buzzer; // Small speaker for sounds
// ===== HELPER FUNCTIONS =====
// Wait briefly for Serial Monitor to open (won't wait forever)
void waitForSerial(uint16_t timeout_ms = 1200) {
uint32_t start = millis();
while (!Serial && (millis() - start < timeout_ms)) {
// Just waiting...
}
}
// Nudge motors safely with automatic stop
void nudge(int left, int right, uint16_t ms) {
left = constrain(left, -400, 400); // Keep speeds safe
right = constrain(right, -400, 400);
motors.setSpeeds(left, right);
delay(ms);
motors.setSpeeds(0, 0); // Always stop!
delay(200); // Brief pause between moves
}
void setup() {
// ----- Start Serial communication -----
Serial.begin(115200);
waitForSerial(); // Give Serial Monitor time to open
Serial.println(F("[L1] Booting..."));
// ----- Show startup screen on OLED -----
display.clear();
display.setLayout21x8(); // Larger font (21 cols × 8
//rows//
display.gotoXY(0, 0); display.print(F("Lesson 1"));
display.gotoXY(0, 2); display.print(F("Hello, Robot!"));
display.gotoXY(0, 5); display.print(F("Press A to start"));
// ----- Wait for Button A press -----
Serial.println(F("[L1] Waiting for Button A..."));
while (!buttonA.getSingleDebouncedPress()) {
delay(10); // Check every 10ms
}
// ----- Audio & visual feedback -----
Serial.println(F("[L1] Button A pressed!"));
buzzer.playFrequency(440, 200, 15); // Beep! (A4 note)
delay(250);
for (int i = 0; i < 5; i++) {
ledYellow(1); delay(200);
ledYellow(0); delay(200);
}
// ----- Update OLED -----
display.clear();
display.gotoXY(0, 0); display.print(F("Eric's Robot"));
display.gotoXY(0, 2); display.print(F("Watch & listen"));
// ----- Test motors with gentle nudges -----
Serial.println(F("[L1] Motors: forward nudge"));
buzzer.playFrequency(523, 100, 15); // C5 - going forward
nudge(200, 200, 350); // Forward 0.35 seconds
Serial.println(F("[L1] Motors: backward nudge"));
buzzer.playFrequency(392, 100, 15); // G4 - going backward
nudge(-200, -200, 350); // Backward 0.35 seconds
// ----- All done! -----
Serial.println(F("[L1] Test complete. Hello, Robot!"));
buzzer.playFrequency(523, 100, 15); // Victory sound: C5
delay(150);
buzzer.playFrequency(659, 100, 15); // E5
delay(150);
buzzer.playFrequency(784, 300, 15); // G5
display.gotoXY(0, 5); display.print(F("Done!"));
display.gotoXY(0, 6); display.print(F("Open Serial @"));
display.gotoXY(0, 7); display.print(F("115200 for logs"));
}
void loop() {}
// Nothing here for Lesson 1
// Later lessons will use loop() for continuous sensor reading
