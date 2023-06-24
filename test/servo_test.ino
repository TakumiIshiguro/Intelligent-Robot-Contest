/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int val;    // variable to read the value from the analog pin
int angle;
int A;

void setup() {
  myservo.attach(13); // attaches the servo on pin 9 to the servo object
  angle = 0;
}

void loop() {
    // scale it for use with the servo (value between 0 and 180)
  myservo.write(angle);                  // sets the servo position according to the scaled value
  delay(40);

  if(angle == 0)//20
  A = 2;
  if(angle >= 140)
  A = -2;

  if(angle == 140 || angle == 0)
  delay(1500);

  angle += A;      
  myservo.write(angle);                  // 140sets the servo position according to the scaled value
  delay(50);                     // waits for the servo to get there
}
