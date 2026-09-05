---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/Makefile"
  - ":(top)harness/**"
---

# 07 Operations

## Build commands

### Clean and Build GBA ROM
```bash
export PATH=/opt/devkitpro/devkitARM/bin:/opt/devkitpro/tools/bin:$PATH
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=/opt/devkitpro/devkitARM
make -C nitrosurfer clean all && cp nitrosurfer/nitrosurfer.gba nitrosurfer.gba
```

## Test commands

### Headless Screenshot / Smoke Test
```bash
node harness/driver.mjs shot nitrosurfer.gba 120 out/smoke.png
```

### Automated Mechanic & Hurdle Verification
```bash
node -e "
import { EmuHarness } from './harness/driver.mjs';
const gba = await EmuHarness.launch('nitrosurfer.gba');
await gba.waitFrames(15);
await gba.setButton('START', true);
await gba.waitFrames(4);
await gba.setButton('START', false);
await gba.waitFrames(30);
await gba.screenshot('out/gameplay.png');
await gba.close();
"
```

## Deployment

- Web Playable Build: The repository root contains `index.html` and `netlify.toml` which deploy an in-browser WebAssembly emulator player directly to Netlify hosting.
