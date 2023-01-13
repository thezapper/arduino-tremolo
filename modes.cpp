#include <Arduino.h>
#include "modes.h"
#define Serial SerialUSB

// A bight LED means low resistance in the LDR which will direct the signal to ground
// --== BRIGHT IS QUIET ==--

unsigned long timer = 0;
unsigned long cycleTime = 1000000; // us
float hz = 1.0f;
float bpm = 60.0f;

// +------------------------------------------------------------------------+
// | Triangle waveform
// +------------------------------------------------------------------------+
float triangle(unsigned long frameTime)
{
  timer += frameTime;
  if (timer > cycleTime)
    timer -= cycleTime;

  // 1.0      /\
  //         /  \
  // 0.5    /    \
  //       /      \
  // 0.0 -+--------+-
  //     0.0  .5  1.0  <-time
  
  // calculate where we are in the cycle based on the current time
  float pos = (float)timer / (float)cycleTime;

  float value = pos;
  if (pos > 0.5)
    value = 1.0 - pos;
  
  // When pos reaches 0.5 we want the output to be 1.0, so multiply the 
  // output by 2
  return (value * 2.0f);
}

// +------------------------------------------------------------------------+
// | Sawtooth
// | Fade in or out then jump back to the opposite end
// +------------------------------------------------------------------------+
float sawtooth(bool up, unsigned long frameTime)
{
  timer += frameTime;
  if (timer > cycleTime)
    timer -= cycleTime;

  // 1.0         / |
  //           /   |
  // 0.5     /     |
  //       /       |
  // 0.0 -+--------+-
  //     0.0  .5  1.0  <-time
  
  // calculate where we are in the cycle based on the current time
  float pos = (float)timer / (float)cycleTime;
  
  if (up)
    return (pos);
  else
    return (1.0 - pos);
}

// +------------------------------------------------------------------------+
// | Square wave
// +------------------------------------------------------------------------+
bool isLoud = false;
float square(unsigned long frameTime)
{
  timer += frameTime;
  if (timer > cycleTime)
    timer -= cycleTime;

  if (timer > (cycleTime/2) )
    return 1.0;
  else
    return 0.0;
}

// +------------------------------------------------------------------------+
// | Smooth (sine) wave
// +------------------------------------------------------------------------+
float smooth(unsigned long frameTime)
{
  timer += frameTime;
  if (timer > cycleTime)
    timer -= cycleTime;

  // calculate where we are in the cycle based on the current time
  float pos = (float)timer / (float)cycleTime;
  float val = sin(pos * TWO_PI);

  return ( (val + 1) / 2.0f);
}

// +------------------------------------------------------------------------+
// | Set the rate of the effect.  Accepts a value between 0 - 100
// +------------------------------------------------------------------------+
const int MAX_SPEED = 3000000; // 3 sec cycle/0.3hz
const int MIN_SPEED = 100000;  // 0.1 sec cycle/10hz
const int DIFF = MAX_SPEED - MIN_SPEED;
void setRate(float percent)
{
  //float timeInMicros
  cycleTime = MIN_SPEED + ((percent / 100.0f) * DIFF);
  hz = (float)cycleTime / 1000000.0f; // microseconds
  bpm = hz * 60.0f;
}