/* SPDX-License-Identifier: MIT
 * PL_PWRTH: power and thermal scheduler. v0.1 publishes recommended load-shed
 * plans and heater power only; it does not switch any load.
 */
#include "pl_app_common.h"
#include "pl_pwrth_logic.h"

enum { EID_INIT = 1, EID_PLAN, EID_UNMET, EID_BADLEN, EID_BADCC };

typedef struct
{
    CFE_MSG_TelemetryHeader_t Hdr;
    uint32 Samples, ShedMask, Unmet, Rejected;
    float  AvailableW, DemandW, HeaterW, MarginW;
} PL_PwrthHk_t;

static PL_PwrthConfig_t Cfg;
static PL_PwrthHk_t Hk;
static CFE_SB_PipeId_t Pipe;

static void Process(const CFE_SB_Buffer_t *buf)
{
    CFE_MSG_FcnCode_t cc = 0;
    PL_PwrthSample_t x;
    PL_PwrthPlan_t p;
    CFE_MSG_GetFcnCode(&buf->Msg, &cc);
    if (cc == PL_NOOP_CC) { CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_PWRTH: NOOP"); return; }
    if (cc != PL_SAMPLE_CC) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADCC, CFE_EVS_EventType_ERROR, "PL_PWRTH: bad fcn code"); return; }
    if (!PL_GetPayload(buf, &x, sizeof x)) { Hk.Rejected++; CFE_EVS_SendEvent(EID_BADLEN, CFE_EVS_EventType_ERROR, "PL_PWRTH: bad length"); return; }
    PL_Pwrth_Plan(&Cfg, &x, &p);
    Hk.Samples++;
    if (p.shed_mask != Hk.ShedMask)
        CFE_EVS_SendEvent(EID_PLAN, CFE_EVS_EventType_INFORMATION, "PL_PWRTH: PLAN shed=0x%lx heater=%.0f W margin=%.0f W t=%lu",
                          (unsigned long)p.shed_mask, p.heater_w, p.margin_w, (unsigned long)x.t_s);
    if (p.unmet && !Hk.Unmet)
        CFE_EVS_SendEvent(EID_UNMET, CFE_EVS_EventType_ERROR, "PL_PWRTH: UNMET critical demand t=%lu", (unsigned long)x.t_s);
    Hk.ShedMask = p.shed_mask; Hk.Unmet = p.unmet; Hk.AvailableW = p.available_w; Hk.DemandW = p.demand_w;
    Hk.HeaterW = p.heater_w; Hk.MarginW = p.margin_w;
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(Hk.Hdr));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(Hk.Hdr), true);
}

void PL_PWRTH_AppMain(void)
{
    uint32 run = CFE_ES_RunStatus_APP_RUN;
    CFE_SB_Buffer_t *buf;
    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    PL_Pwrth_DefaultConfig(&Cfg);
    memset(&Hk, 0, sizeof Hk);
    CFE_MSG_Init(CFE_MSG_PTR(Hk.Hdr), CFE_SB_ValueToMsgId(PL_PWRTH_HK_MID), sizeof Hk);
    if (CFE_SB_CreatePipe(&Pipe, PL_PIPE_DEPTH, "PL_PWRTH_PIPE") != CFE_SUCCESS ||
        CFE_SB_Subscribe(CFE_SB_ValueToMsgId(PL_PWRTH_CMD_MID), Pipe) != CFE_SUCCESS)
        run = CFE_ES_RunStatus_APP_ERROR;
    else
        CFE_EVS_SendEvent(EID_INIT, CFE_EVS_EventType_INFORMATION, "PL_PWRTH: initialized v0.1 (recommendations only)");
    while (CFE_ES_RunLoop(&run))
    {
        CFE_Status_t rc = CFE_SB_ReceiveBuffer(&buf, Pipe, PL_RX_TIMEOUT_MS);
        if (rc == CFE_SUCCESS) Process(buf);
        else if (rc != CFE_SB_TIME_OUT && rc != CFE_SB_NO_MESSAGE) run = CFE_ES_RunStatus_APP_ERROR;
    }
    CFE_ES_ExitApp(run);
}
