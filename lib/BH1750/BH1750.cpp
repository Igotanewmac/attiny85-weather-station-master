

// bh1750 

#include <Arduino.h>


#include <TinyWireM.h>

#ifndef tw
#define tw TinyWireM
#endif

#define BH1750ADDRESS 0x23




// low resolution oneshot.
uint16_t bh1750lowresoneshot() {

    // get a 16 bit high resolution mode 1 oneshot reading


    // say hello to the chip
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00000001 ); // power on
    tw.endTransmission();

    // zero out the reading register
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00000111 ); // reset register
    tw.endTransmission();

    // initiate sensing
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00100000 ); // initiate sensing
    tw.endTransmission();

    // this can take up to 180ms
    //delay( 200 );

    uint16_t retval = 0;

    while ( retval == 0 ) {
    tw.requestFrom( BH1750ADDRESS , 2 );
    retval = tw.receive() << 8;
    retval += tw.receive();
    }

    return retval;

}






void bh1750lowresoneshotrawstart() {

    // get a 16 bit high resolution mode 1 oneshot reading

    // say hello to the chip
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00000001 ); // power on
    tw.endTransmission();
    // zero out the reading register
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00000111 ); // reset register data to zero
    tw.endTransmission();
    // initiate sensing
    tw.beginTransmission( BH1750ADDRESS );
    tw.send( 0b00100000 ); // initiate sensing
    tw.endTransmission();

    // this can take up to 180ms
    //delay( 200 );

    return;
}



// low resolution oneshot.
void bh1750lowresoneshotrawend( uint8_t *globalcache ) {

    // get a 16 bit high resolution mode 1 oneshot reading

    globalcache[6] = 0;

    tw.requestFrom( BH1750ADDRESS , 2 );
    globalcache[6] = tw.receive();
    globalcache[7] = tw.receive();
    
    return;

}




























//

