// Validates save-state-based memory writes for GBA and GB/GBC.
//
// Two levels:
//  - Pure-function: patching a captured save state lands at the right address for
//    both consoles (deterministic; proves the write offset math).
//  - Live end-to-end (GB): write_memory applied to the running machine sticks, using
//    a ROM that leaves WRAM alone so the value isn't overwritten by gameplay.
//
//   node tests/write.mjs   (exit 0 = pass, 1 = fail)

import { writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { EmuHarness } from '../driver.mjs';
import * as gbaMem from '../memory.mjs';
import * as gbMem from '../gb-memory.mjs';

let failed = 0;
const ok = (name, cond, detail) => {
  console.log(`${cond ? 'PASS' : 'FAIL'}: ${name}${detail ? ` — ${detail}` : ''}`);
  if (!cond) failed++;
};

// --- GBA: pure-function write offset correctness ---
{
  const gba = await EmuHarness.launch('../anguna.gba', { timeout: 30000 });
  try {
    await gba.waitFrames(120);
    const s = await gba.saveState();
    gbaMem.writeMemory(s, 0x02000000, Buffer.from([0xef, 0xbe, 0xad, 0xde])); // EWRAM
    gbaMem.writeMemory(s, 0x03000010, Buffer.from([1, 2, 3, 4]));             // IWRAM
    ok('GBA patch lands in EWRAM', gbaMem.readU32(s, 0x02000000) === 0xdeadbeef,
      `0x${gbaMem.readU32(s, 0x02000000).toString(16)}`);
    ok('GBA patch lands in IWRAM', gbaMem.readMemory(s, 0x03000010, 4).equals(Buffer.from([1, 2, 3, 4])));
  } finally { await gba.close(); }
}

// --- GB: pure-function + live end-to-end ---
{
  const rom = Buffer.alloc(0x8000, 0);
  rom[0x100] = 0x00; rom[0x101] = 0xc3; rom[0x102] = 0x50; rom[0x103] = 0x01;
  let x = 0; for (let i = 0x134; i <= 0x14c; i++) x = (x - rom[i] - 1) & 0xff; rom[0x14d] = x;
  const code = [0x3e, 0x91, 0xe0, 0x40, 0x18, 0xfe]; // LCD on; halt loop (never writes WRAM)
  for (let i = 0; i < code.length; i++) rom[0x150 + i] = code[i];
  await mkdir(path.resolve('out'), { recursive: true });
  const romPath = path.resolve('out', 'write.gb');
  await writeFile(romPath, rom);

  const gba = await EmuHarness.launch(romPath, { timeout: 30000 });
  try {
    await gba.waitFrames(30);

    // Pure-function
    const s = await gba.saveState();
    gbMem.writeMemory(s, 0xc020, Buffer.from([0xaa, 0xbb]));
    ok('GB patch lands in WRAM', gbMem.readMemory(s, 0xc020, 2).equals(Buffer.from([0xaa, 0xbb])));

    // Live end-to-end (write_memory applies over frames internally)
    await gba.writeU8(0xc010, 0x42);
    ok('GB live writeU8 sticks (WRAM)', (await gba.readU8(0xc010)) === 0x42, `0x${(await gba.readU8(0xc010)).toString(16)}`);
    await gba.writeU16(0xc020, 0xbeef);
    ok('GB live writeU16 little-endian sticks', (await gba.readU16(0xc020)) === 0xbeef, `0x${(await gba.readU16(0xc020)).toString(16)}`);
  } finally { await gba.close(); }
}

process.exit(failed ? 1 : 0);
