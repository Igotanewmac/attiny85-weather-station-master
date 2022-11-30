
// i2c fram utility routines


#include <Arduino.h>





// cursors
#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMUSED 0x0002
#define FRAMCURSOREEPROMFREE 0x0004
#define FRAMCURSOREEPROMBANK 0x0006

/// @brief Write 16 bytes of globalcache to data memory
void framwritesensordata( uint8_t *globalcache );








































//