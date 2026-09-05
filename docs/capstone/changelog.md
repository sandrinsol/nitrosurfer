# Changelog

## feat/obs-slick-lateral-slide
- Surface Slick (`OBS_SLICK`) complete rework:
  - Removed 60% speed reduction penalty; full vehicle speed is retained through the slick contact.
  - Ground contact triggers an uncontrolled lateral slide to an adjacent lane (Center lane randomly slides to Left or Right; Left and Right edge lanes slide inward to Center).
  - Smooth 24-frame interpolation physics (`step = TO_FP(2)`) across 48px lane delta, settling vehicle on target lane as timer expires.
  - Added directional 360° spinout animation (Clockwise when sliding right, Counter-Clockwise when sliding left).
  - Locked steering controls during slide with twin rear tire smoke/sparks debris and screeching tire SFX.
  - Safe avoidance preserved for jumping over (`UP`) or smashing through with Nitro.

## feat/signature-hazards-gothic-and-maya
- Maya Temple Trail Signature Hazard Replacement:
  - Replaced redundant "Rolling Sun-Stone Boulder" with the **Ancient Poison Dart Trap Volley** (`TILE_OBS_SIG_MAYA` 960).
  - Flanking carved stone serpent blowpipe totems fire a rapid horizontal volley of jade poison darts across the lane at vehicle head height ($y = 6 \dots 14$).
  - Requires ducking into compact slipstream chassis (`DOWN` / `B`) to slide safely underneath (or smashing through with Nitro).
- Gothic Midnight Signature Hazard Rework:
  - Replaced swooping single overhead gargoyle with **3 Small Stone Gargoyles** spanning all 3 lanes (lanes 0, 1, 2) resting on road asphalt (`TILE_OBS_SIG_GOTHIC` 944).
  - Requires aerial hurdle clearance (`UP`) across the barrier row.

## fix/remove-winter-snowfall
- Winter Snow track: removed falling snowflake particles (`DEBRIS_SNOWFLAKE`) during gameplay for clean road visibility.

## fix/high-sign-shrink-and-barrier-spawn
- `OBS_HIGH_SIGN` Collision Fix:
  - Players must now actively shrink / crouch (`B` / `DOWN`) to slide under the overhead gantry.
  - Passing through at normal vehicle height without shrinking now correctly registers as a crash (`hit = true`).
  - Jumping into the gantry continues to register an airborne collision wipeout.
- `OBS_LOW_BARRIER` Clean Spawning:
  - Removed extraneous duplicate barrier placed behind `OBS_SLICK` in wave pattern 5. Pattern 5 is now dedicated exclusively to the surface slick.
  - Updated wave spawner lane avoidance horizon check (`blocked_mask`) from $Z > 50$ to $Z > 20$, guaranteeing that new obstacles never spawn in the same lane as an approaching obstacle ahead of the player.

## feat/car-select-2000g-unlock
- Streamlined Car Select Screen (`STATE_TITLE`):
  - Bottom box contains exclusively `COINS COLLECTED: %06d`, centered vertically and horizontally ($X = 28, Y = 144$).
  - Removed High Score banner from car selection showroom.
  - Added 2000 Gold vehicle purchasing system:
    - Red and Blue cars are unlocked by default.
    - Pink, Black, Yellow, and Green cars require 2000 Gold to unlock.
    - Added `2000 G` price tags framed on BG1 dark plates under locked car showroom pedestals.
    - Persistent bitmask in SRAM byte 12 tracks unlocked vehicles across boots.
    - Attempting to select locked car without 2000 gold triggers error skid sound and blocks progression.
    - Pressing A with sufficient gold deducts 2000 gold, sets bitmask, plays fanfare sound, and removes price tag.

## map/all@9bd597c
- Built architecture reference index `00-index.md`
- Added topic chapters: `01-architecture.md`, `02-models.md`, `03-conventions.md`, `04-data-flow.md`, `05-dependencies.md`, `06-testing.md`, `07-operations.md`, `08-glossary.md`
- Extracted business logic scenarios: `01-lane-movement.md`, `02-jumping-and-hurdles.md`, `03-sliding-and-drift.md`, `04-obstacle-spawning.md`, `05-coin-magnet-and-items.md`
- Extracted UI/UX surface maps: `01-direction.md`, `02-system.md`, screens (`01-title.md`, `02-gameplay.md`, `03-gameover.md`)

## review/all@9bd597c
- Evaluated 5 backend dimensions: boundary integrity, failure-path risk, audio hardware coupling, spatial efficiency, conventions (0 Critical, 2 Important, 2 Minor)
- Evaluated 3 frontend surfaces: Title, Gameplay HUD, Game Over (0 Critical, 2 Important, 2 Minor)
- Live emulator browser harness exercised
- Produced `docs/capstone/review.md`

