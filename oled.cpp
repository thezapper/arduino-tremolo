#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Arduino.h"
#include "oled.h"

#define Serial SerialUSB

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define CENTRE_WIDTH 64 
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define CENTRE_HEIGHT 32 

#define MAIN_PAGE 0
#define SPEED_PAGE 1
#define MODE_PAGE 2
#define DEPTH_PAGE 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

oled::oled()
{

}

void oled::setup()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
      Serial.println("SSD1306 allocation failed");
      for(;;); // Don't proceed, loop forever 
      // TODO there must be a better way to handle this.
    }

    powerOnMsg();
}

void oled::powerOnMsg()
{
  // Clear the buffer
  display.clearDisplay();

  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.setTextColor(SSD1306_WHITE); // Draw white text
  
  display.setTextSize(2);    
  display.setCursor(10, 8);     

  char message[] = "Jimmy the";
  size_t n = sizeof(message);
  for (int i = 0; i < n; i++)
  {
    char ch = message[i];
    display.write(ch);
    display.display();

    delay(50);
  }
  delay(400);

  display.setTextSize(3);    
  display.setCursor(19, 30);
  display.write("CUNT!");
  display.display();

  delay(800);

}

char* modes[] = {"Saw", "Smooth", "Ramp Up", "Ramp Dn", "Square"};

void oled::setSpeed(int val)
{
  speed = val;
  itoa(val, chSpeed, 10);
  currentParam = SPEED_PAGE; // TODO - fix
  switchBackTimer = 0;
}

void oled::setDepth(int val)
{
  depth = val;
  itoa(val, chDepth, 10);
  currentParam = DEPTH_PAGE; // TODO - fix
  switchBackTimer = 0;
}

void oled::setDwell(int val)
{
}

void oled::setMode(int val)
{
  currentMode = val;
  currentParam = MODE_PAGE; // TODO - fix
  switchBackTimer = 0;
}

void oled::drawModeParam(void) 
{
  display.clearDisplay();
  display.setTextSize(3);

  display.setCursor(0, CENTRE_HEIGHT - 12); 
  display.write(modes[currentMode]);

  display.display();
}

void oled::drawSpeedParam(void) 
{
  display.setTextSize(3);
  display.clearDisplay();

  // sprintf(displayBuff, "Speed:%d", this->speed);
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("Speed");
  display.setCursor(50, 24); 
  display.write(chSpeed);

  drawBar(speed, CENTRE_WIDTH - 50, SCREEN_HEIGHT - 8);

  display.display();
}

void oled::drawDepthParam(void) 
{
  display.setTextSize(3);
  display.clearDisplay();

  // sprintf(displayBuff, "Speed:%d", this->speed);
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("Depth");
  display.setCursor(50, 24); 
  display.write(chDepth);

  drawBar(depth, CENTRE_WIDTH - 50, SCREEN_HEIGHT - 8);

  display.display();
}

void oled::drawDwellParam(void) 
{

}

void oled::drawBar(uint8_t val, uint16_t x, uint16_t y)
{
  // hardcode the bar width to 100px

  // draw the filled part of the bar
  display.fillRect( x, y, val, 4, SSD1306_WHITE);

  // draw the empty part of the bar
  display.drawRect( x + val, y, 100 - val, 4, SSD1306_WHITE);

  // and the little handle
  display.fillCircle(x + val, y + 2, 3, SSD1306_WHITE);

  display.drawFastVLine(x + 50, y - 1, 6, SSD1306_INVERSE);
}

// The main page showing 
void oled::drawMain(void) 
{
  display.clearDisplay();

  // speed graph
  display.setTextSize(NORMAL);
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("Speed");
  drawBar(speed, CENTRE_WIDTH - 50, 20);

  // depth knob
  display.setCursor(0, CENTRE_HEIGHT);     // Start at top-left corner
  display.write("Depth");
  drawBar(depth, CENTRE_WIDTH - 50, CENTRE_HEIGHT + 20);

  display.display();
}

void oled::update(ULONG frameTime)
{
    updateTimer += frameTime;
    if (updateTimer > SCREEN_UPDATE_RATE)
    {
      switch(currentParam) 
      {
        case MAIN_PAGE :
          drawMain();
          break;
        case SPEED_PAGE :
          drawSpeedParam();
          break;
        case DEPTH_PAGE :
          drawDepthParam();
          break;
        case MODE_PAGE :
          drawModeParam();
          break;
      }
        //Serial.println("UpdateOled");
        updateTimer = 0;
    }

    switchBackTimer += frameTime;
    if (switchBackTimer > 2000000)
    {
      currentParam = MAIN_PAGE;
    }
}