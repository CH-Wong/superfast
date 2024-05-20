/*
  Morse.h - Library for flashing Morse code.
  Created by Ya boi Cheef Kief, November 2, 2007.
  Released into the public domain.
*/
#ifndef Ackermann_h
#define Ackermann_h

#include "Arduino.h"

class Ackermann
{
  public:
    Ackermann(float width, float length, int minAngle);
    int outerAngle(int angle);
  private:
    float _width;
    float _length;
    int _minAngle;
    int _angle;
};

#endif
