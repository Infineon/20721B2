
/* To support legacy code */

#include "wiced_platform.h"

#define WICED_GPIO_PIN_BUTTON WICED_GPIO_PIN_BUTTON_1

#undef  WICED_GPIO_BUTTON_SETTINGS
#define WICED_GPIO_BUTTON_SETTINGS                ( GPIO_INPUT_ENABLE | GPIO_PULL_DOWN | GPIO_EN_INT_RISING_EDGE )

#define WICED_BUTTON_PRESSED_VALUE                 1

#define WICED_PUART_TXD                           WICED_P33      /* pin for PUART TXD         */
#define WICED_PUART_RXD                           WICED_P34      /* pin for PUART RXD         */
