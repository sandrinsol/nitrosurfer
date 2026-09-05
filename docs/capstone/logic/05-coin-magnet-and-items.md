---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/game.c"
---

# Logic: Coin Magnet & Powerup Pickups

## Trigger
Player collects a powerup capsule or approaches coins with active magnet.

## Rules
1. **Coin Magnet (`nitrosurfer/source/game.c:425`)**:
   - When `magnet_timer > 0`, any coin within $Z \le 28.0\text{fp}$ accelerates toward `g_player.x`.
   - Uses 8.8 fixed-point vector interpolation (`x_fp += dir * speed`) rather than snapping across lanes.
2. **Nitro Thruster**:
   - Sets `nitro_timer = 240` (~4 seconds).
   - Surges speed, renders twin nitro exhaust flame sprites behind the car (`TILE_NITRO_FLAME`).
   - Smashes through obstacles on collision, awarding $+300$ demolition score without crashing.
3. **Shield Aura**:
   - Sets `shield_active = true`.
   - Absorbs one fatal collision, granting 90 frames of post-hit invulnerability flashing.