## fix/review-findings@9bd597c
- Resolved all 8 review findings (4 backend, 4 frontend):
  - Audio: channel priority locks on Ch1 and Ch2; engine hum no longer clips sweeps; hurdle cancels coin note 2.
  - Spatial partitioning: Z threat-zone early exit in `check_collisions()` saving frame budget on ARM7TDMI.
  - Camera struct: defined typed `Camera` in `include/common.h` and `source/game.c`.
  - Sprites: added high-contrast amber cab roof marker lights to small and large semi-trucks for instant long-range identification.
  - UI/UX: configured GBA hardware blending (`REG_BLDCNT`) for 50% background black fade on Game Over.
  - Rewards: added flashing `*** NEW HIGH SCORE! ***` celebration badge and multi-note fanfare (`sfx_high_score()`).
  - Safety: clamped `coins_collected` at 999 preventing HUD text overflow.

## review/all@9bd597c#2
- Evaluated 5 backend dimensions: state machine integrity, audio semantics, entropy/determinism, coordinate domain consistency, conventions (0 Critical, 2 Important, 2 Minor)
- Evaluated 3 frontend surfaces: Title, Gameplay HUD, Game Over (0 Critical, 2 Important, 2 Minor)
- Live emulator browser harness exercised
- Replaced `docs/capstone/review.md` with updated findings

## fix/review-findings#2@9bd597c
- Resolved all 8 review findings from batch #2 (4 backend, 4 frontend):
  - State machine: implemented `STATE_PAUSE` toggle on `KEY_START` with frozen physics, timers, and spawner.
  - Audio: implemented `sfx_touchdown()` low-frequency noise rumble on Channel 4; replaced improper `sfx_coin()` call on jump touchdown.
  - PRNG: seeded `srand(g_game.frame_count ^ 0x5A5A)` on game start from title screen dwell time.
  - Vector math: unified magnet horizontal pull in world coordinate domain before perspective projection.
  - UI/UX: configured 50% hardware blending dimming (`REG_BLDCNT`) and modal banner for pause screen.
  - Onboarding: added `PAD:STEER  A:JUMP  B:SLIDE` controls legend to Title Screen without line wrapping.
  - HUD feedback: added 8-frame blinking warning tag when Nitro or Magnet has <= 60 frames remaining.
  - Sprites: added ground shadow (`TILE_SHADOW`) beneath crashed car in Game Over mode.

## review/all@9bd597c#3
- Evaluated 5 backend dimensions: arithmetic precision, audio hardware coupling, persistence, display pipeline synchronization, conventions (0 Critical, 2 Important, 2 Minor)
- Evaluated 4 frontend surfaces: Title, Gameplay HUD, Pause Modal, Game Over (0 Critical, 2 Important, 2 Minor)
- Live emulator browser harness exercised
- Replaced `docs/capstone/review.md` with updated findings

## fix/review-findings#3@9bd597c
- Resolved all 8 review findings from batch #3 (4 backend, 4 frontend):
  - Physics/Math: added `distance_fp` sub-meter 8.8 fixed-point accumulator, resolving integer division truncation so distance meters tick up continuously from run start.
  - Audio: implemented `audio_pause()` and `audio_resume()` cutting Channel 1 volume to 0 during pause.
  - Persistence: added battery-backed cartridge SRAM routines (`sram_load_high_score()` / `sram_save_high_score()`) at `0x0E000000` with `'NITR'` header.
  - UI/UX: updated pause screen to overwrite with blank spaces during blink-off cycle, eliminating font artifacts.
  - Animation: added 1px vertical idling vibration micro-bounce to title screen car sprite.

## review/all@9bd597c#4
- Evaluated 5 backend dimensions: boundary integrity, dead assets, typing erosion, audio arbitration, conventions (0 Critical, 2 Important, 2 Minor)
- Evaluated 4 frontend surfaces: Title, Gameplay HUD, Pause Modal, Game Over (0 Critical, 2 Important, 2 Minor)
- Live emulator browser harness exercised
- Replaced `docs/capstone/review.md` with updated findings

