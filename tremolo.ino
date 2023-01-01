#include <Arduino.h>
#define Serial SerialUSB

#include "modes.h"
#include "input.h"

const float DAC_MAX_VOLTAGE = 4.0f;
const int STEPS_PER_VOLT = 1024 / DAC_MAX_VOLTAGE;

enum MODE
{
  TRIANGLE = 0,
  RAMP_UP,
  RAMP_DOWN,
  SQUARE
};

MODE currentMode = TRIANGLE;

int OutputLed = DAC0; // this shines on the LDR
int IndicatorLed = PIN_A1;  // this flashes as an indicator
int indicatorLevel = 0;
int minBrightness = 2.5f * STEPS_PER_VOLT;
int maxBrightness = DAC_MAX_VOLTAGE * STEPS_PER_VOLT;
int brightness = maxBrightness;  // how bright the LED is
int fadeAmount = 1;  // how many points to fade the LED by

float brightPercent = 0;  // the percentage of the maximum output.
int shiftedBrightness= 0;
int brightnessRange = maxBrightness - minBrightness;
//float ratio = (float)brightnessRange / (float)maxBrightness;

bool buttonStateChanged[2] = {0};

#define NUM_BUTTONS 3
buttonVars buttonStates[NUM_BUTTONS];

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  delay(500); // wait for serial

  
  Serial.println("Max Brightness: " + String(maxBrightness));
  // declare pin 9 to be an output:
  pinMode(IndicatorLed, OUTPUT);
  analogWriteResolution(10);

  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  buttonStates[0].pinNo = 7;
  buttonStates[0].state = PinStatus::HIGH;
  buttonStates[0].lastAction = 0;
  buttonStates[1].pinNo = 8;
  buttonStates[1].state = PinStatus::HIGH;
  buttonStates[1].lastAction = 0;
  buttonStates[2].pinNo = 9;
  buttonStates[2].state = PinStatus::HIGH;
  buttonStates[2].lastAction = 0;  
}

int indicatorBrightness(int brt)
{
  shiftedBrightness = brightness - minBrightness; 
  brightPercent = (float)shiftedBrightness / (float)brightnessRange;

  //return (255 - (brightPercent * 255)); // invert so a bright LED is a loud sound
  return ( brightPercent * 255); // invert so a bright LED is a loud sound
}

ULONG lastUpdate = millis();
unsigned int frames = 0;
char fps[8] = {0};

void loop() 
{

  ULONG now = millis();
  ULONG delta = now - lastUpdate;
  frames++;
  if (delta > 5000)
  {
    sprintf(fps, "FPS:%d", frames/5);
    Serial.println(fps);

    lastUpdate += delta;
    frames = 0;
  }
  

  gatherInput(buttonStates, NUM_BUTTONS);

  switch (currentMode)
  {
    case MODE::TRIANGLE:
    {
      triangle();
      break;
    }
    case MODE::RAMP_DOWN:
    {
      sawtooth(true);
      break;
    }
    case MODE::RAMP_UP:
    {
      sawtooth(false);
      break;
    }
    case MODE::SQUARE:
    {
      square();
      break;
    }
  }

  indicatorLevel = indicatorBrightness(brightness);
  //Serial.println("");
  // Serial.print(shiftedBrightness);
  // Serial.print(" ... ");
  // Serial.print(brightPercent);
  // Serial.print(" ... ");
  // Serial.println(ratio);
  
  // set the brightness of pin 9:
  analogWrite(OutputLed, brightness);
  analogWrite(IndicatorLed, indicatorLevel);

  delay(1);
}
