#include "Consts.h"
#include <EEPROM.h>
#include "GameAssets.h"
#include <stdint.h>

GameAssets::GameAssets() {
  pinMode(lcdBrightnessPin, OUTPUT);
}

void GameAssets::initializeGameAssets() {
  // LCD setup
  lcdBegin(16, 2);
  lcdSetBrightness(EEPROM.read(eepromLcdBrightnessAddress));

  // Matrix setup
  matrixShutdown(0, false);                                            // turn off power saving, enables display
  byte matrixIntensity = EEPROM.read(eepromMatrixLedIntensityAddress);
  matrixSetIntensity(0, matrixIntensity);                              // sets brightness (0~15 possible values)
  matrixClearDisplay(0);                                               // clear screen
}

LiquidCrystal& GameAssets::getLcd() {
  return lcd;
}

void GameAssets::lcdBegin(uint8_t cols, uint8_t rows) {
  lcd.begin(cols, rows);
}

void GameAssets::lcdClear() {
  lcd.clear();
}

void GameAssets::lcdCreateChar(uint8_t addr, uint8_t *character) {
  lcd.createChar(addr, character);
}

void GameAssets::lcdPrint(const char *string) {
  lcd.print(string);
}

void GameAssets::lcdSetBrightness(uint8_t brightness) {
  analogWrite(lcdBrightnessPin, brightness);
}

void GameAssets::lcdSetCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void GameAssets::lcdWrite(uint8_t character) {
  lcd.write(character);
}

LedControl& GameAssets::getMatrix() {
  return matrix;
}

void GameAssets::matrixClearDisplay(int addr) {
  matrix.clearDisplay(addr);
}

void GameAssets::matrixSetIntensity(int addr, int intensity) {
  matrix.setIntensity(addr, intensity);
}

void GameAssets::matrixSetLed(int addr, int row, int col, boolean state) {
  matrix.setLed(addr, row, col, state);
}

void GameAssets::matrixSetRow(int addr, int row, byte value) {
  matrix.setRow(addr, row, value);
}

void GameAssets::matrixShutdown(int addr, bool status) {
  matrix.shutdown(addr, status);
}