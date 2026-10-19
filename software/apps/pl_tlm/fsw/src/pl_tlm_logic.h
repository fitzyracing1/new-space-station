/* SPDX-License-Identifier: MIT
 * Telemetry logger with single-event-upset (SEU) accounting (pure C).
 */
#ifndef PL_TLM_LOGIC_H
#define PL_TLM_LOGIC_H
#include <stdint.h>
#include "pl_msg.h"

#define PL_TLM_LOG_DEPTH 128

typedef struct
{
    uint32_t t_s;
    uint16_t source_mid;
    uint16_t code;
    uint32_t value;
} PL_LogRec_t;

typedef struct
{
    PL_LogRec_t ring[PL_TLM_LOG_DEPTH];
    uint32_t head, count, dropped;
    /* SEU accounting */
    uint32_t have_prev, prev_corr, prev_uncorr, prev_boot, first_t, last_t;
    uint64_t seu_corrected_total, seu_uncorrected_total;
    uint32_t resets_seen;
} PL_TlmState_t;

void PL_Tlm_Init(PL_TlmState_t *s);
void PL_Tlm_Log(PL_TlmState_t *s, uint32_t t_s, uint16_t mid, uint16_t code, uint32_t value);
/* Returns number of NEW uncorrected upsets in this sample. */
uint32_t PL_Tlm_SeuSample(PL_TlmState_t *s, const PL_TlmSample_t *in);
/* Corrected upsets per day since first sample (0 if span < 1 s). */
double PL_Tlm_SeuRatePerDay(const PL_TlmState_t *s);
#endif
