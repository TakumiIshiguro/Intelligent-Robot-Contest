const int WswitchPin = 51;
const int BswitchPin = 53;

const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensor3Pin = A2;
const int sensor4Pin = A3;
const int sensor5Pin = A4;
const int sensor6Pin = A5;
const int sensor7Pin = A6;
const int sensor8Pin = A7;

// ラインの閾値
double threshold = 500; // デフォルトの閾値
double white = 0;
double black = 0;
void setup() {
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
  // フォトリフレクタの値を読み取る
  int sensor1Value = analogRead(sensor1Pin);
  int sensor2Value = analogRead(sensor2Pin);
  int sensor3Value = analogRead(sensor3Pin);
  int sensor4Value = analogRead(sensor4Pin);
  int sensor5Value = analogRead(sensor5Pin);
  int sensor6Value = analogRead(sensor6Pin);
  int sensor7Value = analogRead(sensor7Pin);
  int sensor8Value = analogRead(sensor8Pin);
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
  if (white != 0 && black != 0){
    //白黒が決定したら実行
    determineThreshold();
  }
  

  // センサーの値をシリアルモニターに表示
  if (white == 0 && black == 0 && switch_W == LOW){
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
  
  // ラインの検出処理
  //何かする
  // 他のフォトリフレクタの処理も同様に追加する
}
void determineThreshold() {//後で書き換え
  // 白黒が決定した場合に閾値を決定する処理
  
  threshold = (black + white) / 2;
  // シリアルモニタに閾値を表示
  Serial.print("Threshold: ");
  Serial.println(threshold);
}
void determineWhite() {
   double total = 0;

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
   double total = 0;

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