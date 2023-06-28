#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

int centerX = 
int centerY =

int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

Servo myservo;

void setup() {
  Wire.begin();

  myservo.attach(5);
  angle = 0;
  myservo.write(angle);

  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("start");

  pinMode(motorPin, OUTPUT);

  delay(2000);
}

void loop() {    
  if (Serial.available()) {
    char header[2];
    int16_t x, y;
    
    if (Serial.readBytes(header, 2) != 2 || header[0] != 'X' || header[1] != 'Y') {
      return; 
    }
    
    if (Serial.readBytes((char*)&x, 2) != 2) {
      return; // x座標の読み込みに失敗した場合は終了
    }
    
    if (Serial.readBytes((char*)&y, 2) != 2) {
      return; // y座標の読み込みに失敗した場合は終了
    }

    Serial.print("x: ");
    Serial.println(x);
    Serial.print(", y: ");
    Serial.println(y);

    right_correctionX = senterX - x;
    left_correctionX = right_correctionX * -1;
    correctionY = senterY - y;

    right_speed = 2048 + right_correctionX + correctionY;
    left_speed = 2048 + left_correctionX + correctionY;

    Serial.print("right_correctionX :");
    Serial.println(right_correctionX);
    Serial.print("left_correctionX :");
    Serial.println(left_correctionY);
    Serial.print("correctionY :");
    Serial.pritnln(correctionY);
    Serial.print("right_speed :")
    Serial.println(right_speed);
    Serial.print("left_speed :")
    Serial.println(left_speed);
   
    jrk1.setTarget(right_speed);
    jrk2.setTarget(left_speed);

    if (centerX < x&&centerY < y){
      jrk1.setTarget();
      jrk2.setTarget();
    }
    if (centerX < x&&senterY > y){
      jrk1.setTarget();
      jrk2.setTarget();
    }
    if (centerX > x&&centerY < y){
      jrk1.setTarget();
      jrk2.setTarget();
    }
    if (centerX > x&&centerY > y){
      jrk1.setTarget();
      jrk2.setTarget();
    }
    if (? < x < ? && ? < y < ?){
      
    }


}




 


