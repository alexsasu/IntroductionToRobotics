#pragma once
#include "Consts.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Leaderboard.h"
#include "Settings.h"

class Game {
  private:
    // Game states
    bool gameLoaded;
    bool gameInProgress;
    bool gameFinished;

    // Game mechanics
    byte matrix[matrixSize][matrixSize];
    bool matrixChanged = true;
    byte xPlayerPos;
    byte yPlayerPos;
    byte xLastPlayerPos;
    byte yLastPlayerPos;
    unsigned long playerLastMoveTime;
    byte xFoodPos;
    byte yFoodPos;
    byte xLastFoodPos;
    byte yLastFoodPos;
    byte foodLedState;
    unsigned long foodLastBlink;
    byte foodBlinkInterval;
    unsigned long programTime;

    // In-game stats
    char playerName[playerNameLength + 1];
    char difficulty;
    int score;
    byte lives;

    // Post game info
    int currentInfo;
    byte currentInfoLcdRow;
    bool postGameInfoDisplayed;
    bool postGameInfoInit;
    char postGameInfo[postGameInfoLinesCount][postGameInfoMaxLettersPerLine + 1];
    int playersBestedCount;

  public:
    Game();

    bool getGameLoaded();
    bool getGameInProgress();
    bool getGameFinished();

    void loadGame(GameAssets&, Settings&);
    void carryOutGame(GameAssets&, Controls&);
    void displayGameStats(GameAssets&);
    void highlightFood(GameAssets&);
    void updateMatrix(GameAssets&);
    void updatePositions();
    void generateFood();
    void endGame(GameAssets&, Leaderboard&, Controls&, Sounds&);
    void initializePostGameInfo(int);
    void showPostGameInfo(GameAssets&, Controls&, Sounds&);
};