

// This is the global header included in all files!


#include <Arduino.h>



#include <TinyWireM.h>
// Utility definition for TinyWireM
#define wire TinyWireM



// led pin(s)!
// Pin of red output LED
#define LED_RED 3
// Pin of yellow output LED
#define LED_YEL 4

// i2c side io pin
// input pin for now.
#define PIN_TRIGGER1 1


// cursors
#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMUSED 0x0002
#define FRAMCURSOREEPROMFREE 0x0004
#define FRAMCURSOREEPROMBANK 0x0006

#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51


#define DATAMEMORYMAXSIZE 32768



// cursors
#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMSTRIPE 0x0002
#define FRAMCURSOREEPROMSTRIPE 0x0004
#define FRAMCURSOREEPROMBANK 0x0006





// bh1750
#include <Arduino.h>


#include <TinyWireM.h>

#ifndef tw
#define tw TinyWireM
#endif

#define BH1750ADDRESS 0x23



















// utility defiitions for int main(int argc, char const *argv[])
#define GETBIT00 ( flags & 0b0000000000000001 )
#define GETBIT01 ( ( flags & 0b0000000000000010 ) >> 1 )
#define GETBIT02 ( ( flags & 0b0000000000000100 ) >> 2 )
#define GETBIT03 ( ( flags & 0b0000000000001000 ) >> 3 )
#define GETBIT04 ( ( flags & 0b0000000000010000 ) >> 4 )
#define GETBIT05 ( ( flags & 0b0000000000100000 ) >> 5 )
#define GETBIT06 ( ( flags & 0b0000000001000000 ) >> 6 )
#define GETBIT07 ( ( flags & 0b0000000010000000 ) >> 7 )
#define GETBIT08 ( ( flags & 0b0000000100000000 ) >> 8 )
#define GETBIT09 ( ( flags & 0b0000001000000000 ) >> 9 )
#define GETBIT10 ( ( flags & 0b0000010000000000 ) >> 10 )
#define GETBIT11 ( ( flags & 0b0000100000000000 ) >> 11 )
#define GETBIT12 ( ( flags & 0b0001000000000000 ) >> 12 )
#define GETBIT13 ( ( flags & 0b0010000000000000 ) >> 13 )
#define GETBIT14 ( ( flags & 0b0100000000000000 ) >> 14 )
#define GETBIT15 ( ( flags & 0b1000000000000000 ) >> 15 )

#define SETBIT00 flags |= 0b0000000000000001;
#define SETBIT01 flags |= 0b0000000000000010;
#define SETBIT02 flags |= 0b0000000000000100;
#define SETBIT03 flags |= 0b0000000000001000;
#define SETBIT04 flags |= 0b0000000000010000;
#define SETBIT05 flags |= 0b0000000000100000;
#define SETBIT06 flags |= 0b0000000001000000;
#define SETBIT07 flags |= 0b0000000010000000;
#define SETBIT08 flags |= 0b0000000100000000;
#define SETBIT09 flags |= 0b0000001000000000;
#define SETBIT10 flags |= 0b0000010000000000;
#define SETBIT11 flags |= 0b0000100000000000;
#define SETBIT12 flags |= 0b0001000000000000;
#define SETBIT13 flags |= 0b0010000000000000;
#define SETBIT14 flags |= 0b0100000000000000;
#define SETBIT15 flags |= 0b1000000000000000;


#define CLRBIT00 flags &= 0b1111111111111110;
#define CLRBIT01 flags &= 0b1111111111111101;
#define CLRBIT02 flags &= 0b1111111111111011;
#define CLRBIT03 flags &= 0b1111111111110111;
#define CLRBIT04 flags &= 0b1111111111101111;
#define CLRBIT05 flags &= 0b1111111111011111;
#define CLRBIT06 flags &= 0b1111111110111111;
#define CLRBIT07 flags &= 0b1111111101111111;
#define CLRBIT08 flags &= 0b1111111011111111;
#define CLRBIT09 flags &= 0b1111110111111111;
#define CLRBIT10 flags &= 0b1111101111111111;
#define CLRBIT11 flags &= 0b1111011111111111;
#define CLRBIT12 flags &= 0b1110111111111111;
#define CLRBIT13 flags &= 0b1101111111111111;
#define CLRBIT14 flags &= 0b1011111111111111;
#define CLRBIT15 flags &= 0b0111111111111111;













//