#ifndef oled_h
#define oled_h

typedef unsigned long ULONG;

#define SCREEN_UPDATE_RATE 100000 //100ms
#define TINY 1
#define NORMAL 2
#define LARGE 3
#define MASSIVE 1

class oled
{
  public:
    oled();
    void setup();
    void update(ULONG frameTime);

    void setSpeed(int val);
    void setDepth(int val);
    void setDwell(int val);
    void setMode(int val);


  private:
    int speed = 0;
    int depth = 0;
    int dwell = 0;
    char chSpeed[3] = {'0','0','0'};
    char chDepth[3] = {'0','0','0'};
    char mode[8] = {0};
    ULONG updateTimer = 0;
    ULONG switchBackTimer = 0;

    int currentParam = 0;
    int currentMode = 0;

    void powerOnMsg();
    void drawSpeedParam();
    void drawDepthParam();
    void drawDwellParam();
    void drawModeParam();
    void drawMain();
    void drawBar(uint8_t val, uint16_t x, uint16_t y);

};

#endif