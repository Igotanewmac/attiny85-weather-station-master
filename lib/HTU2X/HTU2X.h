

// htu2x humidity sensor

#include <Arduino.h>

// temperature
uint16_t htu2x_gettemperature();

// humidity
uint16_t htu2x_gethumidity();



void htu2xgetallraw( uint8_t *globalcache );

void htu2xgettemperaturerawstart();

void htu2xgettemperaturerawend( uint8_t *globalcache );


void htu2xgethumidityrawstart();


void htu2xgethumidityrawend( uint8_t *globalcache );



//