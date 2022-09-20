//signal from raspberry pi (pick up orange Inventory) 
int Signal1 = 2;
//signal from raspberry pi (pick up blue Inventory) 
int Signal2 = 4;

#include <Servo.h>
Servo a01; //base
Servo a12; //hinge
Servo a23; //hinge2
Servo a34; //hinge3
Servo a45; //hinge4

//Defining the arduino output pins
#define pin_a01 3
#define pin_a12 5
#define pin_a23 6
#define pin_a34 9
#define pin_a45 11


//initializing signals
int Signal1State = 0;
int Signal2State = 0;

//initializing positions
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

//Offset values are used for caliberations 
const int off1 = 0;
const int off2 = 0;
const int off3 = 0;
const int off4 = 10;

//Gripper's open position degree
const int opened5 = 70;
//Gripper's close position degree
const int closed5 = 100;

//Define null value for positions
#define nc 999

//Define the idle position for robotic arm. We have 4 motors for the joints and one for the gripper (this is a 5 DOF robotic arm).
const int Idle1 = 90;
const int Idle2 = 140;
const int Idle3 = 30;
const int Idle4 = 160;
int Idle5 = nc;

//Define the sleep position for robotic arm 
const int Sleep01 = 90;
const int Sleep12 = 140;
const int Sleep23 = 30;
const int Sleep34 = 160;
int Sleep45 = closed5;

//Define the boolian value (either the servo motors are awake or not).
bool awake = false;

//Define the number of steps for servo motors.
int Steps = 10;

//Defining the delays between steps (to control the speed of servos. The higher the delay, the slower the movement)
int ServoSpeedFast = 12;
int ServoSpeedNormal = 15;
int ServoSpeedSlow = 20;
const int MediumPre = 4;
const int SlowPre = 6;

//Define the movement functions

void moveServos(int target1 = Idle1, int target2 = Idle2, int target3 = Idle3, int target4 = Idle4, int target45 = Idle5);
void Sleep(int target1 = Sleep01, int target2 = Sleep12, int target3 = Sleep23, int target4 = Sleep34, int target45 = Sleep45);
void WakeUp(int target1 = Idle1, int target2 = Idle2, int target3 = Idle3, int target4 = Idle4, int target45 = Idle5);
void BackToSleep(int target1 = Sleep01, int target2 = Sleep12, int target3 = Sleep23, int target4 = Sleep34, int target45 = Sleep45);

//zPickUpCorrection is used for caliberation in Z axis. 
void pickUpToDrop(int pickUp, int drop, int zPickUpCorrection = 0);
void pickBackToDropBack(int pickUp, int drop, int zPickUpCorrection = 0);

void setup() {

  //This section is the setup at initiation. the main code happens in th evoid loop.
  a01.attach(pin_a01);
  a12.attach(pin_a12);
  a23.attach(pin_a23);
  a34.attach(pin_a34);
  a45.attach(pin_a45);


  //No delay here !! Servos require a command immediately after being attached !
  
  Sleep();
  delay(500);
  WakeUp();
  delay(500);

  //Setting up the serial monitor user interface for debugging
  Serial.begin(9600);
  
  pinMode(Signal1, INPUT);
  pinMode(Signal2, INPUT);

  delay(500);
}

void loop() {
  
  //Check for pick up signal
  CheckSignals();
  
  //Print the signal in serial monitor
  Serial.println("Signal1State =");
  Serial.println(Signal1State);
  Serial.println("Signal2State =");
  Serial.println(Signal2State);
  
  //if signal exist move the box from storage to AGV
  if (Signal1State == 1) {
    
      pickUpStorageOToAGV(173, 78); //(location of storage (degrees of the base servo), location of AGV (degrees of the base servo))
      delay(1000);
      
  }
  
  
  else if (Signal2State == 1) {
    
      pickUpStorageBToAGV(8, 78); //(location of storage (degrees of the base servo), location of AGV (degrees of the base servo))
      delay(1000);
      
  }

}

