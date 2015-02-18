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

