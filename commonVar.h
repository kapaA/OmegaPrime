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
        File name: commonVar.h
        Version:   v0.0
        Date:      2014-10-31
    =========================================================================
*/

#ifndef __COMON_VAR_H__
#define __COMON_VAR_H__

#ifndef ARDUINO
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#endif


/**
 * Typedef struct containing the payload structure
 * Size: 6 bytes
*/
typedef struct{
  uint8_t data[2];         // 2 bytes
  unsigned long seqNum;    // 4
}  pload;  

/**
 * Typedef struct containing the header structure
 * Size: 2 bytes
*/
typedef struct{
  uint8_t  type    :4;    // 4 bits
  uint8_t srcAddr  :6;    // 6 bits
  uint8_t destAddr :6;    // 6 bits
}  head;

/*
** ==========================================================================
**                       Extern Global variables
** ==========================================================================
*/


extern configuration config;


#endif

