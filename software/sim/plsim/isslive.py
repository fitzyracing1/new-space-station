# SPDX-License-Identifier: MIT
"""OPTIONAL adapter: public ISS telemetry (ISSLive via Lightstreamer) -> Ping Lite 'E' rows.

Feasibility: the public feed is served by Lightstreamer at https://push.lightstreamer.com,
adapter set "ISSLIVE", schema TimeStamp/Value/... (Lightstreamer ISSLive example client,
github.com/Lightstreamer/Lightstreamer-example-ISSLive-client-javascript). Item IDs below
come from the ISS-Mimic project's telemetry table (github.com/ISS-Mimic/Mimic,
Pi/database_initialize.py). It is a small public subset; it has no humidity item we use,
so RH is marked invalid. UNITS ARE NOT VERIFIED: pressures are assumed to be mmHg (TBC).
This is read-only monitoring of public data; nothing is ever sent to the ISS.

Requires: pip install lightstreamer-client-lib   (not needed for CI)
"""
import sys
import time

SERVER, ADAPTER = "https://push.lightstreamer.com", "ISSLIVE"
ITEMS = {
    "USLAB000053": "ppo2",   # LAB_MCA_ppO2
    "USLAB000055": "ppco2",  # LAB_MCA_ppCO2
    "USLAB000058": "p",      # LAB_PCA_Cabin_Pressure
    "USLAB000059": "temp",   # LAB1P6_CCAA_In_T1
}
MMHG_TO_KPA = 0.133322


def to_row(t_s, vals, pressure_unit="mmHg"):
    """Builds an 'E' CSV row. vals: dict ppo2/ppco2/p/temp in feed units."""
    k = MMHG_TO_KPA if pressure_unit == "mmHg" else 1.0
    valid = 0
    out = []
    for i, key in enumerate(["ppo2", "ppco2", "p", "rh", "temp"]):
        v = vals.get(key)
        if v is None:
            out.append("0")
            continue
        valid |= 1 << i
        out.append(f"{float(v) * (k if key in ('ppo2', 'ppco2', 'p') else 1.0):.4f}")
    return f"E,{int(t_s)},{valid}," + ",".join(out)


def stream(seconds=60, out=sys.stdout):  # pragma: no cover - needs network + optional lib
    from lightstreamer.client import LightstreamerClient, Subscription, SubscriptionListener

    vals, t0 = {}, time.time()

    class L(SubscriptionListener):
        def onItemUpdate(self, upd):
            vals[ITEMS[upd.getItemName()]] = upd.getValue("Value")
            out.write(to_row(time.time() - t0, vals) + "\n")
            out.flush()

    client = LightstreamerClient(SERVER, ADAPTER)
    sub = Subscription("MERGE", list(ITEMS), ["TimeStamp", "Value"])
    sub.addListener(L())
    client.connect()
    client.subscribe(sub)
    time.sleep(seconds)
    client.disconnect()


if __name__ == "__main__":  # pragma: no cover
    out = sys.stdout
    out.write("# LIVE PUBLIC ISS TELEMETRY via ISSLive/Lightstreamer (units TBC)\n")
    stream(int(sys.argv[1]) if len(sys.argv) > 1 else 60, out)
