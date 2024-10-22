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

  // configure SPI to the input parameters 
  SPI->CR1 |= _VAL2FLD(SPI_CR1_BR, br);
  SPI->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
  SPI->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);

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
  SPI->CR1 |= SPI_CR1_SPE;
};
    

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send) {
  
  // wait until the transmit buffer is empty (we're making sure there is nothing still to send)
  while(!(SPI_SR_TXE & SPI->SR));

  // write to data register to be sent
  *(volatile char *) (&SPI->DR) = send;

  // wait until the read buffer is not empty (we're waiting for all the data to be sent and then for data to be received)
  while(!(SPI_SR_RXNE & SPI->SR));  

  char received = (volatile char) SPI->DR;
  // read data from the data register
  return received; 
};


 
