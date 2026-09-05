---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)harness/**"
---

# 06 Testing

## Strategy

Testing for the Game Boy Advance embedded target cannot rely on typical host OS unit test runners (like Jest or Pytest) because execution depends on Cycle-accurate Game Boy Advance hardware registers, OAM DMA, and BIOS interrupts.

The project uses an in-tree automated test harness (`harness/driver.mjs`) that boots the compiled `.gba` binary inside a headless Chromium instance running the EmulatorJS mGBA WASM core.

## Test doubles and drivers

- **Harness Driver (`harness/driver.mjs`)**:
  - `EmuHarness.launch(romPath)`: Boots emulator core headlessly.
  - `waitFrames(n)`: Steps emulation forward deterministically by exact video frame count.
  - `setButton(btn, down)`: Injects gamepad inputs (`KEY_A`, `KEY_B`, `KEY_START`, `KEY_LEFT`, `KEY_RIGHT`).
  - `readMemory(addr, len)`: Reads live GBA bus memory through save-state parsing.
  - `screenshot(path)`: Captures lossless PNG frame buffers directly from emulator canvas.

## Automated test coverage

1. **Golden Screen Verification**: Title screen, active gameplay, jump apex, and game over screens compared against golden references.
2. **Mechanic Assertions**:
   - Spawner fairness test: Simulates 400 frames of gameplay and asserts `minOpenLanes >= 1` across all active obstacle waves.
   - Input buffering test: Verifies that directional taps during lane changes or slide recoveries are queued and executed without dropped inputs.
   - Jump priority and hurdle test: Asserts OAM 0 priority, checks `hurdled` flag and `hurdle_timer > 0` upon clearing a roadblock.
