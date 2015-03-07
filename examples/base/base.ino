/*  ============================================================================
    Copyright (C) 2015 Achuthan Paramanathan.
    RF24 lib. provided by 2011 J. Coliz <maniacbug@ymail.com>
    ============================================================================
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.
    ============================================================================
    Revision Information:
        File name: base.ino
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

/*============================================================================*/
/*                           Hardware configuration                           */
/*============================================================================*/
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

SimpleTimer timer;

/*============================================================================*/
/*                           LOCAL VARIABLES                                  */
/*============================================================================*/

unsigned char seq_id = 0;

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
  
  uint16_t fromNode[20];
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
  printf("\n\rBase\n\r");
  
  radio.begin();
  radio.enableAckPayload();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  radio.printDetails();
  
  // the function is called every routineTime interval
  timer.setInterval(1000, printOut);
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
  timer.run();
  
  if ( radio.available() )
  {
    frame fr;
    bool done = false;
    while (!done)
    {
      done = radio.read( &fr, sizeof(fr) );
      stats.total_rx++;
      stats.fromNode[fr.source]++;
    }

    frame fr_ack;
    fr_ack.source = 0;
    fr_ack.destination = fr.source;
    fr_ack.type = 2;
    seq_id++;
    fr_ack.sequence = seq_id;
    
    radio.writeAckPayload( 1, &fr_ack, sizeof(fr_ack) );
  }
}



/*==============================================================================
** Function...: loop
** Return.....: void
** Description: main function
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void printOut(void)
{
  char s[100];
  
  sprintf(s,"%d %d %d %d %d %d | %d ",stats.fromNode[1], stats.fromNode[2],stats.fromNode[3],stats.fromNode[4],stats.fromNode[5],stats.fromNode[6], stats.total_rx);
  Serial.println(s);
  
  stats.total_rx = 0;
  stats.total_tx = 0;
  stats.failed_tx = 0;
  stats.successful_tx = 0;
  stats.fromNode[1] = 0;
  stats.fromNode[2] = 0;
  stats.fromNode[3] = 0;
  stats.fromNode[4] = 0;
  stats.fromNode[5] = 0;
  stats.fromNode[6] = 0;
}