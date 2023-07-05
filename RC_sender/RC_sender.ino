//RF Setup Tx setup
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define refreshDelay 100 //ms

// RF address through which two modules communicate.
byte address[6] = "node1";
// Pins for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8
// MOSI = 11, MISO = 12, SCK = 13

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

bool send_confirm;

void setup() {

    Serial.begin(9600);
    Serial.println("Joystick Tx Starting");

    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
//    radio.setDataRate(RF24_250KBPS);
    radio.stopListening();
//    radio.setRetries(3,5); // delay, count
}

void loop() {
  const char text[] = "Hello World";
  send_confirm = radio.write(&text, sizeof(text));

  Serial.print(dataArray[0]);
  Serial.print(dataArray[1]);
  Serial.print(dataArray[2]);
  delay(10);
  
  if (send_confirm) {
      Serial.println("Tx success");
  }
  else {
      Serial.println("Tx failed");
  }
  delay(refreshDelay);
}















//// RC Car Tx
//// Joystick Read-out setup
//
//#include <Joystick.h>
//
//// Pins for Joystick
//#define pinX A0
//#define pinY A1
//#define pinButton 5
//int smash;
//
//Joystick joystick(pinX, pinY, pinButton);
//
////RF Setup Tx setup
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//
//#define refreshDelay 100 //ms
//
//// RF address through which two modules communicate.
//byte address[6] = "node1";
//
//// Pins for nRF24L01
//#define CE_PIN 7
//#define CSN_PIN 8
//// MOSI = 11, MISO = 12, SCK = 13
//
//// Initialize the angles of the joystick
//int input_posX = 90;    // variable to store the servo position, start straight (90deg)
//int input_posY = 90;    // variable to store the acceleration, start at 90deg
//int buttonState;
//int dataArray[3];
//
//RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
//
//bool send_confirm;
//
//void setup() {
//
//    Serial.begin(9600);
//
//    Serial.println("Joystick Tx Starting");
//
//    radio.begin();
//
//    radio.setDataRate( RF24_250KBPS );
//    radio.openWritingPipe(address);
//    radio.setPALevel(RF24_PA_LOW);
//    
//    radio.stopListening();
//    radio.setRetries(3,5); // delay, count
//}
//
//void loop() {
//  dataArray[0] = joystick.angleX();      
//  dataArray[1] = joystick.angleY();
//  dataArray[2] = joystick.buttonState();
//
//  Serial.print("Data Sent: ");
//  Serial.print(dataArray[0]);
//  Serial.print("\t");
//  Serial.print(dataArray[1]);
//  Serial.print("\t");
//  Serial.print(dataArray[2]);
//  Serial.print("\t");
//  delay(refreshDelay);
//  
//  send_confirm = radio.write( &dataArray, sizeof(dataArray) );
//  if (send_confirm) {
//      Serial.println("Tx success");
//  }
//  else {
//      Serial.println("Tx failed");
//  }
//
//}
