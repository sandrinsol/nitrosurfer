// Proves, for GB/GBC (gambatte), the two properties a test harness must have:
//   1. Input actually affects the game (button press changes RAM *and* the frame).
//   2. Runs are deterministic (identical scripts -> bit-identical frames).
//
//   node tests/gb-determinism.mjs   (exit 0 = pass, 1 = fail)

import { writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { GBAHarness } from '../driver.mjs';
import { compare } from '../assert.mjs';

let failed = 0;
const ok = (name, cond, detail) => {
  console.log(`${cond ? 'PASS' : 'FAIL'}: ${name}${detail ? ` — ${detail}` : ''}`);
  if (!cond) failed++;
};

// Interactive 32KB DMG ROM: each loop, read the action buttons, store the nibble at
// WRAM 0xC000, and set the background palette to 0xFF while any button is held (0xE4
// otherwise). So holding A changes both memory (0xC000) and the screen (white->black).
function makeInteractiveGbRom() {
  const rom = Buffer.alloc(0x8000, 0);
  rom[0x100] = 0x00; rom[0x101] = 0xc3; rom[0x102] = 0x50; rom[0x103] = 0x01; // nop; jp 0x150
  Buffer.from('INPUTTEST').copy(rom, 0x134);
  let x = 0; for (let i = 0x134; i <= 0x14c; i++) x = (x - rom[i] - 1) & 0xff; rom[0x14d] = x;
  const code = [
    0x3e, 0x91, 0xe0, 0x40,             // 0x150 ld a,0x91; ldh (40),a   (LCDC on)
    0x3e, 0x10, 0xe0, 0x00,             // 0x154 ld a,0x10; ldh (00),a   (select action buttons)
    0xf0, 0x00, 0xf0, 0x00,             // 0x158 ldh a,(00); ldh a,(00)  (read P1, settle)
    0xe6, 0x0f,                         // 0x15c and 0x0F
    0xea, 0x00, 0xc0,                   // 0x15e ld (C000),a
    0xfe, 0x0f,                         // 0x161 cp 0x0F  (0x0F = nothing pressed)
    0x28, 0x04,                         // 0x163 jr z,+4 -> 0x169
    0x3e, 0xff,                         // 0x165 ld a,0xFF (pressed)
    0x18, 0x02,                         // 0x167 jr +2 -> 0x16B
    0x3e, 0xe4,                         // 0x169 ld a,0xE4 (idle)
    0xe0, 0x47,                         // 0x16b ldh (47),a  (BGP)
    0xc3, 0x54, 0x01,                   // 0x16d jp 0x154  (loop)
  ];
  for (let i = 0; i < code.length; i++) rom[0x150 + i] = code[i];
  return rom;
}

await mkdir(path.resolve('out'), { recursive: true });
const romPath = path.resolve('out', 'gb-input.gb');
await writeFile(romPath, makeInteractiveGbRom());

// One scripted run: idle snapshot, then hold A, capturing RAM + frames.
async function run() {
  const gba = await GBAHarness.launch(romPath, { timeout: 30000 });
  await gba.waitFrames(60);
  const idleMem = await gba.readU8(0xc000);
  const idleShot = await gba.screenshot();
  await gba.setButton('A', true);
  await gba.waitFrames(30);
  const pressedMem = await gba.readU8(0xc000);
  const pressedShot = await gba.screenshot();
  await gba.setButton('A', false);
  await gba.close();
  return { idleMem, pressedMem, idleShot, pressedShot };
}

const a = await run();
const b = await run();

// 1. Input effect — memory and frame both change when A is held.
ok('GB input changes RAM (0xC000)', a.idleMem === 0x0f && a.pressedMem !== a.idleMem,
  `idle=0x${a.idleMem.toString(16)} pressed=0x${a.pressedMem.toString(16)}`);
const frameDiff = compare(a.idleShot, a.pressedShot).fraction;
ok('GB input changes the frame', frameDiff > 0.5, `${(frameDiff * 100).toFixed(1)}% differ`);

// 2. Determinism — matched checkpoints are bit-identical across runs.
const idleDet = compare(a.idleShot, b.idleShot).fraction;
const pressDet = compare(a.pressedShot, b.pressedShot).fraction;
ok('GB deterministic idle frame', idleDet === 0, `${(idleDet * 100).toFixed(3)}% differ`);
ok('GB deterministic post-input frame', pressDet === 0, `${(pressDet * 100).toFixed(3)}% differ`);
ok('GB deterministic RAM', a.pressedMem === b.pressedMem);

process.exit(failed ? 1 : 0);
