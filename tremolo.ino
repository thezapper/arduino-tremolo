#include <Arduino.h>
#define Serial SerialUSB

#include "modes.h"
#include "input.h"

const float DAC_MAX_VOLTAGE = 4.0f;
const int STEPS_PER_VOLT = 1024 / DAC_MAX_VOLTAGE;

enum MODE
{
  TRIANGLE = 0,
  SMOOTH,  
  RAMP_UP,
  RAMP_DOWN,
  SQUARE
};

MODE currentMode = SQUARE;

int OutputLed = DAC0; // this shines on the LDR
int IndicatorLed = PIN_A1;  // this flashes as an indicator
int indicatorLevel = 0;
int minBrightness = 2.5f * STEPS_PER_VOLT;
int maxBrightness = (DAC_MAX_VOLTAGE * STEPS_PER_VOLT) - 1;
int brightness = maxBrightness;  // how bright the LED is
int fadeAmount = 1;  // how many points to fade the LED by

float brightPercent = 0;  // the percentage of the maximum output.
int shiftedBrightness= 0;
int brightnessRange = maxBrightness - minBrightness;
//float ratio = (float)brightnessRange / (float)maxBrightness;

#define NUM_BUTTONS 3
buttonVars buttonStates[NUM_BUTTONS];

// button mappings
#define MODE_BTN 2

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  delay(500); // wait for serial

  
  Serial.println("Max Brightness: " + String(maxBrightness));
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

// +------------------------------------------------------------------------+
// | fastRampUp
// | to avoid pops from just cutting the volume to 0 instantly.
// +------------------------------------------------------------------------+
void fastRampUp()
{
  for (int i = 0; i < brightnessRange; i+=8)
  {
    analogWrite(OutputLed, minBrightness + (i) );
    delay(1);
  }
}

ULONG lastFrame = millis();
ULONG frameTime = 0;
ULONG fpsTimer = 0;
unsigned int frames = 0;
char fps[8] = {0};

void loop() 
{

  ULONG now = millis();
  frameTime = now - lastFrame;
  fpsTimer += frameTime;
  frames++;
  if (fpsTimer > 3000)
  {
    sprintf(fps, "FPS:%d", frames/3);
    Serial.println(fps);

    fpsTimer = 0;
    frames = 0;
  }
  lastFrame = now;
  
  gatherInput(buttonStates, NUM_BUTTONS);

  // process the input
  if (buttonStates[MODE_BTN].doPress)
  {
    switch(currentMode) 
    {
      case MODE::TRIANGLE : 
        currentMode = MODE::SMOOTH;
        Serial.println("SMOOTH");
        break;
      case MODE::SMOOTH : 
        currentMode = MODE::RAMP_UP;
        Serial.println("RAMP_UP");
        break;
      case MODE::RAMP_UP : 
        currentMode = MODE::RAMP_DOWN;
        Serial.println("RAMP_DOWN");
        break;
      case MODE::RAMP_DOWN : 
        currentMode = MODE::SQUARE;
        Serial.println("SQUARE");
        break;
      case MODE::SQUARE : 
        currentMode = MODE::TRIANGLE;
        Serial.println("TRIANGLE");
        break;
    }

    buttonStates[MODE_BTN].doPress = false;
  }

  float amplitude = 0;
  switch (currentMode)
  {
    // the mode functions return a value between 0.0 and 1.0 representing 
    // amplitude at that point in time
    case MODE::TRIANGLE:
    {
      amplitude = triangle(frameTime);
      break;
    }
    case MODE::SMOOTH:
    {
      amplitude = smooth(frameTime);
      break;
    }
    case MODE::RAMP_DOWN:
    {
      amplitude = sawtooth(true, frameTime);
      break;
    }
    case MODE::RAMP_UP:
    {
      amplitude = sawtooth(false, frameTime);
      break;
    }
    case MODE::SQUARE:
    {
      // TODO - Handle fast ramp situation
      amplitude = square(frameTime);
      break;
    }
  }

  int offset = amplitude * brightnessRange;
  brightness = minBrightness + offset;

  indicatorLevel = indicatorBrightness(brightness);
  
  analogWrite(OutputLed, brightness);
  analogWrite(IndicatorLed, indicatorLevel);
}
