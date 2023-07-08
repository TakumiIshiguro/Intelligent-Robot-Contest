#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

int angle;

int centerX = 480;
int centerY = 375;

int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;
int before_right_speed;
int before_left_speed;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

Servo myservo;

void setup() {
  Wire.begin();

  myservo.attach(5);

  myservo.write(angle);
  
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

    right_correctionX = centerX - x;
    left_correctionX = right_correctionX * -1;
    correctionY = centerY - y;

    right_speed = 2048 + right_correctionX + correctionY;
    left_speed = 2048 + left_correctionX + correctionY;
   
    if (right_speed > 3048||right_speed < 2048){
      right_speed = 2048;
    }
    if (left_speed > 3048||left_speed < 2048){
      left_speed = 2048;
    }

    jrk1.setTarget(right_speed);
    jrk2.setTarget(left_speed);

    Serial.print("right:");
    Serial.println(right_speed);
    Serial.print("left:");
    Serial.println(left_speed);

    if (before_right_speed = right_speed){
      jrk1.stopMotor();      
    }
    if (before_left_speed = left_speed){
      jrk2.stopMotor();
    }
    
    before_right_speed = right_speed;
    before_left_speed = left_speed;
  
    delay (10);
  }
}