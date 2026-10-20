---
title: "Shell-Off Starship Mars Habitat"
subtitle: "Concept design study v0.1"
author: "Joshua Almeida (fitzyracing1)"
date: "19 October 2026"
---

# 1. Summary

A Starship lands on Mars carrying, inside its payload section, a **factory-installed, self-supporting pressure habitat**. The stainless outer shell acts only as a launch, entry and landing fairing. After landing, the shell over the payload section is cut free and **discarded** (baseline). The inner habitat then stands on the ship's lower section as a plinth, and the crew covers it with regolith for radiation shielding.

The concept accepts that the shell is expendable. SpaceX's stated plan is a large fleet with many ships per Mars window [1][3], so recovering panels is not required. That makes the removal sequence simpler: sever, push off, walk away.

**Status:** this is a concept study. All masses in this document are **author estimates**, labelled as such. Every external figure is cited.

# 2. Reference figures (cited)

| Item | Value | Source |
|---|---|---|
| Starship diameter | 9 m | [1][2] |
| Usable payload envelope | 8 m dia; extended height up to 22 m | [2] |
| Forward payload volume | ">1,000 m³" pressurized (2019 site); "about 1,100 m³" (2019 page) | [3][4] |
| Payload to Mars surface | 100+ t (with propellant transfer) | [2][3] |
| Mars surface pressure | ~6.36 mbar (~0.64 kPa) mean | [5] |
| Mars gravity | 3.71 m/s² | [5] |
| Solar irradiance at Mars | 586.2 W/m² (top of atmosphere) | [5] |
| Surface GCR dose equivalent (Gale) | ~0.64 mSv/day | [6] |
| Cruise dose equivalent (MSL) | ~1.84 mSv/day | [7] |
| Kilopower/KRUSTY | 1 kWe-class ground test, 2018 | [8] |
| NASA Fission Surface Power target | 40 kWe class | [9] |
| BEAM expandable on ISS | flight-proven since 2016 | [10] |

# 3. Architecture

**Baseline: hybrid rigid core + composite ribs + inflatable/soft-goods pressure liner.**

- **Habitat pressure vessel (HPV).** Bladder/restraint layers like BEAM [10], 8 m diameter to fit the 8 m payload envelope [2]. A composite rib cage and four composite deck rings support it. A rigid central core (4 m class, est.) carries the hatches, utilities and ladder/lift, and the liner seals to it. The vessel is pressurized to full differential on Earth during acceptance testing.
- **Load path in flight.** Launch and entry loads go through the stainless shell. The HPV hangs from discrete **load links** at the deck rings. These are latched pin-and-clevis or frangible bolts.
- **Load path on Mars.** The HPV rests on a thrust ring above the aft tank dome. The lower ship (tanks, engines, skirt) stays in place as the plinth. The HPV is designed to stand on its own under Mars gravity and wind, plus its internal pressure.
- **Volume (geometric, not SpaceX):** an 8 m × 18 m cylinder gives about 905 m³ gross. That matches SpaceX's ~1,000 m³ class claim [3]. Net habitable volume will be well below this once you subtract decks, equipment and the core.

![Side section before and after shell removal (schematic)](/workspace/mars-habitat/section.png)

# 4. Shell removal, discard baseline

Because the shell is waste, no panel needs to come off intact.

1. **Pre-cut seams.** Longitudinal and circumferential seams are built in at the factory: linear-shaped-charge or frangible joints [11], or bolted latches as a no-pyro option. The seams sit above the aft tank dome and divide the payload-section shell into 4–6 petals (est.). The nose cone is a separate cap.
2. **Release load links** from inside the HPV or remotely.
3. **Fire seams or unlatch them in sequence.** Spring or gas jacks at the petal bases push each petal outward. Hinges at the base let each petal fall away from the HPV, with no crane needed. The nose cap is lifted off and dropped clear by a small crane or by rigging, or a gantry from a precursor cargo ship does it. The cap is the hardest step: see Risks.
4. **Leave it.** Petals and cap lie where they fall, or a rover drags them at least one ship length downwind. The keep-out zone protects against sharp edges, glint, and dust traps.

*Optional, later:* scavenging the discarded steel for berm walls, landing-pad facing or shelter roofing is allowed but not planned.

