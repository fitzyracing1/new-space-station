#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Scripted shadow-mode scenario against the real cFS build:
# boots core-cpu1, replays SYNTHETIC telemetry through CI_LAB (UDP 1234),
# then checks the Ping Lite events in the console log.
set -uo pipefail
SW="$(cd "$(dirname "$0")/.." && pwd)"
EXE="${PL_CFS_WORK:-$SW/build/cfs}/build-native_std/exe/cpu1"
LOG="$SW/build/shadow_scenario.log"
CSV="$SW/build/synthetic_shadow_scenario.csv"
mkdir -p "$SW/build"
(cd "$SW/sim" && python3 -m plsim.synthetic --step 120 > "$CSV")
cd "$EXE"
./core-cpu1 > "$LOG" 2>&1 &
PID=$!
for i in $(seq 1 30); do grep -q "OPERATIONAL" "$LOG" && break; sleep 1; done
sleep 2
# CI_LAB ingests at most 11 packets per 500 ms by default, so pace at 20/s
(cd "$SW/sim" && python3 -m plsim.replay "$CSV" --mode udp --rate 20)
sleep 5
kill -INT $PID 2>/dev/null; sleep 2; kill -9 $PID 2>/dev/null; wait $PID 2>/dev/null
fail=0
expect() { if grep -q -- "$1" "$LOG"; then echo "PASS: $1"; else echo "FAIL: missing '$1'"; fail=1; fi; }
reject() { if grep -q -- "$1" "$LOG"; then echo "FAIL: found '$1'"; fail=1; else echo "PASS: no '$1'"; fi; }
for a in PL_ECLSS PL_PWRTH PL_OPS PL_TLM; do expect "$a: initialized v0.1"; done
expect "SHADOW MODE (no command outputs)"
expect "PL_ECLSS: FAULT_SET bit=3"
expect "PL_ECLSS: FAULT_SET bit=19"
expect "PL_ECLSS: FAULT_SET bit=24"
expect "PL_OPS: MODE CREWED -> UNCREWED_NOMINAL"
expect "PL_OPS: MODE UNCREWED_NOMINAL -> UNCREWED_AUTONOMOUS"
expect "-> SAFE_HOLD"
expect "PL_PWRTH: PLAN shed="
expect "PL_TLM: SEU_UNCORRECTED +1"
expect "PL_TLM: processor reset seen"
reject "bad length"
reject "bad fcn code"
echo "log: $LOG"
exit $fail
