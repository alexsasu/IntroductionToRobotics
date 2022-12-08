#include "StartingScreen.h"

StartingScreen::StartingScreen() {
  startingScreenInit = false;
}

bool StartingScreen::getStartingScreenInit() {
  return startingScreenInit;
}

void StartingScreen::displayStartingScreen(GameAssets &gameAssets) {
  gameAssets.lcdPrint(" Scaffold Racer ");
  gameAssets.lcdSetCursor(0, 1);
  gameAssets.lcdPrint("   >Press JS<   ");

  startingScreenInit = true;
}