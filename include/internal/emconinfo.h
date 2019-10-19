#ifndef _EMCONINFO_H_
#define _EMCONINFO_H_

//////////////////////////////////////////////////////////////////////////////
//    this file defined the embedded controller info
//    this data structure contains all the information about the device.
#include "brcm_fw_types.h"


#ifdef __cplusplus
extern "C" {
#endif


#define EMCONINFO_FLAG_BONDED_DEVICE                  0x1
#define EMCONINFO_FLAG_LINK_ENCRYPTED                 0x2

#define BD_ADDRESS_SIZE     6
//
typedef struct
{
    // 4 bytes aligned.
    UINT16          connHandle;
    UINT8           flag;

    UINT8           peerAddressType;

    // 4 bytes aligned
    UINT8           peerAddress[BD_ADDRESS_SIZE]; // this is 6 bytes peer address.
    UINT16          connInterval; // connection interval in frames.

    // 4 bytes aligned
    UINT16          connLatency;
    UINT16          supervisionTimeout;

} EMCONINFO_DEVINFO;

// all the macros need this variable to be exported.
extern EMCONINFO_DEVINFO emConInfo_devInfo;

//////////////////////////////////////////////////////////////////////////////
//                          public interfaces.
void emconinfo_init(EMCONINFO_DEVINFO *pdevInfo);

void emconinfo_setPtr(EMCONINFO_DEVINFO *pdevInfo);
EMCONINFO_DEVINFO *emconinfo_getPtr(void);

int  emconinfo_getConnHandle(void);
void emconinfo_setConnHandle(UINT16 connHandle);

UINT8 *emconninfo_getPeerAddr(void);

void emconninfo_setPeerAddrType(int type);
int  emconninfo_getPeerAddrType(void);

int emconninfo_getConnInterval(void);
void emconninfo_setConnInterval(int interval);

int  emconninfo_getSlaveLatency(void);
void emconninfo_setSlaveLatency(int latency);

int emconninfo_getSupervisionTimeout(void);
void emconninfo_setSupervisionTimeout(int timeout);


int emconninfo_deviceBonded(void);
void emconninfo_setDeviceBondedFlag(void);
void emconninfo_clearDeviceBondedFlag(void);


int emconninfo_islinkEncrypted(void);
void emconninfo_clearLinkEncryptedFlag(void);
void emconninfo_setLinkEncryptedFlag(void);

// This is for C++ compiler. WIN32 uses C++
#ifdef __cplusplus
}
#endif

#endif // end of #ifndef _EMCONINFO_H_
