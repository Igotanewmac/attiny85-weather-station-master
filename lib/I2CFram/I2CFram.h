
// i2c fram utility routines


#include <Arduino.h>

// cursors
#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMUSED 0x0002
#define FRAMCURSOREEPROMFREE 0x0004

/// @brief Write 16 bytes of globalcache to data memory, return true if full
uint8_t framwritesensordata( uint8_t *globalcache );








































//