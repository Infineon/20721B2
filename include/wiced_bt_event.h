/*
 * Copyright 2020, Cypress Semiconductor Corporation or a subsidiary of
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
 /** @file
 *
 *  This implements the application-thread level event handling for WICED Apps
 */
#ifndef __WICED_BT_EVENT_H__
#define __WICED_BT_EVENT_H__
#include "wiced.h"

/*
 * Size of historical failure code size
 *
 * @note This definition value is not permitted to change.
 */
#define WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_HISTORY_SIZE 5

typedef enum
{
     WICED_SERIALIZATION_EVENT = 1,
}wiced_bt_internal_events_t;

/*
 * WiCED APP Event Serialization Failure Code
 *
 * @note This failure code is used to check the failure status if the return value
 *       of wiced_app_event_serialize() utility is FALSE.
 */
typedef enum wiced_app_event_serialization_failure_code
{
    /*
     * 0x00: Invalid failure code. This is the default value and shall be ignored while debugging.
     */
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_INVALID                  = 0x00,

    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_NOT_INITIALIZED          = 0x10,

    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_NO_MEMORY                = 0x20,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_QUEUE_FULL               = 0x21,

    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_MUTEX_CREATE_FAIL        = 0x30,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_MUTEX_GET_FAIL           = 0x31,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_MUTEX_PUT_FAIL           = 0x32,

    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_EVENT_SET_FAIL           = 0x40,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_EVENT_INVALID_VALUE      = 0x41,

    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_EVENT_QUEUE_ADD_FAIL     = 0x50,

    /* Failure code for handling event. */
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_HANDLER_MUTEX_GET_FAIL   = 0x60,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_HANDLER_MUTEX_PUT_FAIL   = 0x61,
    WICED_APP_EVENT_SERIALIZATION_FAILURE_CODE_HANDLER_EVENT_SET_FAIL   = 0x62,

} wiced_app_event_serialization_failure_code_t;

 /* The serialization queue will have these callbacks */
typedef struct
{
    // The function to invoke in application thread context
    int (*fn)(void*);

    // Any arbitrary data to be given to the callback. wiced_app_event_serialize Caller has to allocate and free once serialized event handled
    void* data;
} wiced_app_event_srzn_cb_t;


/*
 *This function lets you serialize a call onto the application thread.
 *
 *@param[in]    fn          : serialized call back on serialization
 *
 *@param[in]    data        : data to be handled in serialized call

 * @return      wiced_bool_t

 * Note: It is application's responsibility freeing data pointer
 */
wiced_bool_t wiced_app_event_serialize( int (*fn)(void*), void* data);

/*
 * WiCED Application Serialization Event handling debug callback.
 *
 * @param[in]   error code: Refer to the error codes defined in the section,
 *              "Failure code for handling the event", in enum,
 *              wiced_app_event_serialization_failure_code_t.
 *
 * @note        This is used for debugging the serialization event handler in application layer
 */
typedef void (wiced_app_event_debug_callback_t)(wiced_app_event_serialization_failure_code_t error_code);

/*
 * WiCED Application Event Serialization Handler DeBug Callback Register
 *
 * @param[in]   callback
 *
 * @note        While an error in occurred in the serialization event handler, the registered
 *              callback will be called to indicate current situation with error code defined
 *              in wiced_app_event_serialization_failure_code_t.
 */
void wiced_app_event_serialization_debug_callback_register( wiced_app_event_debug_callback_t *callback );

#endif //__WICED_BT_EVENT_H__
