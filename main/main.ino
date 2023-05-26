#include "pitches.h"

#define ANALOG_RANGE (1<<10) - 1
#define TOLERANCE 100
#define NUM_READS_FOR_AVERAGE 5

#define NUM_ANALOG_PINS 3
uint8_t ANALOG_PINS[] = {A0, A3, A5};

#define TONE_PIN 8
#define NUM_NOTES 7
#define EIGHTH_NOTE_DURATION 100
int NOTES[] = {NOTE_G4, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_F4, 0, NOTE_F4};
int NOTE_DURATIONS[] = {1, 2, 1, 2, 2, 1, 3};
#define PAUSE_FACTOR 1.3

uint16_t initialLevels[NUM_ANALOG_PINS];

void getInitialLevels(uint16_t initialLevels[]) {
  int average;

  for (uint8_t i = 0; i < NUM_ANALOG_PINS; i++) {
    average = 0;
    for (uint8_t j = 0; j < NUM_READS_FOR_AVERAGE; j++) {
      average += analogRead(ANALOG_PINS[i]);
      delay(100); // delay between reads for better generalization
    }

    initialLevels[i] = average / NUM_READS_FOR_AVERAGE;
  }
}

bool isLaserTripped() {
  uint16_t level;
  for (uint8_t i = 0; i < NUM_ANALOG_PINS; i++) {
    level = analogRead(ANALOG_PINS[i]);
    if (level + TOLERANCE < initialLevels[i]) {
      return true;
    }

    // for stability
    delay(1);
  }

  return false;
}

void playAlarm() {
  for (uint8_t i = 0; i < NUM_NOTES; i++) {
    tone(TONE_PIN, NOTES[i], NOTE_DURATIONS[i]*EIGHTH_NOTE_DURATION);
    delay(NOTE_DURATIONS[i]*EIGHTH_NOTE_DURATION*PAUSE_FACTOR);
  }
}

void log() {
  // print initial levels for reference
  for (uint8_t i = 0; i < NUM_ANALOG_PINS-1; i++) {
    Serial.print(initialLevels[i]);
    Serial.print(",");
  }
  Serial.println(initialLevels[NUM_ANALOG_PINS-1]);

  // print current levels
  for (uint8_t i = 0; i < NUM_ANALOG_PINS-1; i++) {
    Serial.print(analogRead(ANALOG_PINS[i]));
    Serial.print(",");
  }
  Serial.println(analogRead(ANALOG_PINS[NUM_ANALOG_PINS-1])); 
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  getInitialLevels(initialLevels);
}

// the loop routine runs over and over again forever:
void loop() {
  if (isLaserTripped()) {
    playAlarm();
  }

  log();
  delay(100);
}
