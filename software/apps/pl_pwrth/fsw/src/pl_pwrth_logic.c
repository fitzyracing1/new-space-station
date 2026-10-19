/* SPDX-License-Identifier: MIT */
#include "pl_pwrth_logic.h"
#include <string.h>

/* ILLUSTRATIVE LOAD TABLE (estimates loosely scaled to the ~55 kW EOL v0.2
 * power budget). Not a design power budget. */
void PL_Pwrth_DefaultConfig(PL_PwrthConfig_t *c)
{
    static const PL_Load_t def[] = {
        {"eclss_core", 9000.0f, 0}, {"avionics", 3000.0f, 0}, {"thermal_pumps", 4000.0f, 0},
        {"comm", 1500.0f, 1},       {"galley", 2500.0f, 4},   {"payloads_nasa", 12000.0f, 3},
        {"payloads_comm", 6000.0f, 5}, {"exercise", 1500.0f, 4}, {"lighting", 1500.0f, 2},
    };
    memset(c, 0, sizeof *c);
    memcpy(c->loads, def, sizeof def);
    c->nloads = sizeof def / sizeof def[0];
    c->batt_max_discharge_w = 30000.0f;
    c->soc_min_pct = 30.0f;
    c->heater_setpoint_c = 20.0f;
    c->heater_w_per_c = 800.0f;
    c->heater_max_w = 4000.0f;
}

void PL_Pwrth_Plan(const PL_PwrthConfig_t *c, const PL_PwrthSample_t *in, PL_PwrthPlan_t *o)
{
    uint32_t i;
    int prio;
    float supply = in->eclipse ? 0.0f : (in->array_w > 0.0f ? in->array_w : 0.0f);
    if (in->soc_pct > c->soc_min_pct) supply += c->batt_max_discharge_w;

    memset(o, 0, sizeof *o);
    o->available_w = supply;
    if (in->cabin_temp_c < c->heater_setpoint_c)
    {
        o->heater_w = (c->heater_setpoint_c - in->cabin_temp_c) * c->heater_w_per_c;
        if (o->heater_w > c->heater_max_w) o->heater_w = c->heater_max_w;
    }
    o->demand_w = o->heater_w;
    for (i = 0; i < c->nloads; i++) o->demand_w += c->loads[i].w;

    /* Shed highest priority number first; never shed priority 0. */
    for (prio = 255; prio >= 1 && o->demand_w > supply; prio--)
    {
        for (i = 0; i < c->nloads && o->demand_w > supply; i++)
        {
            if (c->loads[i].priority == (uint8_t)prio && !(o->shed_mask & (1u << i)))
            {
                o->shed_mask |= 1u << i;
                o->demand_w -= c->loads[i].w;
            }
        }
    }
    o->unmet = (o->demand_w > supply) ? 1u : 0u;
    o->margin_w = supply - o->demand_w;
}
