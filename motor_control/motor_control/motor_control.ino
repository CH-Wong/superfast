/*
        Arduino Brushless Motor Control
     by Dejan, https://howtomechatronics.com
*/

#include <Servo.h>
#include <Joystick.h>
#define pinX A0
#define pinY A1
#define pinButton 5
Servo ESC;     // create servo object to control the ESC
Joystick joystick(pinX, pinY, pinButton);
int potValue;  // value from the analog pin
int input_posX = 90;    // variable to store the servo position, start straight (90deg)
int input_posY = 90;    // variable to store the acceleration, start at 90deg
int buttonState;
int Value = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ey boss");
  // Attach the ESC on pin 9
  ESC.attach(9,700,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.write(90);
 
}

void loop() {
 Value =    joystick.angleY();
  
  Serial.println(Value);
  ESC.write(Value);    // Send the signal to the ESC
}
