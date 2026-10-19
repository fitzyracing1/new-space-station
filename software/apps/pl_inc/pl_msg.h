/*
 * Ping Lite flight software v0.1 (prototype, NOT certified)
 * SPDX-License-Identifier: MIT
 *
 * Message IDs and payload layouts shared by the Ping Lite cFS apps and
 * the Python harness (software/sim/plsim/ccsds.py must match this file).
 * All payloads are little-endian, 4-byte aligned, no padding.
 */
#ifndef PL_MSG_H
#define PL_MSG_H

#include <stdint.h>

/* Command (input) message IDs: the harness sends these through CI_LAB. */
#define PL_ECLSS_CMD_MID 0x18F0
#define PL_PWRTH_CMD_MID 0x18F1
#define PL_OPS_CMD_MID   0x18F2
#define PL_TLM_CMD_MID   0x18F3

/* Telemetry (output) message IDs. The apps only ever publish these. */
#define PL_ECLSS_HK_MID 0x08F0
#define PL_PWRTH_HK_MID 0x08F1
#define PL_OPS_HK_MID   0x08F2
#define PL_TLM_HK_MID   0x08F3

/* Function codes (same for every app) */
#define PL_NOOP_CC   0
#define PL_SAMPLE_CC 1 /* one sensor/telemetry sample (payload below) */
#define PL_ACK_CC    2 /* ops only: ground acknowledges SAFE_HOLD     */

typedef struct
{
    uint32_t t_s;        /* sample time, seconds since scenario epoch */
    uint32_t valid_mask; /* bit i set = field i valid                  */
    float    ppo2_kpa;
    float    ppco2_kpa;
    float    p_kpa;
    float    rh_pct;
    float    temp_c;
} PL_EclssSample_t;

typedef struct
{
    uint32_t t_s;
    uint32_t eclipse; /* 1 = in eclipse */
    float    array_w;
    float    soc_pct;
    float    cabin_temp_c;
} PL_PwrthSample_t;

typedef struct
{
    uint32_t t_s;
    uint32_t crew_aboard;  /* 0/1 */
    uint32_t aos;          /* 1 = ground link available */
    uint32_t eclss_level;  /* 0 none, 1 caution, 2 warning, 3 emergency */
} PL_OpsSample_t;

typedef struct
{
    uint32_t t_s;
    uint32_t edac_corrected;   /* raw hardware counter (may wrap or reset) */
    uint32_t edac_uncorrected; /* raw hardware counter                     */
    uint32_t boot_count;       /* increments on processor reset            */
} PL_TlmSample_t;

#endif
