/* SPDX-License-Identifier: MIT */
#include "pl_ops_logic.h"
#include <string.h>

/* PLACEHOLDER thresholds (estimates). Real values come from the comm
 * coverage analysis and the SOW s1.8.1.1 autonomy requirements. */
void PL_Ops_DefaultConfig(PL_OpsConfig_t *c)
{
    c->t_auto_s = 15u * 60u;
    c->t_safe_s = 4u * 3600u;
}

void PL_Ops_Init(PL_OpsState_t *s, const PL_OpsConfig_t *c)
{
    memset(s, 0, sizeof *s);
    s->cfg = *c;
    s->mode = PL_MODE_CREWED;
}

const char *PL_Ops_ModeName(PL_Mode_t m)
{
    switch (m)
    {
        case PL_MODE_CREWED: return "CREWED";
        case PL_MODE_UNCREWED_NOMINAL: return "UNCREWED_NOMINAL";
        case PL_MODE_UNCREWED_AUTONOMOUS: return "UNCREWED_AUTONOMOUS";
        case PL_MODE_SAFE_HOLD: return "SAFE_HOLD";
        default: return "?";
    }
}

static int set_mode(PL_OpsState_t *s, PL_Mode_t m)
{
    if (s->mode == m) return 0;
    s->mode = m;
    s->transitions++;
    return 1;
}

int PL_Ops_Step(PL_OpsState_t *s, const PL_OpsSample_t *in)
{
    uint32_t los_s = 0;
    if (!s->initialized)
    {
        s->initialized = 1;
        s->mode = in->crew_aboard ? PL_MODE_CREWED : PL_MODE_UNCREWED_NOMINAL;
    }
    if (!in->aos)
    {
        if (!s->in_los) { s->in_los = 1; s->los_start_s = in->t_s; }
        los_s = in->t_s - s->los_start_s;
    }
    else
    {
        s->in_los = 0;
    }

    /* Crew aboard: the crew is in charge, supervisor stays in CREWED. */
    if (in->crew_aboard) return set_mode(s, PL_MODE_CREWED);
    if (s->mode == PL_MODE_CREWED) return set_mode(s, PL_MODE_UNCREWED_NOMINAL);

    /* SAFE_HOLD is latched until a ground acknowledgement. */
    if (s->mode == PL_MODE_SAFE_HOLD) return 0;
    if (in->eclss_level >= 2) return set_mode(s, PL_MODE_SAFE_HOLD);
    if (s->in_los && los_s >= s->cfg.t_safe_s) return set_mode(s, PL_MODE_SAFE_HOLD);
    if (s->in_los && los_s >= s->cfg.t_auto_s) return set_mode(s, PL_MODE_UNCREWED_AUTONOMOUS);
    if (!s->in_los) return set_mode(s, PL_MODE_UNCREWED_NOMINAL);
    return 0;
}

int PL_Ops_GroundAck(PL_OpsState_t *s, uint32_t crew_aboard, uint32_t aos)
{
    if (s->mode != PL_MODE_SAFE_HOLD || !aos) return 0;
    return set_mode(s, crew_aboard ? PL_MODE_CREWED : PL_MODE_UNCREWED_NOMINAL);
}
