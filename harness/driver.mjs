// GBA test harness driver.
//
// Boots the bundled EmulatorJS mgba core in headless Chromium and exposes an
// mGBA-scripting-style API (buttons, frame stepping, screenshots, save states)
// so AI/CI can test GBA ROMs with no third-party emulator program.
//
// Usage as a library:
//   import { GBAHarness } from './driver.mjs';
//   const gba = await GBAHarness.launch('../anguna.gba');
//   await gba.waitFrames(300);
//   await gba.tap('START');
//   await gba.screenshot('out/title.png');
//   await gba.close();
//
// Usage as a CLI (quick smoke shot):
//   node driver.mjs shot <rom.gba> [frames=300] [out.png=out/shot.png]

import http from 'node:http';
import { readFile, writeFile, stat, unlink } from 'node:fs/promises';
import { createReadStream } from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import * as gbaMem from './memory.mjs';
import * as gbMem from './gb-memory.mjs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(__dirname, '..'); // the "Emulator Gba" folder

const MIME = {
  '.html': 'text/html', '.js': 'text/javascript', '.mjs': 'text/javascript',
  '.css': 'text/css', '.json': 'application/json', '.wasm': 'application/wasm',
  '.data': 'application/octet-stream', '.gba': 'application/octet-stream',
  '.png': 'image/png', '.map': 'application/json',
};

function startServer(rootDir) {
  const server = http.createServer(async (req, res) => {
    try {
      const urlPath = decodeURIComponent(req.url.split('?')[0]);
      const filePath = path.join(rootDir, path.normalize(urlPath));
      if (!filePath.startsWith(rootDir)) { res.writeHead(403).end(); return; }
      const info = await stat(filePath);
      if (info.isDirectory()) { res.writeHead(404).end(); return; }
      res.writeHead(200, { 'Content-Type': MIME[path.extname(filePath)] || 'application/octet-stream' });
      createReadStream(filePath).pipe(res);
    } catch {
      res.writeHead(404).end('not found');
    }
  });
  return new Promise((resolve) => {
    server.listen(0, '127.0.0.1', () => resolve({ server, port: server.address().port }));
  });
}

export class GBAHarness {
  constructor({ browser, context, page, server, tempRom, system }) {
    this.browser = browser; this.context = context; this.page = page; this.server = server;
    this._tempRom = tempRom; // copied-in ROM to clean up on close (if any)
    this.system = system;    // 'gba' (mgba) or 'gb' (gambatte, covers GB + GBC)
  }

  static async launch(romPath, { headless = true, timeout = 60000 } = {}) {
    const { chromium } = await import('playwright');
    const absRom = path.resolve(process.cwd(), romPath);
    await stat(absRom).catch(() => { throw new Error(`ROM not found: ${absRom}`); });

    // ROM must live under ROOT so the static server can serve it. Copy if needed,
    // and remember the copy so close() can remove it.
    let served = absRom, tempRom = null;
    if (!absRom.startsWith(ROOT + path.sep)) {
      tempRom = path.join(ROOT, 'harness', 'out', `_rom_${process.pid}_${path.basename(absRom)}`);
      await writeFile(tempRom, await readFile(absRom));
      served = tempRom;
    }
    const romUrlPath = '/' + path.relative(ROOT, served).split(path.sep).map(encodeURIComponent).join('/');

    const { server, port } = await startServer(ROOT);
    const browser = await chromium.launch({
      headless,
      args: [
        '--use-gl=angle', '--use-angle=swiftshader',
        '--ignore-gpu-blocklist', '--enable-webgl',
        '--autoplay-policy=no-user-gesture-required',
        '--mute-audio', '--hide-scrollbars',
      ],
    });
    const context = await browser.newContext({
      viewport: { width: 720, height: 480 }, deviceScaleFactor: 1, reducedMotion: 'reduce',
    });
    const page = await context.newPage();
    const errors = [];
    page.on('pageerror', (e) => { errors.push(e.message); });

    const url = `http://127.0.0.1:${port}/harness/page.html?rom=${encodeURIComponent(romUrlPath)}`;
    await page.goto(url, { waitUntil: 'domcontentloaded' });
    try {
      await page.waitForFunction(() => window.EJS_emulator && window.EJS_emulator.gameManager, { timeout });
      // Bound the wait for the 'start' event so a ROM that never boots fails fast.
      await page.evaluate((ms) => Promise.race([
        window.GBA.ready(),
        new Promise((_, rej) => setTimeout(() => rej(new Error('game did not start (ROM may be unsupported by this core)')), ms)),
      ]), timeout);
    } catch (e) {
      await browser.close(); await new Promise((r) => server.close(r));
      if (tempRom) await unlink(tempRom).catch(() => {});
      throw new Error(`Emulator failed to start${errors.length ? ` (page errors: ${errors.join('; ')})` : ''}: ${e.message}`);
    }

    const system = await page.evaluate(() => window.GBA.system);
    return new GBAHarness({ browser, context, page, server, tempRom, system });
  }

