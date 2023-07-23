#include <JrkG2.h>

int limit = 0;
const int WswitchPin = 7;
const int BswitchPin = 6;

const int targetRange = 1200;//あとで調整

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;
int sensorValue[7];
// ラインの閾値
int threshold = 500; // デフォルトの閾値
int white = 0;
int black = 0;
int low_black = 0;
int high_black = 0;

int count;

int target1 = 0;
int target2 = 0;

JrkG2I2C jrk1(11);//右車輪
JrkG2I2C jrk2(12);//左車輪
JrkG2I2C jrk;

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
  int error = calculateError(sensorValue);
  //Serial.print(" Error : ");//エラーの値確認用
  //Serial.println(error);
  int deviation = calculatedeviation(error);
  //Serial.print(" Deviation : ");
  //Serial.println(deviation);
    target1 = 3048 - deviation;
    target2 = 3048 + deviation;
  // ラインの検出処理
  //1if(white != 0 && black != 0){//閾値を決定したら
    //車輪の制御
  delay(10);
 
  jrk1.setTarget(target1);
  jrk2.setTarget(target2);

  if (low_black < sensorValue[0] && sensorValue[0] < high_black && low_black < sensorValue[7] && sensorValue[7] < high_black){
    count += 1;
    delay(1000);
  }
   Serial.println(count);
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