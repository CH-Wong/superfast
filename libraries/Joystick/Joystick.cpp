/*
  Joystick.cpp - Library for control using joystick.
  Created by Ya boi.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Joystick.h"

int _buffer = 5;

Joystick::Joystick(int pinX, int pinY, int pinButton) {
  _pinX = pinX;
  _pinY = pinY;
  _pinButton = pinButton;

  pinMode(_pinButton, INPUT_PULLUP); //This is for HONKYBOI
}

int Joystick::readAngle(int pin) {
  int value = analogRead(pin);
  if (value >= (1024/2-_buffer) && value <=1024/2+_buffer) {
    int output = map(512,0,1023,0,180);
    return output;
  } else {
    int output = map(value,0,1023,0,180);
    return output;
  }
}

int Joystick::angleX() {
  int angle = readAngle(_pinX);
  return angle;
}

int Joystick::angleY() { 
  int angle = readAngle(_pinY);
  return angle;
}

int Joystick::buttonState() {
  int button_out = digitalRead(_pinButton);
  return !button_out;
}
