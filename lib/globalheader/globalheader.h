
#ifndef GLOBALHEADERGUARDFLAG
#define GLOBALHEADERGUARDFLAG 1


// This is the global header included in all files!
#include <Arduino.h>





// led pin(s)!
// Pin of red output LED
#define LED_RED 3
// Pin of yellow output LED
#define LED_YEL 4
// i2c side io pin
// input pin for now.
#define PIN_TRIGGER1 1




#include <TinyWireM.h>
// Utility definition for TinyWireM
//tca9548
#define wire TinyWireM










#define DATAMEMORYMAXSIZE 32768


// cursors

#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMSTRIPE 0x0002
#define FRAMCURSOREEPROMSTRIPE 0x0004
#define FRAMCURSOREEPROMBANK 0x0006



// fram addresses
#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51

// bh1750
#define BH1750ADDRESS 0x23

// bmp085
#define BMP085ADDRESS 0x77

// ds3231
#define DS1307ADDRESS 0x68

// HTU2X
#define HTU2XADDRESS 0x40

// tca9548
// the address of the i2c bus switcher
#define I2CADDRESSBUSMASTER 0x70
// the actual bus id's
// sensors are on bus 0x00
#define I2CBUSIDSENSORS 0x00
// eeprom storage is on bus 0x01
#define I2CBUSIDEEPROM 0x01



#endif








//