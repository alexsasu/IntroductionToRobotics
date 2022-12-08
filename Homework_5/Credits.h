#pragma once
#include "Controls.h"
#include "GameAssets.h"
#include "Sounds.h"

class Credits {
  private:
    bool creditsInit;
    int currentInfoType;
    byte lastInfoType;
    byte currentInfoLcdRow;
    byte currentInfoLetter;
    unsigned long timeSinceLastScroll;
  
  public:
    Credits();

    bool getCreditsInit();

    void initializeCredits(GameAssets&);
    void showCredits(GameAssets&, Controls&, Sounds&);
    void showInfo(byte, byte, byte, GameAssets&);
};