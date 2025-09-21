#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
const byte addresses[][6] = {
  "joy1",   // for J1
  "joy2",   // for J2
  "pot1",   // for P1
  "pot2"    // for P2
};
struct joystick {
  int xVal;
  int yVal;
  int x_pin;
  int y_pin;
};
Servo s1;
Servo s2;

const int DEADZONE=50;//joystick deadzone
const int MAX_STEP=2; //Max degrees to move per loop

int angleX=90;
int anglex2=90;
void setup(){
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);

    radio.openReadingPipe(0, address);
    radio.openReadingPipe(1, address);
    radio.openReadingPipe(2, address);
    radio.openReadingPipe(3, address);
    radio.startListening();

    s1.attach(10);
    s2.attach(11);

    s1.write(angleX);
    s2.write(angleX2);

}
void loop(){
    if(radio.availabel()){
        radio.read(&J1,sizeof(J1));
        radio.read(&J2,sizeof(J2));
        radio.read(&p1,sizeof(p1));
        radio.read(&p2,sizeof(p2));
        //first for the servo 1
        if(abs(J1.xVal)>DEADZONE){
            // Map displacement to step size (larger movement = faster step)
            int step=map(abs(J1.xVal),DEADZONE,512,1,MAX_STEP);
            if(J1.xVal>0){
                angleX+=step;//right
            }
            else{
                angleX-=step;//left
            }
            angleX=contrain(angleX,0,180);
            s1.write(angleX);
        
        }
        //for the servo 2
        if(abs(J2.xVal)>DEADZONE){
            // Map displacement to step size (larger movement = faster step)
            int step=map(abs(J2.xVal),DEADZONE,512,1,MAX_STEP);
            if(J2.xVal>0){
                angleX2+=step;//right
            }
            else{
                angleX2-=step;//left
            }
            angleX2=contrain(angleX2,0,180);
            s1.write(angleX2);
        
        }
        //for the stepper and the servo claw pending
        

    }
}