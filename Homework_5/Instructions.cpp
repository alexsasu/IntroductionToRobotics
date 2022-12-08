#include "Consts.h"
#include "GameAssets.h"
#include "Instructions.h"
#include "LCDUtils.h"
#include "Macros.h"
#include "MatrixUtils.h"

Instructions::Instructions() {
  instructionsInit = false;
}

bool Instructions::getInstructionsInit() {
  return instructionsInit;
}

void Instructions::initializeInstructions(GameAssets &gameAssets) {
  MatrixUtils::displayMatrixIcon(gameAssets, matrixInstructionsIcon);

  currentInstruction = 0;
  currentInstructionLcdRow = 0;

  instructionsInit = true;
}

void Instructions::showInstructions(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte cursorCol = 1;
  
  gameAssets.lcdSetCursor(cursorCol, currentInstructionLcdRow);
  showInstructionLine(currentInstruction, gameAssets);

  byte otherInstruction = !currentInstructionLcdRow ? currentInstruction + 1 : currentInstruction - 1;
  byte otherInstructionLcdRow = 1 - currentInstructionLcdRow;
  gameAssets.lcdSetCursor(cursorCol, otherInstructionLcdRow);
  showInstructionLine(otherInstruction, gameAssets);

  LCDUtils::displayUpDownNavArrows(currentInstruction, currentInstructionLcdRow, instructionLinesCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(INSTRUCTIONS, currentInstruction, currentInstructionLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(INSTRUCTIONS, currentInstruction, currentInstructionLcdRow, instructionLinesCount, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_LEFT) {
    sounds.attemptToPlayMenuNavSound();

    instructionsInit = false;
    currentInstruction = 0;
    currentInstructionLcdRow = 0;
    
    LCDUtils::goToPreviousMenu(gameAssets);
  }
}

void Instructions::showInstructionLine(byte instructionLine, GameAssets &gameAssets) {
  for (int i = instructionLine * instructionCharactersPerLine; i < instructionCharactersPerLine * (instructionLine + 1); ++i) {
    if (i >= instructionsLength) {
      gameAssets.lcdWrite(' ');
    } else {
      gameAssets.lcdWrite(pgm_read_byte_near(instructionsString + i));
    }
  }
}