## fix/review-findings#4@9bd597c
- Resolved all 8 review findings from batch #4 (4 backend, 4 frontend):
  - Graphics: implemented `gen_spark()` for `TILE_SPARK` (tile 76) providing bright 8x8 incandescent multi-pixel spark clusters.
  - Graphics/Logic: wired `TILE_SHIELD_SPARK` to new `DEBRIS_SHIELD_DISCHARGE` particle spawned on forcefield absorption.
  - Audio: implemented Channel 4 noise priority arbitration (`s_ch4_priority` & `s_ch4_lock_timer`) preventing periodic nitro hums from preempting crash rumbles, tire screeches, or shield pops.
  - Architecture/Typing: replaced integer particle types with strongly-typed `DebrisType` enum.
  - UI/UX: repositioned Game Over `PRESS START TO RETRY` from Y=136 to Y=144, eliminating silver border bisect.
  - UI/UX: repositioned Title Screen controls legend from Y=138 to Y=144, centering within bottom panel.
  - UI/UX: unified Game Over distance formatting to 6-digit zero-padded `DISTANCE: %06dm`.
  - Visuals: enabled hardware blending (`ATTR0_BLEND`) on player car sprite during pause state to match dimmed road background.
  - Verification: 23 of 23 mechanical tests passed with 100% pass rate.

## polish/bottom-hud@9bd597c
- Refined bottom HUD container layout, typography, and hardware rendering:
  - Vertical alignment: resolved 4px displacement on wheel icon (`(192, 149)`), perfectly synchronizing it with speed text (`P:204,149`) on identical scanlines $149..156$.
  - Vertical alignment: synchronized coin icon (`(192, 139)`) and coin count (`P:204,139`) on identical scanlines $139..146$.
  - Horizontal symmetry: balanced left margin ($X = 12$) and right margin ($X = 12$) symmetrically.
  - Vertical geometry: achieved pixel-perfect 3px top margin, 2px inter-line separation, and 3px bottom margin across the 24-pixel container.
  - Bevel finish: implemented dual-tone metallic top border (Row 0 = Silver Chrome, Row 1 = Dark Charcoal Bevel, Rows 2..7 = Pitch Black).
  - Unification: unified `render_hud(&spr_idx)` across active gameplay and pause states.
  - Verification: 23 of 23 mechanical tests passed with 100% pass rate.

## polish/centered-hud-kmh@9bd597c
- Centered bottom UI, aligned text with signs, and displayed speed in km/h:
  - Horizontal centering: shifted HUD elements inward from screen edges (Left block at $X = 20..132$, Right block at $X = 142..218$), achieving 20px / 22px balanced side margins centered under the highway lanes.
  - Vertical sub-tile centering: configured `REG_BG0VOFS = 253` during gameplay and pause, scrolling text down 3px so Line 1 and Line 2 sit with equal 4px/5px padding in the 24px container.
  - Sign & metric alignment: coin icon (`(142, 139)`) perfectly aligns with coin count (`154, 136`), and wheel icon (`(142, 147)`) perfectly aligns with speed text (`154, 144`) on identical vertical scanlines.
  - Metric units: formatted speed with explicit unit suffix `%3d km/h` (e.g. `112 km/h`).
  - Verification: 23 of 23 mechanical tests passed with 100% pass rate.

## rework/animation-overhaul-shrink-fire-steel@9bd597c
- Animation Rework & Slide-to-Shrink Architecture Overhaul:
  - Jump spiral removal: completely removed `TILE_JUMP_WHOOSH` sprite allocation and rendering, leaving the airborne car clean with only the compact asphalt tarmac shadow (`TILE_SHADOW_SML`) underneath.
  - Slide $\to$ Shrink refactor: renamed all struct fields, functions, audio routines, feedback timers, and UI strings from `slide` to `shrink` (`is_shrinking`, `shrink_timer`, `shrink_boost_timer`, `shrink_clear_timer`, `player_shrink()`, `sfx_shrink()`, and `B:SHRINK` on Title Screen).
  - Shrinking visuals overhaul: replaced static midsize racer with an agile multi-stage cyber nano-racer (`TILE_PLAYER_SHRINK_0` compression transition and `TILE_PLAYER_SHRINK_1` compact nano-chassis with cyan nano-exhaust micro-jets and a 2px suspension crouch dip).
  - Nitro exhaust fire rework: replaced blue plasma with 4 animated frames of roaring incandescent exhaust fire (`TILE_NITRO_FIRE_0..3` in Sprite Palette 2: white-hot core, vivid golden yellow, blazing orange plume, and red ember tips) erupting directly behind the dual chrome tailpipes.
  - Shield steel coat overhaul: replaced generic green forcefield bubble with a 4-frame heavy steel armor coat overlay (`TILE_STEEL_COAT_F0..F3` in Sprite Palette 2: metallic chrome plates, flared armored wheel arches, window louvers, reinforced bumper rivets, and sweeping specular metallic chrome sheen) encased directly over the car body.
  - Automated verification: 23 of 23 mechanical tests passed with 100% pass rate.