//In this section we define the functions:

void CheckSignals() {

    Signal1State = digitalRead(Signal1);
    Signal2State = digitalRead(Signal2);
    
}

void Sleep(int target1, int target2, int target3, int target4, int target45) {
  // Target values are inputs tothe function (degrees of the servos)
  pos1 = (target1 + off1);
  pos2 = 180 - (target2 + off2);
  pos3 = target3 + off3;
  pos4 = (target4 + off4);
  pos5 = target45;

  a45.write(pos5);
  a34.write(pos4);
  a23.write(pos3);
  a12.write(pos2);
  a01.write(pos1);

  //Idle5 = nc; //Idle position for the gripper = No Change
  delay(500);
}

void WakeUp(int target1, int target2, int target3, int target4, int target45) {

//This function moves the robotic arm to the wake up position.

  moveServos(nc, nc, nc, target4, nc);
  delay(50);
  moveServos(nc, nc, target3, nc, nc);
  delay(50);
  moveServos(nc, target2, nc, nc, nc);
  delay(50);
  moveServos(target1, nc, nc, nc, nc);
  delay(50);
  moveServos(nc, nc, nc, nc, target45);
  delay(50);
}

void BackToSleep(int target1, int target2, int target3, int target4, int target45) {

//This function moves the robotic arm back to the sleep position.

  moveServos(nc, nc, nc, target4, nc);
  delay(50);
  moveServos(nc, nc, nc, nc, target45);
  delay(50);
  moveServos(target1, nc, nc, nc, nc);
  delay(50);
  moveServos(nc, target2, nc, nc, nc);
  delay(50);
  moveServos(nc, nc, target3, nc, nc);
  delay(50);

}

void Idle() {
  moveServos();
  delay(500);
}

void moveServos(int target1, int target2, int target3, int target4, int target45) {

  target1 = (target1 == 999) ? pos1 : target1 + off1;
  target2 = (target2 == 999) ? pos2 : 180 - (target2 + off2);
  target3 = (target3 == 999) ? pos3 : target3 + off3;
  target4 = (target4 == 999) ? pos4 : target4 + off4;
  target45 = (target45 == 999) ? pos5 : target45;

  Steps = max(max(max(max(abs(target1 - pos1), abs(target2 - pos2)), abs(target3 - pos3)), abs(target4 - pos4)), abs(target45 - pos5));

//Defining the steps for the servo movement. The largest (in degrees) movement will be the fastest 

  float step01 = ((float)(target1 - pos1)) / Steps;
  float step12 = ((float)(target2 - pos2)) / Steps;
  float step23 = ((float)(target3 - pos3)) / Steps;
  float step34 = ((float)(target4 - pos4)) / Steps;
  float step45 = ((float)(target45 - pos5)) / Steps;

//Storing the temporary positions:

  float temppos1 = pos1;
  float temppos2 = pos2;
  float temppos3 = pos3;
  float temppos4 = pos4;
  float temppos5 = pos5;

  for (int i = 0; i < Steps; i++) {
    temppos1 = temppos1 + step01;
    temppos2 = temppos2 + step12;
    temppos3 = temppos3 + step23;
    temppos4 = temppos4 + step34;
    temppos5 = temppos5 + step45;

    //Setting the position to the minimum of the temporary positions 

    pos1 = floor(temppos1);
    pos2 = floor(temppos2);
    pos3 = floor(temppos3);
    pos4 = floor(temppos4);
    pos5 = floor(temppos5);

    a01.write(pos1);
    a12.write(pos2);
    a23.write(pos3);
    a34.write(pos4);
    a45.write(pos5);

    //The following code sets the speed of movement at each instance of time. (The Servos start slow, speed up, and finish slow)

    int speedStep = min(i, Steps - i);

    if (speedStep < Steps / SlowPre) {
      delay(ServoSpeedSlow);
    } else if (speedStep < Steps / MediumPre) {
      delay(ServoSpeedNormal);
    } else {
      delay(ServoSpeedFast);
    }

  }

  pos1 = floor(target1);
  pos2 = floor(target2);
  pos3 = floor(target3);
  pos4 = floor(target4);
  pos5 = floor(target45);

  a01.write(pos1);
  a12.write(pos2);
  a23.write(pos3);
  a34.write(pos4);
  a45.write(pos5);

}

