int angle;
float angleRad;
float ackAngle;
int outputAngle;

#include <math.h>

void setup() {
   Serial.begin(9600);
    Serial.println("");
    for (int angle = 5; angle <= 80; angle++) {
      ackermann(angle, 1., 2.);  
    }
    
}

void loop() {

}

float ackermann(int angle, int L, int W) {
//  Serial.print("Input Angle: ");
//  Serial.print(angle);
//
//      Serial.println("");
  
  angleRad = ((angle * 71.) / 4068.)  ;
//  Serial.print("Input Angle Rad: ");
//  Serial.print(angleRad);

//      Serial.println("");
      
  ackAngle = atan( (L * tan(angleRad)) / ((W * tan(angleRad)) + L));
//  Serial.print("Ackerman Angle: ");
//  Serial.print(ackAngle);
  
//  Serial.println("");
  
  outputAngle = (int) ((ackAngle * 4068) / 71);
//  Serial.print("Output Angle: ");
//  Serial.print(outputAngle);
Serial.println("");
Serial.print(angle);
Serial.print(outputAngle);

}



//radians = (degrees * 71) / 4068

//degrees = (radians * 4068) / 71
