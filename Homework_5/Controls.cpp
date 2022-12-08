#include "Controls.h"
#include "Macros.h"

Controls::Controls() {
  // Joystick setup
  pinMode(joySWPin, INPUT_PULLUP);
  joySWReading = HIGH;
  joySWState = HIGH;
  joySWLastState = HIGH;
  joySWLastDebounceTime = 0;
  joyIsNeutral = true;
}

bool Controls::getJoyIsNeutral() {
  return joyIsNeutral;
}

void Controls::setJoyIsNeutral(bool joyIsNeutral) {
  this->joyIsNeutral = joyIsNeutral;
}

bool Controls::joySWChangeState() {
  joySWReading = digitalRead(joySWPin);

  if (buttonDebounce(joySWReading, joySWLastState, joySWLastDebounceTime, joySWDebounceDelay)) {
    if (joySWReading != joySWState) {
      joySWState = joySWReading;

      if (joySWState == HIGH) {
        joySWLastState = joySWReading;
        return true;
      } else {
        joySWLastState = joySWReading;
        return false;
      }
    }
  }

  joySWLastState = joySWReading;
  return false;
}

int Controls::processJoyMenuMovement() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);

  if (joyXValue > joyMaxThreshold && joyXValue - joyMaxThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_UP;
  }

  if (joyXValue < joyMinThreshold && joyMinThreshold - joyXValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_DOWN;
  }

  if (joyYValue < joyMinThreshold && joyMinThreshold - joyYValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_LEFT;
  }

  if (joyYValue > joyMaxThreshold && joyYValue - joyMaxThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_RIGHT;
  }

  if (joyXValue >= joyMinThreshold && joyXValue <= joyMaxThreshold &&
      joyYValue >= joyMinThreshold && joyYValue <= joyMaxThreshold) {
    joyIsNeutral = true;
    return -1;
  }
}

bool Controls::buttonDebounce(byte reading, byte lastButtonState, unsigned long &lastDebounceTime, unsigned int debounceDelay) {
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  return (millis() - lastDebounceTime > debounceDelay);
}