// #include <Adafruit_GFX.h>
// #include <Adafruit_GrayOLED.h>
// #include <Adafruit_SPITFT_Macros.h>
// #include <Adafruit_SPITFT.h>
// #include <gfxfont.h>
// #include <splash.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Arduino.h"

#include "oled.h"

#define Serial SerialUSB

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int updateCount = 0;
char displayBuff[16] = {0};


oled::oled()
{

}

void oled::setup()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

  //display.display();
  //delay(500); // Pause for 0.5 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(1000);
}

void testdrawchar(void) 
{
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  sprintf(displayBuff, "Count:%d", updateCount);
  display.write(displayBuff);
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.

  display.display();
}

ULONG updateTimer = 0;
#define UPDATE_RATE 500000 //100ms
void oled::update(ULONG frameTime)
{
    updateTimer += frameTime;
    if (updateTimer > UPDATE_RATE)
    {
        testdrawchar();
        updateCount++;

        //Serial.println("UpdateOled");
        updateTimer -= UPDATE_RATE;
    }
}