/* SPDX-License-Identifier: MIT */
#include "ut.h"
#include "pl_ops_logic.h"
int main(void)
{
    PL_OpsConfig_t c; PL_OpsState_t s; PL_OpsSample_t x = {0, 1, 1, 0}; uint32_t t;
    PL_Ops_DefaultConfig(&c); PL_Ops_Init(&s, &c);
    CHECK(PL_Ops_Step(&s, &x) == 0); CHECK(s.mode == PL_MODE_CREWED);
    x.crew_aboard = 0; x.t_s = 10; CHECK(PL_Ops_Step(&s, &x) == 1); CHECK(s.mode == PL_MODE_UNCREWED_NOMINAL);
    /* short LOS: stays nominal */
    x.aos = 0; for (t = 20; t < 20 + 14 * 60; t += 60) { x.t_s = t; PL_Ops_Step(&s, &x); }
    CHECK(s.mode == PL_MODE_UNCREWED_NOMINAL);
    /* LOS > 15 min -> autonomous */
    for (; t < 20 + 30 * 60; t += 60) { x.t_s = t; PL_Ops_Step(&s, &x); }
    CHECK(s.mode == PL_MODE_UNCREWED_AUTONOMOUS);
    /* AOS -> back to nominal */
    x.aos = 1; x.t_s = t; PL_Ops_Step(&s, &x); CHECK(s.mode == PL_MODE_UNCREWED_NOMINAL);
    /* LOS > 4 h -> safe hold, latched through AOS until ack */
    x.aos = 0; for (t += 60; t < 6 * 3600; t += 300) { x.t_s = t; PL_Ops_Step(&s, &x); }
    CHECK(s.mode == PL_MODE_SAFE_HOLD);
    x.aos = 1; x.t_s = t; PL_Ops_Step(&s, &x); CHECK(s.mode == PL_MODE_SAFE_HOLD);
    CHECK(PL_Ops_GroundAck(&s, 0, 0) == 0);
    CHECK(PL_Ops_GroundAck(&s, 0, 1) == 1); CHECK(s.mode == PL_MODE_UNCREWED_NOMINAL);
    /* ECLSS warning while uncrewed -> safe hold even with AOS */
    x.eclss_level = 2; x.t_s += 10; PL_Ops_Step(&s, &x); CHECK(s.mode == PL_MODE_SAFE_HOLD);
    /* crew boards -> CREWED overrides */
    x.crew_aboard = 1; x.t_s += 10; PL_Ops_Step(&s, &x); CHECK(s.mode == PL_MODE_CREWED);
    CHECK(PL_Ops_ModeName(PL_MODE_SAFE_HOLD)[0] == 'S');
    UT_DONE("test_ops");
}
