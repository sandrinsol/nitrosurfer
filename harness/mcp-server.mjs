#!/usr/bin/env node
// GBA test harness as an MCP server.
//
// Exposes the emulator to any MCP host (Claude Code, Claude Desktop, Cursor, ...)
// as tools an AI agent can call to test GBA games it builds — no third-party
// emulator program. screenshot returns the frame as an inline image so the agent
// literally sees the game; read_memory reads real RAM out of a save state.
//
// Run: node mcp-server.mjs   (stdio transport)

import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { z } from 'zod';
import path from 'node:path';
import { GBAHarness } from './driver.mjs';
import { readMemory, parseState } from './memory.mjs';
import { assertGolden } from './assert.mjs';

// ---- Emulator session (one ROM at a time) ----
let gba = null;
let romName = null;
const states = new Map(); // id -> Buffer (save states)

function requireRom() {
  if (!gba) throw new Error('No ROM loaded. Call load_rom first.');
}
const text = (s) => ({ content: [{ type: 'text', text: s }] });
const errText = (s) => ({ content: [{ type: 'text', text: s }], isError: true });

const BUTTONS = ['A', 'B', 'L', 'R', 'START', 'SELECT', 'UP', 'DOWN', 'LEFT', 'RIGHT'];
const Button = z.enum(BUTTONS);

const server = new McpServer({ name: 'gba-test-harness', version: '1.0.0' });

server.tool('load_rom', 'Boot a GBA/GB/GBC ROM (.gba/.gb/.gbc) in the headless emulator (replaces any current ROM). The system is auto-detected from the extension. Path is resolved relative to the server\'s working directory.',
  { rom: z.string().describe('Path to the ROM file (.gba, .gb, .gbc, or .zip)') },
  async ({ rom }) => {
    try {
      if (gba) { await gba.close().catch(() => {}); gba = null; states.clear(); }
      gba = await GBAHarness.launch(rom);
      romName = path.basename(rom);
      const dims = await gba.videoDimensions();
      const sys = gba.system === 'gb' ? 'GB/GBC' : 'GBA';
      const ramNote = gba.system === 'gba' ? '' : ' (read_memory is GBA-only; other tools work).';
      return text(`Loaded ${romName} as ${sys}. Video ${dims.w}x${dims.h}. Started at frame ${await gba.frameNum()}.${ramNote}`);
    } catch (e) { gba = null; return errText(`load_rom failed: ${e.message}`); }
  });

server.tool('wait_frames', 'Advance the emulator by N emulated frames (deterministic; ~60 frames = 1 second).',
  { frames: z.number().int().positive().describe('Number of emulated frames to advance') },
  async ({ frames }) => {
    try { requireRom(); await gba.waitFrames(frames); return text(`Advanced to frame ${await gba.frameNum()}.`); }
    catch (e) { return errText(e.message); }
  });

server.tool('press', 'Press a button: hold it for `hold` frames, then release and settle for `release` frames.',
  { button: Button, hold: z.number().int().positive().default(4), release: z.number().int().nonnegative().default(4) },
  async ({ button, hold, release }) => {
    try { requireRom(); await gba.tap(button, hold, release); return text(`Pressed ${button} (hold ${hold}f). Now at frame ${await gba.frameNum()}.`); }
    catch (e) { return errText(e.message); }
  });

server.tool('set_button', 'Hold or release a button without waiting (for combos / precise timing). Follow with wait_frames.',
  { button: Button, pressed: z.boolean() },
  async ({ button, pressed }) => {
    try { requireRom(); await gba.setButton(button, pressed); return text(`${button} ${pressed ? 'held' : 'released'}.`); }
    catch (e) { return errText(e.message); }
  });

server.tool('screenshot', 'Capture the current frame as a PNG image (returned inline so you can see it).',
  {},
  async () => {
    try {
      requireRom();
      const buf = await gba.screenshot();
      return { content: [
        { type: 'text', text: `Frame ${await gba.frameNum()} of ${romName} (240x160).` },
        { type: 'image', data: buf.toString('base64'), mimeType: 'image/png' },
      ] };
    } catch (e) { return errText(e.message); }
  });

