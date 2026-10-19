# SPDX-License-Identifier: MIT
"""Replay recorded or synthetic telemetry into Ping Lite flight software.

  host mode: pipes the CSV through tests/c/build/pl_replay (same C logic, no cFS)
  udp mode:  sends CCSDS command packets to a running cFS via CI_LAB (default UDP 1234)

Usage:
  python -m plsim.replay data.csv --mode host --replay-bin ../tests/c/build/pl_replay
  python -m plsim.replay data.csv --mode udp --host 127.0.0.1 --port 1234 --rate 200
"""
import argparse
import json
import socket
import subprocess
import time

from . import ccsds


def read_rows(path):
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):
                yield line.split(",")


def run_host(csv_text, replay_bin):
    out = subprocess.run([replay_bin], input=csv_text, capture_output=True, text=True, check=True).stdout
    return [json.loads(l) for l in out.splitlines() if l.strip()]


def run_udp(rows, host, port, rate_hz):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    n = 0
    for i, row in enumerate(rows):
        sock.sendto(ccsds.row_to_packet(row, seq=i), (host, port))
        n += 1
        if rate_hz:
            time.sleep(1.0 / rate_hz)
    return n


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("csv")
    ap.add_argument("--mode", choices=["host", "udp"], default="host")
    ap.add_argument("--replay-bin", default="../tests/c/build/pl_replay")
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=1234)
    ap.add_argument("--rate", type=float, default=200.0, help="packets per second (udp)")
    a = ap.parse_args(argv)
    if a.mode == "host":
        for ev in run_host(open(a.csv).read(), a.replay_bin):
            print(json.dumps(ev))
    else:
        print(f"sent {run_udp(list(read_rows(a.csv)), a.host, a.port, a.rate)} packets")


if __name__ == "__main__":
    main()
