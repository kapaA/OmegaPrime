#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <commonVar.h>
#include "printf.h"
#include <avr/sleep.h>
#include <avr/power.h>


RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio

const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t dest_node = 00;  // Address of the other node in Octal format

unsigned long packets_sent;          // How many have we sent already

void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  
  /******************************** This is the configuration for sleep mode ***********************/
  network.setup_watchdog(wdt_1s); //The watchdog timer will wake the MCU and radio every second to send a sleep payload, then go back to sleep
}

void loop(void){
  
  network.update();                  // Check the network regularly
  radio.stopListening();             // Switch to PTX mode. Payloads will be seen as ACK payloads, and the radio will wake up
  network.sleepNode(8,255);            // Sleep the node for 8 cycles of 1second intervals
	
  payload_t payload = { millis(), packets_sent++ };
  RF24NetworkHeader header(dest_node);
  bool ok = network.write(header,&payload,sizeof(payload));
	
  if (ok)
    Serial.println("ok.");
  else
    Serial.println("failed.");
  
}