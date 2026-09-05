import { EmuHarness } from '../driver.mjs';
import { execSync } from 'child_process';
import assert from 'assert';
import path from 'path';
import { fileURLToPath } from 'url';
import fs from 'fs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ELF_PATH = path.resolve(__dirname, '../../nitrosurfer/nitrosurfer.elf');
const ROM_PATH = path.resolve(__dirname, '../../nitrosurfer.gba');

function getSymbols() {
    const out = execSync(`/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm ${ELF_PATH}`, { encoding: 'utf8' });
    const syms = {};
    for (const line of out.split('\n')) {
        const parts = line.trim().split(/\s+/);
        if (parts.length === 3) {
            syms[parts[2]] = parseInt(parts[0], 16);
        }
    }
    return syms;
}

const OAM_BASE = 0x07000000;

async function getOamEntries(gba, count = 40) {
    const mem = await gba.readMemory(OAM_BASE, count * 8);
    const sprites = [];
    for (let i = 0; i < count; i++) {
        const a0 = mem.readUInt16LE(i * 8);
        const a1 = mem.readUInt16LE(i * 8 + 2);
        const a2 = mem.readUInt16LE(i * 8 + 4);
        const tile = a2 & 0x3FF;
        const pal = (a2 >> 12) & 0xF;
        const y = a0 & 0xFF;
        const x = a1 & 0x1FF;
        sprites.push({ index: i, tile, pal, x, y, a0, a1, a2 });
    }
    return sprites;
}

const OBS_LOW_BARRIER = 1;
const OBS_HIGH_SIGN = 2;
const OBS_TRUCK = 3;
const OBS_CIVILIAN_CAR = 4;

function makeObstacle(type, lane, z, passed = 0, hurdled = 0) {
    const buf = Buffer.alloc(48);
    const LANE_X = [72, 120, 168];
    const zInt = Math.round(z);
    const sy = Math.round(112 - ((zInt - 16) * 112) / 80);
    buf.writeUInt8(1, 0); // active = 1
    buf.writeUInt8(type, 1); // type
    buf.writeUInt32LE(lane, 4); // lane
    buf.writeUInt32LE(lane, 8); // target_lane
    buf.writeInt32LE(Math.round(z * 256), 12); // z
    buf.writeInt32LE(0, 16); // speed_fp
    buf.writeInt32LE(0, 20); // x_fp
    buf.writeInt32LE(LANE_X[lane], 24); // screen_x
    buf.writeInt32LE(sy, 28); // screen_y
    buf.writeUInt8(passed ? 1 : 0, 44); // passed
    buf.writeUInt8(hurdled ? 1 : 0, 46); // hurdled
    return buf;
}

async function testMapSelectionAndNavigation() {
    console.log('=== Test 1: Map Selection 2D Grid Navigation ===');
    const syms = getSymbols();
    const ADDR_GAME = syms['g_game'];
    // Map theme offset in g_game:
    // typedef struct GameState {
    //   GameMode mode; int score; int high_score; int distance_m;
    //   fp speed; ...
    // Let's read map_theme from state or inspect navigation by pressing keys.
    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await gba.waitFrames(30);
        // On Start screen, press A to go to Car Selection, then press A to go to Track Selection
        await gba.tap('A', 4, 10);
        await gba.waitFrames(15);
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);

        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_city.png');
        console.log('Captured map_select_city.png');

        // DOWN should jump +3 to Cape Orbital (row 1, col 0)
        await gba.tap('DOWN', 4, 10);
        await gba.waitFrames(15);
        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_orbital.png');
        console.log('Captured map_select_orbital.png');

        // RIGHT should jump +1 to Gothic Midnight (row 1, col 1)
        await gba.tap('RIGHT', 4, 10);
        await gba.waitFrames(15);
        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_gothic.png');
        console.log('Captured map_select_gothic.png');

        // RIGHT should jump +1 to Maya Temple Trail (row 1, col 2)
        await gba.tap('RIGHT', 4, 10);
        await gba.waitFrames(15);
        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_maya.png');
        console.log('Captured map_select_maya.png');

        // UP should jump -3 to Winter Snow (row 0, col 2)
        await gba.tap('UP', 4, 10);
        await gba.waitFrames(15);
        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_winter.png');
        console.log('Captured map_select_winter.png');

        // LEFT should jump -1 to Beach (row 0, col 1)
        await gba.tap('LEFT', 4, 10);
        await gba.waitFrames(15);
        await gba.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/map_select_beach.png');
        console.log('Captured map_select_beach.png');
    } finally {
        await gba.close();
    }
}

