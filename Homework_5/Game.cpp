#include "Game.h"
#include "LCDUtils.h"
#include "Macros.h"
#include "MatrixUtils.h"

Game::Game() {
  gameLoaded = false;
  gameInProgress = false;
}

bool Game::getGameLoaded() {
  return gameLoaded;
}

bool Game::getGameInProgress() {
  return gameInProgress;
}

bool Game::getGameFinished() {
  return gameFinished;
}

void Game::loadGame(GameAssets &gameAssets, Settings &settings) {
  gameAssets.lcdClear();
  gameAssets.lcdSetCursor(6, 1);
  gameAssets.lcdPrint("loading...");

  // Animation for loading the game
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      gameAssets.matrixSetLed(0, row, col, true);
      delay(20);
    }
  }

  LCDUtils::initializeGameStatsIcons(gameAssets);

  // Game mechanics initialization
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      matrix[row][col] = 0;
    }
  }
  matrixChanged = true;
  xPlayerPos = 0;
  yPlayerPos = 0;
  matrix[xPlayerPos][yPlayerPos] = 1;
  xLastPlayerPos = 0;
  yLastPlayerPos = 0;
  playerLastMoveTime = 0;
  randomSeed(analogRead(seedGenerator));
  xFoodPos = random(matrixSize);
  yFoodPos = random(matrixSize);
  xLastFoodPos = 0;
  yLastFoodPos = 0;
  foodLedState = 0;
  foodLastBlink = 0;
  foodBlinkInterval = 100;
  programTime = 0;

  // Game stats initialization
  strcpy(playerName, settings.getPlayerName());
  difficulty = settings.getDifficulty()[0];
  score = 0;
  lives = maxLivesCount;

  // Post game info mechanics initialization
  postGameInfoDisplayed = false;
  postGameInfoInit = false;
  playersBestedCount = 0;

  // Animation for finishing the loading
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      if ((row != xPlayerPos || col != yPlayerPos) &&
          (row != xFoodPos || col != yFoodPos)) {
            gameAssets.matrixSetLed(0, row, col, false);
          }

      delay(20);
    }
  }

  gameLoaded = true;
  gameInProgress = true;
  gameFinished = false;
  gameAssets.lcdClear();
}

void Game::carryOutGame(GameAssets &gameAssets, Controls &controls) {
  displayGameStats(gameAssets);

  if (millis() - playerLastMoveTime > joyMoveInterval) {
    // game logic
    updatePositions();
    playerLastMoveTime = millis();
  }
  highlightFood(gameAssets);
  if (matrixChanged == true) {
    // matrix display logic
    updateMatrix(gameAssets);
    matrixChanged = false;
  }

  if (controls.joySWChangeState()) {
    gameInProgress = false;
    gameFinished = true;
  }
}

void Game::displayGameStats(GameAssets &gameAssets) {
  gameAssets.lcdSetCursor(0, 0);
  gameAssets.lcdPrint("Player:");
  gameAssets.lcdPrint(playerName);

  gameAssets.lcdSetCursor(0, 1);
  char scoreString[MAX_SCORE_LENGTH + 1];
  byte numericalBase = 10;
  itoa(score, scoreString, numericalBase);
  gameAssets.lcdPrint("S:");
  gameAssets.lcdPrint(scoreString);
  
  gameAssets.lcdSetCursor(MAX_SCORE_LENGTH + 3, 1); // 3 = 2 (the "S:" string) + 1 (the space right after the score)

  LCDUtils::displayLcdIcon(gameStatsLivesIconAddr, gameAssets);
  gameAssets.lcdWrite(':');
  gameAssets.lcdWrite(48 + lives);

  gameAssets.lcdWrite(' ');

  LCDUtils::displayLcdIcon(gameStatsDifficultyIconAddr, gameAssets);
  gameAssets.lcdWrite(':');
  gameAssets.lcdWrite(difficulty);
}

void Game::highlightFood(GameAssets &gameAssets) {
  programTime = millis();
  if (programTime - foodLastBlink > foodBlinkInterval) {
    foodLastBlink = programTime;

    foodLedState = !foodLedState;
    matrix[xFoodPos][yFoodPos] = foodLedState;
    gameAssets.matrixSetLed(0, xFoodPos, yFoodPos, matrix[xFoodPos][yFoodPos]);
  }
}

void Game::updateMatrix(GameAssets &gameAssets) {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      gameAssets.matrixSetLed(0, row, col, matrix[row][col]);
    }
  }
}

