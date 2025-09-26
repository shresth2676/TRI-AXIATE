#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
//adding all required libraries 

RF24 radio(10, 9); // CE=10, CSN=9

const byte addresses[][6] = {
  "joy1",   // for J1
  "joy2",   // for J2
  "pot1",   // for P1
  "pot2"    // for P2
};
// defined all the addresses

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

struct SendData {
  int J1x;
  int J1y;
  int P1;
};


Servo s1;
Servo s2;


//defining the constants for the gripper
Servo myServo;
int currentPos = 0; 

//defining the constants for the stepper 
const int MAX_STEPS=200;
const int THRESHOLD=10;
int STEP_DELAY=2000;

long CURRENTSTEPS=0;
int PREVPOT=0;

//defining the constants for the servos 
const int DEADZONE=50;//joystick deadzone
const int MAX_STEP=2; //Max degrees to move per loop

int angleX1=90;
int anglex2=90;

//defining the pins
int step_pin=2;
int dir_pin=3;
//other are the servo pins defined in the void setup()

void setup(){
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);

    radio.openReadingPipe(0, addresses);
    radio.openReadingPipe(1, addresses);
    radio.openReadingPipe(2, addresses);
    radio.openReadingPipe(3, addresses);
    radio.startListening();

    s1.attach(10);
    s2.attach(11);

    s1.write(angleX);
    s2.write(angleX2);

    myServo.attach(9);
    myServo.write(currentPos);

    pinMode(step_pin,OUTPUT);
    pinMode(dir_pin,OUPUT);

     if(radio.availabel()){
        radio.read(&p1,sizeof(p1));
        radio.read(&J2,sizeof(J2));
    }
    PREVjoy=J2.xVal;
    PREVPOT=analogRead(p1.pot_val);
}

void loop(){
    if(radio.availabel()){
        radio.read(&J1,sizeof(J1));
        radio.read(&J2,sizeof(J2));
        radio.read(&p1,sizeof(p1));
    }
    //controlling the servo one
    if(abs(J1.xVal)>DEADZONE){
            // Map displacement to step size (larger movement = faster step)
            int step=map(abs(J1.xVal),DEADZONE,512,1,MAX_STEP);
            if(J1.xVal>0){
                angleX+=step;//right
            }
            else{
                angleX-=step;//left
            }
            angleX1=constrain(angleX1,0,180);
            s1.write(angleX1);
        
        }
        //for the servo 2
    if(abs(J1.yVal)>DEADZONE){
        // Map displacement to step size (larger movement = faster step)
        int step=map(abs(J1.xVal),DEADZONE,512,1,MAX_STEP);
        if(J1.yVal>0){
            angleX2+=step;//right
        }
        else{
            angleX2-=step;//left
        }
        angleX2=constrain(angleX2,0,180);
        s1.write(angleX2);
        
        }
    //controlling the stepper using joystick
    int currJoy = J2.xVal
    // Only react if joystick moved enough
    if (abs(currJoy - prevJoy) > THRESHOLD) {
    // Map joystick to step range
    long targetSteps = map(currJoy, 0, 1023, 0, MAX_STEPS);
    long stepsToMove = targetSteps - currentSteps;

    if (stepsToMove != 0) {
      // Set direction
      digitalWrite(DIR_PIN, stepsToMove > 0 ? HIGH : LOW);

      // Move the required steps
      for (long i = 0; i < abs(stepsToMove); i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(STEP_DELAY);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(STEP_DELAY);
      }

      currentSteps = targetSteps;
    }

    prevJoy = currJoy;
  }
  //controlling the gripper using the potentiometer
  int potVal = analogRead(p1.pot_val);                  // Read potentiometer
  int targetPos = map(potVal, 0, 1023, 0, 180);     // Map to servo angle

    // Move gradually towards target
    if (targetPos > currentPos) {
        for (int pos = currentPos; pos <= targetPos; pos++) {
            myServo.write(pos);
            delay(10);  // Adjust for speed (smaller = faster)
        }
    } 
    else if (targetPos < currentPos) {
        for (int pos = currentPos; pos >= targetPos; pos--) {
            myServo.write(pos);
            delay(10);
        }
    }
    currentPos = targetPos;  // Update current position

    
}