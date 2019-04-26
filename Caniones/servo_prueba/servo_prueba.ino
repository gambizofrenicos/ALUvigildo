#include <Servo.h>

Servo myservoi;  // create servo object to control a servo
Servo myservoc;  // create servo object to control a servo
Servo myservod;  // create servo object to control a servo

void setup() {

}

void loop() {
  
  myservoi.attach(9);
  myservoi.write(100);
  delay(2000);
  myservoc.attach(10);
  myservoc.write(100);
  delay(2000);

  myservod.attach(11);
  myservod.write(100);
  delay(2000);
  
  
}

