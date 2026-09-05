---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/game.c"
  - ":(top)nitrosurfer/source/render.c"
---

# Logic: Jumping, Hurdling & Clearance

## Trigger
Player presses `KEY_A` or `KEY_UP` while grounded, or during an active slide (triggering a slide-cancel jump).

## Rules
1. Ground check: If `!is_jumping` (or `is_sliding`), jump activates. `is_sliding` is immediately canceled.
2. Initial launch velocity `vy` is set to `TO_FP(4.5f)`. `is_jumping = true`. `sfx_jump()` chime plays.
3. Every frame:
   - `y_offset += vy`
   - `vy -= TO_FP(0.32f)` (Gravity)
   - When `y_offset <= 0`, car touches down: `y_offset = 0`, `vy = 0`, `is_jumping = false`, and `landing_squash = 2` (2-frame suspension compression dip).
4. **Sprite Animation States**:
   - Ascent (`vy > 0`): Renders `TILE_PLAYER_JUMP_UP` (nose pitched up, suspension drooped).
   - Descent (`vy <= 0`): Renders `TILE_PLAYER_JUMP_DOWN` (nose pitched down, spoiler elevated).
   - Aerodynamic Wind Whoosh: When airborne ($Y \ge 8\text{px}$), cyan/white whoosh ribbons spawn under wheels (`TILE_JUMP_WHOOSH`).
   - Dynamic Shadow: Ground shadow shrinks to $16\times 8$ (`TILE_SHADOW_SML`) when altitude $> 10\text{px}$.
5. **Roadblock Hurdle Check (`nitrosurfer/source/game.c:505`)**:
   - When passing an `OBS_LOW_BARRIER`:
     - If $Y < 8\text{px}$: Collision registered (`hit = true`).
     - If $Y \ge 8\text{px}$: Clearance successful. If `!hurdled`, marks `hurdled = true`, triggers `sfx_hurdle()`, sets `hurdle_timer = 28`, and awards $+150$ points.
