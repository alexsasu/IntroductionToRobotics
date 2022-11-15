// Arduino program for writing HEX numbers on the display digits of a 4-digit 7-segment display, through the use of a joystick

// joystick directions:
#define UP 1
#define DOWN -1
#define LEFT 1
#define RIGHT -1

// joystick thresholds for which the joystick is considered to have moved, after surpassing them
#define JOY_MIN_THRESHOLD 400
#define JOY_MAX_THRESHOLD 600

const int dataPin = 12;
const int latchPin = 11;
const int clockPin = 10;

const int digit1Pin = 7;
const int digit2Pin = 6;
const int digit3Pin = 5;
const int digit4Pin = 4;

const int displayDigitsCount = 4;
const int displayDigitsPins[displayDigitsCount] = {
  digit1Pin, digit2Pin, digit3Pin, digit4Pin
};

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 3;

const int displayDigitInitialState = B11111100; // number 0
const int bitForDP = 1;

const int encodingsCount = 16;
const int byteEncodings[encodingsCount] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A (10)
  B00111110, // b (11)
  B10011100, // C (12)
  B01111010, // d (13)
  B10011110, // E (14)
  B10001110  // F (15)
};

const unsigned int joySWDebounceDelay = 50;
const unsigned int displayDigitResetSignal = 3000;
const unsigned int dpBlinkInterval = 500;
const unsigned int joySensitivityBypassAmount = 50;

int currentDisplayDigit = 0;
int displayDigitsValues[displayDigitsCount] = {
  displayDigitInitialState, displayDigitInitialState, displayDigitInitialState, displayDigitInitialState
};
int displayDigitsByteEncodingsIndexes[displayDigitsCount] = {
  0, 0, 0, 0
};

volatile bool displayResetJustOccured = false;

volatile byte joySWReading = HIGH;
volatile byte joySWState = HIGH;
int joyXValue;
int joyYValue;
int joyDirection; // the joystick can move in 4 directions: UP, DOWN, LEFT, RIGHT
bool joyIsNeutral = true;
volatile bool joyWasPressed = false;
volatile unsigned long joyPressTime = 0;

unsigned long programTime = 0;
unsigned long programTimeBeforeDPBlinking = 0;

volatile int systemState = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for (int i = 0; i < displayDigitsCount; ++i) {
    pinMode(displayDigitsPins[i], OUTPUT);
    digitalWrite(displayDigitsPins[i], LOW);
  }

  pinMode(joySWPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(joySWPin), joySWChangeState, CHANGE);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  decideStateExecution(systemState);
}

void decideStateExecution(int systemState) {
  if (systemState == 1) {
    executeFirstState();
  } else {
    executeSecondState();
  }
}

void executeFirstState() {
  writeNumbersOnDisplays();

  programTime = millis();

  if (programTime - programTimeBeforeDPBlinking >= dpBlinkInterval) {
    programTimeBeforeDPBlinking = programTime;

    if (displayDigitsValues[currentDisplayDigit] % 2 == 0) { // if the decimal point for the current digit is turned off, then turn it on
      displayDigitsValues[currentDisplayDigit] += bitForDP;
    } else { // else, turn it off
      displayDigitsValues[currentDisplayDigit] -= bitForDP;
    }
  }

  programTime = millis();

  if (joySWReading == joySWState && joySWReading == LOW) {
    if (joyWasPressed) {
      if (programTime - joyPressTime >= displayDigitResetSignal) {
        executeDisplayReset();
        joyWasPressed = false;
      }
    }
  }

  joyXValue = analogRead(joyXPin);
  
  if (joyXValue < JOY_MIN_THRESHOLD && JOY_MIN_THRESHOLD - joyXValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;

    joyDirection = LEFT;
    switchToDifferentDisplayDigit(currentDisplayDigit, joyDirection);
  } 
  
  if (joyXValue > JOY_MAX_THRESHOLD && joyXValue - JOY_MAX_THRESHOLD > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;

    joyDirection = RIGHT;
    switchToDifferentDisplayDigit(currentDisplayDigit, joyDirection);
  }

  if (joyXValue >= JOY_MIN_THRESHOLD && joyXValue <= JOY_MAX_THRESHOLD) {
    joyIsNeutral = true;
  }
}

