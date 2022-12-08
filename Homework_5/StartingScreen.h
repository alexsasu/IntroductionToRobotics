#pragma once
#include "GameAssets.h"

class StartingScreen {
  private:
    bool startingScreenInit;

  public:
    StartingScreen();

    bool getStartingScreenInit();

    void displayStartingScreen(GameAssets&);
};