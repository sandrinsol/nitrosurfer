# GBA Test Harness + MCP Server

Test **GBA / GB / GBC** games with an AI agent — **no third-party emulator**. It
drives the bundled EmulatorJS cores (**mgba** for GBA, **gambatte** for GB/GBC —
the same cores `../index.html` runs for humans) in headless Chromium, selecting the
core automatically from the ROM extension, and exposes it two ways:

- **MCP server** (`mcp-server.mjs`) — any MCP host (Claude Code, Claude Desktop,
  Cursor, …) gets tools to load a ROM, press buttons, **see the screen** (inline
  images), read real RAM, and assert against golden frames. This is the main way
  an AI drives it.
- **Node library** (`driver.mjs`) — the same capabilities as a scriptable class,
  for CI and custom test scripts.

Everything is **deterministic** (verified bit-identical across runs) and reads
**real GBA RAM** out of save states — no core rebuild needed.

## Setup (once)

```bash
cd gba-test-mcp/harness
npm install          # installs deps + Chromium (postinstall)
```

## Use as an MCP server

Run it: `npm run mcp` (stdio transport). Register it with your MCP host — e.g.
Claude Code / Claude Desktop config:

```json
{
  "mcpServers": {
    "gba-test": {
      "command": "node",
      "args": ["/absolute/path/to/gba-test-mcp/harness/mcp-server.mjs"]
    }
  }
}
```

Then the agent has these tools: `load_rom` (`.gba/.gb/.gbc`, system auto-detected),
`wait_frames`, `press`, `set_button`, `screenshot` (returns the frame as an image),
`read_memory` (GBA only), `save_state`, `load_state`, `assert_golden`,
`frame_number`, `reset`, `status`. ROM paths are resolved relative to the server's
working directory.

**Console support:** GBA (mgba) and GB/GBC (gambatte). Input, screenshots, save
states, determinism, and golden asserts work for all three. `read_memory` currently
parses mgba's GBA save state only; GB/GBC RAM reads would need a gambatte save-state
parser (not yet implemented) — the tool returns a clear error for GB/GBC.

## Quick check

```bash
node driver.mjs shot ../anguna.gba 300 out/title.png   # boot, run 300 frames, screenshot
node tests/smoke.mjs                                    # full self-test (boot + render + state round-trip)
```

## Writing a test

```js
import { GBAHarness } from './driver.mjs';

const gba = await GBAHarness.launch('../mygame.gba'); // waits until the game has started
await gba.waitFrames(180);        // let it boot
await gba.tap('START');           // press+release START
await gba.setButton('RIGHT', true);
await gba.waitFrames(60);         // hold RIGHT for 60 frames
await gba.setButton('RIGHT', false);
await gba.screenshot('out/after-move.png');

const save = await gba.saveState();   // Buffer — snapshot the whole machine
// ... do stuff ...
await gba.loadState(save);            // restore exactly

await gba.close();
```

`launch(rom)` accepts any path; ROMs outside this folder are copied into `out/` so
the local static server can serve them.

## API

| Method | Description |
|--------|-------------|
| `GBAHarness.launch(rom, {headless=true, timeout=60000})` | Boot a ROM, resolve once the game has started. |
| `waitFrames(n)` | Advance **n emulated frames** on a boot-anchored absolute clock (see Determinism). |
| `waitUntilFrame(abs)` | Advance to an absolute emulated frame number. |
| `frameNum()` | Current emulated frame number (absolute, from boot). |
| `setButton(name, pressed)` | Hold/release a button. Names: `A B L R START SELECT UP DOWN LEFT RIGHT`. |
| `tap(name, hold=4, release=4)` | Press, hold `hold` frames, release, settle `release` frames. |
| `screenshot(outPath?)` | PNG of the current frame → `Buffer` (and written to `outPath` if given). |
| `saveState()` / `loadState(buf)` | Capture / restore a full save state (`Buffer`). |
| `readMemory(addr, len)` | Read `len` bytes at a GBA bus address → `Buffer`. |
| `readU8/readU16/readU32(addr)` | Read a little-endian integer at a GBA address. |
| `readRegions()` | Typed views `{ ewram, iwram, vram, pram, oam, io }` of current RAM. |
| `hasMemoryAccess()` | `true` only if the core exports *live* RAM (not required for reads). |
| `videoDimensions()` | `{ w, h }` — `240x160` for GBA. |
| `restart()` | Reset the game. |
| `close()` | Shut down the browser and static server. |

