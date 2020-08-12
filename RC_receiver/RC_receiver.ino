/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
#include <Servo.h>
*/

/* Set up RF Receiver
* Default pins:
* SCK: 13
* MISO: 12
* MOSI: 11
*/
 
// Define control pins
RF24 radio(9, 10); // CE, CSN

// Select Channel
const byte address[6] = "00001";

/*
// Set up Servomotor
Servo steeringServo;  // create servo object to control a servo
int pinServo = 7;
int input_pos = 90;    // variable to store the servo position, start straight (90deg)
int curr_pos = 90;
int pos = 0;
*/

void setup() {
  Serial.begin(9600);

/*
  // Initialize servo motor
  steeringServo.write(curr_pos);
  // Set up Joystick read-out.
  steeringServo.attach(pinServo);  // attaches the servo on pin 9 to the servo object

   for (curr_pos = -1; curr_pos <= 90; curr_pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  
  for (curr_pos = 181; curr_pos >= 90; curr_pos -= 1) { // goes from 180 degrees to 0 degrees
    steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
*/
  // Intitialize RF transceiving  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
 if (radio.available()) {
    char text[] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    /*
    radio.read(&input_pos, sizeof(text));
    
    if (input_pos > curr_pos && curr_pos < 180){
      curr_pos += 1;
    }
  
    else if (input_pos < curr_pos && curr_pos > 0){
      curr_pos -= 1;
    }
  
    steeringServo.write(curr_pos);
    */
    delay(5);
  
  }

  
}
