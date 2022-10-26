// Arduino program for controlling each color of an RGB LED (Red, Green, Blue), through the use of three potentiometers

const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

const int minPotValue = 0;
const int maxPotValue = 1023;

const int commonAnode = 0;
const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;

const int minLedValue = 0;
const int maxLedValue = 255;

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;

int redLedValue = 0;
int greenLedValue = 0;
int blueLedValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);

  redLedValue = map(redPotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  greenLedValue = map(greenPotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  blueLedValue = map(bluePotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);

  if (commonAnode) { // the RGB LED has a common anode
    redLedValue = 255 - redLedValue;
    greenLedValue = 255 - greenLedValue;
    blueLedValue = 255 - blueLedValue;
  }

  analogWrite(redLedPin, redLedValue);
  analogWrite(greenLedPin, greenLedValue);
  analogWrite(blueLedPin, blueLedValue);
}