Because the stainless shell is discarded, the habitat bladder has to carry its own MMOD and abrasion layers [10]. It needs these against wind-blown sand as well as during the roughly 1 sol it sits exposed before berming.

# 5. Radiation shielding

The surface dose is about 0.64 mSv/day [6], roughly a third of the cruise dose [7], because the planet blocks half the sky and the atmosphere adds shielding. The thin stainless shell adds little shielding, so discarding it costs little. The plan has three layers:

- **Regolith berm** around the cylinder, built from sandbags filled by a rover or excavator. NASA analyses show that regolith of order tens of g/cm² meaningfully reduces dose [12]. The thickness is TBD by transport analysis.
- **Cap:** a regolith-bag cover, or water/waste walls on the dome deck (option).
- **Storm shelter** on Deck 4, with water and consumables stores around the core, for solar particle events. Limits follow NASA-STD-3001 [13].

# 6. Thermal, dust and pressure

- **Pressure:** outside is about 0.6 kPa [5], so the full cabin ΔP (est. 70 kPa, a reduced-pressure, higher-O₂ atmosphere) is carried by the liner and restraint. The vessel is proof-tested on Earth. The restraint layer has to be sized with margin for creep [10].
- **Thermal:** the mean temperature is about −63 °C [5]. MLI and a dust-tolerant outer cover go over the restraint, and the regolith berm adds thermal mass. A heat-rejection radiator sits on the ground, separately. With the shell gone, the HPV loses its sun shade, so the outer cover also needs a low solar-absorptance finish.
- **Dust:** a suitport or dust lock on Deck 1 [14] and an outer abrasion layer. All mechanisms are placed outside, away from the berm.

# 7. Mass budget (ALL ESTIMATES, author assumptions)

| Element | Est. mass (t) |
|---|---|
| HPV liner, restraint, MMOD/thermal layers | 8 |
| Composite ribs, deck rings, core | 12 |
| Decks, outfitting, crew quarters | 15 |
| ECLSS (regenerative) + spares | 10 |
| Power: 40 kWe-class fission unit (mass TBD from [9]) or solar | 10 |
| ISRU (O₂/H₂O pilot), rover/excavator | 10 |
| Consumables (initial, 6–10 crew) | 20 |
| Separation hardware (seams, jacks, links) | 2 |
| Margin (~15%) | 13 |
| **Total** | **~100** |

That total is at the limit of the cited 100+ t [2]. In practice the power, ISRU and rover are more likely to come down on a precursor cargo ship.

# 8. Deck layout (6–10 crew)

| Deck | Function |
|---|---|
| 1 (lower) | Suitport and dust lock, EVA prep, ECLSS racks, hatch to plinth |
| 2 | Galley, wardroom, medical, exercise |
| 3 | 10 radial quarters around the core (~4.8 m² gross floor each, geometric) |
| 4 | Lab/workshop; storm shelter around the water-walled core |
| Dome | Storage, hygiene, greenhouse trial |

![Deck 3 plan (schematic)](/workspace/mars-habitat/deck.png)

For per-crew volume and consumables, use NASA BVAD [15] and NASA-STD-3001 [13] when sizing. A 6-crew baseline leaves 4 berths for growth or stowage.

# 9. Life support, ISRU and power

- **ECLSS:** ISS-heritage regenerative water recovery and O₂ generation, sized from BVAD [15].
- **ISRU:** MOXIE showed CO₂-to-O₂ electrolysis working on Mars [16], and it is the pilot for scale-up. Ice and regolith water extraction follows later. Habitat O₂, H₂O and N₂/Ar buffer-gas tie-ins go through Deck 1.
- **Power:** fission surface power at the 40 kWe class [9] is the baseline because it works through dust storms. Kilopower heritage is from [8]. Placement is shielded and set back from the habitat. The backup is a solar array: irradiance at Mars is 586 W/m² at the top of the atmosphere [5] and drops further in dust storms.

# 10. Trades and mitigations

