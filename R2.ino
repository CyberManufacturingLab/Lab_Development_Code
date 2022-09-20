//import the libraries
#include<Braccio.h>
#include<Servo.h>

//Define the servo motors

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

//Define signals to initiate the robotic arm movement

int Signal1 = 2;

//Signal 4 is a standby (the wiring for an extra signal got built in the test bed, and can be used for further code developement)
int Signal2 = 4;

int Signal1State = 0;
int Signal2State = 0;

#define nc null

void setup(){
  
  //initial set up of the robotic arm
  Braccio.begin();
  Serial.begin(9600);
  pinMode(Signal1, INPUT);
  pinMode(Signal2, INPUT);
  Idle(); //go to Idle position 

}

void loop () {
  
  //check for pick up signal
  checkSignals();
  
  Serial.println("Signal1State =");
  Serial.println(Signal1State);
  Serial.println("Signal2State =");
  Serial.println(Signal2State);
  
  //if signal exist move the box from AGV to Conveyor
  if (Signal1State == 1) {
    
      AGV_2_CON();
      delay(1000);
  }
  
}

void checkSignals() {
  
  Signal1State = digitalRead(Signal1);
  Signal2State = digitalRead(Signal2);
  
}

void Idle() {
    
   /*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
  
                       //(step delay, M1, M2, M3, M4, M5, M6);
  //go to Idle Position 
  Braccio.ServoMovement(20,           120,  105, 87, 92, 168,  20);
  //Wait 1 second
  delay(1000);
}

void AGV_2_CON(){
  
   /*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
  
                       //(step delay, M1, M2, M3, M4, M5, M6);
  //go to AGV
  Braccio.ServoMovement(20,           116,  130, 0, 0, 168,  20);  
  //Wait 1 second
  delay(1000);
  
  //grab the box
  Braccio.ServoMovement(20,           116,  130, 0, 0, 168,  60);  
  //Wait 1 second
  delay(1000);
  
  //pick up the box
  Braccio.ServoMovement(20,           116,  180, 0, 0, 168,  60);  

  //Wait 0.05 second
  delay(50);

  //take to conveyor 
  Braccio.ServoMovement(20,           119,  123, 158, 125, 168,  60);  
  //Wait 1 second
  delay(1000);

  //release the box  
  Braccio.ServoMovement(20,           119,  123, 158 , 125, 168,  20);  
  //Wait 1 second
  delay(1000);

  Idle();

}

//Extra function for further developement 

//void AGV_2_S4(){ 
////idle, pick up from AGV, 90 R turn, put in storage
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//Braccio.ServoMovement(20, 90, 80, 180, 180, 0, 10); //adjust for AGV 
//delay(500);
//Braccio.ServoMovement(20, 90, 80, 180, 180, 0, 100); //adjust for AGV 
//Braccio.ServoMovement(20, 90, 50, 180, 120, 0, 100);
//delay(2000);
////turn
//Braccio.ServoMovement(20, 179, 50, 180, 120, 0, 100);
//Braccio.ServoMovement(20, 179, 60, 180, 135, 0, 100);
//Braccio.ServoMovement(20, 179, 94, 153, 160, 0, 100);
//Braccio.ServoMovement(20, 179, 111, 153, 160, 0, 100);
//Braccio.ServoMovement(20, 179, 125, 153, 145, 0, 100);
//delay(1000); //release
//Braccio.ServoMovement(20, 179, 125, 153, 145, 0, 10);
//Braccio.ServoMovement(20, 179, 96, 180, 160, 0, 10);
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//}
//
//void S3_2_CNC(){
////idle, take out of left storage, 90 turn with flip, put on cnc transport
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//Braccio.ServoMovement(20, 0, 58, 180, 180, 0, 10);
//Braccio.ServoMovement(20, 0, 115, 180, 135, 0, 10);
//Braccio.ServoMovement(20, 0, 115, 180, 135, 0, 90);
//Braccio.ServoMovement(20, 0, 98, 180, 153, 0, 90);
//Braccio.ServoMovement(20, 0, 90, 155, 130, 0, 90);
//// turn/flip
//Braccio.ServoMovement(20, 95, 90, 155, 130, 0, 90);
//Braccio.ServoMovement(20, 95, 128, 25, 45, 0, 90);
//Braccio.ServoMovement(20, 95, 105, 25, 70, 0, 90);
//Braccio.ServoMovement(20, 95, 100, 13, 92, 0, 90);
//Braccio.ServoMovement(20, 95, 97, 0, 105, 0, 90);
//Braccio.ServoMovement(20, 95, 86, 0, 115, 0, 90);
//Braccio.ServoMovement(20, 95, 70, 0, 135, 0, 90);
//Braccio.ServoMovement(20, 95, 58, 0, 138, 0, 90);
//delay(1000);
//Braccio.ServoMovement(20, 95, 58, 0, 138, 0, 10);
//Braccio.ServoMovement(20, 95, 75, 0, 138, 0, 10);
//    digitalWrite(LedOut, HIGH);
//    delay(500);
//    digitalWrite(LedOut, LOW);
//    delay(500);
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//}
//
//void S4_2_CNC(){
////idle, take out of left storage, 90 turn with flip, put on cnc transport
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//Braccio.ServoMovement(20, 180, 58, 180, 180, 0, 10);
//Braccio.ServoMovement(20, 180, 115, 180, 135, 0, 10);
//Braccio.ServoMovement(20, 180, 115, 180, 135, 0, 90);
//Braccio.ServoMovement(20, 180, 98, 180, 153, 0, 90);
//Braccio.ServoMovement(20, 180, 90, 155, 130, 0, 90);
//// turn/flip
//Braccio.ServoMovement(20, 88, 90, 155, 130, 0, 90);
//Braccio.ServoMovement(20, 88, 128, 25, 45, 0, 90);
//Braccio.ServoMovement(20, 88, 105, 25, 70, 0, 90);
//Braccio.ServoMovement(20, 88, 100, 13, 92, 0, 90);
//Braccio.ServoMovement(20, 88, 97, 0, 105, 0, 90);
//Braccio.ServoMovement(20, 88, 86, 0, 115, 0, 90);
//Braccio.ServoMovement(20, 88, 70, 0, 135, 0, 90);
//Braccio.ServoMovement(20, 88, 58, 0, 138, 0, 90);
//delay(1000);
//Braccio.ServoMovement(20, 88, 58, 0, 138, 0, 10);
//Braccio.ServoMovement(20, 88, 75, 0, 138, 0, 10);
//
//    delay(1000);
//Braccio.ServoMovement(20, 90, 50, 150, 180, 0, 10);
//}