## Screenshot timelines (AI-friendly)

Author a test as plain JSON steps and dump a screenshot timeline the AI reads back:

```bash
node runner.mjs tests/anguna.script.json          # -> out/anguna.script/*.png + timeline.json
```

Step types: `wait` (frames), `tap`/`press`/`release`/`hold` (buttons), `shot` (named
screenshot), `record` (`{frames, every, prefix}` — periodic screenshots), `saveState`/
`loadState` (named, in-memory), and `assert` (golden compare). Example:

```json
{ "rom": "../mygame.gba", "steps": [
  { "wait": 240 },
  { "shot": "title" },
  { "tap": "START" },
  { "hold": "RIGHT", "frames": 90 },
  { "record": { "frames": 300, "every": 30, "prefix": "walk" } },
  { "assert": "goldens/title.png", "threshold": 0.02 }
]}
```

`timeline.json` lists every step with its emulated frame number and screenshot file, so
an AI can walk the run and inspect each PNG.

## Golden-frame diffing

`assert.mjs` (zero-dependency PNG decode/encode) compares a screenshot to a stored
golden and writes a red-highlighted `*.diff.png` on mismatch.

```js
import { compare, assertGolden } from './assert.mjs';
const res = await assertGolden(await gba.screenshot(), 'goldens/title.png', { threshold: 0.02 });
// res.pass, res.fraction, res.diffPath
```

- First run (or `UPDATE_GOLDENS=1 node runner.mjs ...`) **creates/refreshes** goldens.
- `threshold` is the max fraction of differing pixels (default 2%); `tolerance` is the
  per-channel value delta that counts as "different" (default 16, absorbs codec noise).
- `assert` paths are resolved relative to the **script file's** directory.

## How it works

`page.html` configures EmulatorJS headlessly and installs a `window.GBA` bridge
over `EJS_emulator.gameManager` (the core's control object). `driver.mjs` serves
the repo root folder over a throwaway localhost server, launches headless
Chromium with SwiftShader (software WebGL2, so no GPU needed), navigates to the
page, and marshals every call through `page.evaluate`.

## Determinism (verified)

The emulator is deterministic: an identical script produces **bit-identical
frames** across separate runs. `tests/determinism.mjs` enforces this (0.000%
diff at matched checkpoints) and that input actually changes the frame.

The one requirement is to step time on the **absolute clock**: `waitFrames(n)`
accumulates targets anchored at boot (frame 0), so boot jitter can't shift which
emulated frame you land on. Don't `sleep()` in wall-clock time between actions —
always advance with `waitFrames`/`waitUntilFrame`. Goldens are also tied to this
software (SwiftShader) renderer; regenerate them with `UPDATE_GOLDENS=1` after a
Chromium/Playwright upgrade.

## Reading GBA RAM

Assert on actual game variables (HP, score, position, flags, RNG…) by address:

```js
import { readU8, readU16, readU32, parseState } from './memory.mjs';

const hp = await gba.readU16(0x03000010);     // IWRAM
const buf = await gba.readMemory(0x02000000, 64); // 64 bytes of EWRAM
const regions = await gba.readRegions();       // { ewram, iwram, vram, pram, oam, io }
```

Or via the MCP `read_memory` tool (`address`, `length`, `as: hex|u8|u16|u32|s8|s16|s32`).

**How it works (no core rebuild).** The mgba save state is a RASTATE container whose
`MEM ` chunk is a raw `GBASerializedState` (v7). RAM lives at the end of that struct
(`iwram` 32 KB, `wram`/EWRAM 256 KB, plus VRAM/PRAM/OAM/IO). The struct size is found
*structurally* from the trailing extdata directory (verified `0x61000`), so offsets
are correct for any game or save type — nothing is hardcoded per-ROM. Reads reflect
the current frame (a save state is snapshotted under the hood) and honor GBA address
mirroring.

**Limits.** Read-only, at save-state granularity (snapshot whenever you want to
inspect — that's exactly when you assert). Writing/poking RAM would need a core built
with the libretro memory exports; `readSystemRam()`/`hasMemoryAccess()` auto-detect
and use such a core if you ever drop one in, but it isn't required for reads.
