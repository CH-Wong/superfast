/*
  Joystick.cpp - Library for control using joystick.
  Created by Ya boi.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Joystick.h"


int value = 0;
int x_value = 0;
int y_value = 0;
int x_out = 0;
int y_out = 0;
int button_out;
int buffer = 20;

Joystick::Joystick(int pinX, int pinY, int pinButton)
{
   _pinX = pinX;
   _pinY = pinY;
   _pinButton = pinButton;

pinMode(_pinButton, INPUT_PULLUP); //This is for HONKYBOI
}

int Joystick::angleX()
{
   x_value = analogRead(_pinX);
   if (x_value >= (1024/2-buffer) && x_value <=1024/2+buffer)
    {
     x_out = 512;
    }

   else
     {
       x_out = x_value ;
     }

     return map(x_out,0,1023,0,180);

}

int Joystick::angleY()
{
   y_value = analogRead(_pinY);
   if (y_value >= (1024/2-buffer) && y_value <=1024/2+buffer)
    {
     y_out = 512;
    }

   else
     {
       y_out = y_value ;
     }

     return map(y_out,0,1023,0,180);

}
int Joystick::buttonState()
{
    button_out = digitalRead(_pinButton);
    if(button_out == 1){
      return 0;
    }
    else
    {
    return 1;
    }

}
