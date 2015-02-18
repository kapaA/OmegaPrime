/*  ============================================================================
    Copyright (C) 2014 - 2016 Achuthan
    All rights reserved.
    ============================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorization.
    ============================================================================
    Revision Information:
        File name: helloWorld.ino
        Version:   v0.1
        Date:      18.2.2015
    ============================================================================
*/

/*
** =============================================================================
**                   INCLUDE STATEMENTS
** =============================================================================
*/


#include <SPI.h>
#include "commonInterface.h"

/*
** =============================================================================
**                   DEFINES
** =============================================================================
*/



/*
** =============================================================================
**                   LOCAL VARIABELS
** =============================================================================
*/

RF24 radio(9,10);      // nRF24L01(+) radio attached using Getting Started board 
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


/*==============================================================================
** Function...: setup
** Return.....: void
** Description: Initial setup
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void setup(void)
{
  //Start Serial communication
  Serial.begin(57600);
  delay(20);
  //Printf helper function
  printf_begin();
  
  // Start the transceiver  
  radio.begin();
  // Set data rate
  radio.setDataRate(RF24_1MBPS);
  // Set the channel we want to use
  radio.setChannel(17);
  // Set the power level
  radio.setPALevel(RF24_PA_LOW);
  // Set the CRC check level
  radio.setCRCLength(RF24_CRC_8);
  // set the delay between retries (uS) & # of retries
  radio.setRetries(15,15);
  // fixed payload size of 9 bytes
  radio.setPayloadSize(9);
  //Open read/write pipelines
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  //Start the listening
  radio.startListening();

  radio.printDetails();
}


/*==============================================================================
** Function...: loop
** Return.....: void
** Description: main function
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void loop(void){
  
  
}

