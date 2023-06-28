#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

Servo myservo;

int val;
int angle;
int A;

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;

int sensor1Value = analogRead(sensor1Pin);
int sensor2Value = analogRead(sensor2Pin);
int sensor3Value = analogRead(sensor3Pin);
int sensor4Value = analogRead(sensor4Pin);
int sensor5Value = analogRead(sensor5Pin);
int sensor6Value = analogRead(sensor6Pin);
int sensor7Value = analogRead(sensor7Pin);
int sensor8Value = analogRead(sensor8Pin);

const int motorPin = 13;

void moveForward() {
  jrk1.setTarget(2348);
  jrk2.setTarget(2348);
}

void linestop() {
  jrk1.setTarget(2048);
  jrk2.setTarget(2048);
}

void turn() { 
  jrk1.setTarget(2348);
  jrk2.setTarget(1748);

  delay(5000);
}

void pullout() {
  digitalWrite(motorPin, HIGH);
        
  myservo.write(angle);
  angle += 2;

  if (angle == 140){
    digitalWrite(motorPin, LOW);
  }
}


void setup() {
  Wire.begin();

  myservo.attach(5);
  angle = 0;
  myservo.write(angle);

  Serial.begin(9600);
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor5Pin, INPUT);
  pinMode(sensor6Pin, INPUT);
  pinMode(sensor7Pin, INPUT);
  pinMode(sensor8Pin, INPUT);

  pinMode(motorPin, OUTPUT);

  delay(2000);
}

void loop() {  

  int sensor1Value = analogRead(sensor1Pin);
  int sensor2Value = analogRead(sensor2Pin);
  int sensor3Value = analogRead(sensor3Pin);
  int sensor4Value = analogRead(sensor4Pin);
  int sensor5Value = analogRead(sensor5Pin);
  int sensor6Value = analogRead(sensor6Pin);
  int sensor7Value = analogRead(sensor7Pin);
  int sensor8Value = analogRead(sensor8Pin);
  
  delay(1000);
  jrk1.setTarget(2348);
  jrk2.setTarget(2348);

  if(sensor1Value > 650&&sensor2Value > 650&&sensor3Value > 650&&sensor4Value > 650&&sensor5Value > 650&&sensor6Value > 650&&sensor7Value > 650&&sensor8Value > 650){
    jrk1.setTarget(2048);
    jrk2.setTarget(2048);
     delay(10);

     jrk1.setTarget(2348);
     jrk2.setTarget(1748);
     delay(400);

     jrk1.setTarget(2048);
     jrk2.setTarget(2048);
     delay(100);
     
     void pullout();
     delay(100);
  }
}


