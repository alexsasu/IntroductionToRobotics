#include "Controls.h"
#include "Credits.h"
#include "Game.h"
#include "GameAssets.h"
#include "Instructions.h"
#include "LCDUtils.h"
#include "Leaderboard.h"
#include "MainMenu.h"
#include "MatrixUtils.h"
#include "Settings.h"
#include "Sounds.h"
#include "StartingScreen.h"

Controls controls;
Credits credits;
Game game;
GameAssets gameAssets;
Instructions instructions;
Leaderboard leaderboard;
MainMenu mainMenu;
Settings settings;
Sounds sounds;
StartingScreen startingScreen;

int applicationState = UNINITIALIZED;

void setup() {
  Serial.begin(9600);
}

void loop() {
  decideStateExecution();
}

void decideStateExecution() {
  switch (applicationState) {
    case UNINITIALIZED:
      initializeGame();
      break;
    case STARTING_SCREEN:
      displayStartingScreen();
      break;
    case MAIN_MENU:
      utilizeMainMenu();
      break;
    case GAME_STARTED:
      commenceGame();
      break;
    case INSTRUCTIONS:
      displayInstructions();
      break;
    case LEADERBOARD:
      displayLeaderboard();
      break;
    case SETTINGS:
      utilizeSettings();
      break;
    case CREDITS:
      displayCredits();
      break;
    default:
      break;
  }
}

void initializeGame() {
  gameAssets.initializeGameAssets();

  applicationState = STARTING_SCREEN;
}

void displayStartingScreen() {
  if (!startingScreen.getStartingScreenInit()) {
    startingScreen.displayStartingScreen(gameAssets);
  } else if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();
    gameAssets.lcdClear();

    LCDUtils::initializeUpDownNavArrows(gameAssets);

    applicationState = MAIN_MENU;
  }
}

void utilizeMainMenu() {
  if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();
    gameAssets.lcdClear();

    if (!strcmp((char*)pgm_read_word(&mainMenuOptions[mainMenu.getCurrentOption()]), "Start game")) {
      applicationState = GAME_STARTED;
    } else if (!strcmp((char*)pgm_read_word(&mainMenuOptions[mainMenu.getCurrentOption()]), "How to play")) {
      applicationState = INSTRUCTIONS;
    } else if (!strcmp((char*)pgm_read_word(&mainMenuOptions[mainMenu.getCurrentOption()]), "Leaderboard")) {
      applicationState = LEADERBOARD;
    } else if (!strcmp((char*)pgm_read_word(&mainMenuOptions[mainMenu.getCurrentOption()]), "Settings")) {
      applicationState = SETTINGS;
    } else if (!strcmp((char*)pgm_read_word(&mainMenuOptions[mainMenu.getCurrentOption()]), "Credits")) {
      applicationState = CREDITS;
    }
  } else {
    mainMenu.displayMainMenu(gameAssets, controls, sounds);
  }
}

void commenceGame() {
  if (!LCDUtils::goBack) {
    if (!game.getGameLoaded()) {
      game.loadGame(gameAssets, settings);
    } else if (game.getGameInProgress()) {
      game.carryOutGame(gameAssets, controls);
    } else if (game.getGameFinished()) {
      game.endGame(gameAssets, leaderboard, controls, sounds);
    }
  } else {
    applicationState = MAIN_MENU;
    LCDUtils::goBack = false;
  }
}

void displayInstructions() {
  if (!LCDUtils::goBack) {
    if (!instructions.getInstructionsInit()) {
      instructions.initializeInstructions(gameAssets);
    } else {
      instructions.showInstructions(gameAssets, controls, sounds);
    }
  } else {
    MatrixUtils::clearMatrix(gameAssets);
    applicationState = MAIN_MENU;
    LCDUtils::goBack = false;
  }
}

void displayLeaderboard() {
  if (!LCDUtils::goBack) {
    if (!leaderboard.getLeaderboardInit()) {
      leaderboard.initializeLeaderboard(gameAssets);
    } else {
      leaderboard.showLeaderboard(gameAssets, controls, sounds);
    }
  } else {
    MatrixUtils::clearMatrix(gameAssets);
    applicationState = MAIN_MENU;
    LCDUtils::goBack = false;
  }
}

void utilizeSettings() {
  if (!LCDUtils::goBack) {
    if (!settings.getSettingsInit()) {
      settings.initializeSettings(gameAssets);
    } else if (!settings.getOptionWasSelected()) {
      settings.showSettings(gameAssets, controls, sounds);
    } else {
      settings.modifySettings(gameAssets, controls, leaderboard, sounds);
    }
  } else {
    MatrixUtils::clearMatrix(gameAssets);
    applicationState = MAIN_MENU;
    LCDUtils::goBack = false;
  }
}

void displayCredits() {
  if (!LCDUtils::goBack) {
    if (!credits.getCreditsInit()) {
      credits.initializeCredits(gameAssets);
    } else {
      credits.showCredits(gameAssets, controls, sounds);
    }
  } else {
    MatrixUtils::clearMatrix(gameAssets);
    applicationState = MAIN_MENU;
    LCDUtils::goBack = false;
  }
}