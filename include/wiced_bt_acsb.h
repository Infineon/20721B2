/*
 * Copyright 2016-2020, Cypress Semiconductor Corporation or a subsidiary of
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
* WICED Audio ConnectionLess Slave Broadcast (ACSB) API functions
*
*/

#pragma once

#include "wiced.h"
#include "wiced_bt_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t hci_status_t;

typedef uint8_t lt_addr_t;

#define CHANNEL_MAP_LEN     10
typedef uint8_t         wiced_bt_channel_map_t[CHANNEL_MAP_LEN];

/**
 * @addtogroup      wicedbt_av_ascb  WICED Audio Connectionless Slave Broadcast (ACSB)
 * @ingroup         wicedbt_av
 *
 * ACSB interfaces
 * @{
 */

/**
 * @brief ACSB Events.
 *
 * ACSB event received by the HID Host callback (see wiced_bt_hidh_cback_t)
 */
typedef enum
{
    WICED_BT_ACSB_LT_ADDR_RESERVED_EVT = 0,             /**< lt_addr_reserved event */
    WICED_BT_ACSB_SYNC_TRAIN_WRITE_PARAM_EVT,           /**< Write Sync Train Param event */
    WICED_BT_ACSB_SYNC_TRAIN_ENABLE_EVT,                /**< Sync Train Enable event */
    WICED_BT_ACSB_SYNC_TRAIN_RECEIVE_EVT,               /**< Sync Train Receive event */
    WICED_BT_ACSB_SYNC_TRAIN_COMPLETE_EVT,              /**< Sync Train Complete (stopped) event */
    WICED_BT_ACSB_SYNC_TRAIN_RECEIVED_EVT,              /**< Sync Train Received event */
    WICED_BT_ACSB_SET_TRANSMIT_EVT,                     /**< Broadcast Set Transmit */
    WICED_BT_ACSB_SET_RECEIVE_EVT,                      /**< Broadcast Set Receive */
    WICED_BT_ACSB_RECEIVE_TIMEOUT_EVT,                  /**< Broadcast Receive Timeout */
    WICED_BT_ACSB_READ_BUFFER_SIZE_EVT                  /**< Read Broadcast buffer size */
} wiced_bt_acsb_event_t;

/**
 * @brief ACSB Status.
 *
 */
typedef enum
{
    WICED_BT_ACSB_STATUS_OK = 0,            /**< Operation success */
    WICED_BT_ACSB_STATUS_ERROR = 0,         /**< Generic Error */
    WICED_BT_ACSB_STATUS_NYI                /**< Not Yet Implemented */
    /* Add other ACSB status here ... */
} wiced_bt_acsb_status_t;



/**
 * @brief Data associated with WICED_BT_ACSB_LT_ADDR_RESERVED_EVT.
 *
 * This event is received:
 *  - After the wiced_bt_acsb_lt_addr_reserve function is called
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
    lt_addr_t                   lt_addr;        /**< Reserved LT_ADDR */
} wiced_bt_acsb_lt_addr_reserved_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SYNC_TRAIN_WRITE_PARAM_EVT.
 *
 * This event is received:
 *  - After the wiced_bt_acsb_sync_train_write_param function is called
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
    uint16_t                    interval;       /**< Actual Sync Train Interval */
} wiced_bt_acsb_sync_train_write_param_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SYNC_TRAIN_ENABLE_EVT.
 *
 * This event is received:
 *  - After the wiced_bt_acsb_sync_train_enable function is called
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
} wiced_bt_acsb_sync_train_enable_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SYNC_TRAIN_COMPLETE_EVT.
 *
 * This event is received:
 *  - After the the Sync Train stops (either timeout or wiced_bt_acsb_sync_train_enable(FALSE)
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
} wiced_bt_acsb_sync_train_complete_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SYNC_TRAIN_RECEIVE_EVT.
 *
 * This event is received:
 *  - After the wiced_bt_acsb_sync_train_receive is called
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
} wiced_bt_acsb_sync_train_receive_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SYNC_TRAIN_RECEIVED_EVT.
 *
 * This event is received:
 *  - After when a Sync Train is Received
 */
typedef struct
{
    lt_addr_t                   lt_addr;        /**< Reserved LT_ADDR */
    uint16_t                    interval;
    uint32_t                    next_instant;
} wiced_bt_acsb_sync_train_channel_t;

