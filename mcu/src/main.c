/*
File: main.c
Author: Marina Ring, with code provided by Prof. Josh Brake
Email: mring@g.hmc.edu
Date: 10/20/2024
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";
char* ledStr = "<p>LED Control:</p>\
        <form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";
char* tempForm = "<p>Bit Resolution Control:</p>\
        <form action=\"bit8\"><input type=\"submit\" value=\"8-Bit\"></form>\
	<form action=\"bit9\"><input type=\"submit\" value=\"9-Bit\"></form>\
        <form action=\"bit10\"><input type=\"submit\" value=\"10-Bit\"></form>\
        <form action=\"bit11\"><input type=\"submit\" value=\"11-Bit\"></form>\
        <form action=\"bit12\"><input type=\"submit\" value=\"12-Bit\"></form>";
char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[], int old_led_status)
{
	int led_status = old_led_status;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

int updateTempConfig(char request[]) {
  int temp_config = BIT8;

  temp_config = (inString(request, "bit9")) ? BIT9 : temp_config;
  temp_config = (inString(request, "bit10")) ? BIT10 : temp_config;
  temp_config = (inString(request, "bit11")) ? BIT11 : temp_config;
  temp_config = (inString(request, "bit12")) ? BIT12 : temp_config;

  return temp_config;
}


/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////


int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(LED_PIN, GPIO_OUTPUT);
  pinMode(COPI, GPIO_ALT);
  pinMode(CIPO, GPIO_ALT);
  pinMode(SCK, GPIO_ALT);
  pinMode(CS, GPIO_OUTPUT);

  //GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 0b0101); // select CIPO as AF5
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 0b0101); // select PA5 as AF5
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 0b0101); // select PA6 as AF5
  GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 0b0101); // select PA12 as AF5

  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);

  initTIM(TIM15);
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  // initialize SPI
  // CPHA MUST BE SET TO 1
  initSPI(0b111, 1, 1);

  while(1) {
  // initialize temperature sensor
  digitalWrite(CS, 1);
  configureTemp(BIT8);
  digitalWrite(CS, 0);

  }
  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }

    // read temperature
    //float temp_status = readTemp();

    char tempStatusStr[50];
    //sprintf(tempStatusStr, "The temperature is %f deg C", temp_status);

    // Update string with current LED state
    int led_status = updateLEDStatus(request, led_status);

    // Update string with current temp config
    int temp_config_status = updateTempConfig(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");

    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED

    sendString(USART, "<h2>LED Status</h2>");


    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");

    sendString(USART, "<h2>Temperature Status</h2>");
    sendString(USART, tempForm);
    //sendString(USART, "<p>");
    //sendString(USART, tempStatusStr);
    //sendString(USART, "</p>");

  
    sendString(USART, webpageEnd);
  }
}
