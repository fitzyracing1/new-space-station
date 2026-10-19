/* SPDX-License-Identifier: MIT */
#include "ut.h"
#include "pl_tlm_logic.h"
int main(void)
{
    PL_TlmState_t s; PL_TlmSample_t x; uint32_t i;
    PL_Tlm_Init(&s);
    x = (PL_TlmSample_t){0, 100, 0, 1}; CHECK(PL_Tlm_SeuSample(&s, &x) == 0);
    x = (PL_TlmSample_t){3600, 110, 0, 1}; PL_Tlm_SeuSample(&s, &x);
    CHECK(s.seu_corrected_total == 10);
    /* wrap-around */
    PL_Tlm_Init(&s);
    x = (PL_TlmSample_t){0, 0xFFFFFFFEu, 0, 1}; PL_Tlm_SeuSample(&s, &x);
    x = (PL_TlmSample_t){10, 3, 0, 1}; PL_Tlm_SeuSample(&s, &x);
    CHECK(s.seu_corrected_total == 5);
    /* reset: counters restart, raw value is the delta */
    x = (PL_TlmSample_t){20, 2, 1, 2}; CHECK(PL_Tlm_SeuSample(&s, &x) == 1);
    CHECK(s.seu_corrected_total == 7); CHECK(s.seu_uncorrected_total == 1); CHECK(s.resets_seen == 1);
    /* rate */
    PL_Tlm_Init(&s);
    x = (PL_TlmSample_t){0, 0, 0, 0}; PL_Tlm_SeuSample(&s, &x);
    x = (PL_TlmSample_t){86400, 24, 0, 0}; PL_Tlm_SeuSample(&s, &x);
    CHECK_NEAR(PL_Tlm_SeuRatePerDay(&s), 24.0, 1e-9);
    /* ring overflow counts drops */
    PL_Tlm_Init(&s);
    for (i = 0; i < PL_TLM_LOG_DEPTH + 5; i++) PL_Tlm_Log(&s, i, 1, 0, i);
    CHECK(s.count == PL_TLM_LOG_DEPTH); CHECK(s.dropped == 5);
    UT_DONE("test_tlm");
}
