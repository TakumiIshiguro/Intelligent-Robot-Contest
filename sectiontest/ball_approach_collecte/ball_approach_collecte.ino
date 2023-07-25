#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

int val;
int angle;
Servo myservo;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

const int MAX = 150;
const int PIN_MOTER = 13;

const int DIR = 14;
const int STEP = 15;

int centerX = 665;
int centerY = 465;

int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;

int count = 4;
int16_t x, y;

void setup() {
  Wire.begin();

  myservo.attach(5);
  angle = 32;
  myservo.write(angle);

  pinMode(PIN_MOTER, OUTPUT);
  
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, LOW);

  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("start");

  delay(2000);
}

void loop() {
  

  right_correctionX = 0;
  left_correctionX = 0;
  correctionY = 0;

  right_speed = 0;
  left_speed = 0;

  x = 0;
  y = 0;

  if (count == 4) {
    Serial.begin(9600);
    char header[2];
    
    if (Serial.readBytes(header, 2) != 2 || header[0] != 'X' || header[1] != 'Y') {
      Serial.println("error");
      jrk1.stopMotor();
      jrk2.stopMotor();
      return; 
    }
    
    if (Serial.readBytes((char*)&x, 2) != 2) {
      Serial.println("error_x");
      jrk1.stopMotor();
      jrk2.stopMotor();
      return; // x座標の読み込みに失敗した場合は終了
    }
    
    if (Serial.readBytes((char*)&y, 2) != 2) {
      Serial.println("error_y");
      jrk1.stopMotor();
      jrk2.stopMotor();
      return; // y座標の読み込みに失敗した場合は終了
    }

    right_correctionX = centerX - x;
    left_correctionX = right_correctionX * -1;
    correctionY = centerY - y;

    right_speed = 2048 + right_correctionX + correctionY;
    left_speed = 2048 + left_correctionX + correctionY;

    jrk1.setTarget(right_speed);
    jrk2.setTarget(left_speed);

    if (630 < x && x < 670 && 450 < y && y < 475){
      Serial.println("mode_change");
      jrk1.stopMotor();
      jrk2.stopMotor();
 
      for (angle = 0; angle <= 140; angle += 2){
        myservo.write(angle);
        delay(40);
      }
      delay(1000);

      analogWrite(PIN_MOTER, MAX);
      delay(3000);

      for (angle = 140; angle >= 0; angle -= 2){
        myservo.write(angle);
        analogWrite(PIN_MOTER, MAX);
        delay(40);
      }
      delay(1000);

      analogWrite(PIN_MOTER, 0);
      delay(3000);

      digitalWrite(DIR, HIGH);
      for (int i = 0; i < 80; i++){
        digitalWrite(STEP, HIGH);
        delayMicroseconds(15000);
        digitalWrite(STEP, LOW);
        delayMicroseconds(15000);
      }
    }
  }

  if (count == 5){
    Serial.println("count5");
    Serial.end();

    jrk1.setTarget(1848);
    jrk2.setTarget(1848);
    delay(1000);
    count = 4;   
  }

}