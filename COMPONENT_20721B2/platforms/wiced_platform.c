/*
 * Copyright 2016-2021, Cypress Semiconductor Corporation (an Infineon company) or
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

#include "wiced_platform.h"
#include "spar_utils.h"
#include "wiced_transport.h"
#include "hci_control_api.h"

extern wiced_platform_gpio_t platform_gpio_pins[];
extern wiced_platform_led_config_t platform_led[];
extern wiced_platform_button_config_t platform_button[];
extern wiced_platform_gpio_config_t platform_gpio[];
extern size_t platform_gpio_pin_count;
extern size_t led_count;
extern size_t button_count;
extern size_t gpio_count;

extern void wiced_app_hal_init(void );

typedef void (wiced_platform_transport_rx_data_handler)(uint16_t op_code, uint8_t *p_data, uint32_t data_len);
static struct
{
    struct
    {
        wiced_bool_t init;
        wiced_platform_transport_rx_data_handler *p_app_rx_data_handler;
    } transport;

} wiced_audio_platform_cb = {0};

/* utility functions */

/**
 *  \brief Provide utility function for application to register for cb upon button interrupt
 *
 *  \param [in] button select a button from wiced_platform_button_number_t
 *  \param [in] userfn Provide the call back function
 *  \param [in] userdata Data to be provided with the call back
 *  \param [in] trigger_edge To configure interrupt on rising/falling/dual edge
 *
 *  \return none
 *
 */
void wiced_platform_register_button_callback(wiced_platform_button_number_t button, void (*userfn)(void*, uint8_t), void* userdata,
                wiced_platform_button_interrupt_edge_t trigger_edge)
{
    if(button < button_count)
    {
        wiced_hal_gpio_register_pin_for_interrupt(*platform_button[button].gpio, userfn, userdata);
        wiced_hal_gpio_configure_pin(*platform_button[button].gpio, (platform_button[button].config | trigger_edge), platform_button[button].default_state);
    }
}

/**
 *  \brief Return state of the pin when button is pressed
 *
 *  \param [in] button select a button from wiced_platform_button_number_t
 *
 *  \return button pressed value
 *
 */
uint32_t wiced_platform_get_button_pressed_value(wiced_platform_button_number_t button)
{
	return platform_button[button].button_pressed_value;
}

/**
 *  \brief Initialize all the required pins and configure their functionality
 *
 *  \return none
 *
 */
void wiced_platform_init(void)
{
    uint32_t i = 0;

    wiced_app_hal_init();
    /* Configure pins available on the platform with the chosen functionality */
    for (i = 0; i < platform_gpio_pin_count; i++)
    {
        wiced_hal_gpio_select_function(platform_gpio_pins[i].gpio_pin, platform_gpio_pins[i].functionality);
    }

    /* Initialize LEDs and turn off by default */
    for (i = 0; i < led_count; i++)
    {
        wiced_hal_gpio_configure_pin(*platform_led[i].gpio, platform_led[i].config, platform_led[i].default_state);
    }

    /* Initialize buttons with the default configuration */
    for (i = 0; i < button_count; i++)
    {
        wiced_hal_gpio_configure_pin(*platform_button[i].gpio, (platform_button[i].config), platform_button[i].default_state);
    }

    /* Initialize GPIOs with the default configuration */
    for (i = 0; i < gpio_count; i++)
    {
        wiced_hal_gpio_configure_pin(*platform_gpio[i].gpio, (platform_gpio[i].config), platform_gpio[i].default_state);
    }

    /* disable watchdog, set up SWD, wait for attach if ENABLE_DEBUG */
    SETUP_APP_FOR_DEBUG_IF_DEBUG_ENABLED();
    BUSY_WAIT_TILL_MANUAL_CONTINUE_IF_DEBUG_ENABLED();
}

/*
 * platform_transport_status_handler
 */
static void platform_transport_status_handler( wiced_transport_type_t type )
{
    wiced_transport_send_data(HCI_CONTROL_EVENT_DEVICE_STARTED, NULL, 0);
}

/*
 * platform_transport_rx_data_handler
 */
static uint32_t platform_transport_rx_data_handler(uint8_t *p_buffer, uint32_t length)
{
    uint16_t opcode;
    uint16_t payload_len;
    uint8_t *p_data = p_buffer;

    /* Check parameter. */
    if (p_buffer == NULL)
    {
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    // Expected minimum 4 byte as the wiced header
    if (length < (sizeof(opcode) + sizeof(payload_len)))
    {
        wiced_transport_free_buffer(p_buffer);
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    STREAM_TO_UINT16(opcode, p_data);       // Get OpCode
    STREAM_TO_UINT16(payload_len, p_data);  // Gen Payload Length

    switch(opcode)
    {
    //NOTE: add special case if needed
    default:
        if (wiced_audio_platform_cb.transport.p_app_rx_data_handler)
        {
            (*wiced_audio_platform_cb.transport.p_app_rx_data_handler)(opcode, p_data, payload_len);
        }
        break;
    }

    // Freeing the buffer in which data is received
    wiced_transport_free_buffer(p_buffer);

    return HCI_CONTROL_STATUS_SUCCESS;
}

/**
 * wiced_platform_transport_init
 *
 * Initialize the WICED HCI Transport interface.
 * NOTE: it is used on specific audio application and it will call wiced_transport_init in this func.
 *
 * @param[in] p_rx_data_handler : user callback for incoming HCI data.
 *
 * @return  WICED_TRUE - success
 *          WICED_FALSE - fail
 */
wiced_bool_t wiced_platform_transport_init(void *p_rx_data_handler)
{
    wiced_transport_cfg_t cfg = {
                                    .type = WICED_TRANSPORT_UART,
                                    .cfg.uart_cfg = {
                                                        .mode = WICED_TRANSPORT_UART_HCI_MODE,
                                                        .baud_rate = HCI_UART_DEFAULT_BAUD,
                                                    },
                                    .rx_buff_pool_cfg = {
                                                            .buffer_size = 512,
                                                            .buffer_count = 2,
                                                        },
                                    .p_status_handler = platform_transport_status_handler,
                                    .p_data_handler = platform_transport_rx_data_handler,
                                    .p_tx_complete_cback = NULL,
                                };
    wiced_result_t result;

    if (wiced_audio_platform_cb.transport.init)
    {
        return WICED_FALSE;
    }

    /* Initialize the transport. */
    result = wiced_transport_init(&cfg);

    if (result == WICED_BT_SUCCESS)
    {
        wiced_audio_platform_cb.transport.init = WICED_TRUE;
        wiced_audio_platform_cb.transport.p_app_rx_data_handler = p_rx_data_handler;

        return WICED_TRUE;
    }

    return WICED_FALSE;
}
