#ifndef oled_h
#define oled_h

typedef unsigned long ULONG;

void setupOled();
void updateOled(ULONG frameTime);

class oled
{
  public:
    oled();
    void setup();
    void update(ULONG frameTime);

  private:
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};

#endif