/* SPDX-License-Identifier: MIT
 * PL_ECLSS: ECLSS monitor and fault detection, SHADOW MODE ONLY.
 * It subscribes to sensor samples and publishes events and housekeeping.
 * It has no command outputs, by design (see docs/SRS-and-SAP.md SR-SW-001).
 */
#include "pl_app_common.h"
#include "pl_eclss_logic.h"

enum { EID_INIT = 1, EID_FAULT_SET, EID_FAULT_CLEAR, EID_LEVEL, EID_BADLEN, EID_BADCC };

typedef struct
{
    CFE_MSG_TelemetryHeader_t Hdr;
    uint32 Samples, FaultMask, Level, Rejected;
    float  LeakKgPerDay;
} PL_EclssHk_t;

static PL_EclssState_t S;
static PL_EclssHk_t Hk;
static CFE_SB_PipeId_t Pipe;

static void Process(const CFE_SB_Buffer_t *buf)
{
    CFE_MSG_FcnCode_t cc = 0;
    PL_EclssSample_t x;
    const PL_EclssStatus_t *st;
    uint32 prev_level = S.st.level;
    int i;

    CFE_MSG_GetFcnCode(&buf->Msg, &cc);
    if (cc == PL_NOOP_CC) { CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_ECLSS: NOOP (shadow mode)"); return; }
    if (cc != PL_SAMPLE_CC) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADCC, CFE_EVS_EventType_ERROR, "PL_ECLSS: bad fcn code %u", (unsigned)cc); return; }
    if (!PL_GetPayload(buf, &x, sizeof x)) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADLEN, CFE_EVS_EventType_ERROR, "PL_ECLSS: bad length"); return; }

    st = PL_Eclss_Step(&S, &x);
    Hk.Samples++;
    for (i = 0; i < 25; i++)
    {
        if (st->new_faults & (1u << i))
            CFE_EVS_SendEvent(EID_FAULT_SET, CFE_EVS_EventType_ERROR, "PL_ECLSS: FAULT_SET bit=%d t=%lu leak=%.3f kg/day",
                              i, (unsigned long)x.t_s, st->leak_kg_per_day);
        if (st->cleared & (1u << i))
            CFE_EVS_SendEvent(EID_FAULT_CLEAR, CFE_EVS_EventType_INFORMATION, "PL_ECLSS: FAULT_CLEAR bit=%d t=%lu", i,
                              (unsigned long)x.t_s);
    }
    if (st->level != prev_level)
        CFE_EVS_SendEvent(EID_LEVEL, CFE_EVS_EventType_INFORMATION, "PL_ECLSS: LEVEL %lu t=%lu", (unsigned long)st->level,
                          (unsigned long)x.t_s);
    Hk.FaultMask = st->fault_mask; Hk.Level = st->level; Hk.LeakKgPerDay = st->leak_kg_per_day;
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(Hk.Hdr));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(Hk.Hdr), true); /* telemetry only */
}

void PL_ECLSS_AppMain(void)
{
    uint32 run = CFE_ES_RunStatus_APP_RUN;
    PL_EclssConfig_t cfg;
    CFE_SB_Buffer_t *buf;

    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    PL_Eclss_DefaultConfig(&cfg);
    PL_Eclss_Init(&S, &cfg);
    memset(&Hk, 0, sizeof Hk);
    CFE_MSG_Init(CFE_MSG_PTR(Hk.Hdr), CFE_SB_ValueToMsgId(PL_ECLSS_HK_MID), sizeof Hk);
    if (CFE_SB_CreatePipe(&Pipe, PL_PIPE_DEPTH, "PL_ECLSS_PIPE") != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_ECLSS_CMD_MID), Pipe) != CFE_SUCCESS)
        run = CFE_ES_RunStatus_APP_ERROR;
    else
        CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_ECLSS: initialized v0.1, SHADOW MODE (no command outputs)");

    while (CFE_ES_RunLoop(&run))
    {
        CFE_Status_t rc = CFE_SB_ReceiveBuffer(&buf, Pipe, PL_RX_TIMEOUT_MS);
        if (rc == CFE_SUCCESS) Process(buf);
        else if (rc != CFE_SB_TIME_OUT && rc != CFE_SB_NO_MESSAGE) run = CFE_ES_RunStatus_APP_ERROR;
    }
    CFE_ES_ExitApp(run);
}