void pickUpStorageBToAGV(int pickUp, int dropTable) {

  //Pick up from blue storage and move the object to AGV

  int  prePickUp = ( pickUp > Idle1 ) ? pickUp + min(5, abs(Idle1 - pickUp)) : pickUp - min(5, abs(Idle1 - pickUp));

  moveServos(prePickUp, nc, nc, nc, opened5);

  moveServos(pickUp, nc, nc, 144, nc);
  delay(50);
  
  //go to storage
  moveServos(pickUp, 38, 75, nc, nc);
  delay(50);

  //grab the box
  moveServos(nc, nc, nc, nc, closed5);
  delay(50);

  //pick the box
  moveServos(nc, 80, 30, 160, nc);
  delay(50);


  moveServos(nc, Idle2, Idle3, Idle4, nc);
  delay(50);
  Idle();

//  ServoSpeedFast = 30;
//  ServoSpeedNormal = 40;
//  ServoSpeedSlow = 50;
  
  int preDrop = ( dropTable > Idle1 ) ? dropTable + min(5, abs(Idle1 - dropTable)) : dropTable - min(5, abs(Idle1 - dropTable));

  moveServos(preDrop, nc, nc, nc, nc);
  delay(50);
  
  //go to the AGV position
  moveServos(dropTable, 85, 85, 55, nc);
  delay(100);
  
  moveServos(dropTable, 80, nc, 55, nc);
  delay(100);
  
  moveServos(nc, nc, nc, nc, opened5);
  delay(50);
  
  moveServos(nc, 140, 30, 170, nc);
  delay(50);
  
//  ServoSpeedFast = 12;
//  ServoSpeedNormal = 15;
//  ServoSpeedSlow = 20;
  
  Idle();

  delay(1000);
  
}

void pickUpStorageOToAGV(int pickUp, int dropTable) {

  int  prePickUp = ( pickUp > Idle1 ) ? pickUp + min(5, abs(Idle1 - pickUp)) : pickUp - min(5, abs(Idle1 - pickUp));

  moveServos(prePickUp, nc, nc, nc, opened5);

  moveServos(pickUp, nc, nc, 140, nc);
  delay(50);
  
  //go to storage
  moveServos(pickUp, 38, 75, nc, nc);
  delay(50);

  //grab the box
  moveServos(nc, nc, nc, nc, closed5);
  delay(50);

  //pick the box
  moveServos(nc, 80, 30, 160, nc);
  delay(50);


  moveServos(nc, Idle2, Idle3, Idle4, nc);
  delay(50);
  Idle();

//  ServoSpeedFast = 30;
//  ServoSpeedNormal = 40;
//  ServoSpeedSlow = 50;
  
  int preDrop = ( dropTable > Idle1 ) ? dropTable + min(5, abs(Idle1 - dropTable)) : dropTable - min(5, abs(Idle1 - dropTable));

  moveServos(preDrop, nc, nc, nc, nc);
  delay(50);
  
  //go to the AGV position
  moveServos(dropTable, 85, 85, 55, nc);
  delay(100);
  
  moveServos(dropTable, 80, nc, 55, nc);
  delay(100);
  
  moveServos(nc, nc, nc, nc, opened5);
  delay(50);
  
  moveServos(nc, 140, 30, 170, nc);
  delay(50);
  
//  ServoSpeedFast = 12;
//  ServoSpeedNormal = 15;
//  ServoSpeedSlow = 20;
  
  Idle();

  delay(1000);
  
}
