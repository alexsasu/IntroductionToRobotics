#pragma once
#include "Arduino.h"
#include "Consts.h"
#include "GameAssets.h"

class Controls {
  private:
    // Joystick elements
    byte joySWReading;
    byte joySWState;
    byte joySWLastState;
    unsigned long joySWLastDebounceTime;
    bool joyIsNeutral;

  public:
    Controls();

    bool getJoyIsNeutral();
    void setJoyIsNeutral(bool);

    bool joySWChangeState();
    int processJoyMenuMovement();
    bool buttonDebounce(byte, byte, unsigned long&, unsigned int);
};