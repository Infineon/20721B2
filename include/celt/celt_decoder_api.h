/******************************************************************************
**
**  File Name:   celt_decoder_api.h
**
**  Description: This file has function declarations and structs for integration
**
**  Revision :
**               2015/11/26  hochangw
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#ifndef CELT_DECODER_API_H
#define CELT_DECODER_API_H

#include "celt_scratch.h"


/******************************************************************************
 *
 *  struct CELT_DEC_PARAMS
 *
 *****************************************************************************/

//struct CELT_DEC_PARAMS_TAG
typedef struct
{
    UINT32 celt_dec_scratch_buf[OPUS_CELT_DEC_SCRATCH_SIZE_UINT32];   /* decoder scratch buffer */

//#if defined (USE_AUDIOPLC)
    UINT32 aplc_struct[OPUS_CELT_PLC_STRUCT_SIZE_BYTE/4];  //struct CELT_AUDIOPLC_STRUCT aplc;   /* LCPLC for CELT */
//#endif
    INT32 sampling_rate;                /* 8k, 16k, 24k or 48k */
    INT16 channels;                     /* mono or streo */
    UINT16 pkt_len;                     /* Input packet length (bytes) */
    INT16 *pcmBuffer;                   /* Pointer of output buffer */
    UINT8 *packet;                      /* Pointer of input buffer */

    UINT16 frame_status;                /* Frame status: 0:GOOD, 1:BAD(lost)  */
    INT32  frame_size;                  /* PCM samples per frame per channel, needed for PLC init*/
    INT16  enable_plc;                  /* Enable PLC: 1:enable, 0:disable */
    /* Below variables are used inside CELT */
    void  *dec_handler;                 /* OpusDecoder */

}CELT_DEC_PARAMS;
//typedef struct CELT_DEC_PARAMS_TAG CELT_DEC_PARAMS


/******************************************************************************
 *
 *  Function: CELT_Decoder
 *
 *  Abstract:    Read CELT data and if enough data will run the CEL Decoder and return the number of decoded samples
 *
 *  Param:
 *           * ptrDecParams  :   Pointer to CELT_DEC_PARAMS structure
 *
 *  Return:  Number of PCM samples
 *
 *******************************************************************************/
 extern INT32 CELT_Decoder(CELT_DEC_PARAMS *ptr_dec_param);


 /******************************************************************************
 *
 *  Function: CELT_Decoder_Init
 *
 *  Abstract: InitCELT Decoder
 *
 *  Param:
 *           * ptr_dec_param  : Pointer to CELT_DEC_PARAMS structure
 *
 *  Return:  0 if success
 *
 *******************************************************************************/
 extern INT32 CELT_Decoder_Init(CELT_DEC_PARAMS *ptr_dec_param);



 /******************************************************************************
 *
 *  Function: CELT_Decoder_get_mem_size
 *
 *  Abstract:
 *
 *  Param:
 *           None
 *
 *  Return:  Needed memory size for CELT decoder
 *
 *******************************************************************************/
 extern UINT32 CELT_Decoder_get_mem_size(void);



#endif
