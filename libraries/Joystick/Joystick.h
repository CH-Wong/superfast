/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"

class Joystick
{
  public:
    Joystick(int pinX, int pinY, int pinButton);
    int angle();
    /*int speed();
    bool honk();*/

  private:
    int _pinX;
    int _pinY;
    int _pinButton;

};

#endif
