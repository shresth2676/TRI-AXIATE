#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// NRF24
RF24 radio(9, 10); // CE, CSN pins

// Addresses
const byte J1_ADDR[6] = "J1X1";
const byte J2_ADDR[6] = "J2X2";

const byte P1_ADDR[6] = "P1X1";
const byte P2_ADDR[6] = "P2X2";

// Joystick struct
struct Joystick {
  int xVal;
  int yVal;
};

// Potentiometer struct
struct Potentiometer {
  int val;
};

// Servos
Servo s1; // Joystick 1
Servo s2; // Joystick 2
Servo gripper; // Pot2

// Stepper pins
#define STEP_PIN 2
#define DIR_PIN 3

// Control parameters
const int DEADZONE = 50;
const int MAX_STEP = 2;

int angle1 = 90; // Servo 1
int angle2 = 90; // Servo 2

Joystick J1, J2;
Potentiometer P1, P2;

void setup() {
  // NRF24 setup
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, J1_ADDR);
  radio.openReadingPipe(1, J2_ADDR);
  radio.openReadingPipe(2, P1_ADDR);
  radio.openReadingPipe(3, P2_ADDR);
  radio.startListening();

  // Servo setup
  s1.attach(10);
  s2.attach(11);
  gripper.attach(12);
  s1.write(angle1);
  s2.write(angle2);
  gripper.write(0);

  // Stepper pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  byte pipe;
  while (radio.available(&pipe)) {
    // ===== Joystick 1 → Servo 1 =====
    if (pipe == 0) {
      radio.read(&J1, sizeof(J1));
      if (abs(J1.xVal - 512) > DEADZONE) {
        int step = map(abs(J1.xVal - 512), DEADZONE, 512, 1, MAX_STEP);
        angle1 += (J1.xVal - 512 > 0) ? step : -step;
        angle1 = constrain(angle1, 0, 180);
        s1.write(angle1);
      }
    }

    // ===== Joystick 2 → Servo 2 =====
    else if (pipe == 1) {
      radio.read(&J2, sizeof(J2));
      if (abs(J2.xVal - 512) > DEADZONE) {
        int step = map(abs(J2.xVal - 512), DEADZONE, 512, 1, MAX_STEP);
        angle2 += (J2.xVal - 512 > 0) ? step : -step;
        angle2 = constrain(angle2, 0, 180);
        s2.write(angle2);
      }
    }

    // ===== Potentiometer 1 → Stepper =====
    else if (pipe == 2) {
      radio.read(&P1, sizeof(P1));
      // Example: map pot to 200 steps max
      int steps = map(P1.val, 0, 1023, 0, 200);
      // Simple stepper logic: move stepper to target steps (add your own logic)
      digitalWrite(DIR_PIN, HIGH); // CW
      for (int i = 0; i < steps; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
      }
    }

    // ===== Potentiometer 2 → Gripper Servo =====
    else if (pipe == 3) {
      radio.read(&P2, sizeof(P2));
      int gripAngle = map(P2.val, 0, 1023, 0, 90);
      gripper.write(gripAngle);
    }
  }
}
