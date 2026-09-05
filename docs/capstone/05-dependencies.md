---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/Makefile"
  - ":(top)package.json"
---

# 05 Dependencies

## Toolchains and SDKs

| Dependency | Category | Version | Purpose |
| :--- | :--- | :--- | :--- |
| `devkitARM` | Embedded Toolchain | GCC 14.2.0 / binutils | Cross-compiler toolchain for ARM7TDMI (GBA architecture) |
| `libgba` | Runtime Library | v0.5.0+ | Direct register definitions, interrupt handlers, TTE text engine |
| `gbafix` | ROM Utility | Bundled with devkitPro | Fixes Nintendo GBA cartridge header checksums and logo bitmaps |
| `Node.js` | Host Testing | v20+ / ESM | Test automation execution environment |
| `Playwright` | Browser Automation | v1.40+ | Headless Chromium controller for EmulatorJS test harness |
| `EmulatorJS` | Emulator Core | mgba / gambatte WASM | WebAssembly GBA/GB hardware emulation core |

## Wiring

- `nitrosurfer/Makefile`: Configures compile flags (`-mthumb -mthumb-interwork -O2 -mcpu=arm7tdmi`), includes libgba headers, and invokes `arm-none-eabi-gcc` and `gbafix`.
- `harness/driver.mjs`: Imports node native modules (`http`, `fs`, `path`) and launches Playwright browser automation against EmulatorJS.
