// SimpleTx - the master or the transmitter

#include <Joystick.h>

// Pins for Joystick
int pinX = A0;
int pinY = A1;
int pinButton = 6;

Joystick joystick(pinX, pinY, pinButton);

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// RF address through which two modules communicate.
byte address[6] = "node1";

// Pins for nRF24L01
#define CE_PIN 8
#define CSN_PIN 9

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[10] = "Message 0";
char txNum = '0';


unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second


void setup() {

    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();

    radio.setDataRate( RF24_250KBPS );
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_LOW);
    
    radio.stopListening();
    radio.setRetries(3,5); // delay, count




}

//====================

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {
        // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}
