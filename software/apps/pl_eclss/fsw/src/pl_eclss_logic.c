/* SPDX-License-Identifier: MIT */
#include "pl_eclss_logic.h"
#include <string.h>

/*
 * DEFAULT LIMITS ARE PLACEHOLDERS (estimates), not requirements.
 * Nominal 101.3 kPa / ~21% O2 and the 0.05%/day leak target come from
 * Ping Lite concept-design-v0.2 (A11, SR-21, Section 5.5). All other numbers
 * must be replaced by values derived from NASA-STD-3001 Vol. 2 and
 * CLDP-REQ-1130 before any use beyond simulation.
 */
void PL_Eclss_DefaultConfig(PL_EclssConfig_t *c)
{
    static const float lo[PL_E_NPARAM]  = {19.5f, 0.0f, 97.0f, 25.0f, 18.0f};
    static const float hi[PL_E_NPARAM]  = {23.5f, 0.53f, 103.0f, 75.0f, 27.0f};
    static const float plo[PL_E_NPARAM] = {0.0f, 0.0f, 0.0f, 0.0f, -40.0f};
    static const float phi[PL_E_NPARAM] = {110.0f, 20.0f, 120.0f, 100.0f, 80.0f};
    memcpy(c->lo, lo, sizeof lo);
    memcpy(c->hi, hi, sizeof hi);
    memcpy(c->plaus_lo, plo, sizeof plo);
    memcpy(c->plaus_hi, phi, sizeof phi);
    c->persist = 3;
    c->leak_kg_per_day = 0.34f; /* ~0.05%/day of ~670 kg cabin air (v0.2 s5.5) */
    c->volume_m3 = 560.0f;
    /* Leak sensitivity is limited by pressure-sensor noise: a 0.05%/day leak is
     * only ~0.05 kPa/day. A long, decimated window (64 x 5 min, >= 3 h span)
     * keeps the false-alarm rate low; see docs/SRS-and-SAP.md (open item). */
    c->leak_min_span_s = 3u * 3600u;
    c->leak_period_s = 300u;
}

void PL_Eclss_Init(PL_EclssState_t *s, const PL_EclssConfig_t *cfg)
{
    memset(s, 0, sizeof *s);
    s->cfg = *cfg;
}

const char *PL_Eclss_ParamName(int i)
{
    static const char *n[PL_E_NPARAM] = {"ppO2", "ppCO2", "P", "RH", "T"};
    return (i >= 0 && i < PL_E_NPARAM) ? n[i] : "?";
}

/* Least-squares slope of pressure (kPa/s) over the window. Returns 0 if n<3. */
static int fit_slope(const PL_EclssState_t *s, double *slope, double *tmean_c, uint32_t *span)
{
    uint32_t n = s->win_n, i, idx, t0, tmin = 0xFFFFFFFFu, tmax = 0;
    double st = 0, sp = 0, stt = 0, stp = 0, stemp = 0;
    if (n < 3) return 0;
    t0 = s->win_t[(s->win_head + PL_ECLSS_WIN - n) % PL_ECLSS_WIN];
    for (i = 0; i < n; i++)
    {
        double t, p;
        idx = (s->win_head + PL_ECLSS_WIN - n + i) % PL_ECLSS_WIN;
        t = (double)(s->win_t[idx] - t0);
        p = s->win_p[idx];
        st += t; sp += p; stt += t * t; stp += t * p; stemp += s->win_temp[idx];
        if (s->win_t[idx] < tmin) tmin = s->win_t[idx];
        if (s->win_t[idx] > tmax) tmax = s->win_t[idx];
    }
    if ((n * stt - st * st) == 0.0) return 0;
    *slope = (n * stp - st * sp) / (n * stt - st * st);
    *tmean_c = stemp / n;
    *span = tmax - tmin;
    return 1;
}

