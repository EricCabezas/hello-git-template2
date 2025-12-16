/*
=====================================================
LESSON 5 - Proximity Sensors
=====================================================

PURPOSE:
Test and understand the three proximity sensors.
Display readings and detect objects in front and sides.

BUTTONS:
A (Left)   → Show numeric values mode
B (Center) → Show bar graph mode
C (Right)  → Show detection alerts mode

READINGS:
Values 0-6 (brightness levels) where:
- 0 = nothing detected
- 1-2 = object detected (far)
- 3-4 = object close
- 5-6 = object very close

HARDWARE:
- Zumo 32U4 proximity sensors, OLED, buttons, buzzer

AUTHOR: [Your Name]
DATE:   [Today's Date]
=====================================================
*/

#include <Zumo32U4.h>
// ===== HARDWARE OBJECTS =====
Zumo32U4OLED display;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4Buzzer buzzer;

// ===== CONFIGURATION =====
const int DETECTION_THRESHOLD = 1;    // Minimum to count as "detected"
const int CLOSE_THRESHOLD = 3;        // Considered "close"
const int VERY_CLOSE_THRESHOLD = 5;   // Considered "very close"
// ===== GLOBAL VARIABLES =====
int leftValue = 0;
int frontLeftValue = 0;
int frontRightValue = 0;
int frontValue = 0;
int rightValue = 0;

