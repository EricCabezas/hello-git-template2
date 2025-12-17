/*
=====================================================
LESSON 6 - ENCODER DEMONSTRATION
=====================================================

PURPOSE:
Learn to measure distance and turns using wheel encoders.
Drive precise distances and make accurate turns.

HARDWARE:
- Zumo 32U4 encoders (built-in), motors, OLED, buttons

AUTHOR: Your Name
DATE:   2025-12-16
=====================================================
*/

#include <Zumo32U4.h>

// ===== HARDWARE OBJECTS =====
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;
Zumo32U4OLED display;
Zumo32U4ButtonA buttonA;

// ===== CONFIGURATION =====

// ----- Distance Measurement -----
const float COUNTS_PER_ROTATION = 909.7;
const float WHEEL_DIAMETER_MM = 39.0;
const float WHEEL_CIRCUMFERENCE_MM = 3.14159 * WHEEL_DIAMETER_MM;
const float COUNTS_PER_CM =
    COUNTS_PER_ROTATION / (WHEEL_CIRCUMFERENCE_MM / 10.0);

// ----- Motor Control -----
const int DRIVE_SPEED = 150;

// ----- Turning -----
const float WHEEL_BASE_MM = 85.0;
const float TURN_CIRCUMFERENCE_MM = 3.14159 * WHEEL_BASE_MM;
const float COUNTS_PER_DEGREE =
    (TURN_CIRCUMFERENCE_MM / 360.0) *
    (COUNTS_PER_ROTATION / WHEEL_CIRCUMFERENCE_MM);

// ===== FORWARD DECLARATIONS =====
void displayEncoderCounts();
void driveDistance(float distanceCm);
void turnDegrees(float degrees);

// ===== MAIN PROGRAM =====
void setup() {
    display.setLayout21x8();
    display.clear();
    display.print(F("ENCODER DEMO"));
    display.gotoXY(0, 2);
    display.print(F("Press A to start"));
    display.gotoXY(0, 4);
    display.print(F("Cts/cm: "));
    display.print(COUNTS_PER_CM, 1);

    buttonA.waitForButton();

    for (int i = 3; i > 0; i--) {
        display.clear();
        display.print(F("Starting: "));
        display.print(i);
        delay(1000);
    }
    display.clear();
    display.print(F("Fwd 40cm"));
    driveDistance(40);
    delay(500);
    
    display.clear();
    display.print(F("Back 20cm"));
    driveDistance(-20);  // How do you go backward?
    delay(500);
    
    display.clear();
    display.print(F("Fwd 40cm"));
    driveDistance(40);
    delay(500);
    
    display.clear();
    display.print(F("Done!"));
    display.gotoXY(0, 2);
    display.print(F("Net: 60cm fwd"));
    while(true) { }
}

// ===== HELPER FUNCTIONS =====

void displayEncoderCounts() {
    int leftCount = encoders.getCountsLeft();
    int rightCount = encoders.getCountsRight();
    float distanceCm =
        ((leftCount + rightCount) / 2.0) / COUNTS_PER_CM;

    display.clear();
    display.print(F("L: "));
    display.print(leftCount);
    display.gotoXY(0, 1);
    display.print(F("R: "));
    display.print(rightCount);
    display.gotoXY(0, 3);
    display.print(F("Distance:"));
    display.gotoXY(0, 4);
    display.print(distanceCm, 1);
    display.print(F(" cm"));
}

void driveDistance(float distanceCm) {
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();

    int targetCounts = abs(distanceCm) * COUNTS_PER_CM;
    int speed = (distanceCm > 0) ? DRIVE_SPEED : -DRIVE_SPEED;

    motors.setSpeeds(speed, speed);

    while (abs(encoders.getCountsLeft()) < targetCounts) {
        delay(10);
    }

    motors.setSpeeds(0, 0);
}

void turnDegrees(float degrees) {
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();

    int targetCounts = abs(degrees) * COUNTS_PER_DEGREE;

    int leftSpeed = (degrees > 0) ? DRIVE_SPEED : -DRIVE_SPEED;
    int rightSpeed = -leftSpeed;

    motors.setSpeeds(leftSpeed, rightSpeed);

    while (abs(encoders.getCountsLeft()) < targetCounts) {
        delay(10);
    }

    motors.setSpeeds(0, 0);
}
