/* SPDX-License-Identifier: MIT */
#include "ut.h"
#include "pl_pwrth_logic.h"
int main(void)
{
    PL_PwrthConfig_t c; PL_PwrthPlan_t o; uint32_t i;
    PL_PwrthSample_t sun = {0, 0, 70000.0f, 90.0f, 22.0f};
    PL_PwrthSample_t ecl = {0, 1, 70000.0f, 90.0f, 22.0f};
    PL_PwrthSample_t ecl_low = {0, 1, 0.0f, 20.0f, 22.0f};
    PL_PwrthSample_t cold = {0, 0, 70000.0f, 90.0f, 15.0f};
    PL_Pwrth_DefaultConfig(&c);

    PL_Pwrth_Plan(&c, &sun, &o);
    CHECK(o.shed_mask == 0); CHECK(o.margin_w > 0); CHECK(o.unmet == 0);

    PL_Pwrth_Plan(&c, &ecl, &o); /* 30 kW battery only: must shed, never priority 0 */
    CHECK(o.shed_mask != 0); CHECK(o.margin_w >= 0);
    for (i = 0; i < c.nloads; i++) if (c.loads[i].priority == 0) CHECK(!(o.shed_mask & (1u << i)));
    /* higher-number priorities shed before lower */
    for (i = 0; i < c.nloads; i++) if (c.loads[i].priority == 5) CHECK(o.shed_mask & (1u << i));

    PL_Pwrth_Plan(&c, &ecl_low, &o); /* no supply at all */
    CHECK(o.unmet == 1); CHECK(o.available_w == 0.0f);

    PL_Pwrth_Plan(&c, &cold, &o);
    CHECK_NEAR(o.heater_w, 4000.0, 0.1); /* 5 C * 800 W/C capped at 4 kW */
    UT_DONE("test_pwrth");
}
