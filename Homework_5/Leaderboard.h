#pragma once
#include "Consts.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Highscore.h"
#include "Sounds.h"

class Leaderboard {
  private:
    bool leaderboardInit;
    int currentHighscore;
    byte currentHighscoreLcdRow;
    byte highscoresCount;
    Highscore highscores[maxHighscoresCount];

  public:
    Leaderboard();

    bool getLeaderboardInit();

    void initializeLeaderboard(GameAssets&);
    void readLeaderboardDataFromEeprom();
    void showLeaderboard(GameAssets&, Controls&, Sounds&);
    void showHighscore(byte, byte, GameAssets&);
    int attemptToUpdateLeaderboard(char*, int);
    void resetLeaderboard(GameAssets&);
};