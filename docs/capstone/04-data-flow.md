---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/main.c"
  - ":(top)nitrosurfer/source/game.c"
  - ":(top)nitrosurfer/source/render.c"
---

# 04 Data Flow

## Lifecycles

### 1. Main Game Loop Frame Lifecycle (60 Hz)
- **Hop 1: Key Poll & Input Buffering** (`nitrosurfer/source/main.c:48`):
  - Reads `REG_KEYINPUT` via libgba `scanKeys()`, `keysDown()`, `keysHeld()`.
  - Feeds key events to `player_handle_input()` (`nitrosurfer/source/game.c:134`).
  - Buffers directional or action taps for up to 6 frames in `g_player.buffered_key`.
- **Hop 2: State Machine Update** (`nitrosurfer/source/game.c:43`):
  - In `STATE_TITLE`: Waits for `KEY_START` to initialize fresh run.
  - In `STATE_PLAY`:
    - `player_update()`: Updates lane lerping, jump parabolic velocity, drift timers, and decay timers.
    - `obstacles_update()`: Advances obstacle $Z$ towards player, calculates screen projection, recycles off-screen hazards.
    - `coins_update()`: Moves coins, computes smooth vector magnetic attraction toward `g_player.x`.
    - `powerups_update()`: Moves active powerup capsules.
    - `spawner_update()`: Ticks wave counters and triggers choreographed obstacle patterns and coin arcs.
- **Hop 3: Collision Resolution** (`nitrosurfer/source/game.c:486`):
  - Tests player bounding box against obstacle array.
  - Resolves clearance: Barrier jumped over awards hurdle chime and bonus; Overhead sign slid under passes; Semi-truck triggers crash or shield loss.
  - Collects coins and items, triggering audio chirps and timer refreshes.
- **Hop 4: Render Pass** (`nitrosurfer/source/render.c:268`):
  - Renders HUD text string into Background Layer 1 via TTE.
  - Calculates road strip horizontal scanline shifts.
  - Populates local `s_obj_buffer` in strict priority order (Player Car -> Obstacles -> Coins -> Powerups -> Shadow).
- **Hop 5: Hardware Sync & DMA** (`nitrosurfer/source/render.c:475`):
  - Calls `VBlankIntrWait()` to sleep until scanline 160.
  - DMA copies `s_obj_buffer` into hardware OAM (`0x07000000`) during blanking interval.

### 2. Roadblock Hurdle Interaction Flow
- **Spawn**: Spawner wave 0 instantiates `OBS_LOW_BARRIER` at $Z=40.0\text{fp}$ in player lane.
- **Approach**: Obstacle approaches at current game speed; projected size scales from small ($16\times 16$) to large ($32\times 16$).
- **Jump Execution**: Player presses `KEY_A`. `vy` is set to $4.5\text{fp}$, car pitches nose-up (`TILE_PLAYER_JUMP_UP`), cyan whoosh ribbon spawns beneath tires.
- **Apex & Crossing**: When barrier passes player ($Z \le 12.0\text{fp}$), car is at $Y \ge 8\text{px}$.
- **Hurdle Award**: `check_collisions()` marks obstacle `hurdled = true`, sets `g_player.hurdle_timer = 28`, adds $+150$ points, and calls `sfx_hurdle()`.
- **Feedback Render**: HUD displays `[HURDLE!]` in gold yellow; car renders in front of barrier via OAM index 0 priority.
