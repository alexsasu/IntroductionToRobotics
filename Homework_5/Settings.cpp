#include <EEPROM.h>
#include "LCDUtils.h"
#include "MatrixUtils.h"
#include "Settings.h"
#include <string.h>
#include <stdlib.h>

Settings::Settings() {
  settingsInit = false;
  
  // Initialize settings values
  strcpy(playerName, "AAAAA");
  currentDifficulty = 0;
  strcpy(difficulty, difficultyOptions[currentDifficulty]);
  lcdBrightness = EEPROM.read(eepromLcdBrightnessAddress);
  matrixIntensity = EEPROM.read(eepromMatrixLedIntensityAddress);
  if (EEPROM.read(eepromBuzzerSoundAddress)) {
    strcpy(soundsStatus, "ON ");
  } else {
    strcpy(soundsStatus, "OFF");
  }
}

bool Settings::getSettingsInit() {
  return settingsInit;
}

bool Settings::getOptionWasSelected() {
  return optionWasSelected;
}

char* Settings::getPlayerName() {
  return playerName;
}

char* Settings::getDifficulty() {
  return difficulty;
}

void Settings::initializeSettings(GameAssets &gameAssets) {
  MatrixUtils::displayMatrixIcon(gameAssets, matrixSettingsIcon);

  currentOption = 0;
  currentOptionLcdRow = 0;
  optionWasSelected = false;

  settingsInit = true;
}

void Settings::showSettings(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte currentOptionCursorCol = 0;
  gameAssets.lcdSetCursor(currentOptionCursorCol, currentOptionLcdRow);
  gameAssets.lcdWrite('>');
  gameAssets.lcdPrint((char*)pgm_read_word(&settingsOptions[currentOption]));
  if (strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Del. hiscores")) {
    gameAssets.lcdWrite(':');
  }
  showOptionValue(currentOption, currentOptionLcdRow, gameAssets);

  byte otherOption = !currentOptionLcdRow ? currentOption + 1 : currentOption - 1;
  byte otherOptionCursorCol = 1;
  byte otherOptionLcdRow = 1 - currentOptionLcdRow;
  gameAssets.lcdSetCursor(otherOptionCursorCol, otherOptionLcdRow);
  gameAssets.lcdPrint((char*)pgm_read_word(&settingsOptions[otherOption]));
  if (strcmp((char*)pgm_read_word(&settingsOptions[otherOption]), "Del. hiscores")) {
    gameAssets.lcdWrite(':');
  }
  showOptionValue(otherOption, otherOptionLcdRow, gameAssets);

  LCDUtils::displayUpDownNavArrows(currentOption, currentOptionLcdRow, settingsOptionsCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(SETTINGS, currentOption, currentOptionLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(SETTINGS, currentOption, currentOptionLcdRow, settingsOptionsCount, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_LEFT) {
    sounds.attemptToPlayMenuNavSound();
    LCDUtils::goToPreviousMenu(gameAssets);
    settingsInit = false;
  }

  if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();

    optionWasSelected = true;

    playerNameCurrentLetterPos = 0;
    
    if (strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Del. hiscores")) {
      byte currentOptionCursorCol = 0;
      gameAssets.lcdSetCursor(currentOptionCursorCol, currentOptionLcdRow);
      gameAssets.lcdWrite('-');
    }
  }
}

void Settings::showOptionValue(byte option, byte optionCursorRow, GameAssets &gameAssets) {
  byte optionCursorCol = strlen((char*)pgm_read_word(&settingsOptions[option])) + 2; // 2 = 1 (the ":") + 1 (the space right after the ":")
  gameAssets.lcdSetCursor(optionCursorCol, optionCursorRow);

  if (!strcmp((char*)pgm_read_word(&settingsOptions[option]), "Player")) {
    gameAssets.lcdPrint(playerName);
  } else if (!strcmp((char*)pgm_read_word(&settingsOptions[option]), "Level")) {
    gameAssets.lcdPrint(difficulty);
  } else if (!strcmp((char*)pgm_read_word(&settingsOptions[option]), "LCD bright.")) {
    byte mappedLcdBrightness = map(lcdBrightness, minLcdBrightness, maxLcdBrightness, minLcdMappedBrightness, maxLcdMappedBrightness);
    char lcdBrightnessString[MAX_MENU_OPTION_LENGTH + 1];
    byte numericalBase = 10;
    itoa(mappedLcdBrightness, lcdBrightnessString, numericalBase);

    gameAssets.lcdPrint(lcdBrightnessString);
  } else if (!strcmp((char*)pgm_read_word(&settingsOptions[option]), "Mat bright.")) {
    byte mappedMatrixIntensity = map(matrixIntensity, minMatrixIntensity, maxMatrixIntensity, minMatrixMappedIntensity, maxMatrixMappedIntensity);
    char matrixIntensityString[MAX_MENU_OPTION_LENGTH + 1];
    byte numericalBase = 10;
    itoa(mappedMatrixIntensity, matrixIntensityString, numericalBase);
    
    gameAssets.lcdPrint(matrixIntensityString);
  } else if (!strcmp((char*)pgm_read_word(&settingsOptions[option]), "Sounds")) {
    gameAssets.lcdPrint(soundsStatus);
  }
}

void Settings::modifySettings(GameAssets &gameAssets, Controls &controls, Leaderboard &leaderboard, Sounds &sounds) {
  showOptionValue(currentOption, currentOptionLcdRow, gameAssets);

  if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();
    
    optionWasSelected = false;

    // Hide the position of the currently selected player name letter
    if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Player")) {
      byte cursorCol = strlen((char*)pgm_read_word(&settingsOptions[currentOption])) + 2 + playerNameLength + 1;
      gameAssets.lcdSetCursor(cursorCol, currentOptionLcdRow);
      gameAssets.lcdWrite(' ');
    }
  } else {
    if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Player")) {
      attemptToModifyPlayerName(gameAssets, controls);
    } else if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Level")) {
      attemptToModifyDifficulty(controls);
    } else if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Del. hiscores")) {
      attemptToResetLeaderboard(gameAssets, controls, leaderboard, sounds);
    } else if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "LCD bright.")) {
      attemptToModifyLcdBrightness(gameAssets, controls);
    } else if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Mat bright.")) {
      attemptToModifyMatrixIntensity(gameAssets, controls);
    } else if (!strcmp((char*)pgm_read_word(&settingsOptions[currentOption]), "Sounds")) {
      attemptToModifySounds(controls, sounds);
    }
  }
}

