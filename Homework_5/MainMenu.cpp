#include "Consts.h"
#include "LCDUtils.h"
#include "Macros.h"
#include "MainMenu.h"

MainMenu::MainMenu() {
  currentOption = 0;
  currentOptionLcdRow = 0;
}

byte MainMenu::getCurrentOption() {
  return currentOption;
}

void MainMenu::displayMainMenu(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte currentOptionCursorCol = (lcdRowLength - strlen((char*)pgm_read_word(&mainMenuOptions[currentOption]))) / 2 - 1; // "-1" is for the ">" that indicates the selected option
  gameAssets.lcdSetCursor(currentOptionCursorCol, currentOptionLcdRow);
  gameAssets.lcdWrite('>');
  gameAssets.lcdPrint((char*)pgm_read_word(&mainMenuOptions[currentOption]));

  byte otherOption = !currentOptionLcdRow ? currentOption + 1 : currentOption - 1;
  byte otherOptionCursorCol = (lcdRowLength - strlen((char*)pgm_read_word(&mainMenuOptions[otherOption]))) / 2;
  byte otherOptionLcdRow = 1 - currentOptionLcdRow;
  gameAssets.lcdSetCursor(otherOptionCursorCol, otherOptionLcdRow);
  gameAssets.lcdPrint((char*)pgm_read_word(&mainMenuOptions[otherOption]));

  LCDUtils::displayUpDownNavArrows(currentOption, currentOptionLcdRow, mainMenuOptionsCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(MAIN_MENU, currentOption, currentOptionLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(MAIN_MENU, currentOption, currentOptionLcdRow, mainMenuOptionsCount, gameAssets, sounds);
  }
}