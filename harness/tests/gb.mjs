// GB/GBC support: boot a minimal Game Boy ROM (generated here, no binary checked
// in) via the gambatte core, and confirm input/screenshot/state work and that
// read_memory is cleanly gated to GBA.
//
//   node tests/gb.mjs   (exit 0 = pass, 1 = fail)

import { writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { GBAHarness } from '../driver.mjs';

let failed = 0;
const ok = (name, cond, detail) => {
  console.log(`${cond ? 'PASS' : 'FAIL'}: ${name}${detail ? ` — ${detail}` : ''}`);
  if (!cond) failed++;
};

// Minimal 32KB ROM-only Game Boy program: turn the LCD on, then loop forever.
function makeGbRom() {
  const rom = Buffer.alloc(0x8000, 0);
  rom[0x100] = 0x00; rom[0x101] = 0xc3; rom[0x102] = 0x50; rom[0x103] = 0x01; // nop; jp 0x150
  Buffer.from('TESTROM').copy(rom, 0x134);
  rom[0x147] = 0x00; rom[0x148] = 0x00; rom[0x149] = 0x00; // ROM ONLY, 32KB, no RAM
  let x = 0; for (let i = 0x134; i <= 0x14c; i++) x = (x - rom[i] - 1) & 0xff; rom[0x14d] = x;
  const code = [0x3e, 0x91, 0xe0, 0x40, 0x3e, 0xe4, 0xe0, 0x47, 0x18, 0xfe]; // LCDC on, BGP, jr $
  for (let i = 0; i < code.length; i++) rom[0x150 + i] = code[i];
  return rom;
}

await mkdir(path.resolve('out'), { recursive: true });
const romPath = path.resolve('out', 'testrom.gb');
await writeFile(romPath, makeGbRom());

const gba = await GBAHarness.launch(romPath, { timeout: 30000 });
try {
  ok('detected system is GB', gba.system === 'gb', gba.system);

  const dims = await gba.videoDimensions();
  ok('GB video dimensions 160x144', dims.w === 160 && dims.h === 144, `${dims.w}x${dims.h}`);

  await gba.waitFrames(120);
  ok('frames advance', (await gba.frameNum()) >= 120);

  await gba.tap('A'); // GB has A/B/START/SELECT/dpad; just exercise input
  const png = await gba.screenshot('out/gb-smoke.png');
  ok('screenshot captured', png.length > 0 && png.subarray(0, 8).equals(Buffer.from([137, 80, 78, 71, 13, 10, 26, 10])));

  const st = await gba.saveState();
  ok('save state works', st.length > 16 && st.subarray(0, 7).toString('latin1') === 'RASTATE', `${st.length}B`);
  await gba.loadState(st);

  let threw = false, msg = '';
  try { await gba.readU16(0x03000000); } catch (e) { threw = true; msg = e.message; }
  ok('read_memory is gated to GBA', threw, threw ? 'throws clear error' : 'did NOT gate!');
} finally {
  await gba.close();
}
process.exit(failed ? 1 : 0);
