---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/render.c"
  - ":(top)nitrosurfer/source/gfx_data.c"
---

# UI/UX Design System

## Palettes & Tokens (15-bit BGR555 GBA Hardware)

### Text Tile Engine (Palette Bank 15)
- Color 0: Transparent
- Color 1 (`#{ci:1}`): Crisp White (`RGB8(255, 255, 255)`) - Primary values (Score digits, speed km/h)
- Color 2 (`#{ci:2}`): Arcade Gold Yellow (`RGB8(255, 220, 40)`) - Reward tags (`[HURDLE!]`, `[CLOSE!]`)
- Color 3 (`#{ci:3}`): Vibrant Neon Cyan (`RGB8(0, 240, 255)`) - Labels (`SCORE:`, `DIST:`)
- Color 4 (`#{ci:4}`): High-Heat Orange Red (`RGB8(255, 70, 0)`) - Power tags (`[NITRO!]`)

### Sprite Palettes
- Palette 0: Active Player Supercar (dynamically loaded via `gfx_set_car_color()`)
- Palette 1: Obstacles (Safety Orange & White Barricades, Emerald Highway Overhead Signs, Steel Gray Cargo Trucks)
- Palette 2: Items & FX (Spinning Gold Coins, Cyan Wind Whoosh, Violet Shield Aura)
- Palette 3: Steel Coat Armor & Visual FX (Aura, Sparks)
- Palette Banks 4–9: 6 Supercar Chassis Paint Swatches (simultaneous true-color chiclet rendering)

### Supercar Paint Tokens (15-bit BGR555 GBA Hardware)
- **Cherry Red** (`CAR_COLOR_RED` = 0):
  - Base: `C_RGB(28, 2, 8)` (Vibrant Cherry)
  - Shadow: `C_RGB(18, 1, 4)` (Deep Crimson)
  - Gloss: `C_RGB(31, 8, 14)` (Vermillion Shine)
- **Cobalt Blue** (`CAR_COLOR_BLUE` = 1):
  - Base: `C_RGB(2, 14, 28)` (Deep Royal Cobalt)
  - Shadow: `C_RGB(1, 6, 17)` (Abyssal Navy)
  - Gloss: `C_RGB(10, 22, 31)` (Ice Blue Highlight)
- **Neon Pink** (`CAR_COLOR_PINK` = 2):
  - Base: `C_RGB(30, 6, 22)` (Hot Synthwave Magenta)
  - Shadow: `C_RGB(18, 2, 12)` (Dark Berry)
  - Gloss: `C_RGB(31, 16, 28)` (Candy Pink Highlight)
- **Stealth Black** (`CAR_COLOR_BLACK` = 3):
  - Base: `C_RGB(6, 6, 8)` (Matte Onyx)
  - Shadow: `C_RGB(2, 2, 3)` (Pitch Carbon)
  - Gloss: `C_RGB(14, 15, 18)` (Graphite Sheen)
- **Racing Yellow** (`CAR_COLOR_YELLOW` = 4):
  - Base: `C_RGB(31, 26, 0)` (Pure Racing Gold)
  - Shadow: `C_RGB(22, 16, 0)` (Warm Amber)
  - Gloss: `C_RGB(31, 31, 14)` (Bright Lemon Highlight)
- **Viper Green** (`CAR_COLOR_GREEN` = 5):
  - Base: `C_RGB(2, 24, 8)` (Vivid Viper Emerald)
  - Shadow: `C_RGB(1, 13, 4)` (Deep Forest)
  - Gloss: `C_RGB(10, 31, 16)` (Neon Lime Highlight)

## Typography & Constraints
- Font: GBA libgba default 8x8 bitmap font rendered onto Background Layer 0 (`BG0`, CBB 0 / SBB 31).
- Layout: Bottom HUD container box rendered onto Background Layer 1 (`BG1`, CBB 2 / SBB 30) spanning the full 240px width across rows 17–19 ($Y \in [136, 160]$).
  - Background Masking: Hardware `REG_WIN0` isolates the bottom 24 lines, disallowing `BG2` road tiles to eliminate visual bleed beneath the HUD.
  - Sub-Tile Centering: `REG_BG0VOFS = 253` offsets text vertically by -3 pixels to center the 8x8 glyphs cleanly within the beveled box rows.
  - Line 1 (Y: 139): `SCORE: %06d (x%d)` left-aligned; dedicated OAM Coin icon sprite and `%03d` right-aligned.
  - Line 2 (Y: 148): `DIST: %5dm` left-aligned; dedicated OAM Wheel/Speedometer icon sprite and `%3d` right-aligned.
- OAM Icon Tokens:
  - Coin Icon: 8x8 hardware sprite in Palette 1 positioned at `(216, 139)`.
  - Wheel/Speedometer Icon: 8x8 hardware sprite in Palette 3 positioned at `(216, 148)`.
- Floating Score Popups: Dynamic arcade popup badges (`+150` hurdle, `+200` gantry, `+100` dodge) rendered via the `DebrisParticle` system as rising OAM sprites, replacing static on-HUD text badges.
- Strict limit: All HUD text format strings are bounded to $\le 28$ characters to prevent wrapping outside the container boundary.