## polish/clean-nitro-animated-grey-shield@9bd597c
- Refined Nitro Exhaust Fire & Animated Grey Car Shield:
  - Nitro visual declutter: removed warp speed streak sprites (`TILE_SPEED_STREAK`) on the car's flanks, leaving the dual incandescent exhaust fire plumes completely clean and prominent behind the tailpipes.
  - Animated grey car shield: replaced the overlay sprite with dynamic palette animation. The car sprite itself transforms into a sleek, metallic gunmetal grey (Sprite Palette Bank 3), cycling across 4 animated frames of polished chrome and titanium specular gleam.
  - Seamless animation parity: all player animations (straight driving, steering left/right, jumping, and compact shrinking) naturally adopt the animated metallic grey coat while shielded.
  - Automated verification: 23 of 23 mechanical tests passed with 100% pass rate.

## review/all@9bd597c#5
- Evaluated 5 backend dimensions: boundary integrity, dead assets, typing erosion, audio arbitration, conventions (0 Critical, 1 Important, 0 Minor)
- Evaluated 4 frontend surfaces: Title, Gameplay HUD, Pause Modal, Game Over (0 Critical, 0 Important, 0 Minor)
- Live emulator browser harness exercised
- Replaced `docs/capstone/review.md` with updated findings

## fix/prune-dormant-tiles-nitrosurfer-branding@9bd597c
- Resolved all findings from review #5 and applied "Nitro Surfer" branding:
  - Graphics optimization: deleted dormant generators `gen_speed_streak()` and `gen_steel_coat_frame()` from `nitrosurfer/source/gfx_data.c`, and unhooked their calls from `gfx_init_all()`.
  - Tile memory cleanup: reclaimed 68 dormant sprite tile indices (2,176 bytes of tile memory), remapped `TILE_SHIELD_EXPAND` to tile 272 and `TILE_SHIELD_SPARK` to 276.
  - Game branding overhaul: standardized game title as "Nitro Surfer" across ROM header (`-t"NITRO SURFER"` and `-cNITR`), build targets (`nitrosurfer.gba` / `nitrosurfer.elf`), test harness (`mechanical_tests.mjs`), HTML emulator wrapper (`index.html`), operations manual (`07-operations.md`), and `README.md`.
  - Mechanical verification: 23 of 23 mechanical tests passed with 100% pass rate.

## fix/invisible-crashes-and-gantry-clearance@9bd597c
- Resolved "Invisible Crash" defects and OAM sprite buffer leak:
  - Overhead highway gantry (`OBS_HIGH_SIGN`): Refactored clearance logic. A sports car driving at road level passes cleanly through the archway (`g_obstacles[i].passed = true;`), eliminating the sudden crash on an apparently empty road. If airborne (`is_jumping && y_offset > 4`), the car hits the overhead girder; if shrinking (`is_shrinking`), the car earns the slipstream bonus (+150 pts, clearance popup, sfx).
  - Trailing / grazed obstacle safety: In `check_collisions()`, obstacles with `dx >= hit_width` now mark `g_obstacles[i].passed = true;` whenever `z_int <= 14`, and any obstacle with `z_int < 12` is unconditionally marked `passed = true;`. This prevents close-call grazed obstacles from triggering phantom rear collisions if the player subsequently turns into their lane.
  - Fast drop crouch activation: Updated `player_shrink()` so pressing DOWN while airborne triggers fast touchdown AND engages `is_shrinking = true` for 50 frames, ensuring seamless protection when landing near overhead signs.
  - OAM sprite buffer leak: In `nitrosurfer/source/render.c`, moved `OBJ_ATTR *sp = &s_obj_buffer[spr_idx++];` to execute only *after* the off-screen check (`screen_y - oy >= 135`), eliminating phantom stale sprite leaks and visual ghosting.
  - Automated verification: Verified with targeted emulator reproducer (`test_sign.mjs`), sign girder jump collision test (`test_sign_jump.mjs`), and 3,000-frame continuous bot playthrough (`bot_test.mjs`) surviving with zero crashes.

## fix/hud-bottom-layer-isolation@9bd597c
- Pushed bottom UI layer to strictly top-most rendering layer across all GBA hardware channels:
  - BG Layer Architecture: Moved HUD container box from `BG2` (road) to dedicated `BG1` running at hardware Priority 0 (`BG_PRIO(0)`).
  - Hardware Window Masking: Configured GBA hardware Window 0 (`REG_WIN0H`/`REG_WIN0V`, `REG_WININ`/`REG_WINOUT`) to mask out `BG2` (highway road, shoulders, and markings) in the bottom 24 pixels (`Y: 136..160`).
  - Screen Shake & Dimming Immunity: Isolated `BG1` (HUD) from camera shake offsets (`g_game.screen_shake`) and blend register dimming (`BLDCNT_TGT1_BG1` omitted), guaranteeing the HUD container and typography remain stationary, bright, and rock-solid.
  - Strict Sprite Priority Reordering:
    - Set HUD OAM sprites (Coin icon, Speedometer gear icon) to Priority 0.
    - Set airborne elements (overhead gantry signs, jumping car, score popups) to Priority 1.
    - Set road-level gameplay elements (ground car, barriers, civilian sedans, semi-trucks, turn signals, coins, powerups, debris sparks) to Priority 2.
    - Set drop shadows to Priority 3.
  - Bottom Culling Threshold: Raised obstacle, coin, powerup, and debris culling boundary to `136px`, ensuring entities smoothly occlude behind the metallic border and never bleed or overlap the UI text.
  - Automated Verification: Verified via emulator harness scripts (`verify_hud_layer_topmost.mjs`, `test_obstacle_occlusion.mjs`, `bot_test.mjs`), confirming all obstacles and sprites pass strictly behind the bottom UI container.

