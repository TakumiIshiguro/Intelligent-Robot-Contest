void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // シリアルポートの接続を待機
  }
  Serial.println("start");
}

void loop() {
  if (Serial.available()) {
    char header[2];
    int16_t x, y;
    
    // ヘッダを読み込む
    if (Serial.readBytes(header, 2) != 2 || header[0] != 'X' || header[1] != 'Y') {
      return; // 正しいヘッダがない場合はデータを無視して終了
    }
    
    // x座標を読み込む
    if (Serial.readBytes((char*)&x, 2) != 2) {
      return; // x座標の読み込みに失敗した場合は終了
    }
    
    // y座標を読み込む
    if (Serial.readBytes((char*)&y, 2) != 2) {
      return; // y座標の読み込みに失敗した場合は終了
    }
    
    // 受信した座標データを使って何か処理を行う
    // 例: xとyの値をシリアルモニタに出力する
    Serial.print("x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);
  }
}

