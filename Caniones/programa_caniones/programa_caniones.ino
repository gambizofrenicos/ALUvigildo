#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(11);  
}

void loop() {
  myservo.write(100);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}