## review/all@9bd597c#6
- Evaluated 6 backend dimensions: boundary integrity, dead or unwired code, failure-path risk, internal consistency, test maintenance, conventions (0 Critical, 1 Important, 2 Minor)
- Evaluated 4 frontend surfaces: Title, Gameplay HUD, Pause Modal, Game Over (0 Critical, 1 Important, 1 Minor)
- Live emulator and browser harness exercised
- Replaced `docs/capstone/review.md` with updated findings

## fix/review-findings#6@9bd597c
- Resolved all 5 findings from review #6 across backend, frontend, documentation, and copy:
  - Pruned unwired countdown timers: Deleted `shrink_clear_timer`, `dodge_timer`, `nitro_pickup_timer`, and `shield_pickup_timer` from `Player` struct in `nitrosurfer/include/common.h` and removed all frame decrements and event assignments in `nitrosurfer/source/game.c`, saving 16 bytes of IWRAM and CPU cycles.
  - Low barrier hurdle velocity leeway: Updated `nitrosurfer/source/game.c` obstacle collision logic to grant hurdle clearance if either `FP_TO_INT(y_offset) >= 8` OR `is_jumping && vy > TO_FP(2.0f)`, eliminating takeoff latency clipping during high-speed leaps.
  - Architecture documentation sync: Updated `docs/capstone/01-architecture.md` to document the 3-layer video layout (`BG0` TTE text, `BG1` HUD box, `BG2` 3-lane road) and `REG_WIN0` hardware masking.
  - UI/UX specification sync: Updated `docs/capstone/uiux/02-system.md` and `docs/capstone/uiux/screens/02-gameplay.md` to reflect the bottom HUD bar on `BG1`, `REG_WIN0` isolation, sub-tile text centering (`REG_BG0VOFS = 253`), OAM icon tokens (Coin and Wheel), and floating score popups.
  - Interface copy refinement: Removed `🏎️` from `<h1>` in `index.html` in compliance with §7 interface copy standards.
  - Verification: Clean build generated, symbol map validated, 3,000-frame automated smart bot test survived without crash, targeted hurdle takeoff test passed, and all 4 visual surfaces re-verified via live screenshots.

## feat/car-color-selection@9bd597c
- Added interactive Title Screen Garage Showroom for car chassis paint customization:
  - 6 Supercar Color Themes: Defined custom 15-bit BGR555 hardware palettes for Red (Cherry Red), Blue (Cobalt Blue), Pink (Neon Pink), Black (Stealth Black), Yellow (Racing Yellow), and Green (Viper Green), each with tuned base paint, shadow shading, and gloss reflection highlight colors.
  - Procedural UI Assets: Generated 8x8 rounded glossy chiclet tiles (`TILE_COLOR_SWATCH`) and 8x8 golden chevron cursor indicator tiles (`TILE_COLOR_SELECTOR`).
  - Hardware Palette Bank Mapping: Dedicated Sprite Palette Banks 4..9 (`pal_obj_mem[64..159]`) for simultaneous true-color rendering of all 6 paint chiclets, while Bank 0 dynamically receives the active car palette via DMA (`gfx_set_car_color()`).
  - Interactive Garage Showroom Title UI:
    - Real-time car preview with dynamic engine idling vibration at $(104, 36)$ updating immediately upon color selection.
    - Pulsing chevron color banner at $Y=70$ displaying `<< COLOR NAME >>` with animated chevrons.
    - 6 color chiclets at $Y=82$ spaced across $X \in [76, 156]$ with active chiclet 2px vertical elevation ($Y=80$).
    - Golden chevron cursor indicator (`^`) anchored at $Y=90$ directly beneath the active chiclet.
    - D-pad Left/Right and L/R shoulder button navigation with bidirectional wraparound and audio chime feedback.
    - Zero-friction race start (`KEY_START` or `KEY_A`), preserving complete compatibility with automated test drivers.
    - Symmetrical, balanced onboarding controls legend in bottom beveled metallic HUD box.
  - Gameplay State Color Persistence: Chosen color persists seamlessly across driving, jumping, nano-chassis shrinking, collisions, Game Over screens, and race restarts.
  - Automated Verification: Executed `test_color_selection.mjs` verifying 6-color carousel cycling, wraparound in both directions, and race launch in Cobalt Blue; executed 3,000-frame continuous bot test (`bot_test.mjs`) surviving without crash.

