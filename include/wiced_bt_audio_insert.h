/*
 *  Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 *  Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 *  This software, including source code, documentation and related
 *  materials ("Software"), is owned by Cypress Semiconductor Corporation
 *  or one of its subsidiaries ("Cypress") and is protected by and subject to
 *  worldwide patent protection (United States and foreign),
 *  United States copyright laws and international treaty provisions.
 *  Therefore, you may use this Software only as provided in the license
 *  agreement accompanying the software package from which you
 *  obtained this Software ("EULA").
 *  If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 *  non-transferable license to copy, modify, and compile the Software
 *  source code solely for use in connection with Cypress's
 *  integrated circuit products. Any reproduction, modification, translation,
 *  compilation, or representation of this Software except as specified
 *  above is prohibited without the express written permission of Cypress.
 * 
 *  Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 *  reserves the right to make changes to the Software without notice. Cypress
 *  does not assume any liability arising out of the application or use of the
 *  Software or any product or circuit described in the Software. Cypress does
 *  not authorize its products for use in any products where a malfunction or
 *  failure of the Cypress product may reasonably be expected to result in
 *  significant property damage, injury or death ("High Risk Product"). By
 *  including Cypress's product in a High Risk Product, the manufacturer
 *  of such system or application assumes all risk of such use and in doing
 *  so agrees to indemnify Cypress against all liability.
 */

/** @file
 *  @addtogroup  audio_insert   Audio Insertion
 *  @ingroup     wicedbt
 *
 * Bluetooth Audio Insert Application Programming Interface
 *
 * The Audio Insertion library provides API to allow the Wiced application to Play/Insert sound.
 * If A2DP Stream is already active, the application can change the PCM samples going to the I2S
 * interface. The application receives the PCM samples which 'should' be played. This allow to
 * either Replace or Mix (e.g. add) the Inserted Audio.
 * If A2DP Stream is not active, the I2S interface will be automatically started.
 * Note that this API cannot be used if a SCO (e.g. HFP Call) is already ongoing. In this case
 * the Audio must be inserted using the SCO Hook library.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdint.h>
#include "wiced.h"

/**
*  @addtogroup  audio_insert_data_types        Data Types
*  @ingroup     audio_insert
*
*  <b> Data Types </b> for @b Audio Insert.
*
*  @{
*/

/**
 * @brief Audio Insert Events.
 *
 * Audio Insert events received by the Audio Insert callback (see wiced_bt_audio_insert_callback_t)
 */
typedef enum
{
    WICED_BT_AUDIO_INSERT_EVT_DATA_REQ = 3, /* Request to fill hardware FIFO with audio data */
    WICED_BT_AUDIO_INSERT_EVT_AUDIO_INFO,   /* Indicates the sample rate for the pending audio injection */
    WICED_BT_AUDIO_INSERT_EVT_SCO,          /* indicates start SCO audio injection */
} wiced_bt_audio_insert_event_t;


/**
 * @brief Number of PCM Samples for Audio Insertion
 */
#define WICED_BT_AUDIO_INSERT_PCM_SAMPLE_NB       128

/**
 * @brief Data associated with WICED_BT_AUDIO_INSERT_EVT_DATA_REQ.
 *
 * This event is received when FW requests audio data
 *
 */
typedef struct
{
    int16_t *p_data_in;                     /* Decoded data from A2DP stream */
    int16_t *p_data_out;                    /* Pointer to output buffer */
    uint16_t bufferSize;                    /* size of the buffer that needs to be filled */
} wiced_bt_audio_insert_event_data_req_t;

/**
 * @brief Data associated with LITE_HOST_LRAC_EVT_AUDIO_INFO.
 *
 * This event is received when the FW indicates the Sample Rate and the Buffer size to use
 * for the Audio Insertion
 *
 */
typedef struct
{
    uint16_t sample_rate;                      /* Sample rate for pending audio injection */
    uint16_t nb_samples;                       /* Number of PCM samples (16 bits, Interleaved Stereo) */
} wiced_bt_audio_insert_event_audio_info_t;

/**
 * @brief Union of data associated with NREC events
 *
 */
typedef union
{
    wiced_bt_audio_insert_event_data_req_t      data_req;   /* WICED_BT_AUDIO_INSERT_EVT_DATA_REQ */
    wiced_bt_audio_insert_event_audio_info_t    audio_info; /* WICED_BT_AUDIO_INSERT_EVT_AUDIO_INFO */
    wiced_bt_audio_insert_event_audio_info_t    sco_info;   /* WICED_BT_AUDIO_INSERT_EVT_SCO */
} wiced_bt_audio_insert_event_data_t;

/**
 * Audio Insert Callback function type wiced_bt_audio_insert_callback_t
 *
 *                  WICED Audio Insert Event callback (registered with wiced_bt_audio_insert_init)
 *
 * @param[in]       event: Audio Insert event received
 * @param[in]       p_data : Data (pointer on union of structure) associated with the event
 *
 * @return NONE
 */
typedef void (wiced_bt_audio_insert_callback_t)(wiced_bt_audio_insert_event_t event,
        wiced_bt_audio_insert_event_data_t *p_data);

/**
*  @addtogroup  audio_insert_api_functions       API Functions
*  @ingroup     audio_insert
*
*  <b> API Functions </b> module for @b Audio Insert.
*
*  @{
*/

/**
 *
 * Function         wiced_bt_audio_insert_init
 *
 *                  This function is called for Audio Insert Initialization.
 *                  This function must be called, once, before any other Audio Insert functions.
 *
 * @param[in]       p_callback: Pointer to application Audio Insert callback function
 *
 * @return          Result code (see wiced_result_t)
 *
 */
wiced_result_t wiced_bt_audio_insert_init(wiced_bt_audio_insert_callback_t *p_callback);

/**@} audio_insert_api_functions */

/**
 *
 * Function         wiced_bt_audio_insert_enable
 *
 *                  This function is called to Enable/disable Audio Insert.
 *
 * @param[in]       enable: Enable (1) or Disable (0) Audio Insertion
 * @param[in/out]   sample_rate: IN: Sample Rate proposed by the application
 *                               OUT: Sample Rate to use (decided by HW/HW)
 *
 * @return          Result code (see wiced_result_t)
 *
 */
wiced_result_t wiced_bt_audio_insert_enable(uint8_t enable, uint32_t *p_sample_rate);

#ifdef __cplusplus
}
#endif

/**@} audio_insert */
