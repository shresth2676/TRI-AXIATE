#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24 setup (CE, CSN pins)
RF24 radio(9, 10);

// Unique addresses for each input
const byte J1_ADDR[6] = "J1X1";
const byte J2_ADDR[6] = "J2X2";
const byte P1_ADDR[6] = "P1X1";
const byte P2_ADDR[6] = "P2X2";

// Joystick and potentiometer pins
int J1_x_pin = A0;
int J1_y_pin = A1;
int J2_x_pin = A2;
int J2_y_pin = A3;
int P1_pin   = A4;
int P2_pin   = A5;

// Data structs
struct Joystick {
  int xVal;
  int yVal;
};

struct Potentiometer {
  int val;
};

Joystick J1, J2;
Potentiometer P1, P2;

void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);   // High power for PA+LNA
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();             // TX mode
}

void loop() {
  // Read Joysticks
  J1.xVal = analogRead(J1_x_pin);
  J1.yVal = analogRead(J1_y_pin);

  J2.xVal = analogRead(J2_x_pin);
  J2.yVal = analogRead(J2_y_pin);

  // Read potentiometers
  P1.val = analogRead(P1_pin);
  P2.val = analogRead(P2_pin);

  // Send each input to its own address
  radio.openWritingPipe(J1_ADDR);
  radio.write(&J1, sizeof(J1));

  radio.openWritingPipe(J2_ADDR);
  radio.write(&J2, sizeof(J2));

  radio.openWritingPipe(P1_ADDR);
  radio.write(&P1, sizeof(P1));

  radio.openWritingPipe(P2_ADDR);
  radio.write(&P2, sizeof(P2));
  delay(50);  // adjust as needed for responsiveness
}