int displayMode = 0;    // 0 = numeric, 1 = bars, 2 = alerts
void readAllSensors() {
    proxSensors.read();
    
    leftValue = proxSensors.countsLeftWithLeftLeds();
    frontLeftValue = proxSensors.countsFrontWithLeftLeds();
    frontRightValue = proxSensors.countsFrontWithRightLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    frontValue = max(frontLeftValue, frontRightValue);
}
void drawBar(int row, int value) {
    display.gotoXY(2, row);
    int barLength = constrain(value, 0, 6);
    
    for (int i = 0; i < 6; i++) {
        if (i < barLength) {
            display.print(F("#"));
        } else {
            display.print(F("-"));
        }
    }
    
    display.print(F(" "));
    display.print(value);
}
Zumo32U4Motors motors;
// Replace or modify loop() behavior:
void avoidObstacles() {
    if (frontValue >= CLOSE_THRESHOLD) {
        // Stop!
        motors.setSpeeds(0, 0);
        
        // Turn away from closer side
        if (leftValue > rightValue) {
            motors.setSpeeds(100, 0);  // Turn right
        } else {
            motors.setSpeeds(0, 100);  // Turn left
        }
        delay(200);
    }
    else if (frontValue >= DETECTION_THRESHOLD) {
        // Slow down
        motors.setSpeeds(20, 20);
    }
    else {
        // Clear - drive forward
        motors.setSpeeds(100, 100);
    }
}
void showNumericMode() {
    display.clear();
    display.setLayout21x8();
    
    display.gotoXY(0, 0);
    display.print(F("=== PROXIMITY ==="));
    
    display.gotoXY(0, 2);
    display.print(F("Left:  "));
    display.print(leftValue);
    
    display.gotoXY(0, 3);
    display.print(F("Front: "));
    display.print(frontValue);
    display.print(F(" (L"));
    display.print(frontLeftValue);
    display.print(F(" R"));
    display.print(frontRightValue);
    display.print(F(")"));
    
    display.gotoXY(0, 4);
    display.print(F("Right: "));
    display.print(rightValue);
    
    display.gotoXY(0, 6);
    display.print(F("0=none  6=closest"));
    
    display.gotoXY(0, 7);
    display.print(F("A:num B:bar C:alert"));
}
void showBarGraphMode() {
    display.clear();
    display.setLayout21x8();
    
    display.gotoXY(0, 0);
    display.print(F("=== PROX BARS ==="));
    
    display.gotoXY(0, 2);
    display.print(F("L:"));
    drawBar(2, leftValue);
    
    display.gotoXY(0, 4);
    display.print(F("F:"));
    drawBar(4, frontValue);
    
    display.gotoXY(0, 6);
    display.print(F("R:"));
    drawBar(6, rightValue);
    
    display.gotoXY(0, 7);
    display.print(F("A:num B:bar C:alert"));
}
void showAlertMode() {
    display.clear();
    display.setLayout21x8();
    
    display.gotoXY(0, 0);
    display.print(F("=== DETECTION ==="));
    
    // Left status
    display.gotoXY(0, 2);
    display.print(F("LEFT:  "));
    if (leftValue >= VERY_CLOSE_THRESHOLD) {
        display.print(F("[VERY CLOSE!]"));
    } else if (leftValue >= CLOSE_THRESHOLD) {
        display.print(F("[CLOSE]"));
    } else if (leftValue >= DETECTION_THRESHOLD) {
        display.print(F("[detected]"));
    } else {
        display.print(F("clear"));
    }
    
    // Front status
    display.gotoXY(0, 3);
    display.print(F("FRONT: "));
    if (frontValue >= VERY_CLOSE_THRESHOLD) {
        display.print(F("[VERY CLOSE!]"));
    } else if (frontValue >= CLOSE_THRESHOLD) {
        display.print(F("[CLOSE]"));
    } else if (frontValue >= DETECTION_THRESHOLD) {
        display.print(F("[detected]"));
    } else {
        display.print(F("clear"));
    }
    
    // Right status
    display.gotoXY(0, 4);
    display.print(F("RIGHT: "));
    if (rightValue >= VERY_CLOSE_THRESHOLD) {
        display.print(F("[VERY CLOSE!]"));
    } else if (rightValue >= CLOSE_THRESHOLD) {
        display.print(F("[CLOSE]"));
    } else if (rightValue >= DETECTION_THRESHOLD) {
        display.print(F("[detected]"));
    } else {
        display.print(F("clear"));
    }
    
    // Overall warning
    display.gotoXY(0, 6);
    if (frontValue >= VERY_CLOSE_THRESHOLD) {
        display.print(F("!!! OBSTACLE !!!"));
    } else if (frontValue >= CLOSE_THRESHOLD) {
        display.print(F(">> CAUTION <<"));
    } else {
        display.print(F("Path looks clear"));
    }
    
    display.gotoXY(0, 7);
    display.print(F("A:num B:bar C:alert"));
}
void updateIndicators() {
    // LED: On if anything detected in front
    if (frontValue >= DETECTION_THRESHOLD) {
        ledYellow(1);
    } else {
        ledYellow(0);
    }
    
    // Buzzer: Beep pattern based on closeness
    static unsigned long lastBeep = 0;
    int beepInterval;
    
    if (frontValue >= VERY_CLOSE_THRESHOLD) {
        beepInterval = 100;   // Fast beeping
    } else if (frontValue >= CLOSE_THRESHOLD) {
        beepInterval = 300;   // Medium beeping
    } else if (frontValue >= DETECTION_THRESHOLD) {
        beepInterval = 600;   // Slow beeping
    } else {
        beepInterval = 0;     // No beeping
    }
    
    if (beepInterval > 0 && millis() - lastBeep > beepInterval) {
        buzzer.playNote(NOTE_C(5), 30, 15);
        lastBeep = millis();
    }
}
void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println(F(""));
    Serial.println(F("========================================"));
    Serial.println(F("   LESSON 5 - PROXIMITY SENSORS"));
    Serial.println(F("========================================"));
    Serial.println(F(""));
    Serial.println(F("CONTROLS:"));
    Serial.println(F("  A: Numeric value display"));
    Serial.println(F("  B: Bar graph display"));
    Serial.println(F("  C: Detection alerts"));
    Serial.println(F(""));
    Serial.println(F("VALUES: 0-6 (brightness levels)"));
    Serial.println(F("  0 = nothing detected"));
    Serial.println(F("  1-2 = object far"));
    Serial.println(F("  3-4 = object close"));
    Serial.println(F("  5-6 = object very close"));
    Serial.println(F(""));
    
    proxSensors.initThreeSensors();
    Serial.println(F("Proximity sensors initialized."));
    Serial.println(F("Try moving your hand in front of the robot!"));
    
    display.clear();
    display.setLayout21x8();
    display.gotoXY(0, 0);
    display.print(F("LESSON 5"));
    display.gotoXY(0, 1);
    display.print(F("Proximity Sensors"));
    display.gotoXY(0, 3);
    display.print(F("Wave hand in front"));
    display.gotoXY(0, 4);
    display.print(F("to test sensors"));
    display.gotoXY(0, 6);
    display.print(F("Press any button"));
    display.gotoXY(0, 7);
    display.print(F("to begin..."));
    
    while (!buttonA.getSingleDebouncedPress() &&
           !buttonB.getSingleDebouncedPress() &&
           !buttonC.getSingleDebouncedPress()) {
        delay(10);
    }
    
    displayMode = 0;
}
void loop() {
    readAllSensors();
    
    // Button handling
    if (buttonA.getSingleDebouncedPress()) {
        ledYellow(1); delay(50); ledYellow(0);
        displayMode = 0;
        Serial.println(F("[MODE] Numeric"));
    }
    if (buttonB.getSingleDebouncedPress()) {
        ledYellow(1); delay(50); ledYellow(0);
        displayMode = 1;
        Serial.println(F("[MODE] Bar graph"));
    }
    if (buttonC.getSingleDebouncedPress()) {
        ledYellow(1); delay(50); ledYellow(0);
        displayMode = 2;
        Serial.println(F("[MODE] Alerts"));
    }
    
    // Update display
    switch (displayMode) {
        case 0: showNumericMode(); break;
        case 1: showBarGraphMode(); break;
        case 2: showAlertMode(); break;
    }
    
    // Update indicators
    updateIndicators();
    
    // Serial logging
    Serial.print(F("L:"));
    Serial.print(leftValue);
    Serial.print(F("\tF:"));
    Serial.print(frontValue);
    Serial.print(F(" ("));
    Serial.print(frontLeftValue);
    Serial.print(F("/"));
    Serial.print(frontRightValue);
    Serial.print(F(")\tR:"));
    Serial.print(rightValue);
    
    if (frontValue >= VERY_CLOSE_THRESHOLD) {
        Serial.print(F("\t*** VERY CLOSE ***"));
    } else if (frontValue >= CLOSE_THRESHOLD) {
        Serial.print(F("\t* CLOSE *"));
    } else if (frontValue >= DETECTION_THRESHOLD) {
        Serial.print(F("\t(detected)"));
    }
    Serial.println();
    
    delay(50);
    // Add motor object at top:

// Add countdown in setup() before the button wait:
display.clear();
for (int i = 3; i > 0; i--) {
    display.gotoXY(0, 3);
    display.print(F("Starting in "));
    display.print(i);
    delay(50);
}

}