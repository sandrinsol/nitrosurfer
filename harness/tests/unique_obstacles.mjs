import { EmuHarness } from '../driver.mjs';
import { execSync } from 'child_process';
import assert from 'assert';
import path from 'path';
import { fileURLToPath } from 'url';

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

async function main() {
    console.log('Running unique obstacle tests...');
    const syms = getSymbols();
    const ADDR_OBSTACLES = syms['g_obstacles'];
    const ADDR_DEBRIS = syms['g_debris'];
    const ADDR_COINS = syms['g_coins'];

    // 1. PALM BEACH TEST
    const gbaBeach = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaBeach.waitFrames(25);
        await gbaBeach.tap('A', 4, 10); // car select
        await gbaBeach.waitFrames(15);
        await gbaBeach.tap('A', 4, 10); // track select
        await gbaBeach.waitFrames(15);
        await gbaBeach.tap('RIGHT', 4, 10);
        await gbaBeach.waitFrames(15);
        await gbaBeach.tap('A', 4, 10); // start race
        await gbaBeach.waitFrames(30);

        await gbaBeach.tap('START', 4, 10);
        await gbaBeach.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaBeach.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaBeach.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaBeach.writeU8(ADDR_COINS + c * 36, 0);

        await gbaBeach.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaBeach.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaBeach.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaBeach.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaBeach.waitFrames(5);
        let sprites = await getOamEntries(gbaBeach, 40);
        const beachObs = sprites.filter(s => s.tile >= 360 && s.tile <= 440);
        assert(beachObs.length >= 3, 'Beach obstacles missing in OAM');
        for (const s of beachObs) {
            assert.strictEqual(s.pal, 10, 'Beach obstacles must use Palette 10');
        }
        assert(beachObs.some(s => s.tile === 360), 'Missing Driftwood (360)');
        assert(beachObs.some(s => s.tile === 408), 'Missing Yacht Trailer (408)');
        assert(beachObs.some(s => s.tile === 424), 'Missing Woody Wagon (424)');
        console.log('✓ Palm Beach unique obstacles verified (Pal 10, tiles 360/408/424)');
    } finally {
        await gbaBeach.close();
    }

    // 2. WINTER SNOW TEST
    const gbaWinter = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaWinter.waitFrames(25);
        await gbaWinter.tap('A', 4, 10); // car select
        await gbaWinter.waitFrames(15);
        await gbaWinter.tap('A', 4, 10); // track select
        await gbaWinter.waitFrames(15);
        await gbaWinter.tap('RIGHT', 4, 10);
        await gbaWinter.waitFrames(10);
        await gbaWinter.tap('RIGHT', 4, 10);
        await gbaWinter.waitFrames(15);
        await gbaWinter.tap('A', 4, 10); // start race
        await gbaWinter.waitFrames(30);

        await gbaWinter.tap('START', 4, 10);
        await gbaWinter.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaWinter.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaWinter.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaWinter.writeU8(ADDR_COINS + c * 36, 0);

        await gbaWinter.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaWinter.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaWinter.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaWinter.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaWinter.waitFrames(5);
        let sprites = await getOamEntries(gbaWinter, 40);
        const winterObs = sprites.filter(s => s.tile >= 440 && s.tile <= 520);
        assert(winterObs.length >= 3, 'Winter obstacles missing in OAM');
        for (const s of winterObs) {
            assert.strictEqual(s.pal, 11, 'Winter obstacles must use Palette 11');
        }
        assert(winterObs.some(s => s.tile === 440), 'Missing Ice Ridge (440)');
        assert(winterObs.some(s => s.tile === 488), 'Missing Snowplow (488)');
        assert(winterObs.some(s => s.tile === 504), 'Missing Alpine SUV (504)');
        console.log('✓ Winter Snow unique obstacles verified (Pal 11, tiles 440/488/504)');
    } finally {
        await gbaWinter.close();
    }

    // 3. CAPE ORBITAL TEST
    const gbaOrbital = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaOrbital.waitFrames(25);
        await gbaOrbital.tap('A', 4, 10); // car select
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('A', 4, 10); // track select
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('DOWN', 4, 10); // Cape Orbital
        await gbaOrbital.waitFrames(15);
        await gbaOrbital.tap('A', 4, 10); // start race
        await gbaOrbital.waitFrames(30);

        await gbaOrbital.tap('START', 4, 10);
        await gbaOrbital.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaOrbital.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaOrbital.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaOrbital.writeU8(ADDR_COINS + c * 36, 0);

        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaOrbital.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaOrbital.waitFrames(5);
        let sprites = await getOamEntries(gbaOrbital, 40);
        const orbitalObs = sprites.filter(s => s.tile >= 520 && s.tile <= 600);
        assert(orbitalObs.length >= 3, 'Orbital obstacles missing in OAM');
        for (const s of orbitalObs) {
            assert.strictEqual(s.pal, 12, 'Orbital obstacles must use Palette 12');
        }
        assert(orbitalObs.some(s => s.tile === 520), 'Missing Cryo Pipe (520)');
        assert(orbitalObs.some(s => s.tile === 568), 'Missing Booster Crawler (568)');
        assert(orbitalObs.some(s => s.tile === 584), 'Missing Security Rover (584)');
        console.log('✓ Cape Orbital unique obstacles verified (Pal 12, tiles 520/568/584)');
    } finally {
        await gbaOrbital.close();
    }

    // 4. GOTHIC MIDNIGHT TEST
    const gbaGothic = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaGothic.waitFrames(25);
        await gbaGothic.tap('A', 4, 10); // car select
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('A', 4, 10); // track select
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('DOWN', 4, 10);
        await gbaGothic.waitFrames(10);
        await gbaGothic.tap('RIGHT', 4, 10); // Gothic Midnight
        await gbaGothic.waitFrames(15);
        await gbaGothic.tap('A', 4, 10); // start race
        await gbaGothic.waitFrames(30);

        await gbaGothic.tap('START', 4, 10);
        await gbaGothic.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaGothic.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaGothic.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaGothic.writeU8(ADDR_COINS + c * 36, 0);

        await gbaGothic.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaGothic.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaGothic.waitFrames(5);
        let sprites = await getOamEntries(gbaGothic, 40);
        const gothicObs = sprites.filter(s => s.tile >= 600 && s.tile <= 680);
        assert(gothicObs.length >= 3, 'Gothic obstacles missing in OAM');
        for (const s of gothicObs) {
            assert.strictEqual(s.pal, 13, 'Gothic obstacles must use Palette 13');
        }
        assert(gothicObs.some(s => s.tile === 600), 'Missing Crypt Slab (600)');
        assert(gothicObs.some(s => s.tile === 648), 'Missing Hearse (648)');
        assert(gothicObs.some(s => s.tile === 664), 'Missing Phantom Jalopy (664)');
        console.log('✓ Gothic Midnight unique obstacles verified (Pal 13, tiles 600/648/664)');
    } finally {
        await gbaGothic.close();
    }

    // 5. MAYA TEMPLE TRAIL TEST
    const gbaMaya = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaMaya.waitFrames(25);
        await gbaMaya.tap('A', 4, 10); // car select
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('A', 4, 10); // track select
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('DOWN', 4, 10);
        await gbaMaya.waitFrames(10);
        await gbaMaya.tap('RIGHT', 4, 10);
        await gbaMaya.waitFrames(10);
        await gbaMaya.tap('RIGHT', 4, 10); // Maya Temple Trail
        await gbaMaya.waitFrames(15);
        await gbaMaya.tap('A', 4, 10); // start race
        await gbaMaya.waitFrames(30);

        await gbaMaya.tap('START', 4, 10);
        await gbaMaya.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaMaya.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaMaya.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaMaya.writeU8(ADDR_COINS + c * 36, 0);

        await gbaMaya.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));
        await gbaMaya.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 60));

        await gbaMaya.waitFrames(5);
        let sprites = await getOamEntries(gbaMaya, 40);
        const mayaObs = sprites.filter(s => s.tile >= 680 && s.tile <= 760);
        assert(mayaObs.length >= 3, 'Maya obstacles missing in OAM');
        for (const s of mayaObs) {
            assert.strictEqual(s.pal, 14, 'Maya obstacles must use Palette 14');
        }
        assert(mayaObs.some(s => s.tile === 680), 'Missing Stone Totem (680)');
        assert(mayaObs.some(s => s.tile === 728), 'Missing Safari Hauler (728)');
        assert(mayaObs.some(s => s.tile === 744), 'Missing Safari Jeep (744)');
        console.log('✓ Maya Temple Trail unique obstacles verified (Pal 14, tiles 680/728/744)');
    } finally {
        await gbaMaya.close();
    }

    // 6. CITY HIGHWAY REGRESSION TEST
    const gbaCity = await EmuHarness.launch(ROM_PATH);
    try {
        await gbaCity.waitFrames(25);
        await gbaCity.tap('A', 4, 10); // car select
        await gbaCity.waitFrames(15);
        await gbaCity.tap('A', 4, 10); // track select
        await gbaCity.waitFrames(15);
        await gbaCity.tap('A', 4, 10); // start race
        await gbaCity.waitFrames(30);

        await gbaCity.tap('START', 4, 10);
        await gbaCity.waitFrames(10);

        for (let i = 0; i < 8; i++) await gbaCity.writeU8(ADDR_OBSTACLES + i * 48, 0);
        for (let d = 0; d < 16; d++) await gbaCity.writeU8(ADDR_DEBRIS + d * 28, 0);
        for (let c = 0; c < 16; c++) await gbaCity.writeU8(ADDR_COINS + c * 36, 0);

        await gbaCity.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
        await gbaCity.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_TRUCK, 1, 70));
        await gbaCity.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_CIVILIAN_CAR, 2, 50));

        await gbaCity.waitFrames(5);
        let sprites = await getOamEntries(gbaCity, 40);
        const cityObs = sprites.filter(s => (s.tile === 80 || s.tile === 112 || s.tile === 280));
        assert(cityObs.length >= 3, 'City obstacles missing in OAM');
        for (const s of cityObs) {
            assert.strictEqual(s.pal, 1, 'City obstacles must use Palette 1');
        }
        console.log('✓ City Highway obstacles regression verified (Pal 1, tiles 80/112/280)');
    } finally {
        await gbaCity.close();
    }

    console.log('ALL 6 UNIQUE OBSTACLE HARNESS TESTS PASSED!');
}

main().catch(err => {
    console.error('FAIL:', err);
    process.exit(1);
});
