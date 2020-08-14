#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Joystick.h>

RF24 radio(9, 10); // CE, CSN     
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL}; //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

int pinX = A0;
int pinY = A1;
int pinButton = 7;
int input_pos = 90;
boolean button_state = 0;

Joystick joystick(pinX, pinY, pinButton);    


void setup() {
  Serial.begin(9600);
  radio.begin();                  //Starting the Wireless communication
  radio.stopListening();          //This sets the module as transmitter
 // radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.setPALevel(RF24_PA_LOW);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
 
}

void loop(){
  
  input_pos = joystick.angle();
  Serial.println(input_pos);
  
    
  const char text[32] = "S";
  radio.write(&text, sizeof(text));               
  //radio.write(input_pos);                  //Sending the message to receiver
  Serial.println(text); 
  delay(100);
}
