#include <Servo.h>

// ======== Stepper Setup ========
#define STEP_PIN 2
#define DIR_PIN 3
#define POT_PIN A0

const int MAX_STEPS = 200;    // Full stepper range
const int STEP_THRESHOLD = 2; // Ignore small noise
int STEP_DELAY = 2000;        // Microseconds between step pulses

long currentSteps = 0;
int prevPot = 0;

// ======== Servo Setup ========
Servo servo1;   // Joystick 1 → Servo 1
Servo servo2;   // Joystick 2 → Servo 2

#define JOY1_X A1   // Joystick 1 X-axis
#define JOY2_X A2   // Joystick 2 X-axis

const int DEADZONE = 50;  // Ignore small noise near center
const int MAX_STEP = 2;   // Max degrees to move per loop
int angle1 = 90;          // Start at mid position
int angle2 = 90;


void setup() {
  // Stepper
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  prevPot = analogRead(POT_PIN);

  // Servos
  servo1.attach(10);
  servo2.attach(11);
  servo1.write(angle1);
  servo2.write(angle2);
}

void loop() {
  // ======== Stepper Control (Potentiometer) ========
  int currPot = analogRead(POT_PIN);

  if (abs(currPot - prevPot) > STEP_THRESHOLD) {
    long targetSteps = map(currPot, 0, 1023, 0, MAX_STEPS);
    long stepsToMove = targetSteps - currentSteps;

    if (stepsToMove != 0) {
      digitalWrite(DIR_PIN, stepsToMove > 0 ? HIGH : LOW);

      for (long i = 0; i < abs(stepsToMove); i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(STEP_DELAY);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(STEP_DELAY);
      }
      currentSteps = targetSteps;
    }
    prevPot = currPot;
  }

  // ======== Servo 1 Control (Joystick 1) ========
  int x1 = analogRead(JOY1_X) - 512;  // Center offset
  if (abs(x1) > DEADZONE) {
    int step = map(abs(x1), DEADZONE, 512, 1, MAX_STEP);
    if (x1 > 0) angle1 += step;   // Right
    else        angle1 -= step;   // Left
    angle1 = constrain(angle1, 0, 180);
    servo1.write(angle1);
  }

  // ======== Servo 2 Control (Joystick 2) ========
  int x2 = analogRead(JOY2_X) - 512;
  if (abs(x2) > DEADZONE) {
    int step = map(abs(x2), DEADZONE, 512, 1, MAX_STEP);
    if (x2 > 0) angle2 += step;   // Right
    else        angle2 -= step;   // Left
    angle2 = constrain(angle2, 0, 180);
    servo2.write(angle2);
  }

  delay(15); // Smooth servo motion
}
