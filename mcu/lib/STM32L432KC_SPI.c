// STM32L432KC_SPI.c
// Marina Ring
// mring@g.hmc.edu
// 10/18/2024
// Functions that set up SPI peripherals on an STM32 microcontroller

#include "STM32L432KC_SPI.h"

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
 *          1: data changed on leading edge of clk and captured on next edge)
 * Refer to the datasheet for more low-level details. */ 
void initSPI(int br, int cpol, int cpha) {
  SPI_TypeDef * SPI = SPI1;

  // configure SPI to the input parameters 
  SPI->CR1 |= (br << SPI_CR1_BR_Pos);
  SPI->CR1 |= (cpol << SPI_CR1_CPOL_Pos);
  SPI->CR1 |= (cpha << SPI_CR1_CPOL_Pos);

  // enable half-duplex mode
  SPI->CR1 |= SPI_CR1_BIDIMODE;
  SPI->CR1 |= SPI_CR1_BIDIOE; 

  // receive data with MSB first 
  SPI->CR1 &= ~SPI_CR1_LSBFIRST;
  
  // bypass NSS, control SPI through software
  SPI->CR1 |= SPI_CR1_SSI;
  SPI->CR1 |= SPI_CR1_SSM;

  // set MCU as controller
  SPI->CR1 |= SPI_CR1_MSTR;

  // set 8 bit package
  SPI->CR2 |= (0b0111 << SPI_CR2_DS_Pos);
  SPI->CR2 |= SPI_CR2_FRXTH;

  // enable SSOE control bit
  SPI->CR2 |= SPI_CR2_SSOE;

  // enable SPI
  // I think maybe I need to wait to do this until after I configure data to send to temperature sensor?
  // SPI->CR1 |= SPI_CR1_SPE;
};
    

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send) {
  
  

};


 
