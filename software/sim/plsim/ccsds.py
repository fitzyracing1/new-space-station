# SPDX-License-Identifier: MIT
"""CCSDS command packets for the Ping Lite cFS apps (must match apps/pl_inc/pl_msg.h).

cFE v7 default (non-EDS) command header = 6-byte CCSDS primary header
+ 2-byte secondary header (function code, checksum). Payloads are little-endian.
"""
import struct

MID = {"E": 0x18F0, "P": 0x18F1, "O": 0x18F2, "A": 0x18F2, "T": 0x18F3}
NOOP_CC, SAMPLE_CC, ACK_CC = 0, 1, 2

# kind -> (struct format, number of fields after kind)
PAYLOAD = {
    "E": "<II5f",   # t, valid_mask, ppo2, ppco2, p, rh, temp
    "P": "<II3f",   # t, eclipse, array_w, soc_pct, cabin_temp_c
    "O": "<IIII",   # t, crew_aboard, aos, eclss_level (filled by app from ECLSS HK)
    "T": "<IIII",   # t, edac_corr, edac_uncorr, boot_count
}


def _checksum(pkt: bytes) -> int:
    c = 0xFF
    for b in pkt:
        c ^= b
    return c


def command(mid: int, cc: int, payload: bytes = b"", seq: int = 0) -> bytes:
    total = 8 + len(payload)
    hdr = struct.pack(">HHH", mid, 0xC000 | (seq & 0x3FFF), total - 7)
    sec = bytes([cc & 0x7F, 0])
    pkt = bytearray(hdr + sec + payload)
    pkt[7] = _checksum(pkt)
    return bytes(pkt)


def row_to_packet(row, seq=0) -> bytes:
    """row: list of strings from the CSV format documented in tests/c/pl_replay.c."""
    kind = row[0]
    if kind == "A":
        return command(MID["A"], ACK_CC, b"", seq)
    vals = row[1:]
    fmt = PAYLOAD[kind]
    if kind == "O":
        vals = vals[:3] + ["0"]
    conv = []
    for code, v in zip(fmt[1:].replace("5f", "fffff").replace("3f", "fff"), vals):
        conv.append(int(v) if code == "I" else float(v))
    return command(MID[kind], SAMPLE_CC, struct.pack(fmt, *conv), seq)
