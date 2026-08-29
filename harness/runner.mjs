// Input-script runner: drive a ROM through a list of steps and dump a screenshot
// timeline the AI can read back. Steps are plain JSON so an AI can author them.
//
//   node runner.mjs <script.json> [outDir]
//
// Script shape:
//   {
//     "rom": "../anguna.gba",
//     "steps": [
//       { "wait": 240 },
//       { "shot": "title" },
//       { "tap": "START" },
//       { "hold": "RIGHT", "frames": 90 },
//       { "record": { "frames": 300, "every": 30, "prefix": "walk" } },
//       { "assert": "goldens/title.png", "threshold": 0.02, "name": "title" },
//       { "saveState": "checkpoint" },
//       { "loadState": "checkpoint" }
//     ]
//   }
//
// Output: <outDir>/NN-*.png screenshots + <outDir>/timeline.json manifest.
// Exit code is nonzero if any `assert` step fails.

import { readFile, writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { EmuHarness } from './driver.mjs';
import { assertGolden } from './assert.mjs';

const scriptPath = process.argv[2];
if (!scriptPath) {
  console.error('Usage: node runner.mjs <script.json> [outDir]');
  process.exit(1);
}
const script = JSON.parse(await readFile(path.resolve(process.cwd(), scriptPath), 'utf8'));
const outDir = path.resolve(
  process.cwd(),
  process.argv[3] || path.join('out', path.basename(scriptPath).replace(/\.json$/i, ''))
);
await mkdir(outDir, { recursive: true });

const gba = await EmuHarness.launch(script.rom);
const timeline = [];
const states = new Map();
let seq = 0;
let failures = 0;

const shotFile = (name) => `${String(seq++).padStart(3, '0')}-${name}.png`;

async function snap(name) {
  const file = shotFile(name);
  await gba.screenshot(path.join(outDir, file));
  return file;
}

try {
  for (let i = 0; i < script.steps.length; i++) {
    const step = script.steps[i];
    const frame = await gba.frameNum();
    const entry = { i, frame };

    if ('wait' in step) {
      entry.action = `wait ${step.wait}`;
      await gba.waitFrames(step.wait);
    } else if ('tap' in step) {
      entry.action = `tap ${step.tap}`;
      await gba.tap(step.tap, step.hold ?? 4, step.release ?? 4);
    } else if ('press' in step) {
      entry.action = `press ${step.press}`;
      await gba.setButton(step.press, true);
    } else if ('release' in step) {
      entry.action = `release ${step.release}`;
      await gba.setButton(step.release, false);
    } else if ('hold' in step) {
      const frames = step.frames ?? 30;
      entry.action = `hold ${step.hold} ${frames}f`;
      await gba.setButton(step.hold, true);
      await gba.waitFrames(frames);
      await gba.setButton(step.hold, false);
    } else if ('shot' in step) {
      entry.action = `shot ${step.shot}`;
      entry.file = await snap(step.shot);
    } else if ('record' in step) {
      const { frames = 300, every = 30, prefix = 'frame' } = step.record;
      entry.action = `record ${frames}f/${every}`;
      entry.files = [];
      for (let f = 0; f < frames; f += every) {
        await gba.waitFrames(every);
        entry.files.push(await snap(`${prefix}-${await gba.frameNum()}`));
      }
    } else if ('saveState' in step) {
      entry.action = `saveState ${step.saveState}`;
      states.set(step.saveState, await gba.saveState());
    } else if ('loadState' in step) {
      entry.action = `loadState ${step.loadState}`;
      const st = states.get(step.loadState);
      if (!st) throw new Error(`no saved state named "${step.loadState}"`);
      await gba.loadState(st);
    } else if ('assert' in step) {
      const name = step.name || path.basename(step.assert).replace(/\.png$/i, '');
      entry.action = `assert ${name}`;
      const candidate = await gba.screenshot(path.join(outDir, shotFile(name)));
      const res = await assertGolden(candidate, path.resolve(path.dirname(path.resolve(process.cwd(), scriptPath)), step.assert), {
        threshold: step.threshold ?? 0.02,
      });
      entry.assert = {
        golden: path.relative(process.cwd(), res.goldenPath),
        created: res.created, pass: res.pass,
        fraction: Number((res.fraction ?? 0).toFixed(4)),
        ...(res.diffPath ? { diff: path.relative(process.cwd(), res.diffPath) } : {}),
      };
      if (!res.pass) { failures++; console.error(`  ✗ assert ${name}: ${(res.fraction * 100).toFixed(2)}% differ (> ${(res.threshold ?? 0.02) * 100}%)`); }
      else console.log(`  ✓ assert ${name}${res.created ? ' (golden created)' : ''}`);
    } else {
      entry.action = 'unknown';
      console.error(`  ! step ${i}: unknown step`, step);
    }
    timeline.push(entry);
  }
} finally {
  await gba.close();
}

await writeFile(
  path.join(outDir, 'timeline.json'),
  JSON.stringify({ rom: script.rom, outDir: path.relative(process.cwd(), outDir), pass: failures === 0, steps: timeline }, null, 2)
);

console.log(`\nTimeline -> ${path.relative(process.cwd(), path.join(outDir, 'timeline.json'))}  (${seq} screenshots, ${failures} failed assert${failures === 1 ? '' : 's'})`);
process.exit(failures ? 1 : 0);
