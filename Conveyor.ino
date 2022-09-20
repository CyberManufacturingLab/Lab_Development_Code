//Import the libraries
#include <Servo.h>

//Define the servo motor for the gripper
Servo servo1;
int servoPin = 3;
int pos = 0;

//off is used for calibration.
const int off = 0;
int Steps = 10;
int ServoSpeedFast = 0;
int ServoSpeedNormal = 1;
int ServoSpeedSlow = 2;
const int MediumPre = 4;
const int SlowPre = 6;

#include <Stepper.h>
int Signal1 = 2;
int Signal2 = 4;

int Signal1State = 0;
int Signal2State = 0;

// change this to the number of steps on your motor
#define STEPS 300

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);

void setup() {

  Serial.begin(9600);
  servo1.attach(servoPin);
  moveServos(0);
  
  // set the speed of the motor to 60 RPMs
  stepper.setSpeed(60);

  pinMode(Signal1, INPUT);
  pinMode(Signal2, INPUT);
  
}

void loop() {
  
  //check for the conveyor movement initiation signal
  CheckSignals();

  //Print in the signal in serial monitor for debugging
  Serial.println("Signal1State =");
  Serial.println(Signal1State);
  Serial.println("Signal2State =");
  Serial.println(Signal2State);
  
  if (Signal1State == 1) {

      moveServos(50);
      delay(1000);
   
      //Move the conveyor 1000 steps to get to CNC position 
      stepper.step(1000);

      //Wait 49 seconds for the CNC drawing to finish.
      delay(49000);
      
      //Move the conveyor 1780 steps to get to robotic arm 3 position 
      stepper.step(1780);
   
      moveServos(0);

      delay(5000);
      
      //Move the conveyor back to robotic arm 2 position 
      stepper.step(-2780);
      
      
  }

}

void CheckSignals() {

    Signal1State = digitalRead(Signal1);
    Signal2State = digitalRead(Signal2);
    
}

void moveServos(int target) {

  //This function moves the servo for the gripper.
  target = (target == 999) ? pos : target + off;


  Steps = abs(target - pos);

  float step01 = ((float)(target - pos)) / Steps;

  float temppos = pos;

  for (int i = 0; i < Steps; i++) {
    temppos = temppos + step01;

    pos = floor(temppos);

    servo1.write(pos);


    int speedStep = min(i, Steps - i);

    if (speedStep < Steps / SlowPre) {
      delay(ServoSpeedSlow);
    } else if (speedStep < Steps / MediumPre) {
      delay(ServoSpeedNormal);
    } else {
      delay(ServoSpeedFast);
    }

  }

  pos = floor(target);

  servo1.write(pos);


}
