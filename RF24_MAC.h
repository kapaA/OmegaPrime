/*  =========================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved.
    =========================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorization.
    =========================================================================
    Revision Information:
        File name: RF24_MAC.h
        Version:   v0.0
        Date:      18-02-2015
    =========================================================================
*/

#ifndef __RF24_MAC_H__
#define __RF24_MAC_H__

class RF24;

struct RF24MacHeader
{
  uint8_t destination;
  uint8_t source;
  uint8_t type;
  uint8_t seq_id;

  static uint8_t seqCnt;

  /*Default constructor*/
  RF24MacHeader() {}

  /* constructor for the upper layer */
  RF24MacHeader(uint8_t _dest, uint8_t _src, uint8_t _type, uint8_t _seq): destination(_dest), source(_src), type(_type), seq_id(seqCnt++) {}

};



class RF24_MAC
{

private:

  uint8_t aSlotTime; // The Slot Time (in microseconds) that the MAC uses for defining the PIFS and DIFS periods.
  uint8_t aSIFSTime; // Short interframe space
  uint8_t aDIFSTime; // Distributed (coordination function) interframe space
  uint8_t aMacReTx;  // Number of time MAC retransmits a packet

  RF24& radio; /**< Underlying radio driver, provides link/physical layers */


public: 
  /**
   * Construct the medium access control protocol
   * @param _radio The underlying radio driver instance
   */
  RF24_MAC(RF24& _radio);

  /**
   * Bring up the mac
   * @warning Be sure to 'begin' the radio first.
   * @param _channel The RF channel to operate on
   * @param _soruce  The MAC address of this node
   */
  void begin(uint8_t _channel, uint8_t _mac_address );

  void setSlotTime(uint8_t sTime);
  void setSifs(uint8_t sifs);
  void setDifs(uint8_t difs);
  void setMacReTx(uint8_t reTx);

};


#endif // __RF24_MAC_H__