typedef struct
{
    uint8_t                     hci_status; /**< HCI Status. */
    wiced_bt_device_address_t   bdaddr;
    uint32_t                    clock_offset;
    wiced_bt_channel_map_t      channel_map;
    uint8_t                     service_data;
    int8_t                      rssi;
    uint8_t                     channel_nb;
    wiced_bt_acsb_sync_train_channel_t channels[2];
} wiced_bt_acsb_sync_train_received_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SET_TRANSMIT_EVT.
 *
 * This event is received:
 *  - After when a Broadcast Transmission is started/stopped
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
    lt_addr_t                   lt_addr;        /**< LT_ADDR */
    uint16_t                    interval;
    uint16_t                    conn_handle;
} wiced_bt_acsb_set_transmit_t;

/**
 * @brief Data associated with WICED_BT_ACSB_SET_RECEIVE_EVT.
 *
 * This event is received:
 *  - After when a Broadcast Reception is started/stopped
 */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
    wiced_bt_device_address_t   bdaddr;        /**< HCI Status. */
    lt_addr_t                   lt_addr;        /**< LT_ADDR */
    uint16_t                    conn_handle;
} wiced_bt_acsb_set_receive_t;

/**
 * @brief Data associated with WICED_BT_ACSB_RECEIVE_TIMEOUT_EVT.
 */
typedef struct
{
    lt_addr_t                   lt_addr;        /**< LT_ADDR */
    uint16_t                    conn_handle;
} wiced_bt_acsb_receive_timeout_t;

/**
 * @brief Data associated with WICED_BT_ACSB_READ_BUFFER_SIZE_EVT.
 * */
typedef struct
{
    hci_status_t                hci_status;     /**< HCI Status. */
    lt_addr_t                   lt_addr;        /**< Reserved LT_ADDR */
    uint16_t                    buffer_nb;
    uint16_t                    buffer_size;
} wiced_bt_acsb_read_buffer_size_t;

/**
 * @brief Union of data associated with ACSB events
 *
 */
typedef union
{
    wiced_bt_acsb_lt_addr_reserved_t        lt_addr_reserved;   /**< WICED_BT_ACSB_LT_ADDR_RESERVED_EVT */
    wiced_bt_acsb_sync_train_write_param_t  sync_train_param;   /**< WICED_BT_ACSB_SYNC_TRAIN_WRITE_PARAM_EVT */
    wiced_bt_acsb_sync_train_enable_t       sync_train_enable;  /**< WICED_BT_ACSB_SYNC_TRAIN_ENABLE_EVT */
    wiced_bt_acsb_sync_train_complete_t     sync_train_complete;/**< WICED_BT_ACSB_SYNC_TRAIN_COMPLETE_EVT */
    wiced_bt_acsb_sync_train_receive_t      sync_train_receive; /**< WICED_BT_ACSB_SYNC_TRAIN_RECEIVE_EVT */
    wiced_bt_acsb_sync_train_received_t     sync_train_received;/**< WICED_BT_ACSB_SYNC_TRAIN_RECEIVED_EVT */
    wiced_bt_acsb_set_transmit_t            set_transmit;       /**< WICED_BT_ACSB_SET_TRANSMIT_EVT */
    wiced_bt_acsb_set_receive_t             set_receive;        /**< WICED_BT_ACSB_SET_RECEIVE_EVT */
    wiced_bt_acsb_receive_timeout_t         receive_timeout;    /**< WICED_BT_ACSB_RECEIVE_TIMEOUT_EVT */
    wiced_bt_acsb_read_buffer_size_t        read_buffer_size;   /**< WICED_BT_ACSB_READ_BUFFER_SIZE_EVT */
} wiced_bt_acsb_data_t;


/**
 * ASCB Callback function type wiced_bt_acsb_cback_t
 *
 * WICED ACSB Event callback (registered with wiced_bt_acsb_enable)
 *
 * @param[in]       event: ACSB event received
 * @param[in]       p_data : Data (pointer on union of structure) associated with the event
 *
 * @return NONE
 */
typedef void (wiced_bt_acsb_cback_t)(wiced_bt_acsb_event_t event, wiced_bt_acsb_data_t *p_data);

/**
 *
 * Function         wiced_bt_acsb_init
 *
 *                  This function is called for ACSB Initialization.
 *                  This function must be called, once, before any other ACSB functions.
 *
 * @return          None
 *
 */
void wiced_bt_acsb_init(void);

