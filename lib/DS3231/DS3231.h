// rtc clock functions header

#include <Arduino.h>


// check if clock has stopped
uint8_t clockHasStopped();
uint32_t getTimeAndDate();
void setonesecondalarm();
void clearalarms();
