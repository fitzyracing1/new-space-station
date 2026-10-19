/* SPDX-License-Identifier: MIT */
#include "pl_tlm_logic.h"
#include <string.h>

void PL_Tlm_Init(PL_TlmState_t *s) { memset(s, 0, sizeof *s); }

void PL_Tlm_Log(PL_TlmState_t *s, uint32_t t_s, uint16_t mid, uint16_t code, uint32_t value)
{
    PL_LogRec_t *r = &s->ring[s->head];
    r->t_s = t_s; r->source_mid = mid; r->code = code; r->value = value;
    s->head = (s->head + 1) % PL_TLM_LOG_DEPTH;
    if (s->count < PL_TLM_LOG_DEPTH) s->count++;
    else s->dropped++;
}

/* Counter delta that tolerates 32-bit wrap. A processor reset (boot_count
 * change) restarts the counters from zero, so the new raw value is the delta. */
static uint32_t delta(uint32_t prev, uint32_t now, int reset)
{
    if (reset) return now;
    return now - prev; /* modular arithmetic handles wrap */
}

uint32_t PL_Tlm_SeuSample(PL_TlmState_t *s, const PL_TlmSample_t *in)
{
    uint32_t dc, du;
    int reset;
    if (!s->have_prev)
    {
        s->have_prev = 1;
        s->first_t = in->t_s;
        s->prev_corr = in->edac_corrected;
        s->prev_uncorr = in->edac_uncorrected;
        s->prev_boot = in->boot_count;
        s->last_t = in->t_s;
        return 0;
    }
    reset = (in->boot_count != s->prev_boot);
    if (reset) s->resets_seen++;
    dc = delta(s->prev_corr, in->edac_corrected, reset);
    du = delta(s->prev_uncorr, in->edac_uncorrected, reset);
    s->seu_corrected_total += dc;
    s->seu_uncorrected_total += du;
    s->prev_corr = in->edac_corrected;
    s->prev_uncorr = in->edac_uncorrected;
    s->prev_boot = in->boot_count;
    s->last_t = in->t_s;
    if (dc) PL_Tlm_Log(s, in->t_s, PL_TLM_CMD_MID, 1, dc);
    if (du) PL_Tlm_Log(s, in->t_s, PL_TLM_CMD_MID, 2, du);
    if (reset) PL_Tlm_Log(s, in->t_s, PL_TLM_CMD_MID, 3, in->boot_count);
    return du;
}

double PL_Tlm_SeuRatePerDay(const PL_TlmState_t *s)
{
    uint32_t span = s->last_t - s->first_t;
    if (span < 1) return 0.0;
    return (double)s->seu_corrected_total * 86400.0 / (double)span;
}
