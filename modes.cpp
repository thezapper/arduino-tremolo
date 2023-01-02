#include <Arduino.h>
#include "modes.h"
#define Serial SerialUSB

// A bight LED means low resistance in the LDR which will direct the signal to ground
// --== BRIGHT IS QUIET ==--

// +------------------------------------------------------------------------+
// | Triangle waveform
// +------------------------------------------------------------------------+
int triangle()
{
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= minBrightness ) 
  {
    brightness = minBrightness;
    fadeAmount = -fadeAmount;
  }

  if (brightness >= maxBrightness) 
  {
    brightness = maxBrightness;
    fadeAmount = -fadeAmount;
  }
}

// +------------------------------------------------------------------------+
// | fastRampUp
// | to avoid pops from just cutting the volume to 0 instantly.
// +------------------------------------------------------------------------+
const int FAST_RAMP_DELAY = 1;
void fastRampUp()
{
  for (int i = 0; i < brightnessRange; i+=8)
  {
    analogWrite(OutputLed, minBrightness + (i) );
    delay(FAST_RAMP_DELAY);
  }
}

// +------------------------------------------------------------------------+
// | Sawtooth
// | Fade in or out then jump back to the opposite end
// +------------------------------------------------------------------------+
int sawtooth(bool up)
{
  fadeAmount = abs(fadeAmount);
  if (!up)
  {
    fadeAmount = -fadeAmount;
  }
  brightness = brightness + fadeAmount;

  if (up) // fade in
  {
    if (brightness >= maxBrightness) 
    {
      brightness = minBrightness;
    }
  }
  else // fade out
  {
    if (brightness <= minBrightness ) 
    {
      fastRampUp();
      brightness = maxBrightness;
    }
  } 

  return (brightness);
}

// +------------------------------------------------------------------------+
// | Square wave
// +------------------------------------------------------------------------+
const int DELAY = 500; // waveform delay in ms
bool isLoud = false;
unsigned long timer = 0;
int square(unsigned long frameTime)
{
  timer += frameTime;
  if (timer > DELAY)
  {
    // currently loud and need to cut the volume by ramping down quickly to avoid pops
    if (isLoud)
    {
      fastRampUp();
      brightness = maxBrightness; // min
    }
    else
    {
      brightness = minBrightness;
    }
    isLoud = !isLoud;
    timer = 0;

  }
}