  frameNum() { return this.page.evaluate(() => window.GBA.frameNum()); }
  videoDimensions() { return this.page.evaluate(() => window.GBA.videoDimensions()); }
  waitFrames(n) { return this.page.evaluate((n) => window.GBA.waitFrames(n), n); }
  waitUntilFrame(abs) { return this.page.evaluate((a) => window.GBA.waitUntilFrame(a), abs); }
  setButton(name, pressed) { return this.page.evaluate(([n, p]) => window.GBA.setButton(n, p), [name, pressed]); }
  tap(name, hold = 4, release = 4) { return this.page.evaluate(([n, h, r]) => window.GBA.tap(n, h, r), [name, hold, release]); }
  restart() { return this.page.evaluate(() => window.GBA.restart()); }

  async screenshot(outPath) {
    const b64 = await this.page.evaluate(() => window.GBA.screenshot());
    const buf = Buffer.from(b64, 'base64');
    if (outPath) await writeFile(path.resolve(process.cwd(), outPath), buf);
    return buf;
  }

  async saveState() {
    const b64 = await this.page.evaluate(() => window.GBA.saveState());
    return Buffer.from(b64, 'base64');
  }
  async loadState(buf) {
    const b64 = Buffer.from(buf).toString('base64');
    await this.page.evaluate((b) => window.GBA.loadState(b), b64);
  }

  /** Whether the loaded core exposes live RAM exports (bundled mgba core does not;
   *  memory reads instead go through save-state parsing, which always works). */
  hasMemoryAccess() { return this.page.evaluate(() => window.GBA.hasMemoryAccess()); }
  async readSystemRam() {
    const r = await this.page.evaluate(() => window.GBA.readSystemRam());
    return r ? Buffer.from(r.base64, 'base64') : null;
  }

  get _mem() { return this.system === 'gb' ? gbMem : gbaMem; }

  /** Parse a fresh save state into typed RAM regions.
   *  GBA: { ewram, iwram, vram, pram, oam, io }. GB/GBC: { wram, vram, sram, hram, io }. */
  async readRegions() { return this._mem.parseState(await this.saveState()); }

  /** Read `length` bytes at a bus address. GBA e.g. 0x03000000 (IWRAM); GB/GBC e.g.
   *  0xC000 (WRAM), 0xFF80 (HRAM). Snapshots a save state, so it reflects the current
   *  frame. Addresses are interpreted for whichever console the loaded ROM is. */
  async readMemory(addr, length = 1) { return this._mem.readMemory(await this.saveState(), addr, length); }
  async readU8(addr) { return (await this.readMemory(addr, 1)).readUInt8(0); }
  async readU16(addr) { return (await this.readMemory(addr, 2)).readUInt16LE(0); }
  async readU32(addr) { return (await this.readMemory(addr, 4)).readUInt32LE(0); }

  async close() {
    await this.browser?.close();
    await new Promise((r) => this.server?.close(r));
    if (this._tempRom) await unlink(this._tempRom).catch(() => {});
  }
}

// ---- CLI ----
if (import.meta.url === `file://${process.argv[1]}`) {
  const [cmd, rom, framesArg, outArg] = process.argv.slice(2);
  if (cmd !== 'shot' || !rom) {
    console.error('Usage: node driver.mjs shot <rom.gba> [frames=300] [out.png=out/shot.png]');
    process.exit(1);
  }
  const frames = Number(framesArg || 300);
  const out = outArg || 'out/shot.png';
  const gba = await GBAHarness.launch(rom);
  await gba.waitFrames(frames);
  await gba.screenshot(out);
  console.log(`Ran ${frames} frames of ${rom}; screenshot -> ${out} (frame ${await gba.frameNum()})`);
  await gba.close();
}
