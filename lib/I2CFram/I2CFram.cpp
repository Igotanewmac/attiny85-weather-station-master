
// i2c fram utility routines


#include <TinyWireM.h>
#define wire TinyWireM


#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51

uint8_t framworkmemory = I2CADDRESSFRAM0;
uint8_t framdatamemory = I2CADDRESSFRAM1;

#define DATAMEMORYMAXSIZE 32768



// cursors
#define FRAMCURSORFRAMFREE 0x0000
#define FRAMCURSORFRAMSTRIPE 0x0002
#define FRAMCURSOREEPROMSTRIPE 0x0004
#define FRAMCURSOREEPROMBANK 0x0006



// bus switcher
#include "TCA9548.h"




/// @brief Fetches a cursor from working fram.
/// @param cursorid which cursor to retrive
/// @return the 16 bit cursor data value
uint16_t framfetchcursor( uint8_t cursorid ) {

    wire.beginTransmission( framworkmemory );
    wire.write( (uint8_t)( ( cursorid >> 8 ) & 0xFF ) );
    wire.write( (uint8_t)( cursorid & 0xFF ) );
    wire.endTransmission();
    wire.requestFrom( framworkmemory  , 2 );
    uint16_t cursor = 0;
    cursor += (uint16_t)(wire.read() << 8);
    cursor += (uint16_t)(wire.read());
    return cursor;

}







/// @brief Writes a cursor to working fram
/// @param cursorid which cursor to write
/// @param newcursorvalue the new value to write
void framwritecursor( uint8_t cursorid , uint16_t newcursorvalue ) {

    // store data memory cursor
    wire.beginTransmission( framworkmemory );
    wire.write( (uint8_t)( ( cursorid >> 8 ) & 0xFF ) );
    wire.write( (uint8_t)( cursorid & 0xFF ) );
    wire.write( (uint8_t)( ( ( newcursorvalue >> 8 ) & 0xFF ) ) );
    wire.write( (uint8_t)( ( newcursorvalue & 0xFF ) ) );
    wire.endTransmission();

}






/// @brief write the sensor data to the fram cache
/// @param globalcache the global cache array
void framwritesensordata( uint8_t *globalcache ) {
    
    // fetch datamemorycursor
    uint16_t datamemorycursor = 0;
    datamemorycursor = framfetchcursor( FRAMCURSORFRAMFREE );

    // write out 16 bytes of globalcache at the right location
    wire.beginTransmission( framdatamemory );
    wire.write( (uint8_t)( ( datamemorycursor >> 8 ) & 0xFF ) );
    wire.write( (uint8_t)( ( datamemorycursor & 0xFF ) ) );
    
    for ( int i = 0 ; i < 16 ; i++ ) {
        wire.write( globalcache[i] );
    }

    wire.endTransmission();
    
    datamemorycursor += 16;

    framwritecursor( FRAMCURSORFRAMFREE , datamemorycursor );

    // all done, return to caller
    return;

}








// fram to eeprom striping.


// copy one stripe from fram to eeproms
void framcopyonestripe() {

    // read in the stripe of 64 bytes from fram

    // 1. switch to fram bus
    

    // 2. read FRAMCURSORFRAMUSED cursor

    // 3. read 64 bytes from framworkingmemory

    // 4. increment FRAMCURSORFRAMUSED

    // 5. write back FRAMCURSORFRAMUSED



    // write out a 64 byte stripe

    // 1. read FRAMCURSOREEPROMBANK

    // 2. read FRAMCURSOREEPROMSTRIPE

    // 3. switch to eeprom bus

    // 4. write 64 bytes of data to EEPROMBANK at EEPROMSTRIPE

    // 5. increment EEPROMBANK

    // 6. if EEMPROMBANK == 8 then EEPROMBANK == 0;

    // 7. if EEPROMBANK == 0 then EEPROMSTRIPE++;

    // 8. switch to fram bus

    // 9. write EEPROMBANK cursor

    // 10. write EEPROMSTRIPE cursor

}
































//