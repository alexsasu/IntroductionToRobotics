#pragma once
#include "Arduino.h"
#include "Consts.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Leaderboard.h"
#include "Macros.h"
#include "Sounds.h"

class Settings {
  private:
    bool settingsInit;
    int currentOption;
    byte currentOptionLcdRow;
    bool optionWasSelected;

    char playerName[MAX_MENU_OPTION_LENGTH + 1];
    byte playerNameCurrentLetterPos;
    byte currentDifficulty;
    char difficulty[MAX_MENU_OPTION_LENGTH + 1];
    int lcdBrightness;
    byte matrixIntensity;
    char soundsStatus[MAX_MENU_OPTION_LENGTH + 1];
  
  public:
    Settings();

    bool getSettingsInit();
    bool getOptionWasSelected();
    char* getPlayerName();
    char* getDifficulty();

    void initializeSettings(GameAssets&);
    void showSettings(GameAssets&, Controls&, Sounds&);
    void showOptionValue(byte, byte, GameAssets&);
    void modifySettings(GameAssets&, Controls&, Leaderboard&, Sounds&);
    void attemptToModifyPlayerName(GameAssets&, Controls&);
    void attemptToModifyDifficulty(Controls&);
    void attemptToResetLeaderboard(GameAssets&, Controls&, Leaderboard&, Sounds&);
    void attemptToModifyLcdBrightness(GameAssets&, Controls&);
    void attemptToModifyMatrixIntensity(GameAssets&, Controls&);
    void attemptToModifySounds(Controls&, Sounds&);
    void optionIsSelected(GameAssets&);
};