## feat/3x2-car-grid-selection@9bd597c
- Transformed Title Screen Car Selection into a 3x2 Grid displaying whole cars:
  - 3x2 Grid Display Cases: Structured 6 showroom display boxes across 3 columns and 2 rows on BG1 Screen Block 30 (`se_mem[30]`), with 8x5 tiles per box (64x40 pixels) perfectly centered across screen coordinates.
  - Whole Car Presentation: Replaced color swatches with full 32x32 player supercar sprites (`TILE_PLAYER_STRAIGHT`) inside all 6 display cases simultaneously, rendered in their true hardware paint colors using Sprite Palette Banks 4..9:
    - Box (0, 0): Whole Cherry Red Supercar at (24, 36)
    - Box (1, 0): Whole Cobalt Blue Supercar at (104, 36)
    - Box (2, 0): Whole Neon Pink Supercar at (184, 36)
    - Box (0, 1): Whole Stealth Black Supercar at (24, 84)
    - Box (1, 1): Whole Racing Yellow Supercar at (104, 84)
    - Box (2, 1): Whole Viper Green Supercar at (184, 84)
  - Visual Focus & Card Highlighting:
    - Unselected boxes rendered with sleek metallic silver/chrome bevel borders (`TILE_HUD_BOX_*`, Pal 2) and pitch black interiors.
    - Active selection highlighted with an illuminated arcade-gold glowing double-border and crisp white inner rim (`TILE_SEL_BOX_*`, Pal 0).
    - Selected car rumbles with a 1px vertical engine idle bounce (`(g_game.frame_count & 16)`).
  - 2D D-Pad Navigation:
    - `KEY_LEFT` / `KEY_RIGHT`: Navigates horizontally through cars with wraparound.
    - `KEY_UP` / `KEY_DOWN`: Navigates vertically between Row 0 and Row 1 while preserving column position.
    - Sound effect chime (`sfx_coin()`) triggered on every move.
  - Seamless Transition: `render_clear_title_boxes()` zeroes rows 0..16 on BG1 upon game start, leaving only the bottom HUD box.
  - Automated Verification: Created `test_3x2_grid.mjs` verifying 2D navigation (UP/DOWN/LEFT/RIGHT across all 6 grid cells), clean transition to `STATE_PLAY`, and racing with the chosen car.

## feat/maps-beach-winter-and-map-selection@9bd597c
- Added 2 new track map themes (Palm Beach, Winter Snow) and a Track Map Selection screen (3 boxes in one row):
  - Track Map Selection Screen (`STATE_SELECT_MAP`):
    - Presented after car selection on Title Screen upon pressing `A` or `START`.
    - Features 3 side-by-side preview cases in 1 row on BG1 Screen Block 30 (`se_mem[30]`), spanning rows 4..10 (56px tall) across columns 2..9 (City), 11..18 (Beach), and 20..27 (Winter).
    - Multi-Palette Simultaneous Previews: BG Palette Bank 2 (City Metropolis), BG Palette Bank 3 (Palm Beach), and BG Palette Bank 4 (Winter Snow) render authentic preview thumbnails:
      - City Preview: Skyscraper silhouettes, glowing window grids, dark road asphalt.
      - Beach Preview: Coastal sunny sky, roadside tropical palm trees, turquoise ocean waves, golden beach sand.
      - Winter Preview: Alpine twilight sky, snow-capped mountain peaks, snowy evergreen pines, pure white snowbanks.
    - Active card illuminated with an arcade-gold glowing double-border (`TILE_SEL_BOX_*`, Pal 0); unselected cards use themed borders (`TILE_HUD_BOX_*`).
    - Pulsing chevron track title banner at $Y=16$ (`<< CITY HIGHWAY >>`, `<< PALM BEACH >>`, `<< WINTER SNOW >>`).
    - Centered box labels at $Y=90$ (`[CITY]`, `[BEACH]`, `[WINTER]`) with active track highlighting.
    - Dedicated dark panel on BG1 ($Y \in [96, 127]$) framing the subtitle and blinking prompt `> PRESS START TO RACE <` against the road.
    - Symmetrical bottom legend: `PAD: CHOOSE MAP  A: GO` / `B: BACK TO CARS  START: GO`.
    - D-Pad `LEFT` / `RIGHT` cycles maps with bidirectional wrap and audio chimes (`sfx_coin()`).
    - `KEY_B` returns cleanly to Car Selection screen (`sfx_pause()`), clearing map preview cases.
  - Palm Beach Track Theme (`MAP_BEACH`):
    - Custom 15-bit BGR555 hardware palette (`bg_pal_beach`) with golden sand dunes, turquoise ocean surf, sunbaked asphalt, and coral red/sandstone curbs.
    - Left shoulder: Turquoise ocean waves and golden sand dunes.
    - Right shoulder: Golden beach sand with roadside tropical palm trees with lush green fronds and brown bark trunks.
  - Winter Snow Track Theme (`MAP_WINTER`):
    - Custom 15-bit BGR555 hardware palette (`bg_pal_winter`) with pure powder white snow, ice crystal cyan, frosted dark asphalt, and ice-blue curbs.
    - Shoulders: White snowbanks, curved snowdrifts, and snow-covered evergreen pine trees.
    - Dynamic Weather FX: Procedural 8x8 crystal snowflake sprites (`DEBRIS_SNOWFLAKE`) drifting and fluttering downward across the highway with sine wobble physics.
  - Automated Verification: Created `test_map_selection.mjs` verifying map select screen navigation, cycling between City, Beach, and Winter, B button return to car selection, and in-game gameplay verification of Palm Beach and Winter Snow tracks.

