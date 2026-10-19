---
title: "Ping Lite trade study: could the ISS be saved, and how would it fit with Ping Lite?"
subtitle: "ISS end-of-life options rated on feasibility, cost, schedule and risk, and what they mean for the 2030–2032 gap and the CLDC proposal"
date: "19 October 2026, draft 2"
---

# How to read this document

* **Status:** pre-Phase-A trade study. **Every number is either quoted from a named source (Appendix A) or marked as an estimate by this study.** No ISS-specific engineering data were available to us; ratings are judgements.
* **Baseline:** Ping Lite v0.2: one integrated cylinder (7.2 m OD, ≈560 m³, 6 crew continuous, ≈55 kW EOL, ≈93 t) launched on one Starship; planning launch **Q4 2032**, compressed option Q4 2030 (low likelihood); 420 km / 51.6° (concept-design-v0.2.md §§1, 13).
* **Requirement driver:** CLDC RFP 80JSC027R0004 assumes ISS deorbit at the end of CY2030 (SOW §1.0, p. 6); proposals due 8 Dec 2026.

## Bottom line

1. **The ISS cannot realistically be "saved" as an orbiting asset after 2030 by anyone other than NASA and its partners.** NASA's own 2024 analysis rejected boosting, commercial handover, disassembly and module reuse, and found deorbit by the US Deorbit Vehicle (USDV) "the safest and only viable method" [S1].
2. **The only lever that matters is a NASA decision to extend ISS operations.** GAO reports NASA will assess in **2027** whether to launch the USDV in 2029 or extend operations [S3]. NASA has said the USOS primary structure has "high confidence" for beyond 2030 and the USDV "can dwell on the ground" during an extension [S1].
3. **Ping Lite cannot dock with or take over the ISS.** On the current plan the last crew leaves in mid-2030 and reentry is notionally January 2031 [S4], about two years before Ping Lite's Q4 2032 launch. No bridge or docking mission closes that gap unless NASA extends the ISS.
4. **Recommendation:** do **not** put an ISS rescue, docking or handover in the CLDC proposal as a baseline or priced option. Add a short, unpriced "ISS-extension compatibility" paragraph and an optional ISS-hardware-reuse offer. Spend the effort on the schedule instead (Section 7).

# 1. NASA's current deorbit plan and the USDV

| Item | Fact | Source |
|---|---|---|
| Decision | Controlled deorbit with a US-developed deorbit vehicle, into a remote ocean area, is "the best option" | NASA ISS Deorbit Analysis Summary, 2024 [S1] |
| USDV contractor | SpaceX, selected June 2024; single award, total potential value \$843 M; NASA buys the launch separately and owns and operates the vehicle | NASA release [S2] |
| Contract status | Ceiling \$843.3 M; \$425.6 M obligated; \$91.7 M outlaid; period of performance to 31 Mar 2031 (record modified 9 Apr 2026) | USAspending-derived record [S5] |
| Vehicle | Dragon-based, with an enlarged trunk carrying extra propellant and Draco thrusters | ESA ISS community briefing, Oct 2025 [S4]; GAO [S3] |
| Delivery date in RFP | Desired 1 Aug 2028, required 1 May 2029 | NASA USDV vignette [S6] |
| Timeline | Orbit lowering from mid-2028 (R−2.5 yr); USDV launch mid-2029 (R−1.5 yr); last crew arrives mid-2029 and departs mid-2030, 6–8 months before reentry; notional reentry Jan 2031 | ESA briefing [S4]; GAO says reentry "end of 2030 or early 2031" [S3] |
| Debris footprint | ≤6,000 km in an uninhabited area of the South Pacific | NASA USDV vignette [S6] |
| Decision point | In 2027 NASA will assess whether to launch the USDV in 2029 or extend ISS operations | GAO-26-107805 (May 2026) [S3] |
| Commercial demo target | Commercial stations to demonstrate 4 crew for 30 days, target no later than 2030 | GAO [S3] |

**Implication:** the window for anything to happen *to* the ISS (docking, handover, salvage) closes in mid-2030, when crew leave. After that the station is uncrewed and descending.

# 2. Condition of the station

## 2.1 Structure and life

* The original core elements were designed for a **30-year structural life**; FGB and Node 1 will reach **32 years** in 2030 [S1].
* Primary structure (modules, truss, radiators) "cannot be repaired or replaced practically"; solar arrays, ECLSS, communications and science hardware can [S1].
* All five agencies have completed life-extension analyses **through at least 2028**; NASA's preliminary analysis shows **high confidence the USOS primary structure would support operations beyond 2030**, with detailed analysis under way [S1].
* Later modules are younger: Destiny 2001; Columbus and Kibo 2008 [S1].

