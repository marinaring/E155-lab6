// DS1722.h
// Marina Ring
// mring@g.hmc.edu
// October 20, 2024
// This is the header file for all of the functions related to communication with a DS1722 temperature sensor

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include <math.h>
#include <stm32l432xx.h>
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define BIT8 0b000
#define BIT9 0b001
#define BIT10 0b010
#define BIT11 0b011
#define BIT12 0b100

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void configureTemp(int resolution);

float readTemp(void);

float convertTempToFloat(char read_lsb, char read_msb);

#endif