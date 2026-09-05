---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/include/common.h"
  - ":(top)nitrosurfer/source/game.c"
---

# 02 Models

## Entities

| Entity | Definition Site | Storage | Purpose |
| :--- | :--- | :--- | :--- |
| `Player` | `nitrosurfer/include/common.h:58` | IWRAM (`0x03000150`) | Car state: lane, x/y coords, velocity, timers, status flags |
| `Obstacle` | `nitrosurfer/include/common.h:87` | IWRAM array (`0x03000450`) | Road hazards: type, lane, Z position, screen projection, hurdle flags |
| `Coin` | `nitrosurfer/include/common.h:100` | IWRAM array (`0x03000550`) | Collectible currency: lane, X/Z coords, screen projection, anim frame |
| `WorldPowerup` | `nitrosurfer/include/common.h:110` | IWRAM array (`0x03000630`) | Collectible items: nitro, magnet, shield |
| `GameState` | `nitrosurfer/include/common.h:120` | IWRAM (`0x030001a0`) | Run score, distance, coins, current speed, camera scroll, timers |

## Fields and types

### `Player`
- `lane`: `int` (0=Left, 1=Center, 2=Right)
- `x`: `FIXED` (8.8 fixed-point horizontal position across 240px width)
- `target_x`: `FIXED` (8.8 target lane center)
- `y_offset`: `FIXED` (8.8 vertical jump height above road)
- `vy`: `FIXED` (8.8 vertical jump velocity)
- `is_jumping`: `bool` (airborne flag)
- `is_sliding`: `bool` (low-profile ducking flag)
- `slide_timer`: `int` (frames remaining in slide)
- `tilt`: `int` (-1=left, 0=straight, +1=right banking)
- `shield_active`: `bool` (crash immunity buffer)
- `nitro_timer`: `int` (speed surge and obstacle smash frames)
- `magnet_timer`: `int` (coin attraction frames)
- `invuln_timer`: `int` (post-shield hit recovery flashing)
- `is_crashing`: `bool` (terminal spinout state)
- `crash_frame`: `int` (crash animation counter)
- `buffered_key`: `u16` (6-frame input buffer storage)
- `buffer_timer`: `int` (input buffer decay counter)
- `landing_squash`: `int` (2-frame suspension compression dip)
- `close_call_timer`: `int` (near-miss HUD flash frames)
- `slide_boost_timer`: `int` (slipstream speed boost duration)
- `hurdle_timer`: `int` (airborne roadblock clearance feedback frames)

### `Obstacle`
- `active`: `bool` (in-world flag)
- `type`: `ObstacleType` (`OBS_LOW_BARRIER=0`, `OBS_HIGH_SIGN=1`, `OBS_TRUCK=2`)
- `lane`: `int` (0, 1, or 2)
- `z`: `FIXED` (8.8 depth distance, spawns at 40.0, reaches car at 0.0)
- `screen_x`: `int` (projected pixel X)
- `screen_y`: `int` (projected pixel Y)
- `scale`: `int` (1=small 16x16, 2=large 32x32/32x16)
- `passed`: `bool` (passed behind player threshold)
- `close_call_awarded`: `bool` (near-miss bonus awarded)
- `hurdled`: `bool` (successful jump hurdle bonus awarded)

### `Coin`
- `active`: `bool`
- `lane`: `int`
- `x_fp`: `FIXED` (8.8 smooth magnet tracking position)
- `z`: `FIXED` (8.8 depth distance)
- `screen_x`: `int`, `screen_y`: `int`
- `scale`: `int`
- `anim_frame`: `int` (0..3 rotating sheen animation)

### `GameState`
- `state`: `State` (`STATE_TITLE=0`, `STATE_PLAY=1`, `STATE_GAMEOVER=2`)
- `score`: `u32` (distance + pickups + bonuses)
- `coins`: `u32` (coins collected)
- `distance_m`: `u32` (meters traveled)
- `speed`: `FIXED` (8.8 world scroll rate, baseline 0.40fp, max 0.85fp)
- `road_scroll`: `int` (software scanline strip offset)
- `spawn_timer`: `int` (obstacle wave counter)
- `wave_pattern`: `int` (0..3 curated spawner director index)
- `coin_spawn_timer`: `int`
- `powerup_spawn_timer`: `int`
- `screen_shake`: `int` (impact trauma frames)
- `high_score`: `u32`
- `frame_count`: `u32`

## Relationships

- `Player` interacts with `Obstacle`, `Coin`, and `WorldPowerup` arrays via `check_collisions()` in `nitrosurfer/source/game.c:486`.
- `GameState` tracks run metrics and camera scroll driven by `g_game.speed`.

## Boundaries

- State structs are instantiated exclusively in IWRAM (`0x03000000`).
- Renderer converts 8.8 fixed-point world coordinates (`x`, `y_offset`, `z`) into integer screen coordinates (`screen_x`, `screen_y`) via `project_to_screen()` in `nitrosurfer/source/render.c:134`.

## Validation

- Clamped bounds:
  - `g_player.x` is clamped to road limits `[INT_TO_FP(70), INT_TO_FP(170)]`.
  - `g_game.speed` is clamped between `SPEED_BASE` (0.40fp) and `SPEED_MAX` (0.85fp).
  - Wave spawner strictly validates that at least one lane remains open in every wave.

## Schema

- No SQL database or migrations. Persistent high scores are preserved in Game Boy Advance SRAM/Flash (`0x0E000000`) if configured.
