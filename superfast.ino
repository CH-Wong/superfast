#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pins for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8
// MOSI = 11, MISO = 12, SCK = 13
#define refreshDelay 0 //ms
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

// Initialize the angles of the joystick
int dataArray[3];

#include <Servo.h>
#define ESC_PIN 9
int curr_pos = 90;
int pos = 0;

// create servo object to control the ESC
Servo steeringServo;

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

  // Attach the Servo motor to controller.
  steeringServo.attach(ESC_PIN);  // attaches the servo on pin 9 to the servo object
  // steeringServo.steeringServo(ESC_PIN,700,2000); // (pin, min pulse width, max pulse width in microseconds) 
  steeringServo.write(curr_pos);

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
      int angleX = dataArray[0];
      int angleY = dataArray[1];
      int buttonState = dataArray[2];

      // Serial.print(F("Received "));
      // Serial.print(bytes);  // print the size of the payload
      // Serial.print(F(" bytes on pipe "));
      // Serial.print(pipe);  // print the pipe number
      // Serial.print(F(": "));
      Serial.print(angleX);  // print the payload's value
      Serial.print("\t");
      Serial.print(angleY);  // print the payload's value
      Serial.print("\t");
      Serial.println(buttonState);  // print the payload's value

      curr_pos = steer(angleY, curr_pos);
      accelerate(angleX);
      honk(buttonState);

      // to make this example readable in the serial monitor
      delay(refreshDelay);  // slow transmissions down by 1 second
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

int steer(int angle, int curr_pos) {
  // Slowly change the angle instead of sending it immediately
  if (angle > curr_pos && curr_pos < 180){
    curr_pos += 1;
  }

  else if (angle < curr_pos && curr_pos > 0){
    curr_pos -= 1;
  }

  Serial.print(curr_pos);
  Serial.print("\t");
  Serial.println(angle);
  steeringServo.write(curr_pos);
  delay(15);

  return curr_pos;
}

void honk(bool buttonState) {
  if (buttonState){
    Serial.println("HONK!");
  }
}

void accelerate(int angle) {
  Serial.print("Acceleration Data: ");
  Serial.println(angle);
}