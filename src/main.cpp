/*
=====================================================
LESSON 5 - Proximity Sensors (Complete)
=====================================================

PURPOSE:
Test and understand the three proximity sensors.
Display readings, detect objects, and demonstrate
basic autonomous obstacle avoidance.

BUTTONS:
A (Left)   → Show numeric values mode
B (Center) → Show bar graph mode
C (Right)  → Show detection alerts mode
A (Hold)   → Switch to Autonomous Obstacle Avoidance mode (Mode 3)

READINGS:
Values 0-6 (brightness levels) where:
- 0 = nothing detected
- 1-2 = object detected (far)
- 3-4 = object close
- 5-6 = object very close

HARDWARE:
- Zumo 32U4 proximity sensors, OLED, buttons, buzzer, motors

AUTHOR: [Eric Cabezas]
DATE:   [12/17/2025]
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
Zumo32U4Motors motors; // Integrated here for better organization

// ===== CONFIGURATION =====
const int DETECTION_THRESHOLD = 3;    // Minimum to count as "detected"
const int CLOSE_THRESHOLD = 4;        // Considered "close"
const int VERY_CLOSE_THRESHOLD = 5;   // Considered "very close"

// ===== GLOBAL VARIABLES =====
int leftValue = 0;
int frontLeftValue = 0;
int frontRightValue = 0;
int frontValue = 0;
int rightValue = 0;

// 0 = numeric, 1 = bars, 2 = alerts, 3 = autonomous avoidance
int displayMode = 0;    

// ===== HELPER FUNCTIONS =====

void readAllSensors() {
    proxSensors.read();
    
    // Read sensor counts. Note: The Zumo reads the side sensors using the
    // opposite side's LED (left LED for right sensor and vice versa).
    // The library functions abstract this for clarity.
    leftValue = proxSensors.countsLeftWithLeftLeds();
    frontLeftValue = proxSensors.countsFrontWithLeftLeds();
    frontRightValue = proxSensors.countsFrontWithRightLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    // Use the max of the two front sensors for a conservative detection
    frontValue = max(frontLeftValue, frontRightValue);
}

void drawBar(int row, int value) {
    display.gotoXY(2, row);
    // Constrain value to the 0-6 range, as defined
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

void avoidObstacles() {
    if (frontValue >= CLOSE_THRESHOLD) {
        // --- OBSTACLE IS CLOSE (THRESHOLD >= 5) ---
        // Stop immediately
        motors.setSpeeds(0, 0);
        
        // Turn away from the closer side
        if (leftValue > rightValue) {
            // Left is closer, turn right
            motors.setSpeeds(100, -100);  // Turn in place right
        } else {
            // Right is closer or equal, turn left
            motors.setSpeeds(-100, 100);  // Turn in place left
        }
        delay(200); // Wait briefly during the turn
        motors.setSpeeds(0, 0); // Stop after turn
    }
    else if (frontValue >= DETECTION_THRESHOLD) {
        // --- OBSTACLE IS DETECTED (THRESHOLD >= 1) ---
        // Slow down to prevent running into it
        motors.setSpeeds(100, 100);
    }
    else {
        // --- PATH IS CLEAR ---
        // Drive forward at cruising speed
        motors.setSpeeds(150, 150);
    }
}

// ===== DISPLAY MODE FUNCTIONS =====

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

void showAutonomousMode() {
    display.clear();
    display.setLayout21x8();
    
    display.gotoXY(0, 0);
    display.print(F("=== AUTONOMOUS ==="));
    display.gotoXY(0, 1);
    display.print(F("Obstacle Avoidance"));
    
    // Show current front reading
    display.gotoXY(0, 3);
    display.print(F("Front: "));
    display.print(frontValue);
    drawBar(3, frontValue);
    
    // Show current action
    display.gotoXY(0, 5);
    if (frontValue >= CLOSE_THRESHOLD) {
        display.print(F("Action: EVADING!"));
    } else if (frontValue >= DETECTION_THRESHOLD) {
        display.print(F("Action: Slowing..."));
    } else {
        display.print(F("Action: Forward"));
    }
    
    display.gotoXY(0, 7);
    display.print(F("Press A/B/C to stop"));

    // Crucially, run the avoidance logic
    avoidObstacles();
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

// ===== MAIN PROGRAM =====

void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println(F("========================================"));
    Serial.println(F("   LESSON 5 - PROXIMITY SENSORS"));
    Serial.println(F("========================================"));
    
    proxSensors.initThreeSensors();
    Serial.println(F("Proximity sensors initialized."));
    
    display.clear();
    display.setLayout21x8();
    display.gotoXY(0, 0);
    display.print(F("LESSON 5"));
    display.gotoXY(0, 1);
    display.print(F("Proximity Sensors"));
    display.gotoXY(0, 3);
    display.print(F("Press A/B/C to"));
    display.gotoXY(0, 4);
    display.print(F("select display mode"));
    display.gotoXY(0, 5);
    display.print(F("Hold A for Auto"));
    
    // Wait for any button press to exit the welcome screen
    while (!buttonA.getSingleDebouncedPress() &&
           !buttonB.getSingleDebouncedPress() &&
           !buttonC.getSingleDebouncedPress()) {
        delay(10);
    }
    
    // Check for "Autonomous" mode trigger
    if (buttonA.isPressed()) {
        displayMode = 3; // Start in Autonomous mode
    } else {
        displayMode = 0; // Start in Numeric mode (default A button mode)
    }

    // New: Countdown before starting the main loop
    motors.setSpeeds(0, 0); // Ensure motors are off
    for (int i = 3; i > 0; i--) {
        display.clear();
        display.gotoXY(0, 3);
        display.print(F("Starting in "));
        display.print(i);
        delay(1000);
    }
}

void loop() {
    readAllSensors();
    
    // Button handling - Only check buttons if not in Autonomous mode
    // (In autonomous mode, any button press acts as a stop/exit)
    if (displayMode != 3) {
        if (buttonA.getSingleDebouncedPress()) {
            ledYellow(1); delay(50); ledYellow(0);
            displayMode = 0; // Numeric
            Serial.println(F("[MODE] Numeric"));
        }
        if (buttonB.getSingleDebouncedPress()) {
            ledYellow(1); delay(50); ledYellow(0);
            displayMode = 1; // Bar graph
            Serial.println(F("[MODE] Bar graph"));
        }
        if (buttonC.getSingleDebouncedPress()) {
            ledYellow(1); delay(50); ledYellow(0);
            displayMode = 2; // Alerts
            Serial.println(F("[MODE] Alerts"));
        }
    } else {
        // If in Autonomous mode, any press returns to a manual mode and stops motors
        if (buttonA.getSingleDebouncedPress() || buttonB.getSingleDebouncedPress() || buttonC.getSingleDebouncedPress()) {
             motors.setSpeeds(0, 0);
             displayMode = 0; // Exit to Numeric display mode
             Serial.println(F("[MODE] Autonomous STOPPED"));
        }
    }
    
    // Update display and run logic
    switch (displayMode) {
        case 0: showNumericMode(); break;
        case 1: showBarGraphMode(); break;
        case 2: showAlertMode(); break;
        case 3: showAutonomousMode(); break; // Run autonomous logic
    }
    
    // Update indicators (LED and Buzzer)
    updateIndicators();
    
    // Serial logging (Skipped if in autonomous mode to prevent overwhelming the serial output)
    if (displayMode != 3) {
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
    }
    
    delay(50); // General delay for sensor polling/display update speed
}