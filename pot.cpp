#include "Arduino.h"
#include "pot.h"

char dbg[64] = {0};

#define SAMPLE_TIME 10000
#define NUM_SAMPLES 10
#define Serial SerialUSB


pot::pot(int pin, int max) : pin(pin), maxPinVal(max)
{
}

bool pot::hasChanged()
{
  return false;
}

// returns a 0.0 - 100.0 value
float pot::update(u_long frameTime)
{
  float retVal = -1.0;
  timer += frameTime;

  if (timer > SAMPLE_TIME) // 10ms
  {
    // take a bunch of samples
    valueNow = analogRead(pin);
    smoothingTotal += valueNow;
    smoothingSamples++;

    if (smoothingSamples == NUM_SAMPLES)
    {
      // work out the average over the 5 samples (100ms) and
      // see if the value has changed much since the last time,
      // if not, the pot probably isn't moving and it's just jitter
      float ave = ((float)smoothingTotal / 10.0f);

      float diff = abs(valuePrev - ave);

      if (diff > 0.9f)
      {
        sprintf(dbg, "total:%d, ave:%f, diff:%f", smoothingTotal, ave, diff);
        Serial.println(dbg);

        percent = (ave / 255.0f) * 100.0f;
        retVal = percent;

        // save the average for these 5 samples
        valuePrev = ave;
      }

      smoothingTotal = 0;
      smoothingSamples = 0;
    }

    timer = 0;
  }

  return retVal;
}
