---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/game.c"
---

# Logic: Lane Movement & Evasion

## Trigger
Player presses `KEY_LEFT` or `KEY_RIGHT` (or action is retrieved from the 6-frame input buffer).

## Rules
1. If already at left boundary (`lane == 0`), `KEY_LEFT` is ignored.
2. If already at right boundary (`lane == 2`), `KEY_RIGHT` is ignored.
3. Otherwise, `lane` increments or decrements. Target horizontal coordinate is set to `LANE_X(lane)` (`nitrosurfer/source/game.c:147`).
4. In `player_update()`, `x` approaches `target_x` with fixed-point lerp (`dx * 5 / 16`).
5. When `x != target_x`, car tilts (`tilt = -1` or `+1`), displaying banking sprite (`TILE_PLAYER_LEFT` or `TILE_PLAYER_RIGHT`).
6. **Dynamic Hitbox Evasion**: While actively dodging (`tilt != 0` or `lane != obstacle.lane`), the collision width threshold drops from 18px down to 12px, forgiving corner clips (`nitrosurfer/source/game.c:500`).
