// Arduino program for modifying the leds on a 7-segment display through the use of a joystick

const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int ledsNo = 8;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 3;

const int joyDirectionsNo = 4;

const int joyInferiorThreshold = 400;
const int joySuperiorThreshold = 600;

const unsigned int joySWDebounceDelay = 50;
const unsigned int displayResetSignal = 3000;
const unsigned int ledBlinkInterval = 500;
const unsigned int joySensitivityBypassAmount = 50;

const bool commonAnode = false;
const int ledsPins[ledsNo] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte ledsStates[ledsNo] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};
int ledsPaths[ledsNo][joyDirectionsNo] = {
//  UP  DOWN  LEFT  RIGHT
   -1,  6,    5,    1,   // LED a
    0,  6,    5,   -1,   // LED b
    6,  3,    4,    7,   // LED c
    6, -1,    4,    2,   // LED d
    6,  3,   -1,    2,   // LED e
    0,  6,   -1,    1,   // LED f
    0,  3,   -1,   -1,   // LED g
   -1, -1,    2,   -1    // LED dp
};

int currentLed = 7;
byte blinkingLedState = LOW;

volatile bool displayResetJustOccured = false;

volatile byte joySWReading = HIGH;
volatile byte joySWState = HIGH;
int joyXValue;
int joyYValue;
int joyDirection = -1; // the joystick can move in 4 directions: UP - 0, DOWN - 1, LEFT - 2, RIGHT - 3
bool joyIsNeutral = true;
volatile bool joyWasPressed = false;
volatile unsigned long joyPressTime = 0;

unsigned long programTime = 0;
unsigned long programTimeBeforeLedChange = 0;

volatile int systemState = 1;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < ledsNo; ++i) {
    pinMode(ledsPins[i], OUTPUT);
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
  digitalWrite(ledsPins[currentLed], blinkingLedState ^ commonAnode);

  programTime = millis();

  if (joySWReading == joySWState && joySWReading == LOW) {
    if (joyWasPressed) {
      if (programTime - joyPressTime >= displayResetSignal) {
        executeDisplayReset();
        joyWasPressed = false;
      }
    }
  }

  programTime = millis();

  if (programTime - programTimeBeforeLedChange >= ledBlinkInterval) {
    programTimeBeforeLedChange = programTime;

    if (blinkingLedState == LOW) {
      blinkingLedState = HIGH;
    } else {
      blinkingLedState = LOW;
    }
  }

  joyXValue = analogRead(joyXPin);
  joyYValue = analogRead(joyYPin);

  if (joyYValue > joySuperiorThreshold && joyYValue - joySuperiorThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    joyDirection = 0;
    digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);

    attemptRepositioning(currentLed, joyDirection);
  } 
  
  if (joyYValue < joyInferiorThreshold && joyInferiorThreshold - joyYValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    joyDirection = 1;
    digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);
    
    attemptRepositioning(currentLed, joyDirection);
  } 
  
  if (joyXValue < joyInferiorThreshold && joyInferiorThreshold - joyXValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    joyDirection = 2;
    digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);
    
    attemptRepositioning(currentLed, joyDirection);
  } 
  
  if (joyXValue > joySuperiorThreshold && joyXValue - joySuperiorThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    joyDirection = 3;
    digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);
    
    attemptRepositioning(currentLed, joyDirection);
  }

  if (joyXValue >= joyInferiorThreshold && joyXValue <= joySuperiorThreshold &&
      joyYValue >= joyInferiorThreshold && joyYValue <= joySuperiorThreshold) {
    joyIsNeutral = true;
  }
}

void executeSecondState() {
  digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);

  joyYValue = analogRead(joyYPin);

  if (((joyYValue < joyInferiorThreshold && joyInferiorThreshold - joyYValue > joySensitivityBypassAmount) || 
       (joyYValue > joySuperiorThreshold && joyYValue - joySuperiorThreshold > joySensitivityBypassAmount)) && joyIsNeutral) {
    joyIsNeutral = false;
    ledsStates[currentLed] = !ledsStates[currentLed];
  }

  if (joyYValue >= joyInferiorThreshold && joyYValue <= joySuperiorThreshold) {
    joyIsNeutral = true;
  }
}

void attemptRepositioning(int& currentLed, int joyDirection) {
  if (ledsPaths[currentLed][joyDirection] != -1) {
    digitalWrite(ledsPins[currentLed], ledsStates[currentLed] ^ commonAnode);
    currentLed = ledsPaths[currentLed][joyDirection];
  }
}

void executeDisplayReset() {
  for (int i = 0; i < ledsNo; ++i) {
    ledsStates[i] = LOW;
    digitalWrite(ledsPins[i], ledsStates[i] ^ commonAnode);
  }
  currentLed = 7;
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
