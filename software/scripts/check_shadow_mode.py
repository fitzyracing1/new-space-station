#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Static check for SR-SW-001 (shadow mode): Ping Lite apps may only transmit
their own housekeeping telemetry, and may only reference *_CMD_MID to subscribe."""
import pathlib, re, sys

apps = pathlib.Path(__file__).resolve().parents[1] / "apps"
bad = []
for src in sorted(apps.glob("pl_*/fsw/src/*_app.c")):
    text = src.read_text()
    for m in re.finditer(r"CFE_SB_TransmitMsg\(([^;]*)\);", text):
        if "Hk.Hdr" not in m.group(1):
            bad.append(f"{src.name}: transmits something other than its HK: {m.group(0)}")
    for line in text.splitlines():
        if "_CMD_MID" in line and "CFE_SB_Subscribe" not in line:
            bad.append(f"{src.name}: non-subscribe use of a command MID: {line.strip()}")
    if "CFE_SB_TransmitMsg" not in text:
        bad.append(f"{src.name}: no telemetry output found (unexpected)")
print("\n".join(bad) if bad else "shadow-mode static check: PASS (apps transmit HK telemetry only)")
sys.exit(1 if bad else 0)
