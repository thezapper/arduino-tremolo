#include <Arduino.h>
#include "input.h"
#define Serial SerialUSB

#define DEBOUNCE_TIME 15

void gatherInput(buttonVars *btnArray, byte numButtons)
{
  ULONG now = millis();
  char out[32] = {0};

  buttonVars *btn = 0;
  // loop over all the buttons
  for (int i = 0; i < numButtons; i++)
  {      
    btn = &btnArray[i];

    // get the button current state
    int state = digitalRead(btn->pinNo);

    ULONG delta = now - btn->lastAction;
    //sprintf(out, "--- Delta: %d", delta);
    //Serial.println(out);

    if (delta < DEBOUNCE_TIME)
      continue;

    if (state == PinStatus::LOW)
    {
      if ( btn->state == PinStatus::HIGH )
      {
        sprintf(out, "%d Down", btn->pinNo);
        Serial.println(out);

        // mark the button ready for a press action
        btn->doPress = true;

        btn->state = PinStatus::LOW;
        btn->lastAction = now;
      }
      else
      {
        if (delta > 500)
        {
        sprintf(out, "%d Repeat", btn->pinNo);
          Serial.println(out);
          btn->lastAction = now;
        }
      }
    }
    else // button up
    {
      if (btn->state == PinStatus::LOW)
      {
        sprintf(out, "%d Release", btn->pinNo);
        Serial.println(out);
        btn->state = PinStatus::HIGH;
        btn->lastAction = now;
      }
    }
  }
} 