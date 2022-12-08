#pragma once
#include "Consts.h"
#include "LedControl.h"
#include "LiquidCrystal.h"
#include <stdint.h>

class GameAssets {
  private:
    // LCD elements
    LiquidCrystal lcd = LiquidCrystal(rs, en, d4, d5, d6, d7);

    // Matrix elements
    LedControl matrix = LedControl(dinPin, clockPin, loadPin, driverNo);

  public:
    GameAssets();
    
    void initializeGameAssets();

    // LCD elements
    LiquidCrystal& getLcd();
    void lcdBegin(uint8_t, uint8_t);
    void lcdClear();
    void lcdCreateChar(uint8_t, uint8_t*);
    void lcdPrint(const char*);
    void lcdSetBrightness(uint8_t);
    void lcdSetCursor(uint8_t, uint8_t);
    void lcdWrite(uint8_t);

    // Matrix elements
    LedControl& getMatrix();
    void matrixClearDisplay(int);
    void matrixSetIntensity(int, int);
    void matrixSetLed(int, int, int, boolean);
    void matrixSetRow(int, int, byte);
    void matrixShutdown(int, bool);
};