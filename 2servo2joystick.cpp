#include <Servo.h>

Servo servo1;   // Controlled by Joystick 1
Servo servo2;   // Controlled by Joystick 2

#define JOY1_X A0    // Joystick 1 axis
#define JOY2_X A1    // Joystick 2 axis

const int DEADZONE = 50;   // Ignore small noise near center
const int MAX_STEP = 2;    // Maximum degrees to move per loop

int angle1 = 90;           // Start at mid position
int angle2 = 90;

void setup() {
  servo1.attach(10);
  servo2.attach(11);

  servo1.write(angle1);    // Start both at 90°
  servo2.write(angle2);
}

void loop() {
  int x1 = analogRead(JOY1_X) - 512;   // Center offset
  int x2 = analogRead(JOY2_X) - 512;

  // ===== Joystick 1 → Servo 1 =====
  if (abs(x1) > DEADZONE) {
    int step = map(abs(x1), DEADZONE, 512, 1, MAX_STEP);
    if (x1 > 0) angle1 += step;   // Right
    else        angle1 -= step;   // Left
    angle1 = constrain(angle1, 0, 180);
    servo1.write(angle1);
  }

  // ===== Joystick 2 → Servo 2 =====
  if (abs(x2) > DEADZONE) {
    int step = map(abs(x2), DEADZONE, 512, 1, MAX_STEP);
    if (x2 > 0) angle2 += step;   // Right
    else        angle2 -= step;   // Left
    angle2 = constrain(angle2, 0, 180);
    servo2.write(angle2);
  }

  delay(15); // Small delay for smooth movement
}
