/*
 * Copyright 2016-2024, Cypress Semiconductor Corporation (an Infineon company) or
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

 /*
********************************************************************
*    File Name: keyscan.h
*
*    Abstract: This file defines a keyscan driver
*
*
********************************************************************
*/

#ifndef __KEYSCAN_DRIVER_H__
#define __KEYSCAN_DRIVER_H__

#include "brcm_fw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**  \addtogroup keyscanQueue
 *  \ingroup keyscan
 */

/*! @{ */
/**
* Defines a keyscan driver.
*
* The keyscan interface is practically defined as a queue from the consumer's
* perspective. Key up/down events are seen as a stream coming from the driver.
* In addition the interface provides the user with the ability to reset the
* HW as well as turn keyscanning on/off.
*/

/// Up/down flag
enum
{
    KEY_DOWN = 0x0,
    KEY_UP   = 0x1
};

/// Special event codes used internally by the driver
enum
{
    /// Rollover event generated by the keyscan driver in case of an error (ghost or overflow)
    ROLLOVER = 0xff,

    /// Event returned to indicate the end of a scan cycle.
    END_OF_SCAN_CYCLE = 0xfe
};

/// Keyscan  driver constants
enum
{
    ///Mia keyevent HW FIFO size.
    MIA_KEY_EVENT_FIFO_SIZE              = 20,

    ///keyscan FW FIFO size. This FIFO  is implemented with KeyscanQueue.
    KEYSCAN_FW_FIFO_SIZE                 = (2*MIA_KEY_EVENT_FIFO_SIZE + 6),
};

typedef struct
{
    /// Location where the queue starts. Provided during initialization
    BYTE *bufStart;

    /// Maximum size of elements. Provided during initialization
    BYTE elementSize;

    /// Maximum number of elements that can be placed in the queue. Provided during initialization
    BYTE maxNumElements;

    /// Number of elements currently in the queue
    BYTE curNumElements;

    /// Read index into the queue
    BYTE readIndex;

    /// Write index into the queue
    BYTE writeIndex;

    /// Saved write index for rollback.
    BYTE savedWriteIndexForRollBack;

    /// Saved number of elements for rollback.
    BYTE savedNumElements;
} KeyscanQueueState;

#pragma pack(1)

/// Definition of a key event.
typedef PACKED struct
{
    /// Key code. This is the location in the keyscan matrix that is pressed/released.
    /// May be implemented as ((row * numCols) + col) or ((col * numRows) + row.
    BYTE keyCode;

    /// Reserved bits
    BYTE reserved : 6;

    /// Up/down flag
    BYTE upDownFlag : 1;

    /// Should be toggled for every scan cycle in which a new event is queued.
    /// Use of this flag is optional. If used, it allows the consumer to determine whether
    /// an event is detected in the same scan cycle as the previous event or a different one.
    /// Note that this flag does not indicate any separation in time.
    BYTE scanCycleFlag : 1;
}KeyEvent;
#pragma pack()

#ifdef WICED_KEYSCAN_FUNCTION_SUPPORT
void ksq_init(void* buffer, BYTE elementSize, BYTE maxElements);
void ksq_flush(void);
BYTE ksq_getCurNumElements(void);
BOOL32 ksq_putExcludeOverflowSlot(void *elm, BYTE len);
BOOL32 ksq_putIncludeOverflowSlot(void *elm, BYTE len);
void *ksq_getCurElmPtr(void);
void ksq_removeCurElement(void);

BOOL32 ksq_put(void *elm, BYTE len);
void ksq_flush(void);
void ksq_markCurrentEventForRollBack (void);
void ksq_rollbackUptoMarkedEvents(void);
void ksq_putEvent(KeyEvent *event);
void ksq_getEvent(KeyEvent *event);

/// Check if the KeyscanQueue is empty.
INLINE BOOL32 ksq_isEmpty (void)
{
    return (ksq_getCurNumElements() == 0);
}
#endif // WICED_KEYSCAN_FUNCTION_SUPPORT

/* @} */

/**  \addtogroup keyscan
 *  \ingroup HardwareDrivers
*/
/*! @{ */
/**
* Defines the BCM standard keyscan driver. The   BCM standard keyscan driver
* provides the status and control for the keyscan driver. It provides the
* keyEvents to the interface, maintains the queue behind it. It also supports
* keyscanning turning on or off.
*
*/


enum
{
    /// Keycode value if no key is pressed.
    EVENT_NONE                   = 0xfd
};

enum
{
    HW_CTRL_SCAN_CTRL_MASK               = 0x00001,
    HW_CTRL_GHOST_DETECT_MASK            = 0x00004,
    HW_CTRL_INTERRUPT_CTRL_MASK          = 0x00008,
    HW_CTRL_RESET_MASK                   = 0x00010,
    HW_CTRL_RC_EXT_MASK                  = 0x000C0,
    HW_CTRL_ROW_MASK                     = 0x00700,
    HW_CTRL_COL_MASK                     = 0x0F800,
    HW_CTRL_COL_DRIVE_CTRL_MASK          = 0x10000,
    HW_CTRL_ROW_DRIVE_CTRL_MASK          = 0x20000,

    HW_CTRL_SCAN_ENABLE                  = 0x00001,
    HW_CTRL_GHOST_ENABLE                 = 0x00004,
    HW_CTRL_KS_INTERRUPT_ENABLE          = 0x00008,
    HW_CTRL_RESET_ENABLE                 = 0x00010,
    HW_CTRL_CLK_ALWAYS_ON                = 0x40000,
    HW_CTRL_RC_DEFAULT                   = 0x000C0,
    HW_CTRL_ROW_SHIFT                    = 8,
    HW_CTRL_COL_SHIFT                    = 11,
    HW_CTRL_COL_ACTIVE_DRIVE             = 0x10000,
    HW_CTRL_ROW_ACTIVE_DRIVE             = 0x20000,

