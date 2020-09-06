// RC Car Tx
// Joystick Read-out setup

#include <Joystick.h>

// Pins for Joystick
int pinX = A0;
int pinY = A1;
int pinButton = 6;

Joystick joystick(pinX, pinY, pinButton);

//RF Setup Tx setup
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// RF address through which two modules communicate.
byte address[6] = "node1";

// Pins for nRF24L01
#define CE_PIN 8
#define CSN_PIN 9

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

bool send_confirm;


void setup() {

    Serial.begin(9600);

    Serial.println("Joystick Tx Starting");

    radio.begin();

    radio.setDataRate( RF24_250KBPS );
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_LOW);
    
    radio.stopListening();
    radio.setRetries(3,5); // delay, count
}

//====================

void loop() {
  input_pos = joystick.angle()

  Serial.print("Data Sent: ");
  Serial.print(input_pos);
  
  send_confirm = radio.write( &input_pos, sizeof(input_pos) );
  if (send_confirm) {
      Serial.println("  Tx success");
  }
  else {
      Serial.println("  Tx failed");
  }

}
