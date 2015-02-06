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
        File name: vEndNode.ino
        Version:   v0.1
        Date:      2015-02-1
    ============================================================================
*/

/*
** =============================================================================
**                   INCLUDE STATEMENTS
** =============================================================================
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <commonVar.h>

/*
** =============================================================================
**                   DEFINES
** =============================================================================
*/

// #define DEBUG

#define DHT_SENSOR
#define DHTPIN A2
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

/*
** =============================================================================
**                   LOCAL VARIABELS
** =============================================================================
*/

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio

const uint16_t nodeID        = 011;  // Address of this node in Octal format ( 04,031, etc)
const uint16_t baseStationID = 00;  // Address of the base station in Octal format
uint16_t packets_sent = 0;

/*==============================================================================
** Function...: setup
** Return.....: void
** Description: Initial setup
** Created....: 28.11.2014 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ nodeID);
  
}


/*==============================================================================
** Function...: loop
** Return.....: void
** Description: main function
** Created....: 28.11.2014 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void loop(void){
  
  network.update();                  // Check the network regularly

  Serial.print("Sending...");
	
  payload_t payload = { millis(), packets_sent++ };
	
  RF24NetworkHeader header(/*to node*/ baseStationID);
	
  bool ok = network.write(header,&payload,sizeof(payload));
	
  if (ok)
    Serial.println("ok.");
  else
    Serial.println("failed.");
    
  delay(1000);
}

