
// i2c fram utility routines


#include <TinyWireM.h>
#define wire TinyWireM


#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51

uint8_t framworkmemory = I2CADDRESSFRAM0;
uint8_t framdatamemory = I2CADDRESSFRAM1;

#define DATAMEMORYMAXSIZE 32768



uint8_t framwritesensordata( uint8_t *globalcache ) {


    // fetch datamemorycursor
    uint16_t datamemorycursor = 0;
    wire.beginTransmission( framworkmemory );
    wire.write( 0x00 );
    wire.write( 0x00 );
    wire.endTransmission();
    wire.requestFrom( framworkmemory  , 2 );
    datamemorycursor += (uint16_t)(wire.read() << 8);
    datamemorycursor += (uint16_t)(wire.read());


    // write out 16 bytes of globalcache at the right location
    wire.beginTransmission( framdatamemory );
    wire.write( (uint8_t)( ( datamemorycursor >> 8 ) & 0xFF ) );
    wire.write( (uint8_t)( ( datamemorycursor & 0xFF ) ) );
    
    for ( int i = 0 ; i < 16 ; i++ ) {
        wire.write( globalcache[i] );
    }

    wire.endTransmission();
    

    // store data memory cursor
    wire.beginTransmission( framworkmemory );
    wire.write( 0x00 );
    wire.write( 0x00 );
    wire.write( (uint8_t)( ( ( datamemorycursor >> 8 ) & 0xFF ) ) );
    wire.write( (uint8_t)( ( datamemorycursor & 0xFF ) ) );
    wire.endTransmission();



    // return true if full
    if ( datamemorycursor == DATAMEMORYMAXSIZE ) {
        return 1;
    }
    else {
        return 0;
    }
    
}








































//