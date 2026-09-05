---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/**"
---

# 08 Glossary

| Term | Context | Definition |
| :--- | :--- | :--- |
| **OAM** | Hardware | Object Attribute Memory (0x07000000); 1 KB memory holding 128 sprite attribute entries. Lower index draws in front. |
| **VBlank** | Video Timing | Vertical Blanking interval; scanlines 160–227 when electron beam resets, the only safe window to write VRAM/OAM without tearing. |
| **Mode 0** | Display Controller | GBA tiled video mode with four 4bpp/8bpp affine or text background tile layers. |
| **TTE** | Library | Text Tile Engine provided by libgba; handles character rendering and color escape tags into tilemaps. |
| **Fixed-Point (8.8)** | Mathematics | Number format where integer is shifted left by 8 bits (256 units = 1.0); used to avoid slow software floating-point emulation on ARM7. |
| **Charblock** | VRAM | 16 KB VRAM partition holding 512 tile bitmaps. Charblocks 0–3 are backgrounds; 4–5 are sprites. |
| **Screenblock** | VRAM | 2 KB VRAM partition holding a 32x32 tile map. |
| **IWRAM** | Internal Memory | 32 KB high-speed 32-bit internal work RAM located at 0x03000000; zero wait-states. |
| **EWRAM** | External Memory | 256 KB 16-bit external work RAM located at 0x02000000; requires 2 wait-states. |
| **Hurdle** | Gameplay | Aerial maneuver where the car leaps cleanly over a low roadblock barrier ($Y \ge 8\text{px}$), earning $+150$ bonus points. |
| **Close Call** | Gameplay | Near-miss maneuver where the player car grazes past an obstacle ($dx \in [13, 22]$), awarding $+250$ points. |
