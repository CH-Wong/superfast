/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Joystick.h>
#include <Servo.h>

void setup() {
  Serial.begin(9600);

  // Set up Joystick read-out.
  int pinX = A0
  int pinY = A1
  int pinButton = 7

  Joystick joystick(pinX, pinY, pinButton);

  // Set up Servo-motor control
  int pinServo = 9;
  int angle = 90;    // variable to store the servo position, start straight (90deg)
  Servo steeringServo;  // create servo object to control a servo
  steeringServo.attach(pinServo);  // attaches the servo on pin 9 to the servo object
  steeringServo.write(angle)

}

void loop() {
  angle = joystick.angle()
  steeringServo.write(angle);

  Serial.println(angle)

}
