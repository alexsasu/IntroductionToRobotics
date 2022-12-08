#include "Consts.h"
#include "Credits.h"
#include "LCDUtils.h"
#include "Macros.h"
#include "MatrixUtils.h"

Credits::Credits() {
  creditsInit = false;
}

bool Credits::getCreditsInit() {
  return creditsInit;
}

void Credits::initializeCredits(GameAssets &gameAssets) {
  MatrixUtils::displayMatrixIcon(gameAssets, matrixCreditsIcon);

  currentInfoType = 0;
  lastInfoType = 0;
  currentInfoLcdRow = 0;
  currentInfoLetter = 0;
  timeSinceLastScroll = millis() - scrollDelay;

  creditsInit = true;
}

void Credits::showCredits(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte currentInfoTypeCursorCol = 0;
  gameAssets.lcdSetCursor(currentInfoTypeCursorCol, currentInfoLcdRow);
  gameAssets.lcdWrite('>');
  gameAssets.lcdPrint(creditsInfoTypes[currentInfoType]);
  gameAssets.lcdWrite(':');
  byte currentInfoCursorCol = strlen(creditsInfoTypes[currentInfoType]) + 2; // 2 = 1 (the ":") + 1 (the space right after the ":")
  showInfo(currentInfoType, currentInfoLcdRow, currentInfoCursorCol, gameAssets);

  byte otherInfoType = !currentInfoLcdRow ? currentInfoType + 1 : currentInfoType - 1;
  byte otherInfoTypeCursorCol = 1;
  byte otherInfoLcdRow = 1 - currentInfoLcdRow;
  gameAssets.lcdSetCursor(otherInfoTypeCursorCol, otherInfoLcdRow);
  gameAssets.lcdPrint(creditsInfoTypes[otherInfoType]);
  gameAssets.lcdWrite(':');
  byte otherInfoCursorCol = strlen(creditsInfoTypes[otherInfoType]) + 2;
  showInfo(otherInfoType, otherInfoLcdRow, otherInfoCursorCol, gameAssets);

  LCDUtils::displayUpDownNavArrows(currentInfoType, currentInfoLcdRow, creditsInfoCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP || joyMoveDirection == JOY_MOVED_DOWN) {
    lastInfoType = currentInfoType;

    if (joyMoveDirection == JOY_MOVED_UP) {
      LCDUtils::attemptToMoveUp(CREDITS, currentInfoType, currentInfoLcdRow, gameAssets, sounds);
    } else {
      LCDUtils::attemptToMoveDown(CREDITS, currentInfoType, currentInfoLcdRow, creditsInfoCount, gameAssets, sounds);
    }

    if (currentInfoType != lastInfoType) {
      currentInfoLetter = 0;
      timeSinceLastScroll = millis() - scrollDelay;
    }
  } else if (joyMoveDirection == JOY_MOVED_LEFT) {
    sounds.attemptToPlayMenuNavSound();
    LCDUtils::goToPreviousMenu(gameAssets);
    creditsInit = false;
  }
}

void Credits::showInfo(byte infoType, byte infoCursorRow, byte infoCursorCol, GameAssets &gameAssets) {
  gameAssets.lcdSetCursor(infoCursorCol, infoCursorRow);

  byte lettersToPrintCount = lcdRowLength - (1 + strlen(creditsInfoTypes[infoType]) + 3); // 1 = first space of LCD line; 3 = 1 (the ":" after the info type) + 1 (the space before the navigation arrow) + 1 (the navigation arrow)
  if (infoType == currentInfoType) {
    if (millis() - timeSinceLastScroll > scrollDelay) {
      byte infoLength = strlen(creditsInfo[infoType]);
      for (int i = 0; i < lettersToPrintCount; ++i) {
        gameAssets.lcdWrite(creditsInfo[infoType][(currentInfoLetter + i) % infoLength]);
      }

      ++currentInfoLetter;
      if (currentInfoLetter == infoLength) {
        currentInfoLetter = 0;
      }

      timeSinceLastScroll = millis();
    }
  } else {
    for (int letter = 0; letter < lettersToPrintCount; ++letter) {
      gameAssets.lcdWrite(creditsInfo[infoType][letter]);
    }
  }
}