## 2.2 The Russian PrK leak

| Date | Event | Source |
|---|---|---|
| 2019 | Cracks and leaks first appear in the PrK, the Zvezda transfer tunnel to an aft docking port | NASA blog, 5 Jun 2026 [S7]; Space.com on OIG [S8] |
| Feb–Apr 2024 | Leak 2.4 lb/day, then 3.7 lb/day (historic low 0.2 lb/day) | OIG IG-24-020 via Space.com [S8] |
| Aug 2024 | NASA risk rating 5 of 5; NASA and Roscosmos have not agreed what leak rate is "untenable". Closing the hatch would lose a docking port and need extra propellant | OIG IG-24-020 [S8][S9] |
| Week of 1 Jun 2026 | Leak rose to ≈2 lb/day with new suspected areas. A Roscosmos structural repair (cutting a bracket) prompted NASA to put 5 crew in safe haven in Dragon; Roscosmos paused and did not perform it | NASA blog [S7]; Reuters [S10] |
| Jul 2026 | NASA: talks included "a PrK resolution". SpacePolicyOnline: "the problem remains"; the agencies "disagree on the severity and consequences" | SpacePolicyOnline, 16 Jul 2026 [S11] |
| Summer 2026 | NASA: PrK "not currently leaking", held at a deliberately reduced pressure. The Aerospace Safety Advisory Panel called it "one of the most significant safety risks" | ISS Tracker report of ASAP meeting [S12]; Russian press report of NASA briefing [S13] |

**Assessment:** the PrK is contained, not solved. It is in the Russian segment, which provides propulsion and attitude control, so it limits *any* extension or reuse plan that keeps the whole station (estimate/judgement).

# 3. What the ISS costs to operate

| Measure | Value | Source |
|---|---|---|
| ISS operations, maintenance, research, crew and cargo transport, 2013–2023 | ≈\$3 B per year | NASA OIG IG-24-020 [S9] |
| Total including related operations and research (comms, human research, launch services etc.) | ≈\$4.1 B per year, 16% of NASA's FY2023–24 budget | OIG [S9] |
| Share that is crew and cargo transport | ≈60% of the ≈\$3 B | GAO-26-107805 [S3] |
| FY2026 President's request, ISS line | \$920.1 M per year FY2026–30, "maintaining safe operations of ISS at a minimal level" (transport funded separately, \$1,293.8 M in FY2026) | NASA FY2026 budget briefing book [S14] |
| Cost trend | NASA officials say costs "will likely grow" | GAO [S3] |

**Estimate (this study):** each extra year of ISS operation costs NASA in the order of **\$3 B**, so a two-year extension to bridge to a 2032 commercial station is **≈\$6 B** (simple multiplication of the GAO/OIG figure; not a NASA estimate). For comparison, Ping Lite's total v0.2 ROM is \$4–7 B (estimate, concept-design-v0.2.md).

# 4. Legal and ownership issues

* **Ownership:** under the 1998 Intergovernmental Agreement (IGA), Article 6, each Partner owns the elements it provides (ESA holds the European elements) [S15][S16].
* **Registration and jurisdiction:** under Article 5 each Partner registers its flight elements and keeps jurisdiction and control over them and its nationals aboard [S15][S16].
* **Consequence:** NASA cannot transfer the whole station. Any handover or reuse of a non-NASA element "would need to be negotiated with the relevant owner" [S1]. NASA also "does not have the intellectual property for all of the space station's engineering data"; US industry holds much of it [S1].
* **Russian segment:** Russia committed to operations through 2030 only in July 2026 (previously 2028), and seat swaps continue to 2030 [S11]. The segments are interdependent: one person from each side must be aboard at all times [S11], and the Russian segment provides propulsion and attitude control through any boost phase [S1]. Keeping the USOS without the Russian segment means replacing those functions.
* **Disposal duty:** the US Government requires reentering spacecraft to meet a 1-in-10,000 public-risk limit; the ISS must have a controlled reentry, and "the safe deorbit … is the responsibility of all five space agencies" [S1][S2].
* **Congressional direction:** operate through at least 30 Sep 2030 [S1]; the 2030 extension was codified in the 2022 NASA authorization act [S11].

## 4.1 NASA's past studies

NASA's 2024 Deorbit Analysis Summary [S1] evaluated and rejected: uncontrolled reentry; disassembly and return to Earth; disassembly and reuse in LEO; disassembly and deorbit in pieces; boosting to a higher orbit; fragmenting in orbit; transition to a commercial operator; and (conditionally) operations beyond 2030. Key findings used below:

