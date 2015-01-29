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
typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;
#endif

typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;


/**
 * Typedef struct containing the payload structure
 * Size: 28 bytes
*/
typedef struct{
  unsigned long ms;    
  unsigned long counter;
}  payload_t;  



/**
* Typedef struct containing configuration info
*/
typedef struct
{
  uint8_t rolde_id;
  uint8_t frame_mode;
  uint8_t mac_addr;
} configuration;

/*
** ==========================================================================
**                       Extern Global variables
** ==========================================================================
*/


extern configuration config;


#endif