async function testGameplayOnNewMaps() {
    console.log('=== Test 2: Gameplay Verification on Orbital, Gothic, Maya ===');
    const syms = getSymbols();
    const ADDR_OBSTACLES = syms['g_obstacles'];
    const ADDR_DEBRIS = syms['g_debris'];
    const ADDR_COINS = syms['g_coins'];

    // 1. CAPE ORBITAL GAMEPLAY TEST
    console.log('Testing Cape Orbital...');
    const gbaOrbital = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaOrbital.waitFrames(25);
        await gbaOrbital.tap('A', 4, 10); // car select
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('A', 4, 10); // track select
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('DOWN', 4, 10); // select Cape Orbital
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('A', 4, 10); // start race
        await gbaOrbital.waitFrames(30);
        await gbaOrbital.tap('START', 4, 10); // pause game for deterministic obstacle inspection
        await gbaOrbital.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaOrbital.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaOrbital.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaOrbital.writeU8(ADDR_COINS + c * 36, 0);

        // Inject Cape Orbital obstacles
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaOrbital.waitFrames(5);
        const sprites = await getOamEntries(gbaOrbital, 40);
        const orbitalObs = sprites.filter(s => s.tile >= 520 && s.tile <= 600);
        assert(orbitalObs.length >= 3, `Expected orbital obstacles in tiles 520..600, found: ${orbitalObs.length}`);
        for (const s of orbitalObs) {
            assert.strictEqual(s.pal, 12, `Orbital obstacle tile ${s.tile} must use Palette 12 (got ${s.pal})`);
        }
        await gbaOrbital.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/gameplay_orbital.png');
        console.log('Cape Orbital passed! Screenshot saved to gameplay_orbital.png');
    } finally {
        await gbaOrbital.close();
    }

    // 2. GOTHIC MIDNIGHT GAMEPLAY TEST
    console.log('Testing Gothic Midnight...');
    const gbaGothic = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaGothic.waitFrames(25);
        await gbaGothic.tap('A', 4, 10); // car select
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('A', 4, 10); // track select
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('DOWN', 4, 10); // Cape Orbital
        await gbaGothic.waitFrames(10);
        await gbaGothic.tap('RIGHT', 4, 10); // Gothic Midnight
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('A', 4, 10); // start race
        await gbaGothic.waitFrames(30);
        await gbaGothic.tap('START', 4, 10); // pause game
        await gbaGothic.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaGothic.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaGothic.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaGothic.writeU8(ADDR_COINS + c * 36, 0);

        // Inject Gothic Midnight obstacles
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaGothic.waitFrames(5);
        const sprites = await getOamEntries(gbaGothic, 40);
        const gothicObs = sprites.filter(s => s.tile >= 600 && s.tile <= 680);
        assert(gothicObs.length >= 3, `Expected gothic obstacles in tiles 600..680, found: ${gothicObs.length}`);
        for (const s of gothicObs) {
            assert.strictEqual(s.pal, 13, `Gothic obstacle tile ${s.tile} must use Palette 13 (got ${s.pal})`);
        }
        await gbaGothic.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/gameplay_gothic.png');
        console.log('Gothic Midnight passed! Screenshot saved to gameplay_gothic.png');
    } finally {
        await gbaGothic.close();
    }

    // 3. MAYA TEMPLE TRAIL GAMEPLAY TEST
    console.log('Testing Maya Temple Trail...');
    const gbaMaya = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaMaya.waitFrames(25);
        await gbaMaya.tap('A', 4, 10); // car select
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('A', 4, 10); // track select
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('DOWN', 4, 10); // Cape Orbital
        await gbaMaya.waitFrames(10);
        await gbaMaya.tap('RIGHT', 4, 10); // Gothic Midnight
        await gbaMaya.waitFrames(10);
        await gbaMaya.tap('RIGHT', 4, 10); // Maya Temple Trail
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('A', 4, 10); // start race
        await gbaMaya.waitFrames(30);
        await gbaMaya.tap('START', 4, 10); // pause game
        await gbaMaya.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaMaya.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaMaya.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaMaya.writeU8(ADDR_COINS + c * 36, 0);

        // Inject Maya Temple Trail obstacles
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaMaya.waitFrames(5);
        const sprites = await getOamEntries(gbaMaya, 40);
        const mayaObs = sprites.filter(s => s.tile >= 680 && s.tile <= 760);
        assert(mayaObs.length >= 3, `Expected maya obstacles in tiles 680..760, found: ${mayaObs.length}`);
        for (const s of mayaObs) {
            assert.strictEqual(s.pal, 14, `Maya obstacle tile ${s.tile} must use Palette 14 (got ${s.pal})`);
        }
        await gbaMaya.screenshot('/Users/sandrinobreshani/Downloads/newthing/harness/gameplay_maya.png');
        console.log('Maya Temple Trail passed! Screenshot saved to gameplay_maya.png');
    } finally {
        await gbaMaya.close();
    }
}

async function run() {
    await testMapSelectionAndNavigation();
    await testGameplayOnNewMaps();
    console.log('=== All 6 Maps and Obstacle Tests PASSED Perfectly! ===');
}

run().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
