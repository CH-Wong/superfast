#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Joystick.h>

RF24 radio(9, 10); // CE, CSN     
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

int pinX = A0;
int pinY = A1;
int pinButton = 7;
int input_pos = 90;
boolean button_state = 0;

Joystick joystick(pinX, pinY, pinButton);    


void setup() {
  Serial.begin(9600);
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}

void loop(){
  
  input_pos = joystick.angle();
  Serial.println(input_pos);
  
    
  const char text[] = "Suck a fuck";
  radio.write(&text, sizeof(text));               
  //radio.write(input_pos);                  //Sending the message to receiver
  Serial.println(text); 
  delay(1000);
}
