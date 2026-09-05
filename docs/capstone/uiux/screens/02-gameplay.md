---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/render.c"
---

# Screen: Active Gameplay & 3D Highway

## Surface Summary
The primary playable 60 FPS endless runner screen.

- **Visual Elements**:
  - **Bottom HUD Bar**: Stable 2-row stats display enclosed in a beveled metallic box on `BG1` across rows 17–19 ($Y \in [136, 160]$), isolated from road tiles via hardware `REG_WIN0`.
    - Row 1 (Y: 139): `SCORE: %06d (x%d)` left-aligned; dedicated OAM Coin icon and count right-aligned (`[Coin] %03d`).
    - Row 2 (Y: 148): `DIST: %5dm` left-aligned; dedicated OAM Wheel/Speedometer icon and speed right-aligned (`[Wheel] %3d`).
  - **Floating Arcade Score Popups**:
    - `+150`: Flashes in gold when cleanly hurdling over a low barrier.
    - `+150`: Flashes in neon cyan when slipstreaming under an overhead gantry while nano-shrunk.
    - `+200`: Flashes in gold when leaping over a civilian car.
    - `+100`: Flashes in bright white when performing a close-call evasion alongside heavy traffic.
  - **3D Top-Down Parallel Highway**: Scrolled vertically at speed on `BG2`; 3-lane parallel asphalt with dynamic red/white kerbs and striped lane dividers.
  - **Sprite Layer**: Player sports car (OAM 0–2), nano-shrunk compact sprites, ascending/descending pitch sprites, aerodynamic cyan air whoosh under wheels, scaling drop shadow (OAM 30), and HUD hardware icons (OAM 31–32).
  - **Thematic Obstacle & Hazard Sets**:
    - **City Highway** (Sprite Palette 1):
      - *Low Barrier*: Striped yellow/black construction barrier (clears green).
      - *High Sign*: Overhead destination gantry (clears green).
      - *Truck*: Heavy blue freight hauler.
      - *Civilian Car*: Silver commuter coupe.
      - *Stunt Ramp*: High-traction steel chevron launch ramp (`+200 STUNT!` super-jump).
      - *Surface Slick*: Dark rainbow-sheen motor oil pool (causes 30-frame fishtail traction loss).
      - *Signature Hazard*: Heavy industrial chemical tanker truck with pressurized cylinder chassis.
    - **Palm Beach** (Sprite Palette 10):
      - *Low Barrier*: Weathered driftwood logs.
      - *High Sign*: Bamboo tiki pier archway.
      - *Truck*: Coastal yacht hauler flatbed with speedboat.
      - *Civilian Car*: Turquoise woody surfer wagon.
      - *Stunt Ramp*: Bamboo & driftwood surf kicker ramp.
      - *Surface Slick*: Foaming ocean tidal wash pool.
      - *Signature Hazard*: Windblown windsurf rig with vibrant multi-colored sail tumbling across lanes.
    - **Winter Snow** (Sprite Palette 11):
      - *Low Barrier*: Glacial ice ridge barrier with red flags.
      - *High Sign*: Ski gondola cableway gantry.
      - *Truck*: Heavy municipal orange snowplow truck.
      - *Civilian Car*: Alpine 4x4 expedition SUV.
      - *Stunt Ramp*: Packed glacial snow jump with red guide poles.
      - *Surface Slick*: Shimmering black ice patch.
      - *Signature Hazard*: Massive craggy glacial ice boulder rolling down the asphalt.
    - **Cape Orbital** (Sprite Palette 12):
      - *Low Barrier*: Heavy magnetic induction conduit pipe.
      - *High Sign*: Steel aerospace gantry with warning lights.
      - *Truck*: 8-wheel lunar heavy crawler transport.
      - *Civilian Car*: Sleek pressurized titanium rover.
      - *Stunt Ramp*: Magnetic levitation booster strip with cyan LED arrays.
      - *Surface Slick*: Glowing hyper-coolant chemical leak pool.
      - *Signature Hazard*: Pulsing plasma laser barrier cycling between lethal discharge (ON) and safe pass window (OFF).
    - **Gothic Midnight** (Sprite Palette 13):
      - *Low Barrier*: Weathered stone crypt sarcophagus.
      - *High Sign*: Wrought iron cemetery arch with gargoyle crest.
      - *Truck*: Heavy black funeral hearse with silver filigree.
      - *Civilian Car*: Battered 1950s phantom jalopy.
      - *Stunt Ramp*: Sloped stone tomb lid with carved Celtic cross.
      - *Surface Slick*: Bubbling eerie green spectral ectoplasm puddle.
      - *Signature Hazard*: 3 Small Stone Gargoyles sitting across all 3 lanes (lanes 0, 1, 2) on the road asphalt. Must hurdle/leap over (`UP`) to clear.
    - **Maya Temple Trail** (Sprite Palette 14):
      - *Low Barrier*: Ancient carved stone totem stela.
      - *High Sign*: Overgrown aqueduct archway with dangling vines.
      - *Truck*: 6x6 canvas safari expedition supply truck.
      - *Civilian Car*: Rugged jungle patrol 4x4 with snorkel.
      - *Stunt Ramp*: Stepped ancient sun-temple ramp with moss and glyphs.
      - *Surface Slick*: Wet red clay jungle mud trap.
      - *Signature Hazard*: Ancient Poison Dart Trap Volley — flanking carved serpent stone blowpipe totems firing a rapid horizontal jade poison dart barrage across the road at vehicle head height. Duck into slipstream chassis (`DOWN`) or blast through with Nitro to survive.


