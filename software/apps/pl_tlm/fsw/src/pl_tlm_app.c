/* SPDX-License-Identifier: MIT
 * PL_TLM: telemetry logger. Records Ping Lite housekeeping arrivals and
 * counts single-event upsets from EDAC counters (with wrap/reset handling).
 */
#include "pl_app_common.h"
#include "pl_tlm_logic.h"

enum { EID_INIT = 1, EID_SEU, EID_RESET, EID_BADLEN, EID_BADCC };

typedef struct
{
    CFE_MSG_TelemetryHeader_t Hdr;
    uint32 Logged, Dropped, Resets, Rejected;
    uint32 SeuCorrected, SeuUncorrected;
    float  SeuPerDay;
} PL_TlmHk_t;

static PL_TlmState_t S;
static PL_TlmHk_t Hk;
static CFE_SB_PipeId_t Pipe;

static void Process(const CFE_SB_Buffer_t *buf)
{
    CFE_SB_MsgId_t mid = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t cc = 0;
    uint32 v;
    CFE_MSG_GetMsgId(&buf->Msg, &mid);
    v = CFE_SB_MsgIdToValue(mid);
    if (v == PL_ECLSS_HK_MID || v == PL_PWRTH_HK_MID || v == PL_OPS_HK_MID)
    {
        PL_Tlm_Log(&S, 0, (uint16_t)v, 0, 0);
    }
    else
    {
        CFE_MSG_GetFcnCode(&buf->Msg, &cc);
        if (cc == PL_SAMPLE_CC)
        {
            PL_TlmSample_t x;
            uint32 resets = S.resets_seen, du;
            if (!PL_GetPayload(buf, &x, sizeof x)) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADLEN, CFE_EVS_EventType_ERROR, "PL_TLM: bad length"); return; }
            du = PL_Tlm_SeuSample(&S, &x);
            if (du) CFE_EVS_SendEvent(EID_SEU, CFE_EVS_EventType_ERROR, "PL_TLM: SEU_UNCORRECTED +%lu t=%lu", (unsigned long)du, (unsigned long)x.t_s);
            if (S.resets_seen != resets) CFE_EVS_SendEvent(EID_RESET, CFE_EVS_EventType_INFORMATION, "PL_TLM: processor reset seen boot=%lu", (unsigned long)x.boot_count);
        }
        else if (cc != PL_NOOP_CC)
        {
            Hk.Rejected++;
            CFE_EVS_SendEvent(EID_BADCC, CFE_EVS_EventType_ERROR, "PL_TLM: bad fcn code");
            return;
        }
        Hk.Logged = S.count; Hk.Dropped = S.dropped; Hk.Resets = S.resets_seen;
        Hk.SeuCorrected = (uint32)S.seu_corrected_total; Hk.SeuUncorrected = (uint32)S.seu_uncorrected_total;
        Hk.SeuPerDay = (float)PL_Tlm_SeuRatePerDay(&S);
        CFE_SB_TimeStampMsg(CFE_MSG_PTR(Hk.Hdr));
        CFE_SB_TransmitMsg(CFE_MSG_PTR(Hk.Hdr), true);
    }
}

void PL_TLM_AppMain(void)
{
    uint32 run = CFE_ES_RunStatus_APP_RUN;
    CFE_SB_Buffer_t *buf;
    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    PL_Tlm_Init(&S);
    memset(&Hk, 0, sizeof Hk);
    CFE_MSG_Init(CFE_MSG_PTR(Hk.Hdr), CFE_SB_ValueToMsgId(PL_TLM_HK_MID), sizeof Hk);
    if (CFE_SB_CreatePipe(&Pipe, PL_PIPE_DEPTH, "PL_TLM_PIPE") != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_TLM_CMD_MID), Pipe) != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_ECLSS_HK_MID), Pipe) != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_PWRTH_HK_MID), Pipe) != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_OPS_HK_MID), Pipe) != CFE_SUCCESS)
        run = CFE_ES_RunStatus_APP_ERROR;
    else
        CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_TLM: initialized v0.1 (logger + SEU counting)");
    while (CFE_ES_RunLoop(&run))
    {
        CFE_Status_t rc = CFE_SB_ReceiveBuffer(&buf, Pipe, PL_RX_TIMEOUT_MS);
        if (rc == CFE_SUCCESS) Process(buf);
        else if (rc != CFE_SB_TIME_OUT && rc != CFE_SB_NO_MESSAGE) run = CFE_ES_RunStatus_APP_ERROR;
    }
    CFE_ES_ExitApp(run);
}
