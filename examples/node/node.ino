/*  ============================================================================
    Copyright (C) 2015 Achuthan Paramanathan.
    RF24 lib. provided by 2011 J. Coliz <maniacbug@ymail.com>
    ============================================================================
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.
    ============================================================================
    Revision Information:
        File name: node.ino
        Version:   v0.0
        Date:      04-03-2015
    ==========================================================================*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS                               */
/*============================================================================*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SimpleTimer.h"

/*============================================================================*/
/*                           PRIVATE FUNCTIONS                                */
/*============================================================================*/
void printOut(void);
void sendPacket(void);

/*============================================================================*/
/*                           Hardware configuration                           */
/*============================================================================*/
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

SimpleTimer printTimer;

/*============================================================================*/
/*                           LOCAL VARIABLES                                  */
/*============================================================================*/

unsigned char macAdd = 1;
unsigned char seq_id = 0;
unsigned int sendTurn = 0;

struct frame
{
  unsigned char destination;
  unsigned char source;
  unsigned char sequence;
  unsigned char type;
  unsigned char data[28];
};

struct statistics
{
  uint16_t failed_tx;
  uint16_t successful_tx;
  
  uint16_t total_tx;
  uint16_t total_rx;
  
  uint16_t macCDCnt;
};

statistics stats;

/*==============================================================================
** Function...: setup
** Return.....: int
** Description: setup
** Created....: 06.03.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  printf("\n\rNode\n\r");
  
  radio.begin();
  radio.enableAckPayload();
  radio.openWritingPipe(pipe);
  radio.printDetails();
  
  randomSeed(analogRead(0));
  
  // the function is called every routineTime interval
  printTimer.setInterval(1000, printOut);

}  


/*==============================================================================
** Function...: loop
** Return.....: int
** Description: Main function
** Created....: 06.03.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void loop(void)
{
  
  printTimer.run();
  
  
  
  
}  


void sendPacket(void)
{
  frame fr;
  
  fr.destination = 0;
  fr.source = macAdd;
  seq_id++;
  fr.sequence = seq_id;
  fr.type = 1;
  
  radio.write( &fr, sizeof(fr) );
  stats.total_tx++;
  
  if ( radio.isAckPayloadAvailable() )
  {
    radio.read(&fr,sizeof(fr));
  }
}


/*==============================================================================
** Function...: printOut
** Return.....: void
** Description: main function
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void printOut(void)
{
  char s[100];
  
  sprintf(s," %d %d %d %d %d", stats.total_tx, stats.total_rx, stats.successful_tx, stats.failed_tx, stats.macCDCnt);
  Serial.println(s);
  
  stats.total_rx = 0;
  stats.total_tx = 0;
  stats.failed_tx = 0;
  stats.successful_tx = 0;
  stats.macCDCnt = 0;
}  
