#include "libultra_internal.h"
#include "osContInternal.h"

void __osPackRequestData(u8);
void __osContGetInitData(u8 *, OSContStatus *);

u32 _osContInitialized = 0; // probably initialized

extern u64 osClockRate;

// these probably belong in EEPROMlongread or something
u8 _osLastSentSiCmd;
u8 _osContNumControllers;
OSTimer D_80365D28;
OSMesgQueue _osContMesgQueue;
OSMesg _osContMesgBuff[4];

s32 osContInit(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *status) {
    OSMesg mesg;
    u32 ret = 0;
    OSTime currentTime;
    OSTimer timer;
    OSMesgQueue timerMesgQueue;

    if (_osContInitialized) {
        return 0;
    }
    _osContInitialized = 1;
    currentTime = osGetTime();
    if (currentTime < OS_USEC_TO_CYCLES(500000)) {
        osCreateMesgQueue(&timerMesgQueue, &mesg, 1);
        osSetTimer(&timer, 500000 * osClockRate / 1000000 - currentTime, 0, &timerMesgQueue, &mesg);
        osRecvMesg(&timerMesgQueue, &mesg, OS_MESG_BLOCK);
    }
    _osContNumControllers = 4; // diff name than ultralib

    __osPackRequestData(CONT_CMD_REQUEST_STATUS);

    ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
    osRecvMesg(mq, &mesg, OS_MESG_BLOCK);

    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    osRecvMesg(mq, &mesg, OS_MESG_BLOCK);
	
    __osContGetInitDataEx(bitpattern, status);
    _osLastSentSiCmd = CONT_CMD_REQUEST_STATUS;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&_osContMesgQueue, _osContMesgBuff, 1);
    return ret;
}
void __osContGetInitData(u8 *bitpattern, OSContStatus *status) {
    u8 *ptr;
    __OSContRequesFormat requestHeader;
    s32 i;
    u8 bits ;

    bits  = 0;
    ptr = __osContPifRam.ramarray;
    for (i = 0; i < _osContNumControllers; i++, ptr += sizeof(requestHeader), status++) {
        requestHeader = *(__OSContRequesFormat*)ptr;
        status->errnum = CHNL_ERR(requestHeader);
        if (status->errnum == 0) {
            status->type = requestHeader.typel << 8 | requestHeader.typeh;
            status->status = requestHeader.status;

            bits  |= 1 << i;
        }
    }
    *bitpattern = bits ;
}
void __osPackRequestData(u8 command) {
    u8* ptr;
    __OSContRequesFormat requestHeader;
    s32 i;

    for (i = 0; i < ARRLEN(__osContPifRam.ramarray); i++) {
        __osContPifRam.ramarray[i] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    ptr = __osContPifRam.ramarray;
    requestHeader.dummy = CONT_CMD_NOP;
    requestHeader.txsize = CONT_CMD_RESET_TX;
    requestHeader.rxsize = CONT_CMD_RESET_RX;
    requestHeader.cmd = command;
    requestHeader.typeh = CONT_CMD_NOP;
    requestHeader.typel = CONT_CMD_NOP;
    requestHeader.status = CONT_CMD_NOP;
    requestHeader.dummy1 = CONT_CMD_NOP;

    for (i = 0; i < _osContNumControllers; i++) {
        *(__OSContRequesFormat*)ptr = requestHeader;
        ptr += sizeof(requestHeader);
    }
    *ptr = CONT_CMD_END;
}