void Settings::attemptToModifyPlayerName(GameAssets &gameAssets, Controls &controls) {
  byte currentOptionCursorCol = strlen((char*)pgm_read_word(&settingsOptions[currentOption])) + 2 + playerNameLength + 1;
  gameAssets.lcdSetCursor(currentOptionCursorCol, currentOptionLcdRow);
  gameAssets.lcdWrite(48 + playerNameCurrentLetterPos + 1);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP && playerName[playerNameCurrentLetterPos] != 'Z') {
    ++playerName[playerNameCurrentLetterPos];
  } else if (joyMoveDirection == JOY_MOVED_DOWN && playerName[playerNameCurrentLetterPos] != 'A') {
    --playerName[playerNameCurrentLetterPos];
  } else if (joyMoveDirection == JOY_MOVED_LEFT && playerNameCurrentLetterPos != 0) {
    --playerNameCurrentLetterPos;
  } else if (joyMoveDirection == JOY_MOVED_RIGHT && playerNameCurrentLetterPos != playerNameLength - 1) {
    ++playerNameCurrentLetterPos;
  }
}

void Settings::attemptToModifyDifficulty(Controls &controls) {
  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP && strcmp(difficulty, "Hard  ")) {
    ++currentDifficulty;
    strcpy(difficulty, difficultyOptions[currentDifficulty]);
  } else if (joyMoveDirection == JOY_MOVED_DOWN && strcmp(difficulty, "Easy  ")) {
    --currentDifficulty;
    strcpy(difficulty, difficultyOptions[currentDifficulty]);
  }
}

void Settings::attemptToResetLeaderboard(GameAssets &gameAssets, Controls &controls, Leaderboard &leaderboard, Sounds &sounds) {
  gameAssets.lcdClear();
  sounds.attemptToPlayInteractWithMenuOptionSound();

  leaderboard.resetLeaderboard(gameAssets);
  optionWasSelected = false;
}

void Settings::attemptToModifyLcdBrightness(GameAssets &gameAssets, Controls &controls) {
  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP && lcdBrightness < maxLcdBrightness) {
    lcdBrightness += lcdBrightnessIncrement;
    gameAssets.lcdSetBrightness(lcdBrightness);
    EEPROM.update(eepromLcdBrightnessAddress, lcdBrightness);
  } else if (joyMoveDirection == JOY_MOVED_DOWN && lcdBrightness > minLcdBrightness) {
    lcdBrightness -= lcdBrightnessIncrement;
    gameAssets.lcdSetBrightness(lcdBrightness);
    EEPROM.update(eepromLcdBrightnessAddress, lcdBrightness);
  }
}

void Settings::attemptToModifyMatrixIntensity(GameAssets &gameAssets, Controls &controls) {
  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP && matrixIntensity < maxMatrixIntensity) {
    matrixIntensity += matrixIntensityIncrement;
    gameAssets.matrixSetIntensity(0, matrixIntensity);
    EEPROM.update(eepromMatrixLedIntensityAddress, matrixIntensity);
  } else if (joyMoveDirection == JOY_MOVED_DOWN && matrixIntensity > minMatrixIntensity) {
    matrixIntensity -= matrixIntensityIncrement;
    gameAssets.matrixSetIntensity(0, matrixIntensity);
    EEPROM.update(eepromMatrixLedIntensityAddress, matrixIntensity);
  }
}

void Settings::attemptToModifySounds(Controls &controls, Sounds &sounds) {
  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP && !strcmp(soundsStatus, "OFF")) {
    strcpy(soundsStatus, "ON ");
    byte soundsOn = 1;
    sounds.setBuzzerEnabled(soundsOn);
    EEPROM.update(eepromBuzzerSoundAddress, soundsOn);
  } else if (joyMoveDirection == JOY_MOVED_DOWN && !strcmp(soundsStatus, "ON ")) {
    strcpy(soundsStatus, "OFF");
    byte soundsOn = 0;
    sounds.setBuzzerEnabled(soundsOn);
    EEPROM.update(eepromBuzzerSoundAddress, soundsOn);
  }
}