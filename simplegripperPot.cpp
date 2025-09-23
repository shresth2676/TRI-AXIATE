#include <Servo.h>

#define POT_PIN A0
#define SERVO_PIN 9

Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
}

void loop() {
  int potVal = analogRead(POT_PIN);          // 0–1023
  int angle  = map(potVal, 0, 1023, 0, 180);  // Map to 0–180°
  myServo.write(angle);                       // Move servo instantly
  delay(15);                                  // Small delay for smoothness
}
