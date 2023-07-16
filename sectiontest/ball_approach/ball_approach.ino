#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

int centerX = 320;
int centerY = 200;

int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;
int before_right_speed;
int before_left_speed;

//進行方向に対して
JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

void setup() {
  Wire.begin();

  Serial.begin(9600);
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
      Serial.println("error");
      return; 
    }
    
    if (Serial.readBytes((char*)&x, 2) != 2) {
      return; // x座標の読み込みに失敗した場合は終了
      Serial.println("errorX");
    }
    
    if (Serial.readBytes((char*)&y, 2) != 2) {
      return; // y座標の読み込みに失敗した場合は終了
      Serial.println("errorY");
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
 
    Serial.print("x :");
    Serial.println(x);
    Serial.print("y :");
    Serial.println(y);

    if (300 < x && x < 350 && 185 < y && y < 230){
      Serial.println("stop_moter");
      jrk1.stopMotor();
      jrk2.stopMotor();
    }
  }
}