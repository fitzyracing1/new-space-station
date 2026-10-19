# Ping Lite flight software v0.1 (prototype)

> **Status: research prototype. NOT certified, NOT flight software, NOT reviewed by NASA.**
> Nothing here has been through NPR 7150.2D Class A development, NASA-STD-8739.8B
> software assurance or IV&V. All limits and thresholds are **placeholder estimates**.
> All bundled telemetry is **synthetic** unless explicitly labelled otherwise.

Ping Lite is a commercial LEO station concept (see `../docs/concept-design-v0.2.md`,
sections 11B and 11C). This directory is the first cut of its flight software. It is
built on the NASA **Core Flight System (cFS)**, so that NASA's IV&V Program, the
ISS National Lab hosted-compute partners or any reviewer can build and run it
with standard, open tools.

## What it does

Four cFS applications (C, cFE v7 API). Each wraps a pure-C logic module that has
no cFS dependency, so the same code is unit-tested on the host and run inside cFS.

| App | Role | Output |
|---|---|---|
| `pl_eclss` | ECLSS monitor and fault detection: ppO2, ppCO2, total pressure, humidity and temperature limits with persistence; sensor plausibility (isolates a bad sensor instead of raising a false alarm); **leak-rate detection** from a least-squares fit of total pressure converted to kg/day (ideal gas, 560 m³ free volume from the v0.2 concept); caution, warning and emergency levels | Events and housekeeping only. **Shadow mode: no command outputs** |
| `pl_pwrth` | Power and thermal scheduler: supply from arrays (zero in eclipse) plus battery above a state-of-charge floor; priority-based load-shed plan that never sheds priority-0 (life-critical) loads; proportional heater request | Recommended plan (events and HK). Switches nothing |
| `pl_ops` | Uncrewed-operations supervisor: mode manager `CREWED`, `UNCREWED_NOMINAL`, `UNCREWED_AUTONOMOUS` (comm loss > 15 min), `SAFE_HOLD` (comm loss > 4 h, or ECLSS warning while uncrewed; latched until a ground acknowledgement during AOS). Reads ECLSS severity from `pl_eclss` housekeeping | Advisory mode (events and HK) |
| `pl_tlm` | Telemetry logger: ring buffer of Ping Lite housekeeping arrivals; **single-event-upset (SEU) counting** from EDAC corrected and uncorrected counters, handling 32-bit wrap and processor resets; SEU rate per day | Events and HK |

Message IDs and payloads are defined in `apps/pl_inc/pl_msg.h`. Inputs arrive as
CCSDS command packets (`0x18F0`–`0x18F3`) through cFS `CI_LAB` (UDP 1234). The apps
publish only their own housekeeping (`0x08F0`–`0x08F3`). `scripts/check_shadow_mode.py`
enforces this statically in CI.

## Layout

```
software/
  cFS/                 NASA cFS bundle, git submodule pinned to v7.0.1 (Apache 2.0, unmodified)
  apps/pl_*/           Ping Lite cFS apps (MIT); *_logic.c = testable core
  apps/pl_inc/         shared message definitions
  mission/             targets.cmake, startup script and install hooks (overlaid at build time)
  scripts/             build_cfs.sh, run_shadow_scenario.sh, check_shadow_mode.py
  tests/c/             host unit tests and pl_replay (CSV -> logic -> JSON events)
  sim/plsim/           Python harness: synthetic generator, replay (host or UDP), ISSLive adapter
  sim/data/            synthetic_shadow_scenario.csv  (SYNTHETIC)
  docs/SRS-and-SAP.md  requirements and assurance plan skeleton (NPR 7150.2D / NASA-STD-8739.8B)
```

## Build and run (Linux)

```bash
git clone --recurse-submodules https://github.com/fitzyracing1/new-space-station
cd new-space-station/software
make -C tests/c test                         # C unit tests
(cd sim && python3 -m pytest -v tests)       # harness tests + host shadow scenario
scripts/build_cfs.sh                         # builds cFS v7.0.1 + Ping Lite apps (needs cmake, gcc)
scripts/run_shadow_scenario.sh               # boots core-cpu1, replays synthetic telemetry, checks events
```

Replay your own recorded telemetry (CSV format documented in `tests/c/pl_replay.c`):

```bash
cd sim
python3 -m plsim.replay my.csv --mode host --replay-bin ../tests/c/build/pl_replay   # no cFS
python3 -m plsim.replay my.csv --mode udp --rate 20                                  # into running cFS
```

**Optional live public ISS telemetry:** `pip install lightstreamer-client-lib`, then
`python3 -m plsim.isslive 120 > live.csv`. This uses the public ISSLive feed relayed by
Lightstreamer (`https://push.lightstreamer.com`, adapter `ISSLIVE`) and the US Lab ppO2,
ppCO2, cabin pressure and temperature items, as identified by the ISS-Mimic project. The
public feed has no humidity item. **Feed units are not verified** (the adapter assumes
mmHg), and it is read-only. It is not run in CI.

## Shadow-mode scenario (CI)

Synthetic timeline: crew aboard, then a CO₂ excursion, crew departure, a 25-minute comm
loss, an RH sensor failure, a ~2 kg/day leak, a processor reset, an uncorrected SEU and a
ground acknowledgement. Expected results (checked in CI, both host and cFS):
`CREWED→UNCREWED_NOMINAL`, `→UNCREWED_AUTONOMOUS` and back on AOS, ppCO₂-high,
RH-sensor and leak faults, `→SAFE_HOLD`, one uncorrected SEU and one reset.

Known limitation shown by the scenario: leak detection is limited by pressure-sensor noise.
With the placeholder window (64 × 5 min, ≥3 h span) a 2 kg/day leak is declared about
1.5 h after onset. That is far too slow against the v0.2 depressurisation times for large
holes, so this method is for slow leaks only. Fast leaks need a separate rate trigger and
acoustic leak location (v0.2 R6). This is open item **OI-3**.

## Licences and attribution

* Ping Lite code: **MIT** (`LICENSE`).
* NASA cFS: **Apache License 2.0**, © U.S. Government (NASA), used unmodified as a submodule (`cFS/LICENSE`).
* See `NOTICE`. Use of cFS does not imply NASA endorsement.
