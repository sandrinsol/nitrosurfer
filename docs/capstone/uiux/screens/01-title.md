---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/render.c"
---

# Screen: Title & Attract Mode (Garage Showroom 3x2 Grid)

## Surface Summary
The boot screen welcoming the player to Nitro Surfer with an interactive 3x2 Garage Showroom grid where players select their supercar from 6 display cases showing the whole car before hitting the highway.

- **Visual Elements**:
  - Title Header: `=== NITRO SURFER ===` centered at Row 0 ($Y=4$) in Arcade Gold Yellow.
  - Dynamic Color Name Banner: Pulsing chevron header centered at Row 2 ($Y=16$) displaying `<< COLOR NAME >>` (or `<< COLOR NAME [2000 G] >>` when locked) in alternating chevron animations.
  - 3x2 Showroom Display Grid (6 Whole Cars):
    - Row 0 Cases ($Y \in [32, 71]$, Height 40px):
      - Case (0, 0) at $X \in [8, 71]$: Whole Cherry Red Supercar (Sprite Palette Bank 4) - Unlocked by default
      - Case (1, 0) at $X \in [88, 151]$: Whole Cobalt Blue Supercar (Sprite Palette Bank 5) - Unlocked by default
      - Case (2, 0) at $X \in [168, 231]$: Whole Neon Pink Supercar (Sprite Palette Bank 6) - Unlocked for 2000 Gold (`2000 G` price tag at $X=176, Y=72$)
    - Row 1 Cases ($Y \in [80, 119]$, Height 40px):
      - Case (0, 1) at $X \in [8, 71]$: Whole Stealth Black Supercar (Sprite Palette Bank 7) - Unlocked for 2000 Gold (`2000 G` price tag at $X=16, Y=123$)
      - Case (1, 1) at $X \in [88, 151]$: Whole Racing Yellow Supercar (Sprite Palette Bank 8) - Unlocked for 2000 Gold (`2000 G` price tag at $X=96, Y=123$)
      - Case (2, 1) at $X \in [168, 231]$: Whole Viper Green Supercar (Sprite Palette Bank 9) - Unlocked for 2000 Gold (`2000 G` price tag at $X=176, Y=123$)
  - Box Border Styling:
    - Unselected Cases: Sleek metallic silver/chrome bevel borders (`TILE_HUD_BOX_*`, Pal 2) with pitch black interiors.
    - Active Case: Illuminated arcade-gold glowing double-border with crisp white inner rim (`TILE_SEL_BOX_*`, Pal 0).
    - Active Engine Vibration: The selected supercar rumbles with a 1px vertical engine idle bounce (`(g_game.frame_count & 16)`).
  - Bottom Box: Isolated within the beveled metallic HUD container ($Y \in [136, 160]$) on Background Layer 1:
    - Centered text ($X=28, Y=144$): `COINS COLLECTED: %06d` (High score removed).
- **Interactions**:
  - `KEY_LEFT` / `KEY_L`: Moves left across cars with wraparound.
  - `KEY_RIGHT` / `KEY_R`: Moves right across cars with wraparound.
  - `KEY_UP` / `KEY_DOWN`: Moves vertically between rows (Row 0 <-> Row 1).
  - Audio Feedback: Every navigation step plays a crisp chime sound effect (`sfx_coin()`).
  - `KEY_START` / `KEY_A`:
    - If car is unlocked: Confirms car selection and transitions to Track Selection (`STATE_SELECT_MAP`).
    - If car is locked and total coins $\ge 2000$: Deducts 2000 coins, saves to SRAM, unlocks car, plays fanfare (`sfx_high_score()`), and clears the price tag.
    - If car is locked and total coins $< 2000$: Plays error skid sound (`sfx_skid()`) and blocks progression.
