
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

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup() {
   Serial.begin(9600);
   for (curr_pos = -1; curr_pos <= 90; curr_pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position

  }
  for (curr_pos = 181; curr_pos >= 90; curr_pos -= 1) { // goes from 180 degrees to 0 degrees
    steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }

    Serial.println("SimpleRx Starting");
    
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, address);
    radio.setPALevel(RF24_PA_LOW);
    
    radio.startListening();
}

//=============

void loop() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
    getData();
    showData();
}

//==============

void getData() {

}

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
    }
}
