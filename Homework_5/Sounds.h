#pragma once

class Sounds {
  private:
    bool buzzerEnabled;
    int buzzerTone;
    int buzzerOnDuration;

  public:
    Sounds();
    
    void setBuzzerEnabled(int); 

    void attemptToPlayMenuNavSound();
    void attemptToPlayInteractWithMenuOptionSound();
};