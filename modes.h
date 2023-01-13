#ifndef modes_h
#define modes_h

// extern int brightness;
// extern int minBrightness;
// extern int maxBrightness;
// extern int fadeAmount;
// extern int brightnessRange;
// extern int OutputLed;

float triangle(unsigned long frameTime);
float sawtooth(bool up, unsigned long frameTime);
float square(unsigned long frameTime);
float smooth(unsigned long frameTime);

void setRate(float percent);

#endif