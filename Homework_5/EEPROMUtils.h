#include "Arduino.h"
#include <EEPROM.h>

template <class T> int readFromEeprom(int addr, T& value) {
  byte* valueAux = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); ++i) {
    *valueAux++ = EEPROM.read(addr++);
  }
  return i;
}

template <class T> int saveToEeprom(int addr, const T& value) {
  const byte* valueAux = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); ++i) {
    EEPROM.update(addr++, *valueAux++);
  }
  return i;
}