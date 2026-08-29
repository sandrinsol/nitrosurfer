// Regression test that enforces the two properties a test harness must have:
//   1. Input actually affects the game (a pressed button changes the frame).
//   2. Runs are deterministic (identical scripts -> bit-identical frames).
//
//   node tests/determinism.mjs   (exit 0 = pass, 1 = fail)

import { GBAHarness } from '../driver.mjs';
import { compare } from '../assert.mjs';

let failed = 0;
const ok = (name, cond, detail) => {
  console.log(`${cond ? 'PASS' : 'FAIL'}: ${name}${detail ? ` — ${detail}` : ''}`);
  if (!cond) failed++;
};

// A fixed, deterministic scripted run. Returns screenshots at set checkpoints.
async function scriptedRun() {
  const gba = await GBAHarness.launch('../anguna.gba');
  const shots = {};
  await gba.waitFrames(600);
  shots.boot = await gba.screenshot();
  for (const b of ['START', 'A', 'START', 'A', 'START']) await gba.tap(b, 6, 20);
  await gba.waitFrames(120);
  shots.afterInput = await gba.screenshot();
  await gba.close();
  return shots;
}

// --- Run twice ---
const a = await scriptedRun();
const b = await scriptedRun();

// 1. Input effect
const inputDiff = compare(a.boot, a.afterInput).fraction;
ok('input changes the frame', inputDiff > 0.01, `${(inputDiff * 100).toFixed(2)}% differ`);

// 2. Determinism at matched checkpoints
const bootDet = compare(a.boot, b.boot).fraction;
const inputDet = compare(a.afterInput, b.afterInput).fraction;
ok('deterministic boot frame', bootDet === 0, `${(bootDet * 100).toFixed(3)}% differ`);
ok('deterministic post-input frame', inputDet === 0, `${(inputDet * 100).toFixed(3)}% differ`);

process.exit(failed ? 1 : 0);
