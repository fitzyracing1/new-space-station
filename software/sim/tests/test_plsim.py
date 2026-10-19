# SPDX-License-Identifier: MIT
import os
import struct
import subprocess
import pathlib

import pytest

from plsim import ccsds, synthetic, replay, isslive

SW = pathlib.Path(__file__).resolve().parents[2]
BIN = SW / "tests" / "c" / "build" / "pl_replay"


@pytest.fixture(scope="session")
def replay_bin():
    if not BIN.exists():
        subprocess.run(["make", "-s", "-C", str(SW / "tests" / "c"), "all"], check=True)
    return str(BIN)


def test_ccsds_header_and_payload():
    pkt = ccsds.row_to_packet(["E", "10", "31", "21.3", "0.3", "101.3", "45", "22"])
    mid, seq, length = struct.unpack(">HHH", pkt[:6])
    assert mid == 0x18F0 and seq & 0xC000 == 0xC000
    assert length == len(pkt) - 7 and len(pkt) == 8 + 28
    assert pkt[6] == ccsds.SAMPLE_CC
    t, valid, ppo2 = struct.unpack("<IIf", pkt[8:20])
    assert (t, valid) == (10, 31) and abs(ppo2 - 21.3) < 1e-5


def test_ack_packet():
    pkt = ccsds.row_to_packet(["A", "5"])
    assert len(pkt) == 8 and pkt[6] == ccsds.ACK_CC


def test_synthetic_is_labelled():
    assert synthetic.generate(600).startswith("# SYNTHETIC DATA")


def test_isslive_row_conversion():
    row = isslive.to_row(3, {"ppo2": 160.0, "ppco2": 2.0, "p": 760.0, "temp": 22.0})
    f = row.split(",")
    assert f[0] == "E" and int(f[2]) == 0b10111  # RH invalid
    assert abs(float(f[5]) - 101.325) < 0.01


def test_shadow_scenario(replay_bin):
    ev = replay.run_host(synthetic.generate(), replay_bin)
    summary = ev[-1]
    modes = [(e["from"], e["to"]) for e in ev if e.get("event") == "MODE"]
    bits = {e["bit"] for e in ev if e.get("event") == "FAULT_SET"}
    assert ("CREWED", "UNCREWED_NOMINAL") in modes
    assert ("UNCREWED_NOMINAL", "UNCREWED_AUTONOMOUS") in modes
    assert any(to == "SAFE_HOLD" for _, to in modes)
    assert ("SAFE_HOLD", "UNCREWED_NOMINAL") in modes  # ground ack accepted
    assert summary["final_mode"] == "SAFE_HOLD"        # leak persists -> re-latched
    assert 3 in bits          # ppCO2 high (bit 2*1+1)
    assert 16 + 3 in bits     # RH sensor fault
    assert 24 in bits         # leak
    assert summary["seu_uncorrected"] == 1 and summary["resets"] == 1
    assert summary["command_outputs"] == 0  # logic API has no command outputs
