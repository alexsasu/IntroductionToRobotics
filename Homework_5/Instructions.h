#pragma once
#include "Controls.h"
#include "GameAssets.h"
#include "Sounds.h"

class Instructions {
  private:
    bool instructionsInit;
    int currentInstruction;
    byte currentInstructionLcdRow;

  public:
    Instructions();

    bool getInstructionsInit();

    void initializeInstructions(GameAssets&);
    void showInstructions(GameAssets&, Controls&, Sounds&);
    void showInstructionLine(byte, GameAssets&);
};