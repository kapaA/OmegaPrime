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
        Date:      21.2.2015
    ============================================================================
*/

/*
** =============================================================================
**                   INCLUDE STATEMENTS
** =============================================================================
*/

#include "SimpleTimer.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

void printOut(void);

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

RF24 radio(9,10);

SimpleTimer timer;

const uint64_t pipe = 0xE8E8F0F0E1LL;
typedef enum { role_sender = 1, role_receiver } role_e;
const char* role_friendly_name[] = { "invalid", "Sender", "Receiver"};
  
role_e role = role_sender;

struct statistics
{
  uint16_t failed_tx;
  uint16_t successful_tx;
  
  uint16_t total_tx;
  uint16_t total_rx;
  
};



struct frame
{
  unsigned char destination;
  unsigned char source;
  unsigned char sequence;
  unsigned char type;
  unsigned char data[28];
};

statistics stats;

unsigned char seq_id = 0;

/*==============================================================================
** Function...: setup
** Return.....: void
** Description: Initial setup
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

void setup(void)
{

  int routineTime = 1000;
  
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair_pl/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);


  radio.begin();
  radio.enableAckPayload();


  if ( role == role_sender )
  {
    radio.openWritingPipe(pipe);
  }
  else
  {
    radio.openReadingPipe(1,pipe);
  }
  
  
  if ( role == role_receiver )
    radio.startListening();

  radio.printDetails();
  
  // the function is called evry routineTime interval
  timer.setInterval(routineTime, printOut);
}


/*==============================================================================
** Function...: loop
** Return.....: void
** Description: main function
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

void loop(void)
{
  static uint32_t message_count = 0;
  
  frame fr;
  char s[100];
  
  
  timer.run();

  if (role == role_sender)
  {
    fr.destination = 0;
    fr.source = 2;
    seq_id++;
    fr.sequence = seq_id;
    fr.type = 1;
    
    unsigned long time = millis();
    radio.write( &fr, sizeof(fr) );
    stats.total_tx++;

    if ( radio.isAckPayloadAvailable() )
    {
      radio.read(&fr,sizeof(fr));
      stats.successful_tx++;
      
      //sprintf(s,"ACK: %d %d %d %d", fr.destination, fr.source, fr.sequence, fr.type);
      //Serial.println(s);
    }
    else
    {
      stats.failed_tx++;
    }
    
    //delay(2);
  }

  if ( role == role_receiver )
  {
    if ( radio.available() )
    {
      
      
      static unsigned long got_time;
      bool done = false;
      while (!done)
      {
        done = radio.read( &fr, sizeof(fr) );
        stats.total_rx++;
        
        //sprintf(s,"PL: %d %d %d %d", fr.destination, fr.source, fr.sequence, fr.type);
        //Serial.println(s);
        
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
  
  sprintf(s," %d %d %d %d ", stats.total_rx, stats.total_tx, stats.successful_tx, stats.failed_tx);
  Serial.println(s);
  
  stats.total_rx = 0;
  stats.total_tx = 0;
  stats.failed_tx = 0;
  stats.successful_tx = 0;
}