* **Boost:** ISS at ≈415 km, >430,000 kg, has a 1–2 year natural lifetime without reboost. Current crew and cargo vehicles are optimised for that altitude, so crew access higher up would be "severely impacted or even impossible". Mean time between penetrating impacts falls from **≈51 years** at 415 km to **<4 years** at 800 km. A Gateway-PPE-class solar electric system would need **≈3 years** of thrust and **≈15,000 kg** of propellant to reach a 1,000-year orbit, passing through radiation the station was not designed for. Starship docking poses "prohibitive engineering challenges" for structural margins.
* **Fragmentation:** destroying the 420 t station in orbit would create ≈220 million pieces >1 mm, against ≈100 million today.
* **Commercial operator and reuse:** "NASA received no feasible proposals"; modules are "not self-sufficient independent spacecraft", 1990s-era interfaces, and "developing and launching a new module … would likely be cheaper".
* **Return to Earth:** needs at least as many EVAs as assembly (161) and a shuttle-class cargo bay that "does not currently exist"; NASA is working with the Smithsonian on preserving **smaller items** only.

# 5. Options and ratings

Rating scale (judgement by this study): **feasibility** High/Med/Low; **cost** to whoever pays, with order-of-magnitude where a source exists; **schedule** fit to 2030–2032; **risk** to crew, public and programme.

| # | Option | Feasibility | Cost | Schedule | Risk | Notes |
|---|---|---|---|---|---|---|
| O1 | **Baseline: USDV deorbit, Jan 2031** | High (on contract) | \$843 M USDV ceiling + launch [S2] | On plan | Low–Med (single vehicle, PrK) | NASA's selected option [S1] |
| O2 | **Extend ISS operations to ~2032** (NASA decision, 2027) | Med (USOS structure "high confidence" [S1]; needs all partners incl. Russia) | ≈\$3 B/yr, ≈\$6 B for 2 yr (**estimate**) | Fits the Ping Lite gap exactly | Med–High: PrK, ageing primary structure, partner politics | Not in Ping Lite's control. USDV can wait on the ground [S1] |
| O3 | **Boost to a higher storage orbit** | Low | Large: new propulsion/tanker vehicles; SEP ≈3 yr, 15 t propellant [S1] | Poor: no vehicle exists | High: debris impact MTBF <4 yr at 800 km; eventual uncontrolled reentry [S1] | Rejected by NASA; also makes the station uncrewable |
| O4 | **Keep the USOS with a commercial operator** (Russian segment detached/deorbited) | Low | Very high (**estimate**: ISS-class sustaining of \$1 B+/yr plus replacing Russian propulsion/attitude control) | Needs IGA renegotiation before 2030 | High: ownership, IP, 30+-year structure | No company has proposed it [S1] |
| O5 | **Reuse modules** (Axiom model: commercial module attached to ISS, then departs) | **High for new commercial modules**; Low for legacy ISS modules | Axiom: \$140 M NASA contract (2020) for a port [S17] | Axiom PPTM: ISS berthing originally NET 2027, free-flyer 2028 [S17][S18]; latest company update says launch 2028 [S19] | Med | NASA wants PPTM partly to **save ISS research equipment** [S17]. Legacy modules: "not self-sufficient" [S1] |
| O6 | **Return pieces to Earth for museums** | Low for modules; High for small items | Small items: ride Dragon down-mass (cost not public) | Before mid-2030 | Low | Smithsonian preservation of smaller items is already NASA's plan [S1] |
| O7 | **Ping Lite docks with or takes a handover from the ISS before deorbit** | **Nil on baseline** (Ping Lite launches Q4 2032, ISS crew leave mid-2030); Low even on the Q4 2030 compressed option | Ping Lite would need a new propulsion/proximity capability and NASA visiting-vehicle certification (**estimate**: tens to hundreds of \$M) | Only if O2 happens, or if the compressed schedule is achieved | High | Same orbit (420 km / 51.6°) helps; nothing else does |
| O8 | **Bridge mission**: pre-cursor Ping Lite element or payload on ISS before 2030 | Low–Med | **Estimate:** \$100–300 M for a small demo; a habitable element costs most of a station | Tight: hardware by 2029 | Med | Wins A&C credit but adds a second development line |

# 6. What it means for Ping Lite

## 6.1 The gap

