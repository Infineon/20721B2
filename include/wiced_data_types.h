/****************************************************************************
**
** Name:         wiced_data_types.h
**
** Description:  wiced data types header file for VS2010 projects
**
** Copyright (c) Cypress Semiconductor
**
******************************************************************************/

#ifndef WICED_DATA_TYPES_H
#define WICED_DATA_TYPES_H

#define WICED_SUPPRESS_WARNINGS(m) if((m)){;}

#ifdef INCLUDE_STDINT_H
#include "stdint.h"
#else
typedef unsigned int    uint32_t;
typedef signed char     int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
#endif

typedef unsigned int   wiced_bool_t;

#define WICED_FALSE 0
#define WICED_TRUE  1

#endif