## review/all@9bd597c#7
- Evaluated 7 backend dimensions: boundary integrity, dead or unwired code, typing erosion, failure-path risk, test maintenance, internal consistency, conventions (1 Critical, 1 Important, 1 Minor)
- Evaluated 5 frontend surfaces: Title Screen (3x2 Car Grid), Track Map Selection (3 Boxes), Gameplay Highway HUD, Pause Screen, Game Over Screen (0 Critical, 1 Important, 1 Minor)
- Live GBA emulator harness and browser runtime exercised with screenshot capture
- Replaced `docs/capstone/review.md` with updated opinionated findings

## fix/review-findings#7@9bd597c
- Resolved all 5 findings from review #7 across backend, frontend, graphics memory, and documentation:
  - Anchored `GameStateEnum` integer values (`STATE_TITLE=0, STATE_PLAY=1, STATE_PAUSE=2, STATE_GAMEOVER=3, STATE_SELECT_MAP=4`) in `nitrosurfer/include/common.h`, restoring complete ABI compatibility and unpause logic for test harnesses.
  - Eliminated typing erosion by replacing raw integer literal `3` with strongly typed `DEBRIS_POPUP_100` in `nitrosurfer/source/game.c:938`.
  - Implemented `gfx_restore_title_palette()` in `nitrosurfer/source/gfx_data.c` and wired it to `STATE_SELECT_MAP` and `STATE_GAMEOVER` return paths, preventing stale track palette leakage into Title/Map Selection thumbnail previews.
  - Added `KEY_B` navigation handler on `STATE_GAMEOVER` in `nitrosurfer/source/game.c`, allowing players to return directly to the 3x2 Car Selection Grid with vehicle paint choice preserved.
  - Synchronized UI/UX design documentation in `docs/capstone/uiux/screens/03-gameover.md` and `nitrosurfer/source/render.c` to reflect `** CRASHED! **` and `START: RETRY   B: TITLE`.
  - Verification: Clean GBA compilation, 3x2 grid selection test passed (`test_3x2_grid.mjs`), track map selection suite passed (`test_map_selection.mjs`), Game Over B-button navigation & palette test passed (`test_gameover_return.mjs`), and 1,764-frame continuous bot run survived (`bot_test.mjs`).

## feat/side-coin-pickup@9bd597c
- Fixed coin and powerup pickup hitboxes when moving on the side or transitioning lanes:
  - Expanded Z collection window in `coins_update()` and `powerups_update()` from narrow 8-unit range (`Z=12..20`) to full 32px vehicle body length (`Z=8..24`), allowing side doors and rear quarter panels to catch coins as they sweep across lanes.
  - Increased lateral pickup radius `pickup_rad` from restrictive 16px to full physical contact width (24px resting, 26px when moving laterally or steering), covering the complete 32px car width + 16px coin width.
  - Automated Verification: Created `test_side_coin_catch.mjs` verifying fender grazing catch (`dx = 22px`) and dynamic lateral lane switch coin collection (Lane 1 -> Lane 0); executed 1,565-frame continuous bot test (`bot_test.mjs`) with zero regressions.
