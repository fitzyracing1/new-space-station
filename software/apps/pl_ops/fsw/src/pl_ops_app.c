/* SPDX-License-Identifier: MIT
 * PL_OPS: uncrewed-operations supervisor (mode manager, comm-loss handling).
 * v0.1 reports the mode it would select; it commands nothing.
 * The ECLSS severity input comes from PL_ECLSS housekeeping on the bus.
 */
#include "pl_app_common.h"
#include "pl_ops_logic.h"

enum { EID_INIT = 1, EID_MODE, EID_BADLEN, EID_BADCC };

typedef struct
{
    CFE_MSG_TelemetryHeader_t Hdr;
    uint32 Samples, Mode, Transitions, Rejected, EclssLevel;
} PL_OpsHk_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t Hdr;
    uint32 Samples, FaultMask, Level, Rejected;
    float  LeakKgPerDay;
} PL_EclssHkView_t; /* must match PL_EclssHk_t in pl_eclss_app.c */

static PL_OpsState_t S;
static PL_OpsHk_t Hk;
static CFE_SB_PipeId_t Pipe;
static uint32 LastCrew = 1, LastAos = 1;

static void Report(PL_Mode_t prev, uint32 t)
{
    CFE_EVS_SendEvent(EID_MODE, CFE_EVS_EventType_INFORMATION, "PL_OPS: MODE %s -> %s t=%lu", PL_Ops_ModeName(prev),
                      PL_Ops_ModeName(S.mode), (unsigned long)t);
}

static void Process(const CFE_SB_Buffer_t *buf)
{
    CFE_SB_MsgId_t mid = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t cc = 0;
    PL_Mode_t prev = S.mode;
    CFE_MSG_GetMsgId(&buf->Msg, &mid);
    if (CFE_SB_MsgIdToValue(mid) == PL_ECLSS_HK_MID)
    {
        CFE_MSG_Size_t sz = 0;
        CFE_MSG_GetSize(&buf->Msg, &sz);
        if (sz == sizeof(PL_EclssHkView_t)) Hk.EclssLevel = ((const PL_EclssHkView_t *)(const void *)buf)->Level;
        return;
    }
    CFE_MSG_GetFcnCode(&buf->Msg, &cc);
    if (cc == PL_SAMPLE_CC)
    {
        PL_OpsSample_t x;
        if (!PL_GetPayload(buf, &x, sizeof x)) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADLEN, CFE_EVS_EventType_ERROR, "PL_OPS: bad length"); return; }
        if (x.eclss_level < Hk.EclssLevel) x.eclss_level = Hk.EclssLevel;
        LastCrew = x.crew_aboard; LastAos = x.aos;
        Hk.Samples++;
        if (PL_Ops_Step(&S, &x)) Report(prev, x.t_s);
    }
    else if (cc == PL_ACK_CC)
    {
        if (PL_Ops_GroundAck(&S, LastCrew, LastAos)) Report(prev, 0);
    }
    else if (cc != PL_NOOP_CC)
    {
        Hk.Rejected++;
        CFE_EVS_SendEvent(EID_BADCC, CFE_EVS_EventType_ERROR, "PL_OPS: bad fcn code");
        return;
    }
    Hk.Mode = S.mode; Hk.Transitions = S.transitions;
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(Hk.Hdr));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(Hk.Hdr), true);
}

void PL_OPS_AppMain(void)
{
    uint32 run = CFE_ES_RunStatus_APP_RUN;
    PL_OpsConfig_t cfg;
    CFE_SB_Buffer_t *buf;
    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    PL_Ops_DefaultConfig(&cfg);
    PL_Ops_Init(&S, &cfg);
    memset(&Hk, 0, sizeof Hk);
    CFE_MSG_Init(CFE_MSG_PTR(Hk.Hdr), CFE_SB_ValueToMsgId(PL_OPS_HK_MID), sizeof Hk);
    if (CFE_SB_CreatePipe(&Pipe, PL_PIPE_DEPTH, "PL_OPS_PIPE") != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_OPS_CMD_MID), Pipe) != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_ECLSS_HK_MID), Pipe) != CFE_SUCCESS)
        run = CFE_ES_RunStatus_APP_ERROR;
    else
        CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_OPS: initialized v0.1 (advisory mode manager)");
    while (CFE_ES_RunLoop(&run))
    {
        CFE_Status_t rc = CFE_SB_ReceiveBuffer(&buf, Pipe, PL_RX_TIMEOUT_MS);
        if (rc == CFE_SUCCESS) Process(buf);
        else if (rc != CFE_SB_TIME_OUT && rc != CFE_SB_NO_MESSAGE) run = CFE_ES_RunStatus_APP_ERROR;
    }
    CFE_ES_ExitApp(run);
}
