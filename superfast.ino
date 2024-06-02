// This file contains the code for both the receiver and transmitter of a pair of RF devices for a radio-controlled car.

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pins for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8
// MOSI = 11, MISO = 12, SCK = 13
#define refreshDelay 100 //ms
#define SPI_SPEED 10000000

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN, SPI_SPEED);

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Node", "2Node" };
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit

// 0 -> address[0], 1 -> address[1]
bool radioNumber = 0;  
// Used to control whether this node is sending or receiving
// 0 = RX role, 1 = TX role, 
bool role = false;  

#include <Joystick.h>
#define pinX A0
#define pinY A1
#define pinButton 5

Joystick joystick(pinX, pinY, pinButton);

// Initialize the angles of the joystick
int dataArray[3];

#include <Servo.h>
#define ESC_PIN 9
int curr_pos = 90;
int pos = 0;

// create servo object to control the ESC
Servo steeringServo;

#define MOTOR_PIN 10
Servo motorESC;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  pinMode(pinButton, OUTPUT);

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  // print example's introductory prompt
  Serial.println(F("Joystick Rx Initializing..."));

  // role variable is hardcoded to RX behavior, inform the user of this
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // radio.setRetries(15,0);

  radio.setAddressWidth(5);
  radio.setChannel(192);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.disableDynamicPayloads();
  radio.disableAckPayload();
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(0, 0);

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(dataArray));  // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  // additional setup specific to the node's role
  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening();  // put radio in RX mode

    // Attach the Servo motor to controller.
    steeringServo.attach(ESC_PIN);  // attaches the servo on pin 9 to the servo object
    // steeringServo.steeringServo(ESC_PIN,700,2000); // (pin, min pulse width, max pulse width in microseconds)
    steeringServo.write(curr_pos);

    // Attach the Servo motor to controller.
    motorESC.attach(MOTOR_PIN);  // attaches the servo on pin 9 to the servo object
    // steeringServo.steeringServo(ESC_PIN,700,2000); // (pin, min pulse width, max pulse width in microseconds)
    motorESC.write(curr_pos);
  }

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
  Serial.println(F("Initialized."));
}  // setup

void loop() {

  if (role) {
    // This device is a TX node
    // Prepare data for transmission
    dataArray[0] = joystick.angleX();
    dataArray[1] = joystick.angleY();
    dataArray[2] = joystick.buttonState();

    unsigned long start_timer = micros();                // start the timer
    bool report = radio.write( &dataArray, sizeof(dataArray) ); // transmit & save the report
    unsigned long end_timer = micros();                  // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));  // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);  // print the timer result
      Serial.print(F(" us. Sent: "));
      
      Serial.print("Data Transmitted:");
      Serial.print("\tX: ");
      Serial.print(dataArray[0]);
      Serial.print("\tY: ");
      Serial.print(dataArray[1]);
      Serial.print("\tButton: ");
      Serial.print(dataArray[2]);
    } else {
      Serial.println(F("Transmission failed or timed out."));  // payload was not delivered
    }

    // to make this example readable in the serial monitor
    delay(refreshDelay);  // slow transmissions down by 1 second

  } else {
    // This device is a RX node
    // Read the data coming in from the RF signal.

    uint8_t pipe;
    if (radio.available(&pipe)) {               // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();   // get the size of the payload
      radio.read(&dataArray, bytes);             // fetch payload from FIFO
      // Rename the dataArray
      int angleX = dataArray[0];
      int angleY = dataArray[1];
      int buttonState = dataArray[2];

      // Prints to serial for debugging.
      Serial.print("Data Received:\t");
      Serial.print(bytes);  // print the size of the payload
      Serial.print("\tPipe: ");
      Serial.print(pipe);  // print the pipe number
      Serial.print("\tX: ");
      Serial.print(angleX);  // print the payload's value
      Serial.print("\tY: ");
      Serial.print(angleY);  // print the payload's value
      Serial.print("\tButton: ");
      Serial.println(buttonState);  // print the payload's value

      // Update the speed based on AngleY and write it to the ESC motor
      accelerate(angleY);

      // Honk once if the buttonstate was high 
      honk(buttonState);

      steeringServo.write(angleX);

    }
  }  // role

  if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node

      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.stopListening();

    } else if (c == 'R' && role) {
      // Become the RX node

      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      radio.startListening();
    }
  }

}  // loop

void honk(bool buttonState) {
  if (buttonState) {
    Serial.println("HONK!");
    digitalWrite(pinButton, HIGH); // sets the digital pin 13 on
  } else {
    digitalWrite(pinButton, LOW); // sets the digital pin 13 on    
  }
}

void accelerate(int angle) {
  motorESC.write(angle);
}