* ISS crew leave **mid-2030**; reentry **Jan 2031** [S4].
* Ping Lite planning launch **Q4 2032**, Mission A **Q1 2033** (concept-design-v0.2.md §13).
* Gap for Ping Lite: about **2.5 years** of no ISS before first crew (estimate from the two schedules).
* NASA's stated goal is to minimise a gap in continuous presence and to have stations demonstrated by 2030 [S3]. Other providers may fill it: Vast Haven-1 targeted Q1 2027 [S20]; Axiom free-flyer targeted 2028 [S19].

## 6.2 Does a bridge mission or ISS docking help?

* **ISS docking/handover (O7): no.** It is physically impossible on the baseline and depends on a NASA extension (O2) that Ping Lite cannot influence. It also needs a propulsive, certified visiting-vehicle capability the v0.2 station does not have.
* **Bridge payload/element on ISS (O8): weak.** It would earn some A&C and past-performance credit but splits a small team across two developments. It is better served by flying **Ping Lite flight software** on a computer already hosted on the ISS or Axiom: HPE Spaceborne Computer-2, Axiom AxDCU-1, or Voyager's Space Edge unit [S21–S26]. That gives flight evidence for the CLDC software requirements (SOW §§2.10.3, 2.11.2, 2.15.4; NPR 7150.2 and NASA-STD-8739.8B [S27–S28]) with no new hardware (judgement; detail in concept-design-v0.2 §11B).
* **ISS extension (O2): helpful if it happens, and free to us.** If NASA extends to 2031–32, Ping Lite's 420 km / 51.6° orbit and IDSS/CBM ports (v0.2 A9–A10) mean NASA could run a short overlap or move small hardware. Proposal text should say so without depending on it.
* **ISS hardware reuse (O5/O6 variant): cheap and credible.** Ping Lite can offer to host NASA-owned research hardware returned to the ground or held on Axiom PPTM, subject to certification. Rack and locker interfaces are already ISS-heritage-compatible in v0.2 §5A (Mission A: 2 heritage-compatible lockers).

## 6.3 Should it go into the CLDC proposal?

| Item | In proposal? | Why |
|---|---|---|
| ISS rescue, boost or handover | **No** | NASA rejected these [S1]; would read as not understanding the RFP's 2030 baseline (SOW §1.0) |
| ISS docking before deorbit | **No** | Impossible on our schedule |
| "ISS-extension compatibility" statement (co-orbital, IDSS, can overlap if NASA extends) | **Yes, unpriced, ½ page** | Low cost; shows awareness of NASA's 2027 decision [S3] |
| Hosting heritage ISS research hardware | **Yes, as an option** | Supports NASA's equipment-salvage aim [S17]; uses v0.2 heritage interfaces |
| Flight-software demo on a hosted ISS/Axiom computer before mid-2030 | **Yes, as an option** | Software only: ECLSS FDIR, crew-off autonomy, power and thermal scheduling, avionics stack, run in shadow mode. Cost TBD |

# 7. Recommendation

1. **Treat ISS deorbit in early 2031 as fixed** for the proposal; do not propose to save, boost, take over or dock with the ISS.
2. **Add one unpriced paragraph** on ISS-extension compatibility, and an **option to host heritage ISS research hardware**.
3. **Do not fund a bridge element.** The early demo is **software only**: fly Ping Lite flight software (ECLSS FDIR, crew-off autonomy, power and thermal scheduling, avionics stack) in shadow mode on an existing hosted computer (HPE SBC-2, Axiom or Voyager) before mid-2030. Cost TBD.
4. **Close the gap through schedule and teaming, not the ISS:** pursue the compressed Q4 2030 path or the SpaceX hab-Starship discussion (trade-starship-cluster.md), and be ready to explain in the proposal how Ping Lite fits after Axiom and Vast.
5. **Watch the 2027 NASA extension decision** [S3]; if NASA extends to 2032, revisit O7 as a handover and overlap mission.

# Appendix A. Sources

