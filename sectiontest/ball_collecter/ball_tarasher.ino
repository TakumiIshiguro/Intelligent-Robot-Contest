#include <Servo.h>
#include <Wire.h>

int val;
int angle;
int A;

Servo myservo;
const int MAX = 150;
const int PIN_MOTER = 13;

const int DIR = 14;
const int STEP = 15;

void setup(){
  Wire.begin();

  myservo.attach(5);
  myservo.write(angle);
  angle = 0;

  pinMode(PIN_MOTER, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, LOW);

  delay(2000);
}

void loop(){
  myservo.write(0);
  delay(1000);

  for (angle = 0; angle <= 140; angle += 1){
    myservo.write(angle);
    delay(40);
  }
  delay(3000);

  analogWrite(PIN_MOTER, MAX);
  delay(5000);

  for (angle = 140; angle >= 0; angle -= 2){
    myservo.write(angle);
    analogWrite(PIN_MOTER, MAX);
    delay(40);
  }
  delay(3000);

  analogWrite(PIN_MOTER, 0);
  delay(5000);

  digitalWrite(DIR, HIGH);
  for (int i = 0; i < 80; i++){
    digitalWrite(STEP, HIGH);
    delayMicroseconds(15000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(15000);
  }
  delay(5000);

}
  








