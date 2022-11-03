// Arduino program for simulating a crosswalk with semaphores, that emits sounds

const int carSemaphoreRedPin = 11;
const int carSemaphoreYellowPin = 10;
const int carSemaphoreGreenPin = 9;

const int pedestrianSemaphoreRedPin = 8;
const int pedestrianSemaphoreGreenPin = 7;

const int buzzerPin = 6;

const int buttonPin = 3;

byte carSemaphoreRedValue = LOW;
byte carSemaphoreYellowValue = LOW;
byte carSemaphoreGreenValue = LOW;

byte pedestrianSemaphoreRedValue = LOW;
byte pedestrianSemaphoreGreenValue = LOW;

int buzzerTone = 0;
int buzzerOnDuration = 0;
int buzzerPeriodDuration = 0;
unsigned long buzzerLastPeriodStart = 0;

unsigned int debounceDelay = 100;

volatile int systemState = 1;
/*
variable systemState can have 4 values:
  1 - green light for cars, red light for people, no sounds
  2 - yellow light for cars, red light for people, no sounds
  3 - red light for cars, green light for people, beeping sound at a constant interval
  4 - red light for cars, blinking green light for people, beeping sound at a constant interval,
      faster than the beeping in state 3
*/
int secondStateInitiationTime = 8000;
int secondStateDuration = 11000;
int thirdStateDuration = 8000;
int fourthStateDuration = 4000;

unsigned long programTime = 0;
volatile unsigned long lastStateEndTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(carSemaphoreRedPin, OUTPUT);
  pinMode(carSemaphoreYellowPin, OUTPUT);
  pinMode(carSemaphoreGreenPin, OUTPUT);

  pinMode(pedestrianSemaphoreRedPin, OUTPUT);
  pinMode(pedestrianSemaphoreGreenPin, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), initiateCrosswalk, CHANGE);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (systemState == 1) {
    carSemaphoreGreenValue = HIGH;
    pedestrianSemaphoreRedValue = HIGH;

    digitalWrite(carSemaphoreGreenPin, carSemaphoreGreenValue);
    digitalWrite(pedestrianSemaphoreRedPin, pedestrianSemaphoreRedValue);
  } else if (systemState == 2) {
    programTime = millis();

    if (programTime - lastStateEndTime >= secondStateInitiationTime && 
        programTime - lastStateEndTime < secondStateDuration) { // time between car semaphore yellow light and the beginning of system state 3 
      carSemaphoreGreenValue = LOW;
      carSemaphoreYellowValue = HIGH;

      digitalWrite(carSemaphoreGreenPin, carSemaphoreGreenValue);
      digitalWrite(carSemaphoreYellowPin, carSemaphoreYellowValue);
    } else if (programTime - lastStateEndTime >= secondStateDuration) { // we have reached the end of state 2
      systemState = 3;
      lastStateEndTime = millis();
      buzzerLastPeriodStart = millis();
    }
  } else if (systemState == 3) {
    buzzerTone = 1000;
    buzzerOnDuration = 500;
    buzzerPeriodDuration = 1000;

    carSemaphoreYellowValue = LOW;
    carSemaphoreRedValue = HIGH;
    pedestrianSemaphoreRedValue = LOW;
    pedestrianSemaphoreGreenValue = HIGH;

    digitalWrite(carSemaphoreYellowPin, carSemaphoreYellowValue);
    digitalWrite(carSemaphoreRedPin, carSemaphoreRedValue);
    digitalWrite(pedestrianSemaphoreRedPin, pedestrianSemaphoreRedValue);
    digitalWrite(pedestrianSemaphoreGreenPin, pedestrianSemaphoreGreenValue);

    programTime = millis();

    if (programTime - lastStateEndTime < thirdStateDuration) {
      if (programTime - buzzerLastPeriodStart < buzzerPeriodDuration) {
        buzzerLastPeriodStart += buzzerPeriodDuration;
        tone(buzzerPin, buzzerTone, buzzerOnDuration);
      }
    } else { // we have reached the end of state 3
      systemState = 4;
      lastStateEndTime = millis();
      buzzerLastPeriodStart = millis();
    }
  } else if (systemState == 4) {
    buzzerTone = 1500;
    buzzerOnDuration = 250;
    buzzerPeriodDuration = 260;

    programTime = millis();

    if (programTime - lastStateEndTime < fourthStateDuration) {
      if (programTime - buzzerLastPeriodStart < buzzerPeriodDuration) {
        if (pedestrianSemaphoreGreenValue == HIGH) {
          pedestrianSemaphoreGreenValue = LOW;
          
          tone(buzzerPin, buzzerTone, buzzerOnDuration);
        } else {
          pedestrianSemaphoreGreenValue = HIGH;
        }

        buzzerLastPeriodStart += buzzerPeriodDuration;
        
        digitalWrite(pedestrianSemaphoreGreenPin, pedestrianSemaphoreGreenValue);
      }
    } else { // we have reached the end of state 4, and the semaphore will go back to the initial state
      carSemaphoreRedValue = LOW;
      carSemaphoreGreenValue = HIGH;
      pedestrianSemaphoreGreenValue = LOW;

      digitalWrite(carSemaphoreRedPin, carSemaphoreRedValue);
      digitalWrite(carSemaphoreGreenPin, carSemaphoreGreenValue);
      digitalWrite(pedestrianSemaphoreGreenPin, pedestrianSemaphoreGreenValue);

      systemState = 1;
    }
  }
}

void initiateCrosswalk() {
  if (systemState == 1) {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    // if interrupts don't come within the debounce delay, assume there wasn't any noise during the button press
    if (interruptTime - lastInterruptTime > debounceDelay) {
      systemState = 2;
      lastStateEndTime = millis();
    }

    lastInterruptTime = interruptTime;
  }
}
