# Ping Lite: an open concept for a commercial crewed station in LEO

**Website:** https://fitzyracing1.github.io/new-space-station/
**Comment on the station:** https://github.com/fitzyracing1/new-space-station/discussions

**Ping Lite** is an open, early-stage (pre-Phase A) concept for a **commercially owned and operated crewed space station in low Earth orbit**. Its purpose is to continue human presence in LEO after the International Space Station retires at the end of 2030. It is being developed with NASA's **Commercial LEO Destination Contract (CLDC)** solicitation (Final RFP 80JSC027R0004) in mind.

Everything here is public so that engineers, scientists, future users and space fans can review it and help improve it.

- **Flight software v0.1 (prototype, not certified):** [`software/`](software/README.md), NASA cFS-based, MIT.

## The station at a glance (concept v0.2, all values estimates)

| | |
|---|---|
| Architecture | **One integrated core station**: 7.2 m diameter, 5 decks, about 17 m long, outfitted on the ground |
| Launch | **One Starship launch** (the v0.1 design used 5 Falcon launches; a modular Falcon-class design is kept as a fallback) |
| Orbit | 420 km circular, 51.6° inclination (ISS band) |
| Crew | **6 permanent (4 government + 2 commercial)**, up to 10 for 30 days or less |
| Pressurized volume | about 560 m³ (about 305 m³ habitable) |
| Power | about 55 kW orbit-average at end of life (6 × 75 m² solar wings; 18 kW for payloads) |
| NASA science | Sized to NASA Category 3: 58 locker locations (40 heritage-compatible), 3 rack-equivalent spaces, 12 powered payloads, about 170 L cold stowage, vacuum, O₂ and N₂ ports, glovebox, workbench |
| Ports | 4 IDSS docking + 2 berthing. Crew Dragon, Cargo Dragon and Cygnus |
| Launch mass | about 93 t including 25% growth allowance |
| Cost to operational station | about $4–7 B (rough order of magnitude) |
| Launch date | about Q4 2032 planning baseline. A compressed option (Q4 2030) is rated low likelihood |

Main open risks: Starship payload capability (the station is about 93 t, while 2026 test flights carried about 35–40 t), how the station is carried in and released from the Starship payload bay, the single-launch single point of failure, and NASA launch-vehicle category rules.

## Documents

* [`docs/concept-design-v0.2.pdf`](docs/concept-design-v0.2.pdf) / [`.md`](docs/concept-design-v0.2.md): **current design (v0.2, single Starship launch, NASA compliance matrix)**
* [`docs/concept-design-v0.1.pdf`](docs/concept-design-v0.1.pdf) / [`.md`](docs/concept-design-v0.1.md): first concept (5 modules, 5 Falcon launches), kept for reference

## How to give feedback

1. **Discussions:** open-ended comments, ideas and questions: [github.com/fitzyracing1/new-space-station/discussions](https://github.com/fitzyracing1/new-space-station/discussions)
2. **Issues:** specific, actionable design feedback (use the *Design feedback* or *Question* templates)
3. **Pull requests:** corrections or edits to the Markdown design documents

Please cite the section number (for example "v0.2 §5A Science accommodations") when you comment on a specific number.

## Status and disclaimer

This is an independent concept study. It is **not affiliated with or endorsed by NASA, SpaceX or any company named in it**, and no partnership or agreement with any of them exists. Figures from real programs are labelled. Everything else is an engineering estimate and will change.

Contact: Fitzyracing1@gmail.com

## License

[MIT](LICENSE) © 2026 Joshua Almeida

## Plans

This project is free, and its license stays as is. Support it through [GitHub Sponsors](https://github.com/sponsors/fitzyracing1):

- **Free**: use the code, community support.
- **Pro ($49/month)**: priority issues and support.
- **Team ($199/month)**: priority support for your team plus early access.
