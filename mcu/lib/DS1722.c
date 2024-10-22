// DS1722.c
// Marina Ring
// mring@g.hmc.edu
// October 20, 2024
// The functions related to communication with a DS1722 temperature sensor


#include "DS1722.h"


void configureTemp(int resolution) {
  
  // send signal that we are changing the configuration register
  spiSendReceive(0x80);

  // send new configuration
  int default_config = 0b11100000;
  int new_config = (resolution << 1) | default_config; // concatenate default configuration with resolution chosen
  spiSendReceive(new_config);

};


float readTemp(void) {
  digitalWrite(PB6, 1);
  spiSendReceive(0x01);
  char temp_lsb = spiSendReceive(0x00);
  digitalWrite(PB6, 0);
  digitalWrite(PB6, 1);
  spiSendReceive(0x02);
  char temp_msb = spiSendReceive(0x00);
  digitalWrite(PB6, 0);

  return convertTempToFloat(temp_lsb, temp_msb);
};


float convertTempToFloat(char read_lsb, char read_msb) {
  
  float temp_value = 0;

  read_lsb = read_lsb >> 4; // the last 4 bits of the least significant bits are just default zeros
  
  // for the most significant bits, the MSB is the sign bit
  // so, if we have a negative sign, need to take two's compliment before 
  if ((read_msb >> 7) & 1) {
    temp_value = (~read_msb); 
    read_lsb = ~read_lsb + 1; 

    // for the least significant bits, we have the following format (after shifting left by 4):
    // 2^-1  2^-2  2^-3  2^-4
    for (int i = 0; i < 4; i++) {
      int bit_value = (read_lsb >> i) & 1; // get the bit value
      temp_value += (float) bit_value * pow(2, -4 + i); // multiply by the appropriate power
    }

    // change to negative value
    temp_value = -1.0 * temp_value; 

  }
  else {
    temp_value = read_msb;

    for (int i = 0; i < 4; i++) {
      int bit_value = (read_lsb >> i) & 1; // get the bit value
      temp_value += (float) bit_value * pow(2, -4 + i); // multiply by the appropriate power
    }
  }

  return temp_value;
  
};

