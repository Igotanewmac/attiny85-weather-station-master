

// bh1750 

#include "globalheader.h"




// low resolution oneshot.
uint16_t bh1750lowresoneshot() {

    // get a 16 bit high resolution mode 1 oneshot reading


    // say hello to the chip
    wire.beginTransmission( BH1750ADDRESS );
    wire.send( 0b00000001 ); // power on
    wire.endTransmission();

    // zero out the reading register
    wire.beginTransmission( BH1750ADDRESS );
    wire.send( 0b00000111 ); // reset register
    wire.endTransmission();

    // initiate sensing
    wire.beginTransmission( BH1750ADDRESS );
    wire.send( 0b00100000 ); // initiate sensing
    wire.endTransmission();

    // this can take up to 180ms
    //delay( 200 );

    uint16_t retval = 0;

    while ( retval == 0 ) {
    wire.requestFrom( BH1750ADDRESS , 2 );
    retval = wire.receive() << 8;
    retval += wire.receive();
    }

    return retval;

}






void bh1750lowresoneshotrawstart() {

    // get a 16 bit high resolution mode 1 oneshot reading

    // say hello to the chip
    wire.beginTransmission( BH1750ADDRESS );
    wire.send( 0b00000001 ); // power on
    wire.endTransmission();
    // zero out the reading register
    wire.beginTransmission( BH1750ADDRESS );
    wire.send( 0b00000111 ); // reset register data to zero
    wire.endTransmission();
    // initiate sensing
    wire.beginTransmission( BH1750ADDRESS );
    //tw.send( 0b00100001 ); // initiate high2 resolution sensing
    wire.send( 0b00100011 ); // initiate low resolution sensing
    wire.endTransmission();

    // this can take up to 180ms
    //delay( 200 );

    return;
}



// low resolution oneshot.
void bh1750lowresoneshotrawend( uint8_t *globalcache ) {

    // get a 16 bit high resolution mode 1 oneshot reading

    wire.requestFrom( BH1750ADDRESS , 2 );
    globalcache[6] = wire.receive();
    globalcache[7] = wire.receive();
    
    return;

}




























//

