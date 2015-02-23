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
unsigned char macAdd = 1;

struct statistics
{
  uint16_t failed_tx;
  uint16_t successful_tx;
  
  uint16_t total_tx;
  uint16_t total_rx;
  
  uint16_t fromA;
  uint16_t fromB;
  
  uint16_t macCDCnt;
  
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

int macCD = 1;

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
  
  // the function is called every routineTime interval
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
    fr.source = macAdd;
    seq_id++;
    fr.sequence = seq_id;
    fr.type = 1;
    
    
    macCD = 1;
    while(macCD)
    {
      // Listen for a little
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();
      // Did we get a carrier?
      if ( radio.testCarrier() )
      {
        stats.macCDCnt++;
      }
      else
      {
        macCD=0;
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
      }
    }

    delay(100);
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
      
      if (fr.source == 1)
      {
        stats.fromA++;
      }
      else if (fr.source == 2)
      {
        stats.fromB++;
        
      }
      
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
  
  if( role == role_receiver )
  {
    sprintf(s,"%d %d %d %d %d %d ",stats.fromA, stats.fromB, stats.total_rx, stats.total_tx, stats.successful_tx, stats.failed_tx);
    Serial.println(s);
    
    
  }
  else
  {
    sprintf(s," %d %d %d %d %d", stats.total_rx, stats.total_tx, stats.successful_tx, stats.failed_tx, stats.macCDCnt);
    Serial.println(s);
  }
  
  stats.total_rx = 0;
  stats.total_tx = 0;
  stats.failed_tx = 0;
  stats.successful_tx = 0;
  stats.fromA = 0;
  stats.fromB = 0;
  stats.macCDCnt = 0;
  
  
}