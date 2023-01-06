
//extern bool rampDir;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;

struct buttonVars
{
  PinStatus state;
  bool doPress;  // button has just gone from up to down, single shot event.
  bool doRepeat; // button has been held down for a repeat.
  pin_size_t pinNo;
  ULONG lastAction;
};

void checkEncoder(ULONG frameTime);
void gatherInput(buttonVars *btnArray, byte numButtons);