## feat/map-expansion-6maps@b49e21f
- Added 3 new full-featured map environments to Nitro Surfer: Cape Orbital, Gothic Midnight, and Maya Temple Trail:
  - Cape Orbital (`MAP_ORBITAL`, BG Palette Bank 5, Sprite Palette 12):
    - Aerospace rocket launchpad tarmac with grooved concrete slabs, yellow hazard caution striping, and launch service umbilical towers with blinking warning beacons.
    - 4 Bespoke Thematic Obstacles: Cryogenic rocket propellant umbilical fuel pipe trench (`TILE_OBS_ORBIT_PIPE_*` 520..535), launchpad service umbilical gantry arm (`TILE_OBS_ORBIT_GANTRY_*` 536..567), heavy crawler booster transporter (`TILE_OBS_ORBIT_CRAWLER_*` 568..583), and autonomous 6-wheel security rover (`TILE_OBS_ORBIT_ROVER_*` 584..599).
  - Gothic Midnight (`MAP_GOTHIC`, BG Palette Bank 6, Sprite Palette 13):
    - Haunted Transylvanian cobblestone route with spiked wrought-iron cemetery fences, weathered stone mausoleums, and full blood moon mist.
    - 4 Bespoke Thematic Obstacles: Crumbling stone tombstone & crypt slab (`TILE_OBS_GOTH_CRYPT_*` 600..615), wrought-iron cemetery gateway arch (`TILE_OBS_GOTH_GATE_*` 616..647), black gothic funeral hearse (`TILE_OBS_GOTH_HEARSE_*` 648..663), and phantom hot-rod jalopy (`TILE_OBS_GOTH_JALOPY_*` 664..679).
  - Maya Temple Trail (`MAP_MAYA`, BG Palette Bank 7, Sprite Palette 14):
    - Ancient rainforest ruins route with emerald jungle canopy, ancient flagstone pavers, and carved stone serpent idol totems.
    - 4 Bespoke Thematic Obstacles: Ancient carved stone stela totem (`TILE_OBS_MAYA_TOTEM_*` 680..695), ruined stone aqueduct arch with hanging rainforest vines (`TILE_OBS_MAYA_ARCH_*` 696..727), 6x6 canvas expedition safari hauler (`TILE_OBS_MAYA_TRUCK_*` 728..743), and open-top safari 4x4 expedition jeep (`TILE_OBS_MAYA_JEEP_*` 744..759).
- Modernized Track Selection Screen to 3x2 Grid on BG1 (`se_mem[30]`):
  - 6 preview cases arranged in a 3x2 grid matching the supercar showroom: Row 0 (City, Beach, Winter) across rows 4..8, Row 1 (Cape Orbital, Gothic Midnight, Maya Temple Trail) across rows 10..14.
  - Simultaneous multi-palette authentic pixel art thumbnail previews using BG Palette Banks 2..7.
  - Glowing gold border on active selection, sleek dark backdrop panel on rows 15..16 for centered track feature subtitles, and bottom HUD legend with 2D D-Pad navigation (`LEFT`/`RIGHT` cycle, `UP`/`DOWN` row toggle).
- Rigorous Automated Testing & Zero Regressions:
  - Created `harness/tests/map_selection_6maps.mjs` verifying full 2D grid selection and OAM sprite palette / tile allocations across all 3 new maps.
  - Verified 100% physics, hitbox, jump clearance, and shrink collision compatibility.

## feat/max-score-9999999@current
- Upgraded maximum score ceiling from 6 digits (999,999) to 7 digits (9,999,999):
  - Updated gameplay score calculation in `nitrosurfer/source/game.c:add_score()` to clamp at `9999999`.
  - Updated SRAM battery-backed persistence validation and clamp bounds in `sram_load_high_score()` and `sram_save_high_score()` for values up to `9999999`.
  - Expanded and centered all high score displays and in-game HUDs in `nitrosurfer/source/render.c`:
    - Start Screen high score badge centered on 240px screen at $X=20$ (`-- HIGH SCORE: %07d --`) with widened 208px backdrop border on BG1.
    - Title Screen high score badge centered on 240px screen at $X=20$.
    - In-Game HUD: re-architected Line 1 (`SCORE:%07d x%d`) and shifted right-column Coin icon to $X=150$ and coin count to $X=162$, ensuring a clean 12px separation with zero text-sprite overlap even at max streak combo (`x5`).
    - Game Over Screen: cleanly spaced Final Score (`FINAL SCORE:  %07d`), Distance, and Coins, with centered `*** NEW HIGH SCORE! ***` at $X=28, Y=94$ positioned completely above the wrecked smoking car ($Y \in [108, 140]$).
  - Automated Verification:
    - Created `harness/tests/max_score_9999999.mjs` confirming in-game clamping at 9,999,999, Game Over high score update, and SRAM reload persistence on cold reboot.
    - Full regression passed: `test_all_obstacles_comprehensive.mjs` (25/25 tests across all 6 tracks), `accumulated_coins.mjs`, `start_screen.mjs`, and `map_selection_6maps.mjs`.



