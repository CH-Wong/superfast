
// RC Car Rx

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 8
#define CSN_PIN 9

//address through which two modules communicate.
byte address[6] = "node1";

RF24 radio(CE_PIN, CSN_PIN);

// Servo control setup
#include <Servo.h>
Servo steeringServo;
int pinServo = 7;
int currAngle = 90;

int prevState[3];
int joystickState[3]; // this must match dataToSend in the TX
bool newData = false;

// Ackermann Steering


void setup() {
   Serial.begin(9600);

   // Set up Joystick read-out.
   steeringServo.write(currAngle);
   steeringServo.attach(pinServo);  // attaches the servo on pin 9 to the servo object

   for (currAngle = -1; currAngle <= 90; currAngle += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    steeringServo.write(currAngle);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position

  }
  for (currAngle = 181; currAngle >= 90; currAngle -= 1) { // goes from 180 degrees to 0 degrees
    steeringServo.write(currAngle);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }

    Serial.println("RC Car Rx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, address);
    radio.setPALevel(RF24_PA_LOW);                    

    radio.startListening();
}

void loop() {
  if (radio.available()) {
      radio.read( &joystickState, sizeof(joystickState) );
      steering();
      accelerate();
      honk();
      showData();
  }
}

void steering() {
  if (joystickState[0] > currAngle && currAngle < 180){
    currAngle += 3;
  }

  else if (joystickState[0] < currAngle && currAngle > 0){
    currAngle -= 3;
  }
  
  steeringServo.write(currAngle);
  }

void accelerate() {
}

void honk() {
}

void showData() {
  Serial.print("  Angle: ");
  Serial.print(joystickState[0]);
  Serial.print("  Accelerate: ");
  Serial.print(joystickState[1]);
  Serial.print("  Honk: ");
  Serial.print(joystickState[2]);
  Serial.println("");
}
