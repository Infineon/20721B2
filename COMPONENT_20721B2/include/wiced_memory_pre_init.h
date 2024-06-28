/*
 * Copyright 2020-2024, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
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
/** @file
 *
 * support memory allocation tuning
 *
 */
#ifndef __WICED_MEMORY_PRE_INIT_H__
#define __WICED_MEMORY_PRE_INIT_H__

/* Usage:
 * This information shows how to use this memory allocation tuning:
 *
 * Step 1:
 *      In application add this head file:
 *      #include "wiced_memory_pre_init.h"
 *
 * Step 2:
 *      In application add the global variable, g_mem_pre_init:
 *      For example:
 *          WICED_MEM_PRE_INIT_CONTROL g_mem_pre_init =
 *          {
 *              .max_ble_connections = WICED_MEM_PRE_INIT_IGNORE,
 *              .max_peripheral_piconet = WICED_MEM_PRE_INIT_IGNORE,
 *              .max_resolving_list = WICED_MEM_PRE_INIT_IGNORE,
 *              .onfound_list_len = 0,
 *              .max_multi_adv_instances = WICED_MEM_PRE_INIT_IGNORE,
 *          };
 * Step 3:
 *      Modify the value of parameters in g_mem_pre_init (WICED_MEM_PRE_INIT_CONTROL structure).
 *
 *      Please see the following for the parameters of WICED_MEM_PRE_INIT_CONTROL structure.
 *
 *      max_ble_connections     - This is for BLE connections which device could support
 *                                Default value is 8
 *                                Max value is 8, user is able to tuning this value form 0 to 8.
 *
 *      max_peripheral_piconet  - This is for BT connections which device could support
 *                                Default value is 4
 *                                Max value is 4, user is able to tuning this value from 0 to 4.
 *
 *      max_resolving_list      - The Resolving List is used by the Link Layer to resolve
 *                                Resolvable Private Addresses used by advertisers, scanners or initiators.
 *                                This allows the Host to configure the Link Layer to act on a request
 *                                without awakening the Host.
 *                                Default value is 20
 *                                Max value is 20, user is able to tuning this value from 0 to 20.
 *
 *      onfound_list_len        - This is for BLE on found device entries.
 *                                This is for Android. Reference: Google LE HCI spec
 *                                Default value is 20
 *                                Max value is 20, user is able to tuning this value from 0 to 20.
 *
 *      max_multi_adv_instances - This is for device support multiple adverstising event instances
 *                                Default value is 8
 *                                Max value is 8, user is able to tuing this value from 0 to 8.
 */

/**
 * use this define value to indicate no change to parameter
 */
#define WICED_MEM_PRE_INIT_IGNORE (0xff)

/**
 * structure containing paramters to pass for memory pre-init
 */
typedef struct tag_mem_pre_init_control
{
    uint8_t max_ble_connections;
    uint8_t max_peripheral_piconet; /* use to reduce Bluetooth connections */
    uint8_t max_resolving_list;
    uint8_t onfound_list_len;
    uint8_t max_multi_adv_instances;
} WICED_MEM_PRE_INIT_CONTROL;

/**
 * Description:
 *      set pre-init memory allocation parameters.
 *      call this from spar_crt_init function to set parameters prior to allocations
 *      Note: Application don't need to call this function.
 *            Application only need to provide global variable, g_mem_pre_init and tuning the parameters.
 *
 * Input:
 *      mem_pre_init -  Parameters to pre init the memory, please see the above usage for more information.
 * Output:
 *      None
 * Return:
 *      None
 */
void wiced_memory_pre_init(WICED_MEM_PRE_INIT_CONTROL *mem_pre_init);
#endif
