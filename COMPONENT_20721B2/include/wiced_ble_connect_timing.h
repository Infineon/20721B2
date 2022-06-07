/*
 * Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
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
 * support callback notification with timing since last ble connection
 *
 */
#ifndef __WICED_BLE_CONNECT_TIMING_H__
#define __WICED_BLE_CONNECT_TIMING_H__

typedef struct tag_app_connect_timing_data
{
    UINT64  cur_local_time64;               // Free-running local time in uSecs, at instant of callback
    UINT32  time_since_conn_evt;            // Time since Connection Event in uSecs
    UINT16  connection_handle;
    UINT8   rssi;
} BLE_CONNECT_TIMING_APP_DATA;

typedef void (*BLE_CONNECT_TIMING_CB_t)(BLE_CONNECT_TIMING_APP_DATA *app_data);

void ble_connect_timing_enable(BLE_CONNECT_TIMING_CB_t cb, BOOL32 provisioner);
void ble_connect_timing_disable();
void ble_bms_placement_enable(void);
void ble_bms_placement_disable(void);

#endif
