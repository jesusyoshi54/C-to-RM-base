#include "libultra_internal.h"
#include "osContInternal.h"
#include <macros.h>

#ifndef AVOID_UB
OSPifRam __osContPifRam ALIGNED(16);
// ALIGNED8 OSContPackedStruct _osContCmdBuf[7];
UNUSED static u32 unused; // padding between these two variables
u32 _osContPifCtrl;
#else
// Reordered gcc vars above will disturb the aliasing done to access all 8 members of this array, hence AVOID_UB.
OSPifRam __osContPifRam ALIGNED(16);
// ALIGNED8 OSContPackedStruct _osContCmdBuf[8];
#endif

extern u8 _osLastSentSiCmd;
extern u8 _osContNumControllers;

void __osPackReadData(void);
s32 osContStartReadData(OSMesgQueue *mesg) {
    s32 ret = 0;
    s32 i;
    __osSiGetAccess();
    if (_osLastSentSiCmd != 1) {
        __osPackReadData();
        ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
        osRecvMesg(mesg, NULL, OS_MESG_BLOCK);
    }
    // for (i = 0; i < ARRLEN(__osContPifRam.ramarray); i++) {
        // __osContPifRam.ramarray[i] = 0;
    // }

    // __osContPifRam.pifstatus = 0;
    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    _osLastSentSiCmd = 1;
    __osSiRelAccess();
    return ret;
}
void osContGetReadData(OSContPad *data) {
    u8* ptr = __osContPifRam.ramarray;
    __OSContReadFormat readformat;
    s32 i;

    for (i = 0; i < _osContNumControllers; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat*)ptr;
        data->errnum = CHNL_ERR(readformat);
        
        if (data->errnum != 0) {
            continue;
        }

        data->button = readformat.button;
        data->stick_x = readformat.stick_x;
        data->stick_y = readformat.stick_y;
    }
}

void __osPackReadData(void) {
    u8* ptr = __osContPifRam.ramarray;
    __OSContReadFormat readformat;
    int i;

    for (i = 0; i < ARRLEN(__osContPifRam.ramarray); i++) {
        __osContPifRam.ramarray[i] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    readformat.dummy = CONT_CMD_NOP;
    readformat.txsize = CONT_CMD_READ_BUTTON_TX;
    readformat.rxsize = CONT_CMD_READ_BUTTON_RX;
    readformat.cmd = CONT_CMD_READ_BUTTON;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;

    for (i = 0; i < _osContNumControllers; i++) {
        *(__OSContReadFormat*)ptr = readformat;
        ptr += sizeof(__OSContReadFormat);
    }
    
    *ptr = CONT_CMD_END;
}
