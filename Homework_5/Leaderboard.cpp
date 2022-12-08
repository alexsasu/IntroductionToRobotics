#include "EEPROM.h"
#include "EEPROMUtils.h"
#include "LCDUtils.h"
#include "Leaderboard.h"
#include "Macros.h"
#include "MatrixUtils.h"

Leaderboard::Leaderboard() {
  leaderboardInit = false;
}

bool Leaderboard::getLeaderboardInit() {
  return leaderboardInit;
}

void Leaderboard::initializeLeaderboard(GameAssets &gameAssets) {
  MatrixUtils::displayMatrixIcon(gameAssets, matrixLeaderboardIcon);

  currentHighscore = 0;
  currentHighscoreLcdRow = 0;

  readLeaderboardDataFromEeprom();

  leaderboardInit = true;
}

void Leaderboard::readLeaderboardDataFromEeprom() {
  highscoresCount = EEPROM.read(eepromHighscoresCountAddress);
  readFromEeprom(eepromHighscoresCountAddress + 1, highscores);
}

void Leaderboard::showLeaderboard(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte cursorCol = 1;
  
  if (currentHighscore < highscoresCount) {
    showHighscore(currentHighscore, currentHighscoreLcdRow, gameAssets);
  }

  byte otherHighscore = !currentHighscoreLcdRow ? currentHighscore + 1 : currentHighscore - 1;
  if (otherHighscore < highscoresCount) {
    byte otherHighscoreLcdRow = 1 - currentHighscoreLcdRow;
    showHighscore(otherHighscore, otherHighscoreLcdRow, gameAssets);
  }

  LCDUtils::displayUpDownNavArrows(currentHighscore, currentHighscoreLcdRow, highscoresCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(LEADERBOARD, currentHighscore, currentHighscoreLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(LEADERBOARD, currentHighscore, currentHighscoreLcdRow, highscoresCount, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_LEFT) {
    sounds.attemptToPlayMenuNavSound();

    leaderboardInit = false;
    currentHighscore = 0;
    currentHighscoreLcdRow = 0;
    
    LCDUtils::goToPreviousMenu(gameAssets);
  }
}

void Leaderboard::showHighscore(byte highscore, byte highscoreLcdRow, GameAssets& gameAssets) {
  byte cursorCol = 0;
  gameAssets.lcdSetCursor(cursorCol, highscoreLcdRow);
  if (highscore == 9) {
    gameAssets.lcdPrint("10");
  } else {
    gameAssets.lcdWrite(' ');
    gameAssets.lcdWrite(48 + highscore + 1);
  }
  gameAssets.lcdPrint(". ");

  cursorCol = 4;
  gameAssets.lcdSetCursor(cursorCol, highscoreLcdRow);
  gameAssets.lcdPrint(highscores[highscore].playerName);

  gameAssets.lcdWrite(' ');

  char scoreString[MAX_SCORE_LENGTH + 1];
  byte numericalBase = 10;
  itoa(highscores[highscore].value, scoreString, numericalBase);
  gameAssets.lcdPrint(scoreString);
}

int Leaderboard::attemptToUpdateLeaderboard(char* playerName, int score) {
  int highscorePos;
  for (highscorePos = 0; highscorePos < highscoresCount; ++highscorePos) {
    if (score > highscores[highscorePos].value) {
      break;
    }
  }

  if (highscorePos == highscoresCount) {
    if (highscoresCount < maxHighscoresCount) {
      ++highscoresCount;

      strcpy(highscores[highscoresCount - 1].playerName, playerName);
      highscores[highscoresCount - 1].value = score;

      saveToEeprom(eepromHighscoresCountAddress, highscoresCount);
      saveToEeprom(eepromHighscoresCountAddress + 1, highscores);

      return 0;
    } else {
      return -1;
    }
  } else {
    if (highscoresCount < maxHighscoresCount) {
      ++highscoresCount;
    }

    for (int i = highscoresCount - 1; i > highscorePos; --i) {
      strcpy(highscores[i].playerName, highscores[i - 1].playerName);
      highscores[i].value = highscores[i - 1].value;
    }
    strcpy(highscores[highscorePos].playerName, playerName);
    highscores[highscorePos].value = score;

    saveToEeprom(eepromHighscoresCountAddress, highscoresCount);
    saveToEeprom(eepromHighscoresCountAddress + 1, highscores);

    if (highscoresCount < maxHighscoresCount) {
      return highscoresCount - highscorePos - 1;
    } else {
      return highscoresCount - highscorePos;
    }
  }
}

void Leaderboard::resetLeaderboard(GameAssets& gameAssets) {
  EEPROM.update(eepromHighscoresCountAddress, 0);

  byte cursorCol = 2, cursorRow = 0;
  
  gameAssets.lcdSetCursor(cursorCol, cursorRow);
  gameAssets.lcdPrint("Leaderboard");
  cursorCol = 5;
  cursorRow = 1;
  gameAssets.lcdSetCursor(cursorCol, cursorRow);
  gameAssets.lcdPrint("reset!");

  delay(resetHighscoresMessageDuration);

  gameAssets.lcdClear();
}