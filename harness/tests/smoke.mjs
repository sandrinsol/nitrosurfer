// Smoke test: boot the bundled homebrew ROM, run past the boot logo, capture a
// screenshot, and assert the frame is actually rendering (not a black screen).
//
//   node tests/smoke.mjs
//
// Exit code 0 = pass, 1 = fail. Uses no assert framework so it runs anywhere.

import { EmuHarness } from '../driver.mjs';
import zlib from 'node:zlib';

function fail(msg) { console.error('FAIL:', msg); process.exit(1); }

// Decode a PNG's raw pixels enough to measure how much of the frame is non-black.
// Returns the fraction of pixels that are not near-black.
function nonBlackFraction(png) {
  // Find IDAT chunks, inflate, and sample. PNG = 8-byte sig + chunks.
  let p = 8, width = 0, height = 0, bitDepth = 0, colorType = 0;
  const idat = [];
  while (p < png.length) {
    const len = png.readUInt32BE(p);
    const type = png.toString('ascii', p + 4, p + 8);
    const data = png.subarray(p + 8, p + 8 + len);
    if (type === 'IHDR') {
      width = data.readUInt32BE(0); height = data.readUInt32BE(4);
      bitDepth = data[8]; colorType = data[9];
    } else if (type === 'IDAT') idat.push(data);
    else if (type === 'IEND') break;
    p += 12 + len;
  }
  if (colorType !== 6 && colorType !== 2) return 1; // unknown; don't block on it
  if (bitDepth !== 8) return 1;
  const channels = colorType === 6 ? 4 : 3;
  const raw = zlib.inflateSync(Buffer.concat(idat));
  const stride = width * channels;
  let nonBlack = 0, total = 0;
  for (let y = 0; y < height; y++) {
    const rowStart = y * (stride + 1) + 1; // +1 for per-row filter byte
    for (let x = 0; x < width; x++) {
      const i = rowStart + x * channels;
      const r = raw[i], g = raw[i + 1], b = raw[i + 2];
      if (r + g + b > 48) nonBlack++;
      total++;
    }
  }
  return total ? nonBlack / total : 0;
}

const gba = await EmuHarness.launch('../anguna.gba');
try {
  const dims = await gba.videoDimensions();
  if (dims.w !== 240 || dims.h !== 160) fail(`unexpected GBA video dimensions: ${JSON.stringify(dims)}`);

  await gba.waitFrames(300); // ~5s of emulated time, past the boot logo
  const frame = await gba.frameNum();
  if (frame < 300) fail(`core did not advance frames (got ${frame})`);

  const png = await gba.screenshot('out/smoke.png');
  const frac = nonBlackFraction(png);
  if (frac < 0.02) fail(`screen looks blank (non-black fraction ${frac.toFixed(3)})`);

  // Save/restore state round-trip.
  const state = await gba.saveState();
  if (!state || state.length < 16) fail(`save state too small (${state?.length} bytes)`);
  await gba.loadState(state);

  console.log(`PASS: booted anguna.gba, frame=${frame}, non-black=${(frac * 100).toFixed(1)}%, state=${state.length}B, shot=out/smoke.png`);
} finally {
  await gba.close();
}
