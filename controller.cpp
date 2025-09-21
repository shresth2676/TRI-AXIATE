#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pins
int pot1_pin = 8;
int pot2_pin = 11;

// NRF24 setup
RF24 radio(9, 10);

const byte addresses[][6] = {
  "joy1",   // for J1
  "joy2",   // for J2
  "pot1",   // for P1
  "pot2"    // for P2
};
// Joystick struct
struct joystick {
  int xVal;
  int yVal;
  int x_pin;
  int y_pin;
};

// Potentiometer struct
struct potentiometer {
  int pot_pin;
  int pot_val;
};

// Final packet struct (to send everything)
struct SendData {
  int J1x;
  int J1y;
  int J2x;
  int J2y;
  int P1;
  int P2;
};

SendData packet;

void setup() {
  
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);   // use HIGH for PA+LNA
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  // Joysticks
  joystick J1, J2;
  J1.x_pin = A0;  
  J1.y_pin = A1;
  J2.x_pin = A2;  
  J2.y_pin = A3;

  J1.xVal = analogRead(J1.x_pin);
  J1.yVal = analogRead(J1.y_pin);
  J2.xVal = analogRead(J2.x_pin);
  J2.yVal = analogRead(J2.y_pin);

  // Pots
  potentiometer p1, p2;
  p1.pot_pin = A4;  
  p1.pot_val = analogRead(p1.pot_pin);
  p2.pot_pin = A5;  
  p2.pot_val = analogRead(p2.pot_pin);

  // --- Send J1 ---
  radio.openWritingPipe(addresses[0]);
  radio.write(&J1, sizeof(J1));

  // --- Send J2 ---
  radio.openWritingPipe(addresses[1]);
  radio.write(&J2, sizeof(J2));

  // --- Send P1 ---
  radio.openWritingPipe(addresses[2]);
  radio.write(&p1, sizeof(p1));

  // --- Send P2 ---
  radio.openWritingPipe(addresses[3]);
  radio.write(&p2, sizeof(p2));

  //all values sent to the robotic arm
  
  delay(50);
}
