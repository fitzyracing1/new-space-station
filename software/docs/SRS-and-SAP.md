# Ping Lite flight software: requirements and assurance plan (SKELETON, v0.1)

> **Draft skeleton for planning only.** v0.1 software is a **prototype and is not
> certified**. This document shows how Ping Lite intends to meet NPR 7150.2D (Class A)
> and NASA-STD-8739.8B. It does not claim that any requirement is met today. CLDC
> applicability: SOW §2.10.3 (Software Management Plan, DRD CLDC-107), §2.10.5
> (support NASA IV&V), §2.11.2 (software safety and assurance plan meeting the intent of
> NASA-STD-8739.8B, plus third-party IV&V), §2.15.4 (payload software testing), and
> J-11 (pre-CDR IV&V and software assurance summary). NPR 7150.2D is listed in
> J-03. Whether it is a compliance requirement or guidance is open (OI-1).

## 1. Classification (NPR 7150.2D Appendix D, SWE-020)

| CSCI | Function | Proposed class | Safety-critical (NASA-STD-8739.8B) | Rationale |
|---|---|---|---|---|
| PL_ECLSS (flight version) | Atmosphere monitoring, FDIR | **A** | Yes | "required to sustain a safe, habitable environment" |
| PL_PWRTH (flight version) | Power and thermal control | **A** | Yes | Controls electrical power to life-critical loads |
| PL_OPS (flight version) | Mode management, safing | **A** | Yes | Vehicle commanding, crew-off safing |
| PL_TLM | Logging, SEU accounting | A or C (TBD) | TBD | Depends on hazard-report use of its data |
| **v0.1 prototypes (this repo)** | Shadow mode, synthetic data | **Not classified for flight; development prototype** | — | No command outputs; not used for any operational decision |

Classification must be agreed by the software engineering and software assurance technical authorities (SWE-020 note). Status: **not done**.

## 2. Software requirements (initial set, to be baselined)

| ID | Requirement | Verification | v0.1 status |
|---|---|---|---|
| SR-SW-001 | In shadow configuration, PL apps shall publish only their own housekeeping and events and shall issue no commands | Inspection, static check (`check_shadow_mode.py`), test | Implemented; CI static check |
| SR-SW-010 | PL_ECLSS shall declare low/high limit violations for ppO2, ppCO2, P, RH and T after N consecutive samples (N configurable) and clear them when back in limits | Unit test | Implemented (limits are placeholders) |
| SR-SW-011 | PL_ECLSS shall classify implausible or invalid sensor data as a sensor fault and shall not raise or clear process alarms from it | Unit test | Implemented |
| SR-SW-012 | PL_ECLSS shall estimate the leak rate (kg/day) and declare a leak above threshold (default 0.34 kg/day ≈ 0.05%/day, v0.2 SR-21) | Unit test, scenario | Implemented, slow leaks only (OI-3) |
| SR-SW-020 | PL_PWRTH shall never shed priority-0 loads and shall shed in descending priority number until demand ≤ supply | Unit test | Implemented |
| SR-SW-030 | PL_OPS shall enter UNCREWED_AUTONOMOUS after LOS ≥ t_auto and SAFE_HOLD after LOS ≥ t_safe or an ECLSS warning while uncrewed; SAFE_HOLD shall persist until a ground acknowledgement with AOS | Unit test, scenario | Implemented (thresholds placeholders) |
| SR-SW-040 | PL_TLM shall count corrected and uncorrected SEUs across counter wrap and processor reset | Unit test | Implemented |
| SR-SW-1xx | Timing, CPU and memory margins; FDIR response times; crew override (NASA-STD-8719.29) | Analysis, test | **Not started** |
| SR-SW-2xx | Cybersecurity and command authentication | Analysis, test | **Not started** |

Limits must be derived from NASA-STD-3001 Vol. 2 and CLDP-REQ-1130 (not yet obtained; OI-2).

## 3. Development process (NPR 7150.2D mapping, to be completed)

* **Requirements mapping matrix** (Appendix C) for Class A: to do. Planned tailoring and rationale go to the NASA technical authority.
* **Life cycle:** iterative, with formal SRR, PDR and CDR software reviews aligned to CLDC J-11 milestones.
* **Configuration management:** git with signed tags; cFS pinned by submodule hash (`v7.0.1`, commit 088b2fa); flight builds reproducible from a clean checkout.
* **Coding standard:** C99, `-Wall -Wextra -Werror -pedantic` (in place). Planned: MISRA C:2012 subset with deviations recorded, and static analysis (cppcheck, plus a sound analyser TBD).
* **Testing:** unit (in place; 92–100% line coverage of the logic modules), integration on cFS (scripted scenario in place), hardware-in-the-loop (not started), and an **end-to-end test before every flight** (Starliner OFT lesson; concept-design-v0.2 §11C).
* **Structural coverage:** MC/DC coverage of safety-critical code (NPR 7150.2D code-coverage requirement for safety-critical software; exact SWE number to be confirmed) is **not measured yet**.
* **Use of cFS:** open-source bundle, which NASA states "is not a flight distribution… should not be flown as is". Plan: evaluate the Class A-certifiable cFS artefacts (NTRS 20210000650) and/or the Government-use distribution through a Software User Agreement.

## 4. Software assurance and safety (NASA-STD-8739.8B)

* **Independent SA organisation:** to be established (independent of developers).
* **Software safety analysis:** software hazard analysis tied to station hazard reports (CLDP-STD-3102); identification of safety-critical software and controls.
* **SA activities:** requirements, design and code analysis; test witnessing; problem-report trending; audits of test scripts and environments.
* **IV&V:** SOW §2.11.2 third-party IV&V plan. Options: Government IV&V via a J-14 GTA (request due ≈31 Oct 2026) or a reimbursable Space Act Agreement with the NASA IV&V Program; or a commercial IV&V provider. See concept-design-v0.2 §11C.

## 5. Open items

| ID | Item |
|---|---|
| OI-1 | NPR 7150.2D: compliance requirement or guidance under CLDC (NASA Q&A) |
| OI-2 | Obtain CLDP-REQ-1130 and NASA-STD-3001-derived limits; replace placeholders |
| OI-3 | Fast-leak detection (rate trigger, acoustic location); leak-detection latency requirement |
| OI-4 | Target processor and RTOS (v0.1 runs on native Linux only); radiation environment for SEU rates |
| OI-5 | Real recorded telemetry: NASA data agreement for detailed ISS ECLSS data (public feed is a small subset) |
| OI-6 | Hosted shadow-mode demo route (HPE SBC-2, Axiom, Voyager) and host safety review (concept-design-v0.2 §11B) |
