/* SPDX-License-Identifier: MIT
 * Host replay driver: feeds CSV telemetry through the SAME logic modules the
 * cFS apps use and prints one JSON object per event. Used by the Python
 * harness (software/sim) and the CI shadow-mode scenario. No cFS needed.
 *
 * Input lines (header lines starting with '#' are ignored):
 *   E,t,valid_mask,ppo2_kpa,ppco2_kpa,p_kpa,rh_pct,temp_c
 *   P,t,eclipse,array_w,soc_pct,cabin_temp_c
 *   O,t,crew_aboard,aos            (ECLSS level is taken from the last E line)
 *   A,t                            (ground ack of SAFE_HOLD)
 *   T,t,edac_corr,edac_uncorr,boot_count
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl_eclss_logic.h"
#include "pl_pwrth_logic.h"
#include "pl_ops_logic.h"
#include "pl_tlm_logic.h"

int main(void)
{
    char line[512];
    PL_EclssConfig_t ec; PL_EclssState_t es; PL_PwrthConfig_t pc; PL_PwrthPlan_t plan;
    PL_OpsConfig_t oc; PL_OpsState_t os; PL_TlmState_t ts;
    uint32_t last_shed = 0, last_level = 0, last_aos = 1, last_crew = 1;
    int i;
    PL_Eclss_DefaultConfig(&ec); PL_Eclss_Init(&es, &ec);
    PL_Pwrth_DefaultConfig(&pc); PL_Ops_DefaultConfig(&oc); PL_Ops_Init(&os, &oc); PL_Tlm_Init(&ts);

    while (fgets(line, sizeof line, stdin))
    {
        char k = line[0];
        if (k == '#' || k == '\n' || k == '\r') continue;
        if (k == 'E')
        {
            PL_EclssSample_t x; const PL_EclssStatus_t *st;
            if (sscanf(line, "E,%u,%u,%f,%f,%f,%f,%f", &x.t_s, &x.valid_mask, &x.ppo2_kpa, &x.ppco2_kpa,
                       &x.p_kpa, &x.rh_pct, &x.temp_c) != 7) continue;
            st = PL_Eclss_Step(&es, &x);
            for (i = 0; i < 25; i++)
            {
                uint32_t b = 1u << i;
                if (st->new_faults & b)
                    printf("{\"t\":%u,\"app\":\"eclss\",\"event\":\"FAULT_SET\",\"bit\":%d,\"leak_kg_day\":%.3f}\n",
                           x.t_s, i, st->leak_kg_per_day);
                if (st->cleared & b)
                    printf("{\"t\":%u,\"app\":\"eclss\",\"event\":\"FAULT_CLEAR\",\"bit\":%d}\n", x.t_s, i);
            }
            if (st->level != last_level)
                printf("{\"t\":%u,\"app\":\"eclss\",\"event\":\"LEVEL\",\"level\":%u}\n", x.t_s, st->level);
            last_level = st->level;
        }
        else if (k == 'P')
        {
            PL_PwrthSample_t x;
            if (sscanf(line, "P,%u,%u,%f,%f,%f", &x.t_s, &x.eclipse, &x.array_w, &x.soc_pct, &x.cabin_temp_c) != 5)
                continue;
            PL_Pwrth_Plan(&pc, &x, &plan);
            if (plan.shed_mask != last_shed || plan.unmet)
                printf("{\"t\":%u,\"app\":\"pwrth\",\"event\":\"PLAN\",\"shed_mask\":%u,\"heater_w\":%.0f,"
                       "\"margin_w\":%.0f,\"unmet\":%u}\n",
                       x.t_s, plan.shed_mask, plan.heater_w, plan.margin_w, plan.unmet);
            last_shed = plan.shed_mask;
        }
        else if (k == 'O' || k == 'A')
        {
            PL_OpsSample_t x = {0, 0, 0, 0};
            PL_Mode_t prev = os.mode;
            int ch;
            if (k == 'O')
            {
                if (sscanf(line, "O,%u,%u,%u", &x.t_s, &x.crew_aboard, &x.aos) != 3) continue;
                x.eclss_level = last_level; last_aos = x.aos; last_crew = x.crew_aboard;
                ch = PL_Ops_Step(&os, &x);
            }
            else
            {
                if (sscanf(line, "A,%u", &x.t_s) != 1) continue;
                ch = PL_Ops_GroundAck(&os, last_crew, last_aos);
            }
            if (ch)
                printf("{\"t\":%u,\"app\":\"ops\",\"event\":\"MODE\",\"from\":\"%s\",\"to\":\"%s\"}\n", x.t_s,
                       PL_Ops_ModeName(prev), PL_Ops_ModeName(os.mode));
        }
        else if (k == 'T')
        {
            PL_TlmSample_t x; uint32_t du;
            if (sscanf(line, "T,%u,%u,%u,%u", &x.t_s, &x.edac_corrected, &x.edac_uncorrected, &x.boot_count) != 4)
                continue;
            du = PL_Tlm_SeuSample(&ts, &x);
            if (du)
                printf("{\"t\":%u,\"app\":\"tlm\",\"event\":\"SEU_UNCORRECTED\",\"count\":%u}\n", x.t_s, du);
        }
    }
    printf("{\"app\":\"summary\",\"seu_corrected\":%llu,\"seu_uncorrected\":%llu,\"resets\":%u,"
           "\"seu_per_day\":%.2f,\"final_mode\":\"%s\",\"eclss_faults\":%u,\"command_outputs\":0}\n",
           (unsigned long long)ts.seu_corrected_total, (unsigned long long)ts.seu_uncorrected_total, ts.resets_seen,
           PL_Tlm_SeuRatePerDay(&ts), PL_Ops_ModeName(os.mode), es.st.fault_mask);
    return 0;
}
