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
int count = 0;

Servo myservo;
int val;
int angle;
int A;

const int BswitchPin = 6;
const int WswitchPin = 7;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

const int MAX = 50;
const int PIN_MOTER = 13;

const int DIR = 14;
const int STEP = 15;

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
  delay(10);

  jrk1.setTarget(2548);
  jrk2.setTarget(2548);

  Serial.println(count);
  Serial.println(low_black);
  if (low_black <= sensorValue[0] && sensorValue[0] <= high_black && low_black <= sensorValue[7] && sensorValue[7] <= high_black){
    count += 1;
    delay(100);
  }

  if(count == 2){
    jrk1.stopMotor();
    jrk2.stopMotor();
    delay(1000);

    jrk1.setTarget(2548);
    jrk2.setTarget(1548);
    delay(2000);

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

    analogWrite(PIN_MOTER, 0);

    for (angle = 70; angle <= 140; angle += 3){
      myservo.write(angle);
      delay(40);
    }
    delay(1000);

    for (angle = 140; angle >= 0; angle -= 2){
      myservo.write(angle);
      delay(40);
    }

    jrk1.setTarget(1548);
    jrk2.setTarget(2548);
    delay(2000);

    count = 0;
    
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
   Serial.println(low_black);
}