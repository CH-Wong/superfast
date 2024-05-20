#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pins for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8
// MOSI = 11, MISO = 12, SCK = 13
#define refreshDelay 1000 //ms
#define SPI_SPEED 10000000

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN, SPI_SPEED);

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Node", "2Node" };
// It is very helpful to think of an address as a path instead of as
// an identifying device destination
 
// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 0;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit
 
// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

#include <Joystick.h>
#define pinX A0
#define pinY A1
#define pinButton 5

Joystick joystick(pinX, pinY, pinButton);

//   // Attach the ESC on pin 9
//   ESC.attach(9,700,2000); // (pin, min pulse width, max pulse width in microseconds) 
//   ESC.write(90);

// Initialize the angles of the joystick
int input_posX = 90;    // variable to store the servo position, start straight (90deg)
int input_posY = 90;    // variable to store the acceleration, start at 90deg
int buttonState;
int dataArray[3];

#include <Servo.h>
#define ESC_PIN 9

Servo ESC;     // create servo object to control the ESC

void setup() {
 
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
 
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

  // Attach the ESC on pin 9
  ESC.attach(9,700,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.write(90);

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
    dataArray[0] = joystick.angleX();      
    dataArray[1] = joystick.angleY();
    dataArray[2] = joystick.buttonState();

    Serial.print("Data Sent: ");
    Serial.print(dataArray[0]);
    Serial.print("\t");
    Serial.print(dataArray[1]);
    Serial.print("\t");
    Serial.print(dataArray[2]);
    Serial.print("\t");

    unsigned long start_timer = micros();                // start the timer
    bool report = radio.write( &dataArray, sizeof(dataArray) ); // transmit & save the report
    unsigned long end_timer = micros();                  // end the timer
 
    if (report) {
      Serial.print(F("Transmission successful! "));  // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);  // print the timer result
      Serial.print(F(" us. Sent: "));
    } else {
      Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
    }
 
    // to make this example readable in the serial monitor
    delay(refreshDelay);  // slow transmissions down by 1 second
 
  } else {
    // This device is a RX node
 
    uint8_t pipe;
    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
      radio.read(&dataArray, bytes);             // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);  // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);  // print the pipe number
      Serial.print(F(": "));
      Serial.print(dataArray[0]);  // print the payload's value
      Serial.print("\t");
      Serial.print(dataArray[1]);  // print the payload's value
      Serial.print("\t");
      Serial.println(dataArray[2]);  // print the payload's value

      if (dataArray[2]) {
        honk();
      }

      ESC.write(dataArray[1]);    // Send Y-axis  signal to the ESC
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

void honk() {
  if (joystickState[2]){
    Serial.println("HONK!");
  }
}


// // Servo control setup
// #include <Servo.h>
// Servo steeringServoR;
// Servo steeringServoL;
// int pinServoR = 7;
// int pinServoL = 6;
// int currAngle = 90;
// int outerAngle;
// int angleR;
// int angleL;

// int prevState[3];
// int joystickState[3]; // this must match dataToSend in the TX
// bool readSuccess = false;

// // Ackermann Steering
// #include <Ackermann.h>
// Ackermann acker(1., 2., 5);

// void setup() {
//   Serial.begin(9600);
  
//   // Set up Joystick read-out.
//   steeringServoR.attach(pinServoR);  // attaches the servo on pin 9 to the servo object
//   steeringServoL.attach(pinServoL);  // attaches the servo on pin 9 to the servo object
  
//   Serial.println("RC Car Rx Starting");
  
//   radio.begin();
//   radio.setDataRate( RF24_250KBPS );
//   radio.openReadingPipe(1, address);
//   radio.setPALevel(RF24_PA_LOW);                    
  
//   radio.startListening();
// }

// void loop() {
//   if (radio.available()) {
//     char text[32] = "";
//     radio.read(&text, sizeof(text));
//     Serial.println(text);
//   }
//   delay(refreshDelay);
// }

// void steering() {
//   if (currAngle != joystickState[0]) {
//     if (joystickState[0] <= 90) {
//       outerAngle = acker.outerAngle(joystickState[0]);
//       angleR = joystickState[0];
//       angleL = outerAngle;
//     }
//     else if (joystickState[0] > 90) {
//       outerAngle = acker.outerAngle(joystickState[0]);
//       angleR = outerAngle;
//       angleL = joystickState[0];
//     }
  
//     steeringServoR.write(angleR);
//     steeringServoL.write(angleL);

//     currAngle = joystickState[0];
//     delay(50);
    
//   }


// }
// void accelerate() {
// }



// void showData() {
//   Serial.print("  Input Angle: ");
//   Serial.print(joystickState[0]);
//   Serial.print("  Angle R: ");
//   Serial.print(angleR);
//   Serial.print("  Angle L: ");
//   Serial.print(angleL);
  
//   Serial.print("  Accelerate: ");
//   Serial.print(joystickState[1]);
//   Serial.print("  Honk: ");
//   Serial.print(joystickState[2]);
//   Serial.println("");
// }


// /*
//         Arduino Brushless Motor Control
//      by Dejan, https://howtomechatronics.com
// */

// #include <Servo.h>
// #include <Joystick.h>
// #define pinX A0
// #define pinY A1
// #define pinButton 5
// Servo ESC;     // create servo object to control the ESC
// Joystick joystick(pinX, pinY, pinButton);
// int potValue;  // value from the analog pin
// int input_posX = 90;    // variable to store the servo position, start straight (90deg)
// int input_posY = 90;    // variable to store the acceleration, start at 90deg
// int buttonState;
// int Value = 0;

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Ey boss");
//   // Attach the ESC on pin 9
//   ESC.attach(9,700,2000); // (pin, min pulse width, max pulse width in microseconds) 
//   ESC.write(90);
 
// }

// void loop() {
//  Value =    joystick.angleY();
  
//   Serial.println(Value);
//   ESC.write(Value);    // Send the signal to the ESC
// }



// #include <Joystick.h>
// #include <Servo.h>
// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>

// int pinX = A0;
// int pinY = A1;
// int pinButton = 6;

// Joystick joystick(pinX, pinY, pinButton);

// // Set up Servo-motor control

// Servo steeringServo;  // create servo object to control a servo

// int pinServo = 7;
// int input_pos = 90;    // variable to store the servo position, start straight (90deg)
// int curr_pos = 90;
// int pos = 0;

// RF24 radio(7, 8); // CE, CSN

// const byte address[6] = "00001";


// void setup() {
//   Serial.begin(9600);

//   steeringServo.write(curr_pos);
//   // Set up Joystick read-out.
//   steeringServo.attach(pinServo);  // attaches the servo on pin 9 to the servo object

//    for (curr_pos = -1; curr_pos <= 90; curr_pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
//     delay(10);                       // waits 15ms for the servo to reach the position

//   }
//   for (curr_pos = 181; curr_pos >= 90; curr_pos -= 1) { // goes from 180 degrees to 0 degrees
//     steeringServo.write(curr_pos);              // tell servo to go to position in variable 'pos'
//     delay(10);                       // waits 15ms for the servo to reach the position
//   }

//   radio.begin();
//   radio.openReadingPipe(0, address);
//   radio.setPALevel(RF24_PA_MIN);
//   radio.startListening();


// }

// void loop() {
//   input_pos = joystick.angle();
//   if (input_pos > curr_pos && curr_pos < 180){
//     curr_pos += 1;
//   }

//   else if (input_pos < curr_pos && curr_pos > 0){
//     curr_pos -= 1;
//   }

//   steeringServo.write(curr_pos);
//   delay(10);

//   Serial.print(curr_pos);
//   Serial.print("    ");
//   Serial.println(input_pos);

//   if (radio.available()) {
//     char text[32] = "";
//     radio.read(&text, sizeof(text));
//     Serial.println(text);

// }
