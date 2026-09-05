---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-04"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/render.c"
  - ":(top)nitrosurfer/source/game.c"
---

# Screen: Track Map Selection (3 Boxes in One Row)

## Surface Summary
The map selection screen displayed immediately following car selection. Players select between 3 track themes (City Highway, Palm Beach, Winter Snow) presented as 3 side-by-side preview display cases with animated glowing borders and live authentic multi-palette thumbnail graphics.

- **Visual Elements**:
  - Title Header: `=== SELECT TRACK ===` centered at Row 0 ($Y=4$) in Arcade Gold Yellow.
  - Dynamic Track Name Banner: Pulsing chevron header centered at Row 2 ($Y=16$) displaying `<< TRACK NAME >>` in alternating chevron animations (`<< CITY HIGHWAY >>`, `<< PALM BEACH >>`, `<< WINTER SNOW >>`).
  - 3 Display Boxes in One Row ($Y \in [32, 87]$, Height 56px):
    - **Box 0 (City Highway)** at $X \in [16, 79]$: Authentic thumbnail preview with midnight skyscraper silhouettes, illuminated window grids, and asphalt road. Rendered using BG Palette Bank 2.
    - **Box 1 (Palm Beach)** at $X \in [88, 151]$: Authentic thumbnail preview with sunny coastal sky, roadside palm trees, turquoise ocean waves, and golden sand. Rendered using BG Palette Bank 3.
    - **Box 2 (Winter Snow)** at $X \in [160, 223]$: Authentic thumbnail preview with alpine twilight, snowy mountain peaks, evergreen pines, and deep snowbanks. Rendered using BG Palette Bank 4.
  - Box Border Styling:
    - Active Selection: Illuminated arcade-gold glowing double-border with crisp white inner rim (`TILE_SEL_BOX_*`, Pal 0).
    - Unselected Boxes: Themed metallic bevel borders (`TILE_HUD_BOX_*`, Pal 2+c).
  - Centered Box Labels ($Y=90$): `[CITY]`, `[BEACH]`, `[WINTER]` aligned under their respective preview cases with active highlight.
  - Dark Backdrop Panel ($Y \in [96, 127]$): Solid black panel on BG1 framing subtitle and prompt text for maximum contrast against road asphalt.
  - Track Feature Subtitle ($Y=102$): Descriptive feature callout in Cyan (`NEON METROPOLIS ROADWAY`, `COASTAL DUNES & PALMS`, `FROZEN PASS & SNOW BANKS`).
  - Flashing Prompt ($Y=116$): `> PRESS START TO RACE <` pulsing every 32 frames in Arcade Gold.
  - Bottom Legend Box ($Y \in [136, 159]$):
    - Row 1 ($Y=138$): `PAD: CHOOSE MAP   A: GO`
    - Row 2 ($Y=147$): `B: BACK TO CARS   START: GO`
- **Interactions**:
  - `KEY_LEFT` / `KEY_L`: Cycles left through tracks with bidirectional wrap (`sfx_coin()`).
  - `KEY_RIGHT` / `KEY_R`: Cycles right through tracks with bidirectional wrap (`sfx_coin()`).
  - `KEY_B`: Returns cleanly back to Car Selection screen (`sfx_pause()`), clearing map preview cases.
  - `KEY_START` / `KEY_A`: Launches race with the chosen car color and track map theme.
