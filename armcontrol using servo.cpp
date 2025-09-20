#include <Servo.h>

Servo s1;  // Servo 1 (controlled by vertical joystick)
Servo s2;  // Servo 2 (controlled by horizontal joystick)

int ver = A0;  // Joystick vertical axis
int hor = A1;  // Joystick horizontal axis

void setup() {
  s1.attach(10);  // Servo 1 on pin 10
  s2.attach(11);  // Servo 2 on pin 11
}

void loop() {
  int ver_val = analogRead(ver); // 0–1023
  int hor_val = analogRead(hor); // 0–1023

  int angle_1 = map(ver_val, 0, 1023, 0, 180);
  int angle_2 = map(hor_val, 0, 1023, 0, 180);

  s1.write(angle_1);  // Immediate update
  s2.write(angle_2);

  delay(15); // short delay for servo stability
}
