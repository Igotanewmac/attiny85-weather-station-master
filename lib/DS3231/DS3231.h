// rtc clock functions header

#include "globalheader.h"

// check if clock has stopped
uint8_t clockHasStopped();
uint32_t getTimeAndDate();
void gettimeaddateraw( uint8_t *globalcache );
void setonesecondalarm();
void clearalarms();
