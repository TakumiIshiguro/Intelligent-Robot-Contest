void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("start");
}

void loop() {
  if (Serial.available()) {
  int data = Serial.read(); 
  Serial.println(data);
  }
}
