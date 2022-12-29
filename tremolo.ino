#define Serial SerialUSB

#include "modes.h"
#include "input.h"

const float DAC_MAX_VOLTAGE = 3.6;
const int STEPS_PER_VOLT = 1024 / DAC_MAX_VOLTAGE;

enum MODE
{
  TRIANGLE = 0,
  RAMP_UP,
  RAMP_DOWN,
  SQUARE
};

int OutputLed = DAC0; // this shines on the LDR
int IndicatorLed = PIN_A1;  // this flashes as an indicator
int indicatorLevel = 0;
int minBrightness = 1.2 * STEPS_PER_VOLT;
int maxBrightness = 2.7 * STEPS_PER_VOLT;
int brightness = maxBrightness;  // how bright the LED is
int fadeAmount = 1;  // how many points to fade the LED by

float brightPercent = 0;  // the percentage of the maximum output.
int shiftedBrightness= 0;
int brightnessRange = maxBrightness - minBrightness;
float ratio = (float)brightnessRange / (float)maxBrightness;

bool rampDir = true;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  //delay(20);
  Serial.println("Max Brightness: " + String(maxBrightness));
  // declare pin 9 to be an output:
  pinMode(IndicatorLed, OUTPUT);
  analogWriteResolution(10);

  pinMode(9, INPUT_PULLUP);
}

int indicatorBrightness(int brt)
{
  shiftedBrightness = brightness - minBrightness; 
  brightPercent = (float)shiftedBrightness / (float)brightnessRange;

  return (255 - (brightPercent * 255)); // invert so a bright LED is a loud sound
}



void loop() 
{

  gatherInput();

  //triangle();
  sawtooth(rampDir);
  //square();

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
