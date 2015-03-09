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
int backOff(void);
/*
** =============================================================================
**                   DEFINES
** =============================================================================
*/

#define DIFS      40
#define SLOT_TIME  2

/**
 * Enumeration containing the commands for channel access
*/
typedef enum
{
    WAIT_DIFS = 0,
    PERFORM_BACKOFF = 1,
    SEND_RTS = 2,
    WAIT_FOR_CTS = 3,
    SEND_PAY = 4,
    WAIT_FOR_ACK = 5
}CSMA_CMD;

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
unsigned char macAdd =5;

struct statistics
{
  uint16_t failed_tx;
  uint16_t successful_tx;
  
  uint16_t total_tx;
  uint16_t total_rx;
  
  uint16_t fromNode[10];
  
  uint16_t macCDCnt;
};

typedef struct 
{
  unsigned char destination;
  unsigned char source;
  unsigned char sequence;
  unsigned char type;
  unsigned char data[28];
}dataFrame;

statistics stats;

unsigned char seq_id = 0;

int macCD = 1;

dataFrame fr;

/*==============================================================================
** Function...: setup
** Return.....: void
** Description: Initial setup
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

void setup(void)
{
  randomSeed(analogRead(0));
  int routineTime = 1000;
  
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair_pl/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);


  radio.begin();
  //radio.enableAckPayload();

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
 
  
  char s[100];
  int ret = 1;
  
  timer.run();

  if (role == role_sender)
  {
    fr.destination = 0;
    fr.source = macAdd;
    seq_id++;
    fr.sequence = seq_id;
    fr.type = 1;
    channelAccess(WAIT_DIFS);
    ret = channelAccess(PERFORM_BACKOFF);
    
    if(0==ret)
    {
      radio.write( &fr, sizeof(fr) );
      stats.total_tx++;
    }
    //ret = sendPacket();

    //delay(1);
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
      
      /*
      dataFrame fr_ack;
      fr_ack.source = 0;
      fr_ack.destination = fr.source;
      fr_ack.type = 2;
      seq_id++;
      fr_ack.sequence = seq_id;
      
      radio.writeAckPayload( 1, &fr_ack, sizeof(fr_ack) );
      */
      
      stats.fromNode[fr.source]++;
    }
  }
}


/*==============================================================================
** Function...: sendPacket
** Return.....: void
** Description: main function
** Created....: 18.2.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int sendPacket(void)
{
  return 0;
}


int channelAccess(unsigned char action)
{
  int ret = -1;
  unsigned long time;
  unsigned long difsDelta=0;
  
  switch (action)
  {
    case WAIT_DIFS:
     
      // Listen for a little
      radio.startListening();
      
      ret = 0;
      time = micros();
      difsDelta = micros() - time;
      while(difsDelta<DIFS)
      {
        while ( radio.testCarrier() )
        {
          //printf(".");
          time = micros();
        }
        difsDelta = micros() - time;
        //printf("DIFS %lu\n",difsDelta);
      }
      //printf("\n");
    break;
    
    case PERFORM_BACKOFF:
      if(backOff())
      {
        //if CD then return:
        return -2;
        printf("Carrier Detect\n");
      }
      else
      {
        //else no CD then do the transmit
        ret = 0;
        radio.stopListening();
      }
    break;
    
    default:
      ret = -3;
    break;
  }  
  return 0;  
}  


int backOff(void)
{
  int backofSlot = SLOT_TIME;
  // Listen for a little
  radio.startListening();
  unsigned long time;
  unsigned long backOffDelta;
  unsigned int backoffTimer = 0;
  
  backoffTimer = random(1, 4); 
  
  time = micros();
  backOffDelta = micros() - time;
  
  while(!radio.testCarrier())
  {
    backOffDelta = micros() - time;
    if(backOffDelta>=backofSlot)
    {
      backoffTimer--;
      time = micros();
    }
    if(backoffTimer==0)
    {
      return 0;
    }
  }
  return 1;
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
  
  if( role == role_receiver )
  {
    sprintf(s,"%d %d %d %d %d | %d ",stats.fromNode[1], stats.fromNode[2], stats.fromNode[3], stats.fromNode[4], stats.fromNode[5], stats.total_rx);
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
  stats.macCDCnt = 0;
  stats.fromNode[1] = 0;
  stats.fromNode[2] = 0;
  stats.fromNode[3] = 0;
  stats.fromNode[4] = 0;
  stats.fromNode[5] = 0;
  
}