| ID | Source | Used for |
|---|---|---|
| S1 | NASA, *International Space Station Deorbit Analysis Summary*, June 2024, nasa.gov/wp-content/uploads/2024/06/iss-deorbit-analysis-summary.pdf | All alternative assessments; structure life; boost/debris/SEP numbers; ownership and IP; extension; Smithsonian |
| S2 | NASA, "NASA Selects International Space Station US Deorbit Vehicle", June 2024 | SpaceX selection; \$843 M; NASA ownership; partner responsibility |
| S3 | GAO-26-107805, *Low-Earth Orbit: NASA Faces Impending Decisions…*, 2026 | 2027 decision; deorbit sequence; \$3 B/yr; 60% transport; 4-crew 30-day demo by 2030 |
| S4 | ESA, *ISS Community Briefing*, 27 Oct 2025 | USDV design; R−2.5/R−1.5 timeline; last crew dates; Jan 2031 reentry |
| S5 | readthegovcontract.com record for 80JSC024CA002 (USAspending data), modified 9 Apr 2026 | Ceiling, obligated, outlaid, end date |
| S6 | NASA JSC, *USDV vignette*, Mar 2025 | Delivery dates; 6,000 km footprint |
| S7 | NASA Space Station blog, "NASA Provides Update on Space Station Leak", 5 Jun 2026 | PrK history; June 2026 safe haven |
| S8 | Space.com, "Top ISS 'safety risk' is a leak…", Sept 2024 | 2024 leak rates; risk 5/5 |
| S9 | NASA OIG, IG-24-020, Sept 2024 | Costs (\$3 B, \$4.1 B); risks |
| S10 | Reuters, "Two leaks detected on ISS, Russian space agency says", 5 Jun 2026 | Roscosmos statement |
| S11 | SpacePolicyOnline, "Russia Agrees to ISS Operations, Seat-Swaps, Through 2030", 16 Jul 2026 | Russian commitment; PrK disagreement; interdependence; 2022 act |
| S12 | ISS Tracker, report on ASAP meeting and spacewalk briefing, 2026 | ASAP view; PrK not leaking at reduced pressure |
| S13 | www1.ru, "NASA: long-standing air leak in Russian ISS segment defeated", 5 Aug 2026 | NASA (Weigel) statement on no current leaks |
| S14 | NASA, FY2026 Budget Request Summary briefing book, May 2025 | \$920.1 M ISS line; transport line |
| S15 | 1998 ISS Intergovernmental Agreement, US State Dept. TIAS 12927 | Articles 5 and 6 |
| S16 | JAXA space-law library, IGA text and Article 6 | Ownership and registration |
| S17 | Payload Space, "Axiom Space Adjusts Space Station Plans", Dec 2024; NASA, "NASA, Axiom Space Change Assembly Order…" | PPTM first; nadir port; equipment salvage; \$140 M 2020 contract |
| S18 | Axiom Space release, "Axiom Space Accelerates Axiom Station Assembly", Dec 2024 | PPTM-first sequence; free-flyer 2028 |
| S19 | Carrier Space, "Axiom Station Explained", Oct 2026 (secondary; reports Axiom's latest update) | First module launch 2028 |
| S20 | Vast, "Vast Advances Haven-1 Into Integration Phase" | Haven-1 Q1 2027 target |
| S21 | HPE, Spaceborne Computer page and 30 Jan 2024 release "HPE Spaceborne Computer-2 returns to the ISS" | SBC-2 open to proposals via ISS National Lab |
| S22 | ISS National Lab, "HPE Space-based Computing Achieves Significant Milestone", 5 Apr 2022 | Containerised code pushed to SBC-2 |
| S23 | BruhnBruhn Innovation, "Cloud-native AI runs in orbit for the first time", 11 Aug 2025 | k3s workload deployed by git push, 24 Jul 2025 |
| S24 | ISS National Lab, "Orbital Data Center Launching to ISS…", 19 Aug 2025; Axiom, "Orbital Data Centers" and AxODC Node ISS release, 16 Sep 2025; TechRadar | AxDCU-1 on CRS-33; MicroShift; rollback; Node ISS in 2027 |
| S25 | Voyager, Bishop Airlock, Mission Management and Space Edge pages; Red Hat/Voyager release, 11 May 2026 | Hosted payloads; RHEL 10.1 on the ISS Space Edge unit; SHIRE |
| S26 | AWS News Blog, "How We Sent an AWS Snowcone into Orbit", 2022; GeekWire, 2022 | 7-month safety review; SSH; 7 GB model uplink |
| S27 | NASA Software Engineering Handbook (swehb.nasa.gov), SWE-020 and §7.24 (NPR 7150.2D Appendix D) | Class A definition; human-rating software |
| S28 | NPR 7150.2C §3.6.1 (SWE-022); CLDC SOW §2.11.2 | Software assurance per NASA-STD-8739.8 |
| S29 | Lightstreamer ISSLive demo and client (github.com/Lightstreamer) | Public ISS telemetry stream |
| — | CLDC RFP 80JSC027R0004 (SOW §1.0); Ping Lite concept-design-v0.2.md; trade-starship-cluster.md | Requirements and Ping Lite baseline |
| — | **All other numbers** | **Estimates by this study** |
