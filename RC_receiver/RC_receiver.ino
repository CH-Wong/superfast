
// RC Car Rx

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define refreshDelay 100 //ms

// Pins for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8
// MOSI = 11, MISO = 12, SCK = 13

//address through which two modules communicate.
byte address[6] = "node1";

RF24 radio(CE_PIN, CSN_PIN);

// Servo control setup
#include <Servo.h>
Servo steeringServoR;
Servo steeringServoL;
int pinServoR = 7;
int pinServoL = 6;
int currAngle = 90;
int outerAngle;
int angleR;
int angleL;

int prevState[3];
int joystickState[3]; // this must match dataToSend in the TX
bool readSuccess = false;

// Ackermann Steering
#include <Ackermann.h>
Ackermann acker(1., 2., 5);

void setup() {
  Serial.begin(9600);
  
  // Set up Joystick read-out.
  steeringServoR.attach(pinServoR);  // attaches the servo on pin 9 to the servo object
  steeringServoL.attach(pinServoL);  // attaches the servo on pin 9 to the servo object
  
  Serial.println("RC Car Rx Starting");
  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);                    
  
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  delay(refreshDelay);
}

void steering() {
  if (currAngle != joystickState[0]) {
    if (joystickState[0] <= 90) {
      outerAngle = acker.outerAngle(joystickState[0]);
      angleR = joystickState[0];
      angleL = outerAngle;
    }
    else if (joystickState[0] > 90) {
      outerAngle = acker.outerAngle(joystickState[0]);
      angleR = outerAngle;
      angleL = joystickState[0];
    }
  
    steeringServoR.write(angleR);
    steeringServoL.write(angleL);

    currAngle = joystickState[0];
    delay(50);
    
  }


}
void accelerate() {
}

void honk() {
  if (joystickState[2]){
    Serial.println("HONK!");
  }
}

void showData() {
  Serial.print("  Input Angle: ");
  Serial.print(joystickState[0]);
  Serial.print("  Angle R: ");
  Serial.print(angleR);
  Serial.print("  Angle L: ");
  Serial.print(angleL);
  
  Serial.print("  Accelerate: ");
  Serial.print(joystickState[1]);
  Serial.print("  Honk: ");
  Serial.print(joystickState[2]);
  Serial.println("");
}
