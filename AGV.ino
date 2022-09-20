 //www.elegoo.com

//Define the sensor inputs 
#define LT1 digitalRead(10)
#define LT2 digitalRead(4)
#define LT3 digitalRead(2)
#define CUBE !digitalRead(12)
#define STOP digitalRead(A5)

//Define the arduino output pins.(IN1-IN4 are for the wheel motors)
#define ENA 5
#define ENB 11
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9

//define the speed of the motors
#define PRECISE 80
#define STRAIGHT 110
#define TURN 190

int ABS = STRAIGHT;

unsigned long timeSpent = 0;

#define TIMEGAP 1500

void back(){
  
  //This function controls the movement of the motors to make the AGV move backward.
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void forward(){

  //This function controls the movement of the motors to make the AGV move forward.
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left(){

  //This function controls the movement of the motors to make the AGV turns left.
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go left!");
}

void right(){
  
  //This function controls the movement of the motors to make the AGV turns right.
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go right!");
} 

void stop(){
  
  //This function controls the movement of the motors to make the AGV stop.
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, LOW);
   Serial.println("Stop!");
   delay(200);
} 

void setup(){
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  //This loop drives the AGV. When robotic arm 1 places the object AGV goes to robotoc arm 2 position and will wait for robotic arm 2 to remove the object.
  // Once the object is removed, AGV goes back to the robotic arm 1 position. 
  while(!CUBE);
  delay(1500);
  ABS = STRAIGHT;
  timeSpent = millis();
  while(!STOP || (millis()-timeSpent) < TIMEGAP){
    followTrackForward();
  }
  stop();
  ABS = PRECISE;
  while(STOP){
    followTrackForward();
  }
  back();
  delay(20);
  stop();
  delay(20);
  
  while(CUBE);
  delay(4000);
  ABS = STRAIGHT;
  timeSpent = millis();
  while(!STOP || (millis()-timeSpent) < TIMEGAP){
    followTrackForward();
  }
  stop();
  ABS = PRECISE;
  while(STOP){
    followTrackForward();
  }
  back();
  delay(20);
  stop();
  delay(20);

void followTrackForward(){

  //This function reads the sensor values to check if the AGV is following the black line.
  
  if(LT2){
    forward();
  }
  else if(LT1) { 
    left();
    while(LT1);                             
  }   
  else if(LT3) {
    right();
    while(LT3);  
  }

}

void followTrackBackward(){

  //This function reads the sensor values to check if the AGV is following the black line.
  if(LT2){
    back();
    delay(5);
    stop();
    delay(5);
  }
  else if(LT1) { 
    right();
    while(LT1);                             
  }   
  else if(LT3) {
    left();
    while(LT3);  
  }

}
