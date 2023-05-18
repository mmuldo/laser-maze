#include "pitches.h"

#define ANALOG_RANGE (1<<10) - 1
#define TOLERANCE 100
#define NUM_READS_FOR_AVERAGE 5

#define NUM_ANALOG_PINS 2
uint8_t ANALOG_PINS[] = {A0, A1};

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
      delay(100); // delay between reads for stability
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

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  getInitialLevels(initialLevels);
}

// the loop routine runs over and over again forever:
void loop() {
  //noTone(TONE_PIN);
  //tone(TONE_PIN, TONE_NOTE + offset, TONE_DURATION);
  //int pause = TONE_DURATION * 1.30;
  //delay(pause);
  //offset += 20;
  if (isLaserTripped()) {
    playAlarm();
  }
  //} else {
  //  noTone(TONE_PIN);
  //}

  // read the input on analog pin 0:
  int sensorValue0 = analogRead(A0);
  int sensorValue1 = analogRead(A1);
  // print out the value you read:
  Serial.print(initialLevels[0]);
  Serial.print(",");  // delay in between reads for stability
  Serial.println(initialLevels[1]);
  Serial.print(sensorValue0);
  Serial.print(",");  // delay in between reads for stability
  Serial.println(sensorValue1);
  delay(100);
}
