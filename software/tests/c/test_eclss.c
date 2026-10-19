/* SPDX-License-Identifier: MIT */
#include "ut.h"
#include "pl_eclss_logic.h"

static PL_EclssSample_t nominal(uint32_t t, float p)
{
    PL_EclssSample_t s = {t, 0x1F, 21.3f, 0.30f, p, 45.0f, 22.0f};
    return s;
}

int main(void)
{
    PL_EclssConfig_t c; PL_EclssState_t s; const PL_EclssStatus_t *st;
    PL_EclssSample_t x; uint32_t t; int i;
    PL_Eclss_DefaultConfig(&c);

    /* 1. nominal: no faults */
    PL_Eclss_Init(&s, &c);
    for (t = 0; t < 3600; t += 60) { x = nominal(t, 101.3f); st = PL_Eclss_Step(&s, &x); }
    CHECK(st->fault_mask == 0); CHECK(st->level == 0);

    /* 2. high CO2 requires persistence (3 samples) then clears */
    PL_Eclss_Init(&s, &c);
    for (i = 0; i < 2; i++) { x = nominal(i, 101.3f); x.ppco2_kpa = 0.8f; st = PL_Eclss_Step(&s, &x); }
    CHECK(!(st->fault_mask & PL_EF_HIGH(PL_E_PPCO2)));
    x.t_s = 2; st = PL_Eclss_Step(&s, &x);
    CHECK(st->fault_mask & PL_EF_HIGH(PL_E_PPCO2)); CHECK(st->new_faults & PL_EF_HIGH(PL_E_PPCO2)); CHECK(st->level == 2);
    x = nominal(3, 101.3f); st = PL_Eclss_Step(&s, &x);
    CHECK(!(st->fault_mask & PL_EF_HIGH(PL_E_PPCO2))); CHECK(st->cleared & PL_EF_HIGH(PL_E_PPCO2));

    /* 3. low O2 */
    PL_Eclss_Init(&s, &c);
    for (i = 0; i < 3; i++) { x = nominal(i, 101.3f); x.ppo2_kpa = 18.0f; st = PL_Eclss_Step(&s, &x); }
    CHECK(st->fault_mask & PL_EF_LOW(PL_E_PPO2));

    /* 4. implausible sensor -> sensor fault, not a process alarm */
    PL_Eclss_Init(&s, &c);
    for (i = 0; i < 5; i++) { x = nominal(i, 101.3f); x.rh_pct = 250.0f; st = PL_Eclss_Step(&s, &x); }
    CHECK(st->fault_mask & PL_EF_SENSOR(PL_E_RH)); CHECK(!(st->fault_mask & PL_EF_HIGH(PL_E_RH))); CHECK(st->level == 1);
    /* invalid flag also isolates */
    x = nominal(6, 101.3f); x.valid_mask &= ~(1u << PL_E_T); st = PL_Eclss_Step(&s, &x);
    CHECK(st->fault_mask & PL_EF_SENSOR(PL_E_T));

    /* 5. leak-rate: 1 kg/day equivalent pressure decay must be detected, 0.05 kg/day must not.
     * dp/dt [kPa/s] = -m_dot * R*T / (V*M) / 1000 */
    {
        double T = 295.15, kgpd, dpdt; const double cases[2] = {0.05, 1.0}; int k;
        for (k = 0; k < 2; k++)
        {
            kgpd = cases[k];
            dpdt = -(kgpd / 86400.0) * 8.314 * T / (560.0 * 0.02897) / 1000.0;
            PL_Eclss_Init(&s, &c);
            for (t = 0; t <= 4 * 3600; t += 120) { x = nominal(t, (float)(101.3 + dpdt * t)); st = PL_Eclss_Step(&s, &x); }
            if (kgpd < 0.1) { CHECK(!(st->fault_mask & PL_EF_LEAK)); CHECK_NEAR(st->leak_kg_per_day, 0.05, 0.03); }
            else { CHECK(st->fault_mask & PL_EF_LEAK); CHECK_NEAR(st->leak_kg_per_day, 1.0, 0.1); }
        }
    }

    /* 6. leak + low pressure => emergency */
    PL_Eclss_Init(&s, &c);
    for (t = 0; t <= 4 * 3600; t += 120) { x = nominal(t, (float)(99.0 - 0.0004 * t)); st = PL_Eclss_Step(&s, &x); }
    CHECK(st->fault_mask & PL_EF_LEAK); CHECK(st->fault_mask & PL_EF_LOW(PL_E_P)); CHECK(st->level == 3);
    CHECK(PL_Eclss_ParamName(0)[0] == 'p');
    UT_DONE("test_eclss");
}
