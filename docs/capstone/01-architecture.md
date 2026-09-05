---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/**"
  - ":(top)harness/**"
---

# 01 Architecture

## Layers

The codebase is organized into two primary subsystems: the embedded Game Boy Advance game binary (`nitrosurfer`) and the host automated test harness (`harness`).

- **GBA Embedded Core (`nitrosurfer/`)**:
  - **Hardware Abstraction & Platform**: Uses libgba headers (`<gba.h>`) for direct register manipulation (REG_DISPCNT, REG_BG0CNT, OAM, VRAM, VBlank interrupts).
  - **Asset & Graphic Generation (`source/gfx_data.c`)**: Procedural 4bpp tile generation for sprites and backgrounds directly into VRAM Character Blocks.
  - **Audio Engine (`source/audio.c`)**: Direct PSG channel control (Direct Sound DMG Sound 1, 2, 4 for sweep chimes, coin notes, white noise engine rumbles).
  - **Game Simulation (`source/game.c`)**: Pure procedural state machine (Title, Playing, GameOver), physics update, collision detection, and choreographed wave spawner.
  - **Scene Renderer (`source/render.c`)**: 3D pseudo-perspective projection math, OAM shadow buffer population, and Text Tile Engine (TTE) Mode 0 HUD rendering.
  - **Runtime Lifecycle (`source/main.c`)**: Hardware initialization, interrupt service registration, and main 60 FPS loop synchronized with VBlank.

- **Automated Test Harness (`harness/`)**:
  - **Browser Runner (`harness/driver.mjs`)**: Headless Chromium instance running EmulatorJS (mGBA / Gambatte WASM cores) over an ephemeral local HTTP server.
  - **Memory Inspection (`harness/memory.mjs`, `harness/gb-memory.mjs`)**: Save-state snapshot parser resolving GBA bus addresses (IWRAM `0x03000000`, EWRAM `0x02000000`, VRAM `0x06000000`, OAM `0x07000000`).

## Module boundaries

- `source/main.c` imports `render.h`, `game.h`, `audio.h`, `gfx_data.h`, and `common.h`.
- `source/game.c` manages game state (`g_game`, `g_player`, `g_obstacles`, `g_coins`, `g_world_powerups`) and calls `sfx_*()` in `audio.h`. It never calls `render_*()` or manipulates OAM directly.
- `source/render.c` reads state from `g_game`, `g_player`, etc. and writes into local `s_obj_buffer` (OAM shadow buffer), transferring to hardware OAM during VBlank. It never mutates gameplay state.
- `source/audio.c` writes directly to GBA hardware audio registers (`REG_SOUNDCNT_X`, `REG_SOUND1CNT_H`, etc.).
- `source/gfx_data.c` populates sprite and background tile palettes and charblocks in VRAM on boot.

## Entry points

- GBA Game Execution: `nitrosurfer/source/main.c:16` (`int main(void)`)
- Headless Test Driver API: `harness/driver.mjs:55` (`class EmuHarness`)
- CLI Test Smoke Runner: `harness/driver.mjs:189` (`cliMain()`)
- Web Emulator Deployment: `index.html:1` (served via Netlify statically)

## Communication

- In-Game Subsystems: Direct C function calls over static global state structs allocated in IWRAM (`0x03000000`).
- Hardware Synchronization: Hardware VBlank interrupt flag (`REG_IF & IRQ_VBLANK`) polled via `VBlankIntrWait()` via BIOS call 0x05.
- Test Harness to Emulator: Playwright `page.evaluate()` executing EmulatorJS JavaScript bridge (`window.Emu.press()`, `window.Emu.saveState()`, `window.Emu.screenshot()`).

## Composition

- Static singleton composition: All primary structs (`g_game`, `g_player`, `g_obstacles`, `g_coins`, `g_world_powerups`) are declared globally in `nitrosurfer/source/game.c` and externed via `nitrosurfer/include/common.h`.
- Initialization sequence in `main()`:
  1. `irq_init()` and `irq_add(II_VBLANK, NULL)`
  2. `audio_init()`
  3. `render_init()` (Mode 0, TTE setup, palette load, road map generation)
  4. `gfx_init_all()` (tile generation)
  5. `game_init()` (player and game struct reset)

## Frontend

- GBA Hardware Mode 0 (Tile/Text Engine):
  - Screen resolution: 240x160 pixels, 60 FPS progressive.
  - Background Layer 0 (Priority 0, CBB 0, SBB 31): Mode 0 TTE (Text Tile Engine) text overlay displaying formatted Score, Coins, Distance, and Speed with sub-tile centering (`REG_BG0VOFS = 253`).
  - Background Layer 1 (Priority 0, CBB 2, SBB 30): Isolated Bottom HUD Container Box spanning rows 17–19 ($Y \in [136, 160]$) with beveled metallic borders and solid black interior.
  - Background Layer 2 (Priority 3, CBB 2, SBB 28): 3-Lane Parallel Top-Down Highway with alternating asphalt shading and red/white kerbs scrolled vertically in software.
  - Hardware Window 0 (`REG_WIN0`): Masks $X \in [0, 240]$, $Y \in [136, 160]$. Configured with `WININ` enabling BG0, BG1, and OBJ while disabling BG2, completely preventing road texture bleed beneath the bottom HUD.
  - OAM Sprite Layer: 128 hardware sprites (4bpp, 1D mapping):
    - OAM 0–2: Player sports car, thruster flames, and wind whoosh.
    - OAM 3–10: Roadblocks, signs, civilian cars, and heavy cargo trucks.
    - OAM 11–26: Spinning gold coins.
    - OAM 27–29: World power-ups (Nitro canisters, shield barriers, magnetic orbs).
    - OAM 30: Oval scaling drop shadow.
    - OAM 31–32: Dedicated Bottom HUD OAM icons (Coin icon at (216, 139), Speedometer/Wheel icon at (216, 148)).
    - Floating debris particles: Dynamic score popup badges (+150 hurdle, +200 gantry, +100 dodge) rendered as OAM sprites over gameplay action.
    - Title Garage Showroom (3x2 Grid Display Cases):
      - BG1 Screen Block 30 (`se_mem[30]`): Hosts the 3x2 grid of display cases across Rows 4–8 (Row 0) and 10–14 (Row 1). Unselected boxes render with metallic silver beveled tiles (`TILE_HUD_BOX_*`, Pal 2); the active selection renders with an illuminated arcade-gold glowing border (`TILE_SEL_BOX_*`, Pal 0). Cleared seamlessly on game start via `render_clear_title_boxes()`.
      - OAM 0–5: 6 whole 32x32 supercars (`TILE_PLAYER_STRAIGHT`) positioned inside each box:
        - Box (0, 0) at (24, 36): Cherry Red Supercar (Sprite Palette Bank 4)
        - Box (1, 0) at (104, 36): Cobalt Blue Supercar (Sprite Palette Bank 5)
        - Box (2, 0) at (184, 36): Neon Pink Supercar (Sprite Palette Bank 6)
        - Box (0, 1) at (24, 84): Stealth Black Supercar (Sprite Palette Bank 7)
        - Box (1, 1) at (104, 84): Racing Yellow Supercar (Sprite Palette Bank 8)
        - Box (2, 1) at (184, 84): Viper Green Supercar (Sprite Palette Bank 9)
      - Dynamic Engine Vibration: The selected supercar rumbles with a 1px vertical engine idle bounce (`(g_game.frame_count & 16)`).
  - Sprite Palette Allocation:
    - Palette Bank 0: Active Player Supercar (dynamically loaded via `gfx_set_car_color()` based on `g_game.car_color`).
    - Palette Bank 1: City Highway Obstacles (Roadwork Barricades, Overhead Sign Gantries, Civilian Blue Sedans, Cargo Semi-Trucks).
    - Palette Bank 2: Collectibles, HUD Icons & Debris (Coins, Powerups, Star Sparkles, Score Popups).
    - Palette Bank 3: Steel Coat Armor & Visual FX (Aura, Sparks).
    - Palette Banks 4–9: 6 Supercar Chassis Paints (Cherry Red, Cobalt Blue, Neon Pink, Stealth Black, Racing Yellow, Viper Green) for simultaneous true-color whole-car rendering.
    - Palette Banks 4–9: 6 Supercar Chassis Paints (Cherry Red, Cobalt Blue, Neon Pink, Stealth Black, Racing Yellow, Viper Green) for simultaneous true-color whole-car rendering.
    - Palette Bank 10: Palm Beach Obstacles (Weathered Driftwood, Bamboo Tiki Pier Archways, Coastal Yacht Hauler Flatbeds, Retro Woody Surfer Wagons).
    - Palette Bank 11: Winter Snow Obstacles (Glacial Ice Ridges, Alpine Cableway Gantries, Municipal Snowplows, Alpine 4x4 Expedition SUVs).
    - Palette Bank 12: Cape Orbital Obstacles (Cryo Trench Fuel Pipes, Launch Umbilical Gantries, Heavy Crawler Booster Transporters, Autonomous Security Rovers).
    - Palette Bank 13: Gothic Midnight Obstacles (Crumbling Crypt Slabs, Wrought-Iron Cemetery Gates, Black Funeral Hearses, Phantom Hot-Rod Jalopies).
    - Palette Bank 14: Maya Temple Trail Obstacles (Carved Stone Stela Totems, Ruined Aqueduct Archways, 6x6 Expedition Safari Haulers, Safari 4x4 Jeeps).
  - Track Map Selection Screen (6 Boxes in 3x2 Grid on BG1):
    - BG1 Screen Block 30 (`se_mem[30]`): Displays 6 track preview cases in a 3x2 grid across columns 1..8, 11..18, and 21..28 and rows 4..8 (Row 0: City, Beach, Winter) and 10..14 (Row 1: Cape Orbital, Gothic Midnight, Maya Temple Trail).
    - True Multi-Palette Preview: BG Palette Banks 2..7 render simultaneous authentic track thumbnails with skyscrapers, ocean waves & palms, snowy pines, rocket towers, gothic spires, and ancient Mayan pyramids.
    - Active selection glows with Gold border (`TILE_SEL_BOX_*`, Pal 0); unselected cards use themed borders (`TILE_HUD_BOX_*`).
    - Dedicated dark backdrop panel on BG1 across rows 15..16 for high-contrast subtitle legibility.
    - D-Pad 2D navigation: `LEFT` / `RIGHT` cycles sequentially; `UP` / `DOWN` toggles rows; `B` returns to car showroom; `A` / `START` launches race with chosen car and map.
  - Map Themes, Road Features & Thematic Obstacle Sets:
    - `MAP_CITY` (Palette Bank 1):
      - Visuals: Classic neon metropolis highway with grass shoulders, urban skyline, and 600m environmental milestone transitions.
      - Low Barrier (`OBS_LOW_BARRIER`): Construction hazard striped barricade (`TILE_OBS_BARRICADE_LRG` 84, `_PASS` 88).
      - High Sign (`OBS_HIGH_SIGN`): Green highway destination gantry with overhead clearance (`TILE_OBS_SIGN_LRG` 100, `_PASS` 108).
      - Heavy Truck (`OBS_TRUCK`): Blue interstate logistics semi-truck with amber cab clearance lights (`TILE_OBS_TRUCK_LRG` 120).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Blue passenger commuter sedan (`TILE_OBS_CIVILIAN_LRG` 280).
    - `MAP_BEACH` (Palette Bank 10):
      - Visuals: Tropical coastline track with turquoise ocean surf, golden sand dunes, and roadside emerald palm trees with brown bark trunks.
      - Low Barrier (`OBS_LOW_BARRIER`): Weathered tropical driftwood log with green moss patches and branch stubs (`TILE_OBS_BEACH_DRIFTWOOD_LRG` 360, `_PASS` 368).
      - High Sign (`OBS_HIGH_SIGN`): Bamboo tiki pier archway with crossbeams, tiki masks, and glowing green clearance lanterns (`TILE_OBS_BEACH_TIKI_LRG` 376, `_PASS` 392).
      - Heavy Truck (`OBS_TRUCK`): Coastal yacht hauler flatbed trailer with twin axle trailer and luxury white/cyan sport boat (`TILE_OBS_BEACH_TRUCK_LRG` 408).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Retro turquoise woody surfer wagon with woodgrain side panels and rooftop surfboard (`TILE_OBS_BEACH_CIVILIAN_LRG` 424).
    - `MAP_WINTER` (Palette Bank 11):
      - Visuals: Frozen alpine pass with white snowbanks, curved snowdrifts, snow-capped evergreen pine trees, ice-blue curbs, and falling 8x8 crystal snowflakes (`DEBRIS_SNOWFLAKE`).
      - Low Barrier (`OBS_LOW_BARRIER`): Glacial ice ridge barrier with sharp ice spires and red hazard marker flags (`TILE_OBS_WINT_ICE_LRG` 440, `_PASS` 448).
      - High Sign (`OBS_HIGH_SIGN`): Ski gondola cableway gantry with steel pulleys, suspension cables, snowcaps, and auroral pass indicators (`TILE_OBS_WINT_CABLE_LRG` 456, `_PASS` 472).
      - Heavy Truck (`OBS_TRUCK`): Municipal orange snowplow truck with angled steel plow blade, warning beacons, and salt spreader (`TILE_OBS_WINT_TRUCK_LRG` 488).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Alpine 4x4 expedition SUV with dark forest green chassis, roof ski rack with skis, and auxiliary rally lights (`TILE_OBS_WINT_CIVILIAN_LRG` 504).
    - `MAP_ORBITAL` (Palette Bank 12):
      - Visuals: Aerospace launchpad apron with grooved concrete slabs, hazard caution stripes, and launch service umbilical towers with warning beacons.
      - Low Barrier (`OBS_LOW_BARRIER`): Cryogenic rocket propellant umbilical fuel pipe trench (`TILE_OBS_ORBIT_PIPE_LRG` 520, `_PASS` 528).
      - High Sign (`OBS_HIGH_SIGN`): Launchpad service umbilical gantry arm with overhead clearance and status lights (`TILE_OBS_ORBIT_GANTRY_LRG` 536, `_PASS` 552).
      - Heavy Truck (`OBS_TRUCK`): Heavy crawler rocket booster transporter with steel tread chassis (`TILE_OBS_ORBIT_CRAWLER_LRG` 568).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Autonomous 6-wheel aerospace security rover (`TILE_OBS_ORBIT_ROVER_LRG` 584).
    - `MAP_GOTHIC` (Palette Bank 13):
      - Visuals: Haunted Transylvanian mist route with dark mossy cobblestones, spiked wrought-iron cemetery fences, and stone mausoleum crypts.
      - Low Barrier (`OBS_LOW_BARRIER`): Ancient crumbling tombstone and crypt slab (`TILE_OBS_GOTH_CRYPT_LRG` 600, `_PASS` 608).
      - High Sign (`OBS_HIGH_SIGN`): Spiked wrought-iron cemetery gateway archway with gothic lanterns (`TILE_OBS_GOTH_GATE_LRG` 616, `_PASS` 632).
      - Heavy Truck (`OBS_TRUCK`): Black gothic funeral hearse with chrome landau bars (`TILE_OBS_GOTH_HEARSE_LRG` 648).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Phantom hot-rod jalopy with exposed chrome engine headers (`TILE_OBS_GOTH_JALOPY_LRG` 664).
      - Signature Hazard (`OBS_SIGNATURE`): 3 Small Stone Gargoyles spanning every lane on road asphalt (`TILE_OBS_SIG_GOTHIC` 944, requires `UP` hurdle jump).
    - `MAP_MAYA` (Palette Bank 14):
      - Visuals: Ancient jungle ruins trail with emerald rainforest canopy, ancient flagstone trail, and carved stone serpent idol totems.
      - Low Barrier (`OBS_LOW_BARRIER`): Carved ancient stone stela totem with serpent carvings (`TILE_OBS_MAYA_TOTEM_LRG` 680, `_PASS` 688).
      - High Sign (`OBS_HIGH_SIGN`): Ruined stone aqueduct arch with hanging rainforest vines (`TILE_OBS_MAYA_ARCH_LRG` 696, `_PASS` 712).
      - Heavy Truck (`OBS_TRUCK`): 6x6 canvas safari expedition hauler truck (`TILE_OBS_MAYA_TRUCK_LRG` 728).
      - Civilian Traffic (`OBS_CIVILIAN_CAR`): Open-top safari 4x4 expedition jeep with front winch and snorkel (`TILE_OBS_MAYA_JEEP_LRG` 744).
      - Signature Hazard (`OBS_SIGNATURE`): Ancient Poison Dart Trap Volley with flanking serpent blowpipe totems firing jade darts (`TILE_OBS_SIG_MAYA` 960, requires `DOWN` duck/shrink slide).
