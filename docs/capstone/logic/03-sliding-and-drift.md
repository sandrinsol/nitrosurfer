---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/game.c"
---

# Logic: Sliding & Slipstream Drift

## Trigger
Player presses `KEY_B` or `KEY_DOWN` while grounded.

## Rules
1. Ground check: Can only slide if `!is_jumping`.
2. Activation: `is_sliding = true`, `slide_timer = 28` frames (~0.45s).
3. **Midsize Shrink**: Rather than lowering/squashing flat to the ground, the car shrinks into a high-tech ~70% midsize sports car with slipstream sparks and a matching compact drop shadow (`TILE_SHADOW_SML`).
4. **Slipstream Drift Boost**: Shrinking surges game speed (`g_game.speed += TO_FP(0.08f)`) and sets `slide_boost_timer = 24`, displaying `[SHRINK!]` while active and `[DRIFT!]` during speed surge on the HUD.
5. **Agile Hitbox**: Midsize mode narrows collision width to 10px (down from 18px).
6. **Slide-Cancel**: Pressing `KEY_A` during a shrink instantly cancels it into an aerial leap.
7. **Overhead Sign Clearance**:
   - When encountering `OBS_HIGH_SIGN`:
     - If `is_sliding`: Midsize car glides safely underneath the overhead sign.
     - If `!is_sliding`: Collision registered (`hit = true`).
