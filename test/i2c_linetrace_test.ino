#include <JrkG2.h>

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪

JrkG2I2C jrk;

const int WswitchPin = 51;
const int BswitchPin = 53;

const int targetRange = 340;//あとで調整

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;

int sensorValue[0];

// ラインの閾値
int threshold = 500; // デフォルトの閾値
int white = 0;
int black = 0;
int target1 = 0;
int target2 = 0;

void setup() {
  // Set up I2C.
  Wire.begin();
  // ピンの設定
  pinMode(WswitchPin, INPUT_PULLUP);
  pinMode(BswitchPin, INPUT_PULLUP);

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor5Pin, INPUT);
  pinMode(sensor6Pin, INPUT);
  pinMode(sensor7Pin, INPUT);
  pinMode(sensor8Pin, INPUT);

  // シリアル通信の開始
  Serial.begin(9600);
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

  // センサーの値をシリアルモニターに表示
  if (white == 0 || black == 0){
    Serial.print("Sensor 1: ");
    Serial.print(sensorValue[0]);
    Serial.print(" Sensor 2: ");
    Serial.print(sensorValue[1]);
    Serial.print("Sensor 3: ");
    Serial.print(sensorValue[2]);
    Serial.print(" Sensor 4: ");
    Serial.print(sensorValue[3]);
    Serial.print("Sensor 5: ");
    Serial.print(sensorValue[4]);
    Serial.print(" Sensor 6: ");
    Serial.print(sensorValue[5]);
    Serial.print("Sensor 7: ");
    Serial.print(sensorValue[6]);
    Serial.print(" Sensor 8: ");
    Serial.println(sensorValue[7]);
  }

  int error = calculateError(sensorValue);
  Serial.print(" Error : ");
  Serial.println(error);
  int deviation = calculatedeviation(error);
  Serial.print(" Deviation : ");
  Serial.println(deviation);

  //int leftsensor = sensorValue[0] + sensorValue[1] + sensorValue[2] + sensorValue[3];
  //int rightsensor = sensorValue[4] + sensorValue[5] + sensorValue[6] + sensorValue[7];

  //int error = leftsensor - rightsensor;
  //target2 = -target1;
  if (error >= 0){
    target1 = 2348 - deviation;
    target2 = 2348 + deviation;
  }else {
    target1 = 2348 + deviation;
    target2 = 2348 - deviation;
  }

  // ラインの検出処理
  if(white != 0 && black != 0){//閾値を決定したら
    //車輪の制御
  delay(1000);
  Serial.print("target 1: ");
  Serial.print(target1);
  Serial.print(" target 2: ");
  Serial.println(target2);
  
  //jrk1.setTarget(target1);//636
  //jrk2.setTarget(target2);//こっちが速い//300
//0~4095 2048

  //jrk.forceDutyCycleTarget(0);//-600~600 回転速度の制御PWMのHIGHの時間を指定する
  //目標値が高いほどモーターも早くなる
  //setTaegetとの違いがよくわからない
  //setTargetが位置制御でforceDutyCycleTargetが速度制御？

  }
}

void determineThreshold() {//後で書き換え
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
   Serial.print("Black: ");
   Serial.println(black);
}

int calculateError(int sensorValue[]) {
  int sum = 0;
  int weight[] = {-10, -7, -3, -1, 2, 4, 7, 10};//leftはマイナス

  for (int i=0; i < 8; i++){
    sum += sensorValue[i]*weight[i];
  }

  return sum;
}

int calculatedeviation(int error) {
  int deviation = 0;

  deviation = map(error, -threshold*10, threshold*10, 0, targetRange);
  
  return deviation;
}