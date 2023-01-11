#ifndef pot_h
#define pot_h

class pot
{
public:
  pot(int pin, int max = 255);
  float update(u_long frameTime);
  float getVal() { return percent;};
  bool hasChanged();

private:
  int pin = 0;
  int maxPinVal = 255;
  int valueNow = 0;
  int valuePrev = 0;
  float percent = 0.0f;
  int smoothingTotal = 0;
  int smoothingSamples = 0;
  u_long timer = 0;

};

#endif