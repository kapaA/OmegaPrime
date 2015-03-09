#include <RF24MAC.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// MAC uses that radio
RF24MAC mac(radio);

// Address of our node
const uint16_t source_add = 1;
// Address of the other node
const uint16_t dest_add = 0;

void setup(void)
{
	Serial.begin(57600);
	Serial.println("RF24Network/examples/helloworld_tx/");
	
	SPI.begin();
	radio.begin();
	mac.begin(/*channel*/ 90, /*node address*/ source_add);
}

void loop(void)
{
}