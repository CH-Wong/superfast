/*
  Joystick.cpp - Library for control using joystick.
  Created by Ya boi.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Ackermann.h"
#include "math.h"

float angleRad;
int inputAngle;
float outputAngle;

Ackermann::Ackermann(float width, float length, int minAngle)
{
  _width = width;
  _length = length;
  _minAngle = minAngle;
}

int Ackermann::outerAngle(int _angle) {
  inputAngle = _angle - 90;
    // _angle input is from 0 to 180 deg.
   if (abs(inputAngle) < _minAngle) {
     return _angle;
    }

   else {
       angleRad = (abs(inputAngle) * 71.) / 4068.;
       outputAngle = ((atan( (_length * tan(angleRad)) / ((_width * tan(angleRad)) + _length )) * 4068) / 71);

       if (_angle < 90) {
         return (int) 90 - outputAngle;
       }
       else if (_angle > 90) {
         return (int) 90 + outputAngle;
       }
     }

}