/**
 * Function         wiced_bt_acsb_enable
 *
 *                  Enable (Start) ACSB subsystem. This function is used to register callback for
 *                  ACSB event notifications.
 *
 * @param[in]       p_cback   : Callback for ACSB event notification
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_enable(wiced_bt_acsb_cback_t *p_cback);

/**
 * Function         wiced_bt_acsb_lt_addr_reserve
 *
 *                  This function is used to reserve an lt_addr
 *
 * @param[in]       lt_addr   : LT Address to reserve
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_lt_addr_reserve(lt_addr_t lt_addr);

/**
 * Function         wiced_bt_acsb_sync_train_write_param
 *
 *                  This function is used to write the Synchronization Train parameters
 *
 * @param[in]       interval_min   : Minimum Interval
 * @param[in]       interval_max   : Maximum Interval
 * @param[in]       timeout        : Sync Train timeout
 * @param[in]       service_data   : Service Data
 * @param[in]       bcst_bdaddr   : This could be different from the factory programmed BD_ADDR
 * @param[in]       cac_bdaddr    : Used for CAC/HEC/CRC/AFH if non-zero
 * @param[in]       tx_power       : Tx Power
 * @param[in]       rfu            : Reserved for Future Use
 *
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_sync_train_write_param(uint16_t interval_min,
        uint16_t interval_max, uint32_t timeout, uint8_t service_data,
        wiced_bt_device_address_t bcst_bdaddr, wiced_bt_device_address_t cac_bdaddr,
        int8_t sync_train_tx_power, uint32_t rfu);

/**
 * Function         wiced_bt_acsb_sync_train_enable
 *
 *                  This function is used to enable/disable the Synchronization Train
 *
 * @param[in]       enable          : TRUE to Enable, FALSE to Disable
 * @param[in]       test_mode       : Test Mode
 * @param[in]       rfu             : Reserved for Future Use
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_sync_train_enable(wiced_bool_t enable, uint8_t test_mode,
        uint32_t rfu);

/**
 * Function         wiced_bt_acsb_sync_train_receive
 *
 *                  This function is used to receive Synchronization Train
 *
 * @param[in]       bcst_bdaddr    : This can be 0.  If non-zero, it needs to match the
 *                                    Broadcaster's BD_ADDR for FW to send up Sync Train Received event.
 * @param[in]       cac_bdaddr     : Used for CAC/HEC/CRC/AFH if non-zero.
 * @param[in]       scan_timeout:
 * @param[in]       scan_window:
 * @param[in]       scan_interval:
 * @param[in]       rssi_threshold  :
 * @param[in]       rfu             : Reserved for Future Use
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_sync_train_receive(wiced_bt_device_address_t bcst_bdaddr,
        wiced_bt_device_address_t cac_bdaddr, uint16_t scan_timeout, uint16_t scan_window,
        uint16_t scan_interval, int8_t rssi_threshold, uint32_t rfu);

/**
 * Function         wiced_bt_acsb_set_transmit
 *
 *                  This function is used to start/stop Broadcast Transmission
 *
 * @param[in]       enable
 * @param[in]       bdaddr
 * @param[in]       lt_addr
 * @param[in]       lpo_enabled,
 * @param[in]       packet_type
 * @param[in]       interval_min
 * @param[in]       interval_max
 * @param[in]       channel_data,
 * @param[in]       retransmission
 * @param[in]       tx_power
 * @param[in]       flags
 * @param[in]       rfu             : Reserved for Future Use
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_set_transmit(wiced_bool_t enable,
        wiced_bt_device_address_t bdaddr, lt_addr_t lt_addr,
        wiced_bool_t lpo_enabled, uint16_t packet_type, uint16_t interval_min,
        uint16_t interval_max, uint8_t channel_data, uint8_t retransmission,
        int8_t bcst_tx_power, uint8_t flags, uint32_t rfu);

/**
 * Function         wiced_bt_acsb_set_receive
 *
 *                  This function is used to start/stop Broadcast Reception
 *
 * @param[in]       enable
 * @param[in]       bdaddr
 * @param[in]       lt_addr
 * @param[in]       interval,
 * @param[in]       clock_offset
 * @param[in]       next_instant
 * @param[in]       timeout
 * @param[in]       timing_accuracy,
 * @param[in]       retransmission
 * @param[in]       packet_type,
 * @param[in]       afh_channel_map
 * @param[in]       rfu             : Reserved for Future Use
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_set_receive(wiced_bool_t enable,
        wiced_bt_device_address_t bdaddr, lt_addr_t lt_addr, uint16_t interval,
        uint32_t clock_offset, uint32_t next_instant, uint16_t timeout, uint8_t timing_accuracy,
        uint8_t retransmission, uint16_t packet_type, wiced_bt_channel_map_t afh_channel_map,
        uint32_t rfu);

/**
 * Function         wiced_bt_acsb_read_buffer_size
 *
 *                  This function is used to read Broadcast buffer size/number for a channel
 *
 * @param[in]       lt_addr
 *
 * @return          Result code (see wiced_bt_acsb_status_t)
 *                  WICED_BT_ACSB_STATUS_OK if successful, otherwise error. *
 */
wiced_bt_acsb_status_t wiced_bt_acsb_read_buffer_size(lt_addr_t lt_addr);

/** @} */
#ifdef __cplusplus
}
#endif
