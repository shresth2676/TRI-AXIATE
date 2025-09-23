#include <Servo.h>

#define POT_PIN A0
#define SERVO_PIN 5

Servo myServo;

int lastAngle = 0;   // Remember the previous angle

void setup() {
  myServo.attach(SERVO_PIN);
  lastAngle = map(analogRead(POT_PIN), 0, 1023, 0, 180);
  myServo.write(lastAngle);
}

void loop() {
  int potVal = analogRead(POT_PIN);
  int angle  = map(potVal, 0, 1023, 0, 180);

  if (angle > lastAngle) {
    for (int i = lastAngle; i <= angle; i++) {
      myServo.write(i);
      delay(15);
    }
  } 
  else if (angle < lastAngle) {
    for (int i = lastAngle; i >= angle; i--) {
      myServo.write(i);
      delay(15);
    }
  }

  lastAngle = angle; // Update current position
}
