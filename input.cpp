#include <Arduino.h>
#include "input.h"

unsigned long sampleTime1 = millis();
unsigned long sampleTime2 = 0;
unsigned long timeDiff = 0;
const bool BUTTON_DOWN = false; // button press goes to ground
const bool BUTTON_UP = true; 
bool state9 = BUTTON_UP;

void gatherInput()
{
  sampleTime2 = millis();

  timeDiff = sampleTime2 - sampleTime1;
    
  PinStatus status9 = digitalRead(9);

  // button down
  if (status9 == PinStatus::LOW)
  {
    // button was previously up and not pressed within 10ms
    if (timeDiff > 10 && state9 == BUTTON_UP)
    {
      Serial.println("Press");

      state9 = BUTTON_DOWN;
      sampleTime1 = sampleTime2;

      rampDir = !rampDir;
    }

    // button is held down
    if (timeDiff > 500)
    {
      Serial.println("Repeat");
      sampleTime1 = sampleTime2;
    }
  }
  else // button up
  {
    // Button was down
    if (state9 == BUTTON_DOWN)
    {
      Serial.println("Release");
      state9 = BUTTON_UP;
    }
  }

} 