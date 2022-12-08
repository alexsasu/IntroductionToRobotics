#include "Consts.h"
#include <EEPROM.h>
#include "Sounds.h"

Sounds::Sounds() {
  buzzerEnabled = EEPROM.read(eepromBuzzerSoundAddress);
}

void Sounds::setBuzzerEnabled(int buzzerEnabled) {
  this->buzzerEnabled = buzzerEnabled;
}

void Sounds::attemptToPlayMenuNavSound() {
  if (buzzerEnabled) {
    buzzerTone = buzzerMenuNavTone;
    buzzerOnDuration = buzzerMenuNavOnDuration;
    tone(buzzerPin, buzzerTone, buzzerOnDuration);
  }
}

void Sounds::attemptToPlayInteractWithMenuOptionSound() {
  if (buzzerEnabled) {
    buzzerTone = buzzerMenuSelectOptionTone;
    buzzerOnDuration = buzzerMenuSelectOptionOnDuration;
    tone(buzzerPin, buzzerTone, buzzerOnDuration);
  }
}