server.tool('read_memory', 'Read GBA RAM at a bus address (e.g. 0x03000000 IWRAM, 0x02000000 EWRAM). Reads from a fresh save state, so it reflects the current frame.',
  {
    address: z.number().int().nonnegative().describe('GBA bus address, e.g. 0x03000010'),
    length: z.number().int().positive().max(4096).default(4),
    as: z.enum(['hex', 'u8', 'u16', 'u32', 's8', 's16', 's32']).default('hex').describe('Interpretation of the bytes'),
  },
  async ({ address, length, as }) => {
    try {
      requireRom();
      const buf = readMemory(await gba.saveState(), address, length);
      const hex = buf.toString('hex').replace(/(..)/g, '$1 ').trim();
      if (as === 'hex') return text(`@0x${address.toString(16)} [${length}B]: ${hex}`);
      const reader = { u8: 'readUInt8', u16: 'readUInt16LE', u32: 'readUInt32LE', s8: 'readInt8', s16: 'readInt16LE', s32: 'readInt32LE' }[as];
      const val = buf[reader](0);
      return text(`@0x${address.toString(16)} ${as}=${val} (0x${(val >>> 0).toString(16)}) raw: ${hex}`);
    } catch (e) { return errText(e.message); }
  });

server.tool('save_state', 'Snapshot the full machine state under a name you can restore later.',
  { id: z.string().default('default') },
  async ({ id }) => {
    try { requireRom(); states.set(id, await gba.saveState()); return text(`Saved state "${id}" (${states.get(id).length} bytes).`); }
    catch (e) { return errText(e.message); }
  });

server.tool('load_state', 'Restore a previously saved state by name.',
  { id: z.string().default('default') },
  async ({ id }) => {
    try { requireRom(); const st = states.get(id); if (!st) return errText(`No saved state "${id}".`); await gba.loadState(st); return text(`Restored state "${id}". Frame ${await gba.frameNum()}.`); }
    catch (e) { return errText(e.message); }
  });

server.tool('assert_golden', 'Compare the current frame to a stored golden PNG (created on first call). Fails if more than `threshold` fraction of pixels differ.',
  {
    golden: z.string().describe('Path to the golden PNG (created if missing)'),
    threshold: z.number().min(0).max(1).default(0.02),
  },
  async ({ golden, threshold }) => {
    try {
      requireRom();
      const res = await assertGolden(await gba.screenshot(), golden, { threshold });
      if (res.created) return text(`Golden created at ${res.goldenPath} (baseline saved).`);
      const pct = (res.fraction * 100).toFixed(2);
      return res.pass
        ? text(`PASS: ${pct}% pixels differ (<= ${(threshold * 100)}%).`)
        : errText(`FAIL: ${pct}% pixels differ (> ${(threshold * 100)}%). Diff image: ${res.diffPath}`);
    } catch (e) { return errText(e.message); }
  });

server.tool('frame_number', 'Get the current emulated frame number.', {},
  async () => { try { requireRom(); return text(String(await gba.frameNum())); } catch (e) { return errText(e.message); } });

server.tool('reset', 'Reset the current game (soft restart).', {},
  async () => { try { requireRom(); await gba.restart(); return text('Game reset.'); } catch (e) { return errText(e.message); } });

server.tool('status', 'Report whether a ROM is loaded and the current frame.', {},
  async () => text(gba ? `ROM: ${romName} (${gba.system === 'gb' ? 'GB/GBC' : 'GBA'}), frame ${await gba.frameNum()}, ${states.size} saved states${gba.system === 'gba' ? ', read_memory available' : ', read_memory GBA-only'}.` : 'No ROM loaded.'));

// Clean shutdown.
for (const sig of ['SIGINT', 'SIGTERM']) process.on(sig, async () => { try { await gba?.close(); } catch {} process.exit(0); });

const transport = new StdioServerTransport();
await server.connect(transport);
console.error('gba-test-harness MCP server ready (stdio).');
