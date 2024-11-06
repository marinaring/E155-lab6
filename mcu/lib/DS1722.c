// DS1722.c
// Marina Ring
// mring@g.hmc.edu
// October 20, 2024
// The functions related to communication with a DS1722 temperature sensor


#include "DS1722.h"


void configureTemp(int resolution) {
  // send new configuration
  int default_config = 0b11100000;
  int new_config = (resolution << 1) | default_config; // concatenate default configuration with resolution chosen
  
  
  // send signal that we are changing the configuration register
  digitalWrite(CS, 1);
  spiSendReceive(0x80);
  spiSendReceive(new_config);
  digitalWrite(CS, 0);

};


float readTemp(void) {
  digitalWrite(CS, 1);
  spiSendReceive(0x01);
  char temp_lsb = spiSendReceive(0xA1);
  digitalWrite(CS, 0);

  delay_millis(TIM15, 5);

  digitalWrite(CS, 1);
  spiSendReceive(0x02);
  char temp_msb = spiSendReceive(0xA1);
  digitalWrite(CS, 0);

  delay_millis(TIM15, 5);

  return convertTempToFloat(temp_lsb, temp_msb);
};


float convertTempToFloat(char read_lsb, char read_msb) {
  
  int convert = (int) (read_msb << 8) | read_lsb;

  return (float) convert/pow(2, 8);
};