    HW_CTRL_SCAN_DISABLE                 = 0x00000,
    HW_CTRL_GHOST_DISABLE                = 0x00000,
    HW_CTRL_INTERRUPT_DISABLE            = 0x00000,
    HW_CTRL_RESET_DISABLE                = 0x00000,
    HW_CTRL_CLK_AUTO_CTRL                = 0x00000,
    HW_CTRL_COLUMN_PASSIVE_DRIVE         = 0x00000,
    HW_CTRL_ROW_PASSIVE_DRIVE            = 0x00000
};

enum
{
  HW_MIA_STATUS_KEYSCAN_INT_SET_MASK   = 0x00040,
 };

enum
{
    HW_LHL_CTRL_CLR_KEYS                 = 0x0002,

    HW_LHL_STATUS_GHOST                  = 0x0008,
    HW_LHL_STATUS_KEY_FIFO_OVERFLOW      = 0x0004,
    HW_LHL_STATUS_KEYCODE                = 0x0002,

#ifdef KEYSTUCK_ALLOW_SLEEP_SUPPORTED
    /// Stuck-key
    KEYSTUCK                             = 0x00fc,
#endif
};

enum
{
    HW_KEYCODE_MASK                      = 0x000000ff,
    HW_SCAN_CYCLE_MASK                   = 0x40000000,
    HW_KEY_UP_DOWN_MASK                  = (int)0x80000000,

    HW_KEYCODE_SHIFT_COUNT               = 0,
    HW_SCAN_CYCLE_SHIFT_COUNT            = 30,
    HW_KEY_UP_DOWN_SHIFT_COUNT           = 31,


    HW_KEYCODE_GHOST                     = 0xf5,
    HW_KEYCODE_INIT                      = 0xff,

    HW_KEY_UP                            = (int)0x80000000,
    HW_KEY_DOWN                          = 0x00000000
};

enum
{
    CH_SEL_KEYSCAN_ROW_INPUT_50_57      = 0x0000,
    CH_SEL_KEYSCAN_ROW_INPUT_16_23      = 0x0001
};


typedef struct KeyscanRegistration
{
    void (*userfn)(void*);
    void *userdata;
    struct KeyscanRegistration *next;
} KeyscanRegistration;


typedef struct
{
    /// Registration chain of the appplication level keyscan interrupt/event handlers
    KeyscanRegistration* keyscanFirstReg;

    /// Reserved space for keyscan data.
    KeyEvent keyscan_events[KEYSCAN_FW_FIFO_SIZE];

    /// Whether HW polling is done from Keyscan
    BOOL8 keyscan_pollingKeyscanHw;

    // number of key down events that are not yet matched by key up events,
    // which gives the number of keys currently being pressed
    UINT8 keysPressedCount;


} KeyscanState;


#ifdef WICED_KEYSCAN_FUNCTION_SUPPORT
void keyscan_init(void);
void keyscan_init_forSlimboot(void);

/// Reset the keyscan HW. Any existing events should be thrown away.
void keyscan_reset(void);



/// Check if there are any pending key events.
/// \return TRUE if there are pending key events, FALSE otherwise
BOOL32 keyscan_eventsPending(void);

/// Get next key event
/// \param event pointer to where the event should be stored
/// \return TRUE if an event is returned, FALSE otherwise
BOOL32 keyscan_getNextEvent(KeyEvent *event);

/// Disables keyscanning and and any associated wakeup
void keyscan_turnOff(void);

/// Enabled keyscanning and and any associated wakeup functionality. Should only be used
/// if keyscanning was disabled via turnOff()
void keyscan_turnOn(void);

/// Register for notification of changes.
/// Once registered, you CAN NOT UNREGISTER; registration is meant to
/// be a startup activity
/// \param userfn points to the function to call when the interrupt
/// comes and matches one of the masks (if you pass a method, it must
/// be static). The function does not need to clear the interrupt
/// status; this will be done by KeyscanDriver::lhlInterrupt(). The
/// function will be called ONCE per interrupt.
/// \param userdata will be passed back to userfn as-is; it can be used to
/// carry the "this", for example
void keyscan_registerForEventNotification(void (*userfn)(void*), void* userdata);

void keyscan_enableEventDetection(void);

void keyscan_flushHwEvents(void);
void keyscan_miaFreezeCallBack(void);
void keyscan_miaUnfreezeCallBack(void);
void keyscan_enableInterrupt(void);
void keyscan_disableInterrupt(void);
void keyscan_ksInterrupt(void);
void keyscan_restoreActivity(void);
void keyscan_clearKeysPressedCount(void);
UINT8 keyscan_getKeysPressedCount(void);
void keyscan_enableGhostDetection(BOOL32 enable);

void keyscan_hwResetOnce(void);
void keyscan_initHW(void);
void keyscan_configGpios(void);
void keyscan_getEventsFromHWFifo(void);

#ifdef KEYSTUCK_ALLOW_SLEEP_SUPPORTED
UINT8 keyscanGetKeystuckState(void);
BOOL32 isKeyStuck(void);
void changeKeystuckTimeout(UINT32 new_keyscan_stuck_key_in_second);
extern void (*keyscan_stuckKey_callbackApp)(void);

enum {
  STUCKKEY_STATE_INIT,
  STUCKKEY_STATE_NORMAL,
  STUCKKEY_STATE_STUCK,
};
#endif // KEYSTUCK_ALLOW_SLEEP_SUPPORTED
#endif // WICED_KEYSCAN_FUNCTION_SUPPORT

/* @} */

#ifdef __cplusplus
}
#endif

#endif
