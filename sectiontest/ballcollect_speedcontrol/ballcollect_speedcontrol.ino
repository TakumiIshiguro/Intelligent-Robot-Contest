int centerX = 320;
int centerY = 240;

int right_correctionX;
int left_correctionX;
int correctionY;

int right_speed;
int left_speed;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("start");
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

    right_correctionX = centerX - x;
    left_correctionX = right_correctionX * -1;
    correctionY = centerY - y;

    right_speed = 2048 + right_correctionX + correctionY;
    left_speed = 2048 + left_correctionX + correctionY;

    Serial.print("right_correctionX :");
    Serial.println(right_correctionX);
    Serial.print("left_correctionX :");
    Serial.println(left_correctionX);
    Serial.print("correctionY :");
    Serial.println(correctionY);
    Serial.print("right_speed :");
    Serial.println(right_speed);
    Serial.print("left_speed :");
    Serial.println(left_speed);
   
  }
}




 


