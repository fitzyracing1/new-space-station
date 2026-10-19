/* SPDX-License-Identifier: MIT
 * Power and thermal scheduler (pure C). Produces RECOMMENDATIONS only in v0.1.
 */
#ifndef PL_PWRTH_LOGIC_H
#define PL_PWRTH_LOGIC_H
#include <stdint.h>
#include "pl_msg.h"

#define PL_PWRTH_MAX_LOADS 16

typedef struct
{
    const char *name;
    float w;            /* nominal demand */
    uint8_t priority;   /* 0 = never shed (life-critical), larger = shed first */
} PL_Load_t;

typedef struct
{
    PL_Load_t loads[PL_PWRTH_MAX_LOADS];
    uint32_t nloads;
    float batt_max_discharge_w;
    float soc_min_pct;          /* below this, battery support is not counted */
    float heater_setpoint_c;
    float heater_w_per_c;       /* proportional heater gain */
    float heater_max_w;
} PL_PwrthConfig_t;

typedef struct
{
    float available_w;
    float demand_w;   /* after shedding, including heater */
    float heater_w;
    float margin_w;
    uint32_t shed_mask; /* bit i = load i shed */
    uint32_t unmet;     /* 1 if even unsheddable loads exceed supply */
} PL_PwrthPlan_t;

void PL_Pwrth_DefaultConfig(PL_PwrthConfig_t *c);
void PL_Pwrth_Plan(const PL_PwrthConfig_t *c, const PL_PwrthSample_t *in, PL_PwrthPlan_t *out);
#endif
