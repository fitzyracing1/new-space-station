/* SPDX-License-Identifier: MIT
 * Uncrewed-operations supervisor: mode manager with comm-loss handling.
 * v0.1 reports the mode it WOULD select; it commands nothing.
 */
#ifndef PL_OPS_LOGIC_H
#define PL_OPS_LOGIC_H
#include <stdint.h>
#include "pl_msg.h"

typedef enum
{
    PL_MODE_CREWED = 0,
    PL_MODE_UNCREWED_NOMINAL,
    PL_MODE_UNCREWED_AUTONOMOUS, /* LOS longer than t_auto: run stored timeline */
    PL_MODE_SAFE_HOLD            /* LOS longer than t_safe, or ECLSS warning while uncrewed */
} PL_Mode_t;

typedef struct
{
    uint32_t t_auto_s; /* LOS duration before autonomous ops */
    uint32_t t_safe_s; /* LOS duration before safe hold      */
} PL_OpsConfig_t;

typedef struct
{
    PL_OpsConfig_t cfg;
    PL_Mode_t mode;
    uint32_t los_start_s;
    uint32_t in_los;
    uint32_t transitions;
    uint32_t initialized;
} PL_OpsState_t;

void PL_Ops_DefaultConfig(PL_OpsConfig_t *c);
void PL_Ops_Init(PL_OpsState_t *s, const PL_OpsConfig_t *c);
/* Returns 1 if the mode changed. */
int PL_Ops_Step(PL_OpsState_t *s, const PL_OpsSample_t *in);
/* Ground acknowledgement: leaves SAFE_HOLD only when AOS. Returns 1 if changed. */
int PL_Ops_GroundAck(PL_OpsState_t *s, uint32_t crew_aboard, uint32_t aos);
const char *PL_Ops_ModeName(PL_Mode_t m);
#endif