const PL_EclssStatus_t *PL_Eclss_Step(PL_EclssState_t *s, const PL_EclssSample_t *in)
{
    const float v[PL_E_NPARAM] = {in->ppo2_kpa, in->ppco2_kpa, in->p_kpa, in->rh_pct, in->temp_c};
    uint32_t before = s->st.fault_mask, mask = before;
    int i;

    for (i = 0; i < PL_E_NPARAM; i++)
    {
        int valid = (in->valid_mask >> i) & 1u;
        int plaus = valid && v[i] >= s->cfg.plaus_lo[i] && v[i] <= s->cfg.plaus_hi[i];
        if (!plaus)
        {   /* isolate the sensor: flag it, do not raise or clear process alarms */
            mask |= PL_EF_SENSOR(i);
            s->cnt_lo[i] = s->cnt_hi[i] = 0;
            continue;
        }
        mask &= ~PL_EF_SENSOR(i);
        s->cnt_lo[i] = (v[i] < s->cfg.lo[i]) ? s->cnt_lo[i] + 1 : 0;
        s->cnt_hi[i] = (v[i] > s->cfg.hi[i]) ? s->cnt_hi[i] + 1 : 0;
        if (s->cnt_lo[i] >= s->cfg.persist) mask |= PL_EF_LOW(i);
        else if (v[i] >= s->cfg.lo[i]) mask &= ~PL_EF_LOW(i);
        if (s->cnt_hi[i] >= s->cfg.persist) mask |= PL_EF_HIGH(i);
        else if (v[i] <= s->cfg.hi[i]) mask &= ~PL_EF_HIGH(i);
    }

    /* Leak-rate detection from total pressure, only with a plausible P sample */
    s->st.leak_kg_per_day = 0.0f;
    if (!(mask & PL_EF_SENSOR(PL_E_P)))
    {
        double slope, tc;
        uint32_t span;
        if (s->win_n == 0 || in->t_s - s->last_push_t >= s->cfg.leak_period_s)
        {
            s->win_t[s->win_head] = in->t_s;
            s->win_p[s->win_head] = in->p_kpa;
            s->win_temp[s->win_head] = ((in->valid_mask >> PL_E_T) & 1u) ? in->temp_c : 22.0f;
            s->win_head = (s->win_head + 1) % PL_ECLSS_WIN;
            if (s->win_n < PL_ECLSS_WIN) s->win_n++;
            s->last_push_t = in->t_s;
        }
        if (fit_slope(s, &slope, &tc, &span) && span >= s->cfg.leak_min_span_s)
        {
            /* ideal gas, air M = 0.02897 kg/mol, R = 8.314 J/mol/K:
             * dm/dt [kg/s] = V * M / (R*T) * dp/dt [Pa/s] */
            double kgps = s->cfg.volume_m3 * 0.02897 / (8.314 * (tc + 273.15)) * (slope * 1000.0);
            s->st.leak_kg_per_day = (float)(-kgps * 86400.0);
            s->leak_cnt = (s->st.leak_kg_per_day > s->cfg.leak_kg_per_day) ? s->leak_cnt + 1 : 0;
            if (s->leak_cnt >= s->cfg.persist) mask |= PL_EF_LEAK;
            else if (s->st.leak_kg_per_day < 0.5f * s->cfg.leak_kg_per_day) mask &= ~PL_EF_LEAK;
        }
    }

    s->st.new_faults = mask & ~before;
    s->st.cleared = before & ~mask;
    s->st.fault_mask = mask;

    /* Severity: low ppO2, high ppCO2, low P or leak => warning; sensor-only => caution.
     * Emergency when total pressure is low AND a leak is declared. */
    s->st.level = 0;
    if (mask & 0x00FF0000u) s->st.level = 1;
    if (mask & (PL_EF_HIGH(PL_E_RH) | PL_EF_LOW(PL_E_RH) | PL_EF_HIGH(PL_E_T) | PL_EF_LOW(PL_E_T) |
                PL_EF_HIGH(PL_E_PPO2) | PL_EF_HIGH(PL_E_P)))
        s->st.level = 1;
    if (mask & (PL_EF_LOW(PL_E_PPO2) | PL_EF_HIGH(PL_E_PPCO2) | PL_EF_LOW(PL_E_P) | PL_EF_LEAK))
        s->st.level = 2;
    if ((mask & PL_EF_LOW(PL_E_P)) && (mask & PL_EF_LEAK)) s->st.level = 3;
    return &s->st;
}
