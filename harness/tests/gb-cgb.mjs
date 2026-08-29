// Exercises the CGB-only WRAM banking path (SVBK) in the GB memory parser.
//
// A Game Boy Color ROM writes a distinct sentinel (0xB0+bank) into 0xD000 of every
// WRAM bank, then selects SVBK=2. The parser must (a) see 32 KB WRAM, (b) return the
// SVBK-selected bank at 0xD000, and (c) hold distinct data in other banks.
//
//   node tests/gb-cgb.mjs   (exit 0 = pass, 1 = fail)

import { writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { EmuHarness } from '../driver.mjs';

let failed = 0;
const ok = (name, cond, detail) => {
  console.log(`${cond ? 'PASS' : 'FAIL'}: ${name}${detail ? ` — ${detail}` : ''}`);
  if (!cond) failed++;
};

function makeCgbRom() {
  const rom = Buffer.alloc(0x8000, 0);
  rom[0x100] = 0x00; rom[0x101] = 0xc3; rom[0x102] = 0x50; rom[0x103] = 0x01; // nop; jp 0x150
  Buffer.from('CGBBANK').copy(rom, 0x134);
  rom[0x143] = 0xc0; // CGB-only flag -> gambatte runs in CGB mode (32KB WRAM)
  const code = [
    0x3e, 0xb0, 0xea, 0x00, 0xc0,       // 0x150 ld a,0xB0; ld (C000),a   (bank 0 sentinel)
    0x0e, 0x01,                         // 0x155 ld c,1
    // loop (0x157): SVBK=c; write 0xB0+c to 0xD000
    0x79, 0xe0, 0x70,                   // 0x157 ld a,c; ldh (70),a       (SVBK=c)
    0x3e, 0xb0, 0x81, 0xea, 0x00, 0xd0, // 0x15a ld a,0xB0; add a,c; ld (D000),a
    0x0c, 0x79, 0xfe, 0x08,             // 0x160 inc c; ld a,c; cp 8
    0x20, 0xf1,                         // 0x164 jr nz,-15 -> 0x157
    0x3e, 0x02, 0xe0, 0x70,             // 0x166 ld a,2; ldh (70),a       (select SVBK=2)
    0x3e, 0x91, 0xe0, 0x40,             // 0x16a ld a,0x91; ldh (40),a    (LCD on)
    0x18, 0xfe,                         // 0x16e jr $                     (halt loop)
  ];
  for (let i = 0; i < code.length; i++) rom[0x150 + i] = code[i];
  let x = 0; for (let i = 0x134; i <= 0x14c; i++) x = (x - rom[i] - 1) & 0xff; rom[0x14d] = x;
  return rom;
}

await mkdir(path.resolve('out'), { recursive: true });
const romPath = path.resolve('out', 'cgbbank.gbc');
await writeFile(romPath, makeCgbRom());

const gba = await EmuHarness.launch(romPath, { timeout: 30000 });
try {
  await gba.waitFrames(30);

  const r = await gba.readRegions();
  ok('CGB mode: 32KB WRAM', r.wram && r.wram.length === 0x8000, `${r.wram?.length} bytes`);

  const svbk = await gba.readU8(0xff70);
  ok('SVBK selected bank 2', (svbk & 7) === 2, `0x${svbk.toString(16)}`);

  // 0xD000 must return the SVBK-selected bank (2), whose sentinel is 0xB2.
  const banked = await gba.readU8(0xd000);
  ok('0xD000 returns SVBK bank 2 sentinel (0xB2)', banked === 0xb2, `0x${banked.toString(16)}`);

  // 0xC000 is the fixed bank 0 (sentinel 0xB0).
  const fixed = await gba.readU8(0xc000);
  ok('0xC000 is fixed bank 0 (0xB0)', fixed === 0xb0, `0x${fixed.toString(16)}`);

  // Other banks hold distinct data in the raw region (proves selection matters).
  ok('bank 3 slot holds 0xB3', r.wram[3 * 0x1000] === 0xb3, `0x${r.wram[3 * 0x1000].toString(16)}`);
  ok('bank 7 slot holds 0xB7', r.wram[7 * 0x1000] === 0xb7, `0x${r.wram[7 * 0x1000].toString(16)}`);
} finally {
  await gba.close();
}
process.exit(failed ? 1 : 0);
