# Capstone Architecture Reference: Nitro Surfer (GBA)

High-speed endless car runner for Game Boy Advance with 3D pseudo-perspective rendering, 60 FPS gameplay, PSG audio, and an automated headless test harness.

- **Stack**: C99 (devkitARM, libgba), ARM7TDMI assembly/BIOS, Node.js/Playwright/EmulatorJS test harness.
- **Paradigm**: Procedural register-mapped game loop, static IWRAM memory layout, zero-heap runtime.

## Module Map

| Module | Purpose | Entry Points |
| :--- | :--- | :--- |
| `nitrosurfer` | Embedded GBA Game Binary | `nitrosurfer/source/main.c:16` (`main()`) |
| `harness` | Headless WebAssembly Emulator Test Harness | `harness/driver.mjs:55` (`EmuHarness`) |

## Topic Reference

| Topic | File |
| :--- | :--- |
| **Architecture** | [01-architecture.md](01-architecture.md) |
| **Models** | [02-models.md](02-models.md) |
| **Conventions** | [03-conventions.md](03-conventions.md) |
| **Data Flow** | [04-data-flow.md](04-data-flow.md) |
| **Dependencies** | [05-dependencies.md](05-dependencies.md) |
| **Testing** | [06-testing.md](06-testing.md) |
| **Operations** | [07-operations.md](07-operations.md) |
| **Glossary** | [08-glossary.md](08-glossary.md) |

## Logic Scenarios

| Scenario | File |
| :--- | :--- |
| **Lane Movement & Evasion** | [logic/01-lane-movement.md](logic/01-lane-movement.md) |
| **Jumping, Hurdling & Clearance** | [logic/02-jumping-and-hurdles.md](logic/02-jumping-and-hurdles.md) |
| **Sliding & Slipstream Drift** | [logic/03-sliding-and-drift.md](logic/03-sliding-and-drift.md) |
| **Obstacle Wave Spawner** | [logic/04-obstacle-spawning.md](logic/04-obstacle-spawning.md) |
| **Coin Magnet & Items** | [logic/05-coin-magnet-and-items.md](logic/05-coin-magnet-and-items.md) |

## UI/UX Surfaces

| Surface | File |
| :--- | :--- |
| **Design Direction** | [uiux/01-direction.md](uiux/01-direction.md) |
| **Design System & Tokens** | [uiux/02-system.md](uiux/02-system.md) |
| **Title Screen** | [uiux/screens/01-title.md](uiux/screens/01-title.md) |
| **Gameplay Screen** | [uiux/screens/02-gameplay.md](uiux/screens/02-gameplay.md) |
| **Game Over Screen** | [uiux/screens/03-gameover.md](uiux/screens/03-gameover.md) |

## Companion Docs

| File | What it is |
| :--- | :--- |
| [changelog.md](changelog.md) | Append-only execution ledger |
| [review.md](review.md) | Architecture and UI/UX opinionated review |