| Trade | Issue | Mitigation |
|---|---|---|
| Discarded shell | Lose a steel shelter and potential berm material | Fleet volume [3]; optional later scavenging |
| HPV inside fairing | Mass of two structures (shell + HPV) | Soft-goods liner, composite ribs; vehicle shell designed lighter where unpressurized |
| Ship not reflown | One whole Starship expended per habitat | Accepted by Joshua's design choice |
| Exposure before berm | Radiation, UV, abrasion for days | Fast berming robots; crew arrives after setup |
| No crew volume during cruise unless crewed ship | HPV must also serve as cruise habitat for crewed flights | Design HPV to cruise-hab standards |
| Nose cap removal | Heaviest single lift | Precursor gantry or crane, or leave the cap on as a fixed roof (option) |

# 11. Roadmap and Earth-analog demo

1. **v0.2 paper study:** loads, liner sizing, separation sequence.
2. **Subscale liner + rib article** (≈2 m): burst/creep testing to BEAM-style practice [10].
3. **Separation demo:** a full-diameter steel petal segment around a dummy liner, using latched joints first and pyro later. Done at a test site with no people downrange.
4. **Earth analog:** a full-scale HPV in a desert analog, either at MDRS (Mars Society, Utah) [17] or alongside NASA CHAPEA-style long-duration analog campaigns [18]. Stages: shell-off, berming with sandbags, then a 30-day crewed occupation.
5. **Lunar or Mars cargo pathfinder:** uncrewed HPV deployment with telemetry.
6. **Crewed habitat.**

# 12. Top risks

1. **Nose-cap and petal separation near a pressurized soft structure.** A recontact or puncture would be catastrophic. Seams need standoff, petals must fall outward, and the HPV stays unpressurized until separation is complete.
2. **Stability and attachment of the free-standing HPV** on the plinth in Mars wind. A tipped or settled ship affects everything.
3. **Liner creep, abrasion, UV and dust** with no hard shell left.
4. **Mass:** the estimated total is at the 100 t limit.
5. **Regolith berm construction rate,** which drives the dose during setup.
6. **Dependency on SpaceX** for configuration, interfaces and data, none of which are available yet.

# References

1. SpaceX, Starship technical specifications. https://www.spacex.com/vehicles/starship/technical-specifications
2. SpaceX, *Starship Users Guide* rev 1.0, 2020. https://spacex.com.pl/files/2020-04/starship-users-guide-v1.pdf (mirror)
3. SpaceX, Mars page (2019 archive). https://web.archive.org/web/20190403053312/https:/www.spacex.com/mars
4. SpaceX, Starship page (2019 archive). https://web.archive.org/web/20190930163150/https:/www.spacex.com/starship
5. NASA NSSDCA, Mars Fact Sheet. https://nssdc.gsfc.nasa.gov/planetary/factsheet/marsfact.html
6. Hassler D.M. et al., "Mars' surface radiation environment measured with MSL Curiosity rover," *Science* 343, 2014.
7. Zeitlin C. et al., "Measurements of energetic particle radiation in transit to Mars on MSL," *Science* 340, 2013.
8. NASA, Kilopower / KRUSTY test, 2018. https://www.nasa.gov/directorates/stmd/tech-demo-missions-program/kilopower/
9. NASA, Fission Surface Power project. https://www.nasa.gov/space-technology-mission-directorate/tdm/fission-surface-power/
10. NASA, Bigelow Expandable Activity Module (BEAM). https://www.nasa.gov/international-space-station/bigelow-expandable-activity-module/
11. Bement L.J., Schimmel M.L., "A Manual for Pyrotechnic Design, Development and Qualification," NASA TM-110172, 1995.
12. Simonsen L.C., Nealy J.E., "Radiation Protection for Human Missions to the Moon and Mars," NASA TP-3079, 1991.
13. NASA-STD-3001, Space Flight Human-System Standard, Vols 1–2.
14. NASA, Suitport concept (Z-series/SEV suitport) technical reports, NTRS.
15. Anderson M.S. et al., *Life Support Baseline Values and Assumptions Document*, NASA/TP-2015-218570/REV1, 2018.
16. Hoffman J.A. et al., "Mars Oxygen ISRU Experiment (MOXIE)…," *Science Advances* 8, 2022.
17. The Mars Society, Mars Desert Research Station. https://mdrs.marssociety.org/
18. NASA, CHAPEA. https://www.nasa.gov/humans-in-space/chapea/
