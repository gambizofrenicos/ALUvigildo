#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(10);  
}

void loop() {
  myservo.write(100);                  // sets the servo position according to the scaled value
  delay(100);                           // waits for the servo to get there
  myservo.detach();
  delay(10000);
  myservo.attach(10);
  myservo.write(180);
  delay(3000);
}

