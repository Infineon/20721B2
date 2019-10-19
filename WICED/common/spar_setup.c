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
/**
 * The Stackable Patch and Application Runtime
 * C-runtime setup of this SPAR tier
 */
#include "spar_utils.h"
#include "sparcommon.h"
#include <string.h>
#include <stdint.h>
#include <wiced_platform.h>
#include <wiced_sleep.h>

/*****************************************************************
*   External definitions
*
*****************************************************************/
extern void *g_dynamic_memory_MinAddress;
extern void *g_aon_memory_manager_MinAddress;
extern void install_libs(void);
extern void application_start(void);
extern void wiced_app_hal_init(void);

__attribute__((section(".app_init_code")))
void application_start_internal(void)
{
    wiced_platform_init();
    application_start();
}

/* WEAK application_pre_init function. Will be called if not defined anywhere else */
__attribute__((weak))
void wiced_pre_init(void)
{

}

/*****************************************************************
 *   Function: spar_setup()
 *
 *   Abstract: Process the information in .secinfo, copying and
 *   clearing sections as needed.
 *****************************************************************/
__attribute__((section(".spar_setup")))
void SPAR_CRT_SETUP(void)
{
    extern void* spar_iram_bss_begin;
    extern unsigned spar_iram_data_length, spar_iram_bss_length;
    extern void* spar_irom_data_begin, *spar_iram_data_begin, *spar_iram_end, *aon_iram_end;

    // Initialize initialized data if .data length is non-zero and it needs to be copied from NV to RAM.
    if(((uint32_t)&spar_irom_data_begin != (uint32_t)&spar_iram_data_begin) && ((uint32_t)&spar_iram_data_length != 0))
        memcpy((void*)&spar_iram_data_begin, (void*)&spar_irom_data_begin, (uint32_t)&spar_iram_data_length);

    // // Clear the ZI section
    if((uint32_t)&spar_iram_bss_length != 0)
    {
        memset((void*)&spar_iram_bss_begin, 0x00, (uint32_t)&spar_iram_bss_length);
    }

    // And move avail memory above this spar if required
    // Note that if there are other spars will be placed with minimum
    // alignment (because of the linker option to IRAM_SPAR_BEGIN) and itself
    // is responsible for moving the avail mem ptr.
    g_dynamic_memory_MinAddress = (void *)(((uint32_t)&spar_iram_end + 32) & 0xFFFFFFF0);
    if (WICED_SLEEP_COLD_BOOT == wiced_sleep_get_boot_mode())
    {
        g_aon_memory_manager_MinAddress = (void *)(&aon_iram_end);
    }
    // Install included libraries and patches if any
    install_libs();

    // Setup the application start function.
    wiced_bt_app_pre_init = application_start_internal;

    // Call Application pre-initialization function (either the weak or the real (lib) one)
    wiced_pre_init();
}
