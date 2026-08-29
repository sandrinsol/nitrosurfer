# Third-party components & attribution

This project is licensed **GPL-3.0-or-later** (see `LICENSE`), because it bundles
and builds on the following GPL-3.0 software:

- **EmulatorJS** — the emulator frontend and packaged cores in `data/`.
  GPL-3.0. https://github.com/EmulatorJS/EmulatorJS
- **mGBA** (libretro core, WebAssembly, `data/cores/mgba-wasm.data`) — runs GBA.
  MPL-2.0 upstream; distributed here as part of the EmulatorJS bundle.
  https://github.com/mgba-emu/mgba
- **Gambatte** (libretro core, WebAssembly, `data/cores/gambatte-wasm.data`) — runs
  GB/GBC. GPL-2.0-or-later. https://github.com/libretro/gambatte-libretro

Bundled sample game:

- **Anguna** (`anguna.gba`) — freeware homebrew by Nathan Tolbert / Bit Bunch.
  Included so the emulator works out of the box. https://tolberts.net/anguna/

## ROM legality

Only the freeware `anguna.gba` is included. **Do not commit or distribute
commercial ROMs** — supply your own dumps of games you legally own. Test ROMs you
build yourself are of course fine.

## What in this repo is original

The test harness and MCP server (`harness/`) are original work, released under the
same GPL-3.0-or-later as the repo:

- `harness/mcp-server.mjs` — MCP server exposing the emulator as agent tools
- `harness/driver.mjs` — headless Playwright driver
- `harness/memory.mjs` — GBA RAM reads via mgba save-state parsing
- `harness/assert.mjs`, `harness/png.mjs` — golden-frame diffing
- `harness/runner.mjs` — JSON input-script → screenshot-timeline runner
