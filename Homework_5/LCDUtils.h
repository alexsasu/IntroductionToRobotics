#pragma once
#include "Arduino.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Sounds.h"

class LCDUtils {
  private:
    LCDUtils();

  public:
    static bool goBack;

    static void initializeUpDownNavArrows(GameAssets&);
    static void displayUpDownNavArrows(byte, byte, byte, GameAssets&);
    static void initializeGameStatsIcons(GameAssets&);
    static void displayLcdIcon(byte, GameAssets&);
    static void attemptToMoveUp(byte, int&, byte&, GameAssets&, Sounds&);
    static void attemptToMoveDown(byte, int&, byte&, byte, GameAssets&, Sounds&);
    static void goToPreviousMenu(GameAssets&);
};