/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */
#pragma once

#include "fx_api.h"
#include "brcm_fw_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif


/******************************************************
 *                      Macros
 ******************************************************/
#define IMAGE_FILENAME          "image.bin"
/* Only enable ONE of the following options */

/* Enable to make File-X read-only */
//#define FILEX_WRITE_STRATEGY  BLOCK_DEVICE_READ_ONLY

#define FILEX_WRITE_STRATEGY  BLOCK_DEVICE_WRITE_IMMEDIATELY

/* Enable to allow write-behind in File-X */
//#define FILEX_WRITE_STRATEGY  BLOCK_DEVICE_WRITE_BEHIND_ALLOWED

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    char* filename;
    FILE* image_file_handle;
    wiced_block_device_write_mode_t write_mode;
} tester_block_device_specific_data_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
wiced_result_t  spi_block_device_init( wiced_block_device_t* device, wiced_block_device_write_mode_t write_mode );
wiced_result_t  spi_block_device_deinit( wiced_block_device_t* device );
wiced_result_t  spi_block_write( wiced_block_device_t* device, uint32_t start_address, const uint8_t* data, uint32_t size );
wiced_result_t  spi_block_flush( wiced_block_device_t* device );
wiced_result_t  spi_block_read( wiced_block_device_t* device, uint32_t start_address, uint8_t* data, uint32_t size );
wiced_result_t  spi_block_register_callback( wiced_block_device_t* device, wiced_block_device_status_change_callback_t callback );
wiced_result_t  spi_block_status( wiced_block_device_t* device, wiced_block_device_status_t* status );
/******************************************************
 *               Function Definitions
 ******************************************************/

VOID  wiced_filex_driver( FX_MEDIA* media_ptr );

#ifdef __cplusplus
} /*extern "C" */
#endif