void executeSecondState() {
  displayDigitsValues[currentDisplayDigit] = byteEncodings[displayDigitsByteEncodingsIndexes[currentDisplayDigit]] + bitForDP; // the decimal point for the current display digit should be always on in the second system state
  writeNumbersOnDisplays();
  
  joyYValue = analogRead(joyYPin);

  if (joyYValue < JOY_MIN_THRESHOLD && JOY_MIN_THRESHOLD - joyYValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;

    joyDirection = DOWN;
    changeCurrentDigitValue(joyDirection);
  } 
  
  if (joyYValue > JOY_MAX_THRESHOLD && joyYValue - JOY_MAX_THRESHOLD > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;

    joyDirection = UP;
    changeCurrentDigitValue(joyDirection);
  }

  if (joyYValue >= JOY_MIN_THRESHOLD && joyYValue <= JOY_MAX_THRESHOLD) {
    joyIsNeutral = true;
  }
}

void switchToDifferentDisplayDigit(int& currentDisplayDigit, int joyDirection) {
  const int dpState = displayDigitsValues[currentDisplayDigit] % 2;
  displayDigitsValues[currentDisplayDigit] -= dpState; // if the decimal point for the current display digit is turned on, then it will be turned off

  currentDisplayDigit += joyDirection;
  if (currentDisplayDigit < 0) {
    currentDisplayDigit = 3;
  } else if (currentDisplayDigit > 3) {
    currentDisplayDigit = 0;
  }
  displayDigitsValues[currentDisplayDigit] += dpState;
}

void changeCurrentDigitValue(int joyDirection) {
  displayDigitsByteEncodingsIndexes[currentDisplayDigit] += joyDirection;
  if (displayDigitsByteEncodingsIndexes[currentDisplayDigit] < 0) {
    displayDigitsByteEncodingsIndexes[currentDisplayDigit] = 15;
  } else if (displayDigitsByteEncodingsIndexes[currentDisplayDigit] > 15) {
    displayDigitsByteEncodingsIndexes[currentDisplayDigit] = 0;
  }
  displayDigitsValues[currentDisplayDigit] = byteEncodings[displayDigitsByteEncodingsIndexes[currentDisplayDigit]];
}

void sendNumberToDisplay(int numberToDisplay) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
    digitalWrite(latchPin, HIGH);
}

void activateDisplayDigit(int displayDigitNumber) { 
  for (int displayDigit = 0; displayDigit < displayDigitsCount; ++displayDigit) {
    digitalWrite(displayDigitsPins[displayDigit], HIGH);
  }
  digitalWrite(displayDigitsPins[displayDigitNumber], LOW);
}

void writeNumbersOnDisplays() {
  for (int displayDigit = 0; displayDigit < displayDigitsCount; ++displayDigit) {
    sendNumberToDisplay(displayDigitsValues[displayDigit]);
    activateDisplayDigit(displayDigit);
    delay(5);
  }
}

void executeDisplayReset() {
  for (int displayDigit = 0; displayDigit < displayDigitsCount; ++displayDigit) {
    displayDigitsValues[displayDigit] = displayDigitInitialState;
    displayDigitsByteEncodingsIndexes[displayDigit] = 0;
  }
  currentDisplayDigit = 0;
  displayResetJustOccured = true;
}

void joySWChangeState() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  joySWReading = digitalRead(joySWPin);

  // if interrupts don't come within the debounce delay, assume there wasn't any noise during the button press
  if (interruptTime - lastInterruptTime > joySWDebounceDelay) {
    if (systemState == 1) {
      if (joySWReading != joySWState) {
        joySWState = joySWReading;
        
        if (joySWState == HIGH) {
          if (displayResetJustOccured) {
            displayResetJustOccured = false;
          } else {
            systemState = 2;
          }
        } else {
          joyWasPressed = true;
          joyPressTime = millis();
        }
      }
    } else {
      if (joySWReading != joySWState) {
        joySWState = joySWReading;

        if (joySWState == HIGH) {
          systemState = 1;
        }
      }
    }
  }

  lastInterruptTime = interruptTime;
}
