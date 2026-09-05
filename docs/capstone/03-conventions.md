---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/**"
  - ":(top)harness/**"
---

# 03 Conventions

## Paradigm

- **GBA Core (`nitrosurfer/`)**: Pure procedural, flat data structures, hardware register-mapped C99.
  - Zero dynamic heap allocation (`malloc`/`free` are never called; memory is statically pre-allocated in `.bss` / IWRAM).
  - Pure deterministic tick lifecycle: `input_poll()` -> `game_update()` -> `check_collisions()` -> `render_frame()` -> `wait_vblank()`.
- **Harness (`harness/`)**: Modern asynchronous JavaScript (ES Modules, `async`/`await`, Playwright headless page evaluation, Buffer-based binary parsing).

## Typing

- **C Codebase (`nitrosurfer/`)**:
  - Typed with libgba primitive aliases: `u8`, `u16`, `u32`, `s8`, `s16`, `s32`, `bool`.
  - Math represented in 8.8 fixed point (`typedef s32 FIXED`). Macros `INT_TO_FP(x)` `((x) << 8)`, `FP_TO_INT(x)` `((x) >> 8)`, `TO_FP(f)` `((s32)((f) * 256.0f))`.
  - GBA hardware structs use libgba standard bitfields (`OBJ_ATTR`, `ATTR0_*`, `ATTR1_*`, `ATTR2_*`).
  - Zero raw `void*` casts in gameplay logic; typed entity structs only.
- **Test Harness (`harness/`)**:
  - Strict ESM without type-checking annotations.
  - Binary memory offsets documented via hex literals (`0x03000000`, `0x07000000`).

## Error handling

- GBA Embedded runtime has no exceptions, assertions, or abort handlers.
- Bounds checking and invalid state prevention are handled defensively:
  - Sprite count strictly capped (`if (spr_idx >= 120) continue;`).
  - Clamping macros prevent division by zero in 3D projection (`if (z < 1) z = 1;`).
  - Spawner lookahead loops break safely if no lane is available.

## Dependency injection

- Static global allocation with extern headers:
  - `include/common.h` exports global singletons: `extern Player g_player;`, `extern GameState g_game;`, `extern Obstacle g_obstacles[];`.
  - Direct hardware register access via memory-mapped IO defines in `<gba_video.h>` and `<gba_sound.h>`.