void Game::updatePositions() {
  int xValue = analogRead(joyXPin);
  int yValue = analogRead(joyYPin);
  xLastPlayerPos = xPlayerPos;
  yLastPlayerPos = yPlayerPos;
  if (xValue < joyMinThreshold) {
    if (xPlayerPos < matrixSize - 1) {
      xPlayerPos++;
    } else {
      xPlayerPos = 0;
    }
  }
  if (xValue > joyMaxThreshold) {
    if (xPlayerPos > 0) {
      xPlayerPos--;
    } else {
      xPlayerPos = matrixSize - 1;
    }
  }
  if (yValue > joyMaxThreshold) {
    if (yPlayerPos < matrixSize - 1) {
      yPlayerPos++;
    } else {
      yPlayerPos = 0;
    }
  }
  if (yValue < joyMinThreshold) {
    if (yPlayerPos > 0) {
      yPlayerPos--;
    } else {
      yPlayerPos = matrixSize - 1;
    }
  }
  if (xPlayerPos != xLastPlayerPos || yPlayerPos != yLastPlayerPos) {
    matrixChanged = true;
    matrix[xLastPlayerPos][yLastPlayerPos] = 0;
    matrix[xPlayerPos][yPlayerPos] = 1;

    if (xPlayerPos == xFoodPos && yPlayerPos == yFoodPos) {
      generateFood();
    }
  }
}

void Game::generateFood() {
  ++score;
  xLastFoodPos = xFoodPos;
  yLastFoodPos = yFoodPos;

  while (xFoodPos == xLastFoodPos && yFoodPos == yLastFoodPos) {
    xFoodPos = random(matrixSize);
    yFoodPos = random(matrixSize);
  }
}

void Game::endGame(GameAssets &gameAssets, Leaderboard &leaderboard, Controls &controls, Sounds &sounds) {
  if (!postGameInfoDisplayed) {
    MatrixUtils::clearMatrix(gameAssets);
    gameAssets.lcdClear();

    byte cursorCol = 3;
    byte cursorLine = 0;

    gameAssets.lcdSetCursor(cursorCol, cursorLine);
    gameAssets.lcdPrint("GAME OVER!");
    delay(endGameNotificationDelay);

    leaderboard.readLeaderboardDataFromEeprom();
    playersBestedCount = leaderboard.attemptToUpdateLeaderboard(playerName, score);

    gameAssets.lcdClear();

    if (playersBestedCount > 0) {
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      gameAssets.lcdPrint("You bested");

      cursorCol = 2;
      cursorLine = 1;
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      char playersBestedCountString[int(log(playersBestedCount)) + 2];
      byte numericalBase = 10;
      itoa(playersBestedCount, playersBestedCountString, numericalBase);
      gameAssets.lcdPrint(playersBestedCountString);
      gameAssets.lcdPrint(" player(s)!");

      delay(endGameNotificationDelay);

      gameAssets.lcdClear();
    }

    postGameInfoDisplayed = true;
  } else {
    if (!postGameInfoInit) {
      initializePostGameInfo(playersBestedCount);
    } else {
      showPostGameInfo(gameAssets, controls, sounds);
    }
  }
}

void Game::initializePostGameInfo(int playersBestedCount) {
  if (playersBestedCount != -1) {
    strcpy(postGameInfo[0], "      NEW     ");
    strcpy(postGameInfo[1], "   HIGHSCORE  ");
  } else {
    strcpy(postGameInfo[0], "     NO NEW   ");
    strcpy(postGameInfo[1], "  HIGHSCORE :(");
  }

  strcpy(postGameInfo[2], "Your stats:   ");

  strcpy(postGameInfo[3], "*Score: ");
  char scoreString[MAX_SCORE_LENGTH + 1];
  byte numericalBase = 10;
  itoa(score, scoreString, numericalBase);
  strcat(postGameInfo[3], scoreString);

  strcpy(postGameInfo[4], "*Lives left: ");
  char livesString[int(log(maxLivesCount)) + 2];
  itoa(lives, livesString, numericalBase);
  strcat(postGameInfo[4], livesString);

  strcpy(postGameInfo[5], "[JS] Main Menu");

  currentInfo = 0;
  currentInfoLcdRow = 0;
  
  postGameInfoInit = true;
}

void Game::showPostGameInfo(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte cursorCol = 0;
  
  gameAssets.lcdSetCursor(cursorCol, currentInfoLcdRow);
  gameAssets.lcdPrint(postGameInfo[currentInfo]);

  byte otherInfo = !currentInfoLcdRow ? currentInfo + 1 : currentInfo - 1;
  byte otherInfoLcdRow = 1 - currentInfoLcdRow;
  gameAssets.lcdSetCursor(cursorCol, otherInfoLcdRow);
  gameAssets.lcdPrint(postGameInfo[otherInfo]);

  LCDUtils::displayUpDownNavArrows(currentInfo, currentInfoLcdRow, postGameInfoLinesCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(GAME_FINISHED, currentInfo, currentInfoLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(GAME_FINISHED, currentInfo, currentInfoLcdRow, postGameInfoLinesCount, gameAssets, sounds);
  }

  if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();

    LCDUtils::goToPreviousMenu(gameAssets);

    gameFinished = false;
    gameLoaded = false;
  }  
}