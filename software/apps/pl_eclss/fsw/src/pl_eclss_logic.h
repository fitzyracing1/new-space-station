/* SPDX-License-Identifier: MIT
 * ECLSS monitor and fault detection logic (pure C, no cFS dependency).
 * SHADOW MODE: this module only classifies; it has no command outputs.
 */
#ifndef PL_ECLSS_LOGIC_H
#define PL_ECLSS_LOGIC_H
#include <stdint.h>
#include "pl_msg.h"

enum { PL_E_PPO2 = 0, PL_E_PPCO2, PL_E_P, PL_E_RH, PL_E_T, PL_E_NPARAM };

/* Fault bits reported in PL_EclssStatus_t.fault_mask */
#define PL_EF_LOW(i)      (1u << (2 * (i)))
#define PL_EF_HIGH(i)     (1u << (2 * (i) + 1))
#define PL_EF_SENSOR(i)   (1u << (16 + (i)))
#define PL_EF_LEAK        (1u << 24)

#define PL_ECLSS_WIN 64 /* pressure samples kept for leak-rate fit */

typedef struct
{
    float lo[PL_E_NPARAM];
    float hi[PL_E_NPARAM];
    float plaus_lo[PL_E_NPARAM]; /* outside this = sensor fault, not an alarm */
    float plaus_hi[PL_E_NPARAM];
    uint32_t persist;            /* consecutive samples before declaring */
    float leak_kg_per_day;       /* leak alarm threshold                 */
    float volume_m3;             /* free cabin volume                    */
    uint32_t leak_min_span_s;    /* minimum fit window                   */
    uint32_t leak_period_s;      /* decimation: one window sample per period */
} PL_EclssConfig_t;

typedef struct
{
    uint32_t fault_mask;   /* currently declared faults */
    uint32_t new_faults;   /* declared on this sample   */
    uint32_t cleared;      /* cleared on this sample    */
    float leak_kg_per_day; /* positive = mass loss; 0 if not computed */
    uint32_t level;        /* 0 none,1 caution,2 warning,3 emergency */
} PL_EclssStatus_t;

typedef struct
{
    PL_EclssConfig_t cfg;
    uint32_t cnt_lo[PL_E_NPARAM], cnt_hi[PL_E_NPARAM];
    uint32_t win_t[PL_ECLSS_WIN];
    float win_p[PL_ECLSS_WIN];
    float win_temp[PL_ECLSS_WIN];
    uint32_t win_n, win_head, last_push_t;
    uint32_t leak_cnt;
    PL_EclssStatus_t st;
} PL_EclssState_t;

void PL_Eclss_DefaultConfig(PL_EclssConfig_t *cfg);
void PL_Eclss_Init(PL_EclssState_t *s, const PL_EclssConfig_t *cfg);
const PL_EclssStatus_t *PL_Eclss_Step(PL_EclssState_t *s, const PL_EclssSample_t *in);
const char *PL_Eclss_ParamName(int i);
#endif
