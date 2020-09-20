/*
  Joystick.cpp - Library for control using joystick.
  Created by Ya boi.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Ackermann.h"
#include "math.h"

int min_angle = 5;

Joystick::Joystick(float width, float length)
{
  _width = width;
  _length = length;

//pinMode(_pinButton, INPUT_PULLUP); This is for HONKYBOI
}

int Joystick::outerAngle(int _angle)
{
   if (abs(_angle) > min_angle)
    {
     return _angle;
    }

   else
     {
       return  (int) 90 - atan((_width/_length) + tan(90 - _angle));
     }

}
