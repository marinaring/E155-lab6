/**
    Main Header: Contains general defines and selected portions of CMSIS files
    @file main.h
    @author Josh Brake
    @version 1.0 10/7/2020
*/

#ifndef MAIN_H
#define MAIN_H

#include "../lib/STM32L432KC.h"
#include "../lib/DS1722.h"

#define LED_PIN PB5 // LED pin for blinking on Port B pin 5
#define BUFF_LEN 32
#define SPI SPI1
#define COPI PA12
#define CIPO PA6
#define SCK PB3

#endif // MAIN_H