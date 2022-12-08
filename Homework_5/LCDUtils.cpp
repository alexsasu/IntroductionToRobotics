#include "Consts.h"
#include "Macros.h"
#include "LCDUtils.h"

LCDUtils::LCDUtils() {}

bool LCDUtils::goBack = false;

void LCDUtils::initializeUpDownNavArrows(GameAssets &gameAssets) {
  gameAssets.lcdCreateChar(0, lcdUpArrowIcon);
  gameAssets.lcdCreateChar(1, lcdDownArrowIcon);
}

void LCDUtils::displayUpDownNavArrows(byte currentSelection, byte currentSelectionLcdRow, byte displayLinesCount, GameAssets &gameAssets) {
  if ((currentSelectionLcdRow != lcdFirstRow || currentSelection > 0) &&
      (currentSelectionLcdRow != lcdSecondRow || currentSelection > 1)) {
    gameAssets.lcdSetCursor(lcdRowLength - 1, lcdFirstRow);
    LCDUtils::displayLcdIcon(lcdUpArrowIconAddr, gameAssets);
  }

  if ((currentSelectionLcdRow != lcdSecondRow || currentSelection < displayLinesCount - 1) &&
      (currentSelectionLcdRow != lcdFirstRow || currentSelection < displayLinesCount - 2)) {
    gameAssets.lcdSetCursor(lcdRowLength - 1, lcdSecondRow);
    LCDUtils::displayLcdIcon(lcdDownArrowIconAddr, gameAssets);
  }
}

void LCDUtils::initializeGameStatsIcons(GameAssets &gameAssets) {
  gameAssets.lcdCreateChar(2, gameStatsLivesIcon);
  gameAssets.lcdCreateChar(3, gameStatsDifficultyIcon);
}

void LCDUtils::displayLcdIcon(byte addr, GameAssets &gameAssets) {
  gameAssets.lcdWrite(addr);
}

void LCDUtils::attemptToMoveUp(byte displayType, int &currentSelection, byte &currentSelectionLcdRow, GameAssets &gameAssets, Sounds &sounds) {
  if (currentSelection > 0) {
    if ((displayType == INSTRUCTIONS || displayType == LEADERBOARD || displayType == GAME_FINISHED) && currentSelectionLcdRow == 1) {
      currentSelection -= 2;
    } else {
      --currentSelection;
    }
    if (currentSelectionLcdRow == 1) {
      currentSelectionLcdRow = 0;
    }
    if (currentSelection < 0) {
      currentSelection = 0;
    } else {
      sounds.attemptToPlayMenuNavSound();
    }
    gameAssets.lcdClear();
  }
}

void LCDUtils::attemptToMoveDown(byte displayType, int &currentSelection, byte &currentSelectionLcdRow, byte displayLinesCount, GameAssets &gameAssets, Sounds &sounds) {
  if (currentSelection < displayLinesCount - 1) {
    if ((displayType == INSTRUCTIONS || displayType == LEADERBOARD || displayType == GAME_FINISHED) && currentSelectionLcdRow == 0) {
      currentSelection += 2;
    } else {
      ++currentSelection;
    }
    if (currentSelectionLcdRow == 0) {
      currentSelectionLcdRow = 1;
    }
    if (currentSelection >= displayLinesCount) {
      currentSelection = displayLinesCount - 1;
    } else {
      sounds.attemptToPlayMenuNavSound();
    }
    gameAssets.lcdClear();
  }
}

void LCDUtils::goToPreviousMenu(GameAssets &gameAssets) {
  gameAssets.lcdClear();
  LCDUtils::goBack = true;
}