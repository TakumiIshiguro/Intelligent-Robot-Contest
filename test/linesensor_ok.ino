#include<AFMotor.h>

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;
const int line = 400;

void setup() {
  // シリアル通信の初期化
  Serial.begin(9600);
  // センサーのピンを入力モードに設定
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor5Pin, INPUT);
  pinMode(sensor6Pin, INPUT);
  pinMode(sensor7Pin, INPUT);
  pinMode(sensor8Pin, INPUT);

  delay(2000);
}

void loop() {
  delay(100);
  // センサーの値を読み取る
  int sensor1Value = analogRead(sensor1Pin);
  int sensor2Value = analogRead(sensor2Pin);
  int sensor3Value = analogRead(sensor3Pin);
  int sensor4Value = analogRead(sensor4Pin);
  int sensor5Value = analogRead(sensor5Pin);
  int sensor6Value = analogRead(sensor6Pin);
  int sensor7Value = analogRead(sensor7Pin);
  int sensor8Value = analogRead(sensor8Pin);

  // センサーの値をシリアルモニターに表示
  Serial.print("Sensor 1: ");
  Serial.print(sensor1Value);
  Serial.print(" Sensor 2: ");
  Serial.print(sensor2Value);
  Serial.print("Sensor 3: ");
  Serial.print(sensor3Value);
  Serial.print(" Sensor 4: ");
  Serial.print(sensor4Value);
  Serial.print("Sensor 5: ");
  Serial.print(sensor5Value);
  Serial.print(" Sensor 6: ");
  Serial.print(sensor6Value);
  Serial.print("Sensor 7: ");
  Serial.print(sensor7Value);
  Serial.print(" Sensor 8: ");
  Serial.println(sensor8Value);


}