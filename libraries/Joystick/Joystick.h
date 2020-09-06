/*
  Morse.h - Library for flashing Morse code.
  Created by Ya boi Cheef Kief, November 2, 2007.
  Released into the public domain.
*/
#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"

class Joystick
{
  public:
    Joystick(int pinX, int pinY, int pinButton);
    int angleX();
    int angleY();
    int buttonState();
    int getdataArray();
    
    

  private:
    int _pinX;
    int _pinY;
    int _pinButton;

};

#endif
