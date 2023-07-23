#include <JrkG2.h>
#include <Servo.h>
#include <Wire.h>

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;
int sensorValue[7];
int threshold = 500; // デフォルトの閾値
int white = 0;
int black = 0;
int low_black = 0;
int high_black = 0;
int target1 = 0;
int target2 = 0;
const int targetRange = 1200;//あとで調整
int count = 4;
int count_line = 0;

Servo myservo;
int val;
int angle;
int A;

const int BswitchPin = 6;
const int WswitchPin = 7;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

const int MAX = 150;
const int PIN_MOTER = 13;

const int DIR = 14;
const int STEP = 15;

int centerX = 900;
int centerY = 400
;
int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;
int before_right_speed;
int before_left_speed;

void setup() {
  Wire.begin();

  Serial.begin(9600);

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor5Pin, INPUT);
  pinMode(sensor6Pin, INPUT);
  pinMode(sensor7Pin, INPUT);
  pinMode(sensor8Pin, INPUT);

  myservo.attach(5);
  angle = 0;
  myservo.write(angle);

  pinMode(WswitchPin, INPUT_PULLUP);
  pinMode(BswitchPin, INPUT_PULLUP);

  pinMode(PIN_MOTER, OUTPUT);

  delay(2000);
}
void loop() {
 // スイッチの状態を読み取る
  int switch_W = digitalRead(WswitchPin);
  int switch_B = digitalRead(BswitchPin);
  if (switch_W == LOW) {  // スイッチが押された場合
    // 白の決定処理
    determineWhite();
  }
  if (switch_B == LOW){
    //黒の決定処理
    determineBlack();
  }
  if (white != 0 && black != 0 && threshold == 500){
    //白黒が決定したら1度だけ実行
    determineThreshold();
  }
  // フォトリフレクタの値を読み取る
  sensorValue[0] = analogRead(sensor1Pin);
  sensorValue[1] = analogRead(sensor2Pin);
  sensorValue[2] = analogRead(sensor3Pin);
  sensorValue[3] = analogRead(sensor4Pin);
  sensorValue[4] = analogRead(sensor5Pin);
  sensorValue[5] = analogRead(sensor6Pin);
  sensorValue[6] = analogRead(sensor7Pin);
  sensorValue[7] = analogRead(sensor8Pin);
  delay(1000);

  int error = calculateError(sensorValue);
  //Serial.print(" Error : ");//エラーの値確認用
  //Serial.println(error);
  int deviation = calculatedeviation(error);
  //Serial.print(" Deviation : ");
  //Serial.println(deviation);
  target1 = 3048 - deviation;
  target2 = 3048 + deviation;
  jrk1.setTarget(target1);
  jrk2.setTarget(target2);

  Serial.println(count);

  if (low_black <= sensorValue[0] && sensorValue[0] <= high_black && low_black <= sensorValue[7] && sensorValue[7] <= high_black){
    count_line += 1;
    count += count_line;
    delay(1000);
  }

  if(count == 1){
    jrk1.stopMotor();
    jrk2.stopMotor();
    delay(10);

    jrk1.setTarget(2348);
    jrk2.setTarget(1748);
    delay(400);

    jrk1.stopMotor();
    jrk2.stopMotor();
    delay(100);

    analogWrite(PIN_MOTER, MAX);
    delay(3000);

    for (angle = 0; angle <= 70; angle += 2){
      myservo.write(angle);
      analogWrite(PIN_MOTER, MAX);
      delay(40);
    }

    for (angle = 70; angle <= 140; angle += 3){
      myservo.write(angle);
      delay(40);
    }

    delay(1000);
    for (angle = 140; angle >= 0; angle -= 2){
      myservo.write(angle);
      delay(40);
    }
    jrk1.setTarget(1748);
    jrk2.setTarget(2348);
    delay(400);
    count = 0;
    delay(100);
  }

  if (count == 4){
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

      if (900 < x && x < 950 && 380 < y && y < 450){
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

        sensorValue[3] = analogRead(sensor4Pin);
        sensorValue[4] = analogRead(sensor5Pin);
        
        jrk1.setTarget(1748);
        jrk2.setTarget(1748);
        if (low_black <= sensorValue[3] && sensorValue[3] <= high_black || low_black <= sensorValue[4] && sensorValue[4] <= high_black){
          count = 5;
        }
      }    
    }
  }
  
  if (count == 5){
  int deviation = calculatedeviation(error);
  //Serial.print(" Deviation : ");
  //Serial.println(deviation);
  target1 = 2548 - deviation;
  target2 = 2548 + deviation;
  jrk1.setTarget(target1);
  jrk2.setTarget(target2);
    if (2448 <= target1 && target1 <= 2648 && 2448 <= target2 && target2 <= 2648){
      count = 4;
    }
  }
}

void determineThreshold() {
  // 白黒が決定した場合に閾値を決定する処理
  threshold = (black + white) / 2;
  // シリアルモニタに閾値を表示
  Serial.print("Threshold: ");
  Serial.println(threshold);
}

void determineWhite() {
   int total = 0;
   total += analogRead(sensor1Pin);
   total += analogRead(sensor2Pin);
   total += analogRead(sensor3Pin);
   total += analogRead(sensor4Pin);
   total += analogRead(sensor5Pin);
   total += analogRead(sensor6Pin);
   total += analogRead(sensor7Pin);
   total += analogRead(sensor8Pin);
   white = total / 8;//フォトリフレクタの数で割る
   Serial.print("White: ");
   Serial.println(white);
}

void determineBlack() {
   int total = 0;
   total += analogRead(sensor1Pin);
   total += analogRead(sensor2Pin);
   total += analogRead(sensor3Pin);
   total += analogRead(sensor4Pin);
   total += analogRead(sensor5Pin);
   total += analogRead(sensor6Pin);
   total += analogRead(sensor7Pin);
   total += analogRead(sensor8Pin);
   black = total / 8;//フォトリフレクタの数で割る
   low_black = black - 100;
   high_black = black + 100;
   Serial.print("Black: ");
   Serial.println(black);
}

int calculateError(int sensorValue[]) {
  double sum = 0;
  int weight[] = {-10, -7, -4, -1, 1, 4, 7, 10};//leftはマイナス
  for (int i=0; i < 8; i++){
    sum += sensorValue[i]*weight[i];
  }
  return sum;
}

int calculatedeviation(int error) {
  const int max_deviation = targetRange;
  const int min_deviation = -targetRange;
  int deviation = 0;
  deviation = map(error, -threshold*10, threshold*10, -targetRange, targetRange);
  if (deviation > max_deviation){
    deviation = max_deviation;
  }
  if(deviation < min_deviation){
    deviation = min_deviation;
  }
  return deviation;
}