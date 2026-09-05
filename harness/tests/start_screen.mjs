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

async function getOamEntries(gba, count = 20) {
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

async function runTest() {
    console.log('=== Nitro Surfer Start Screen & Navigation Verification ===');
    const syms = getSymbols();
    assert(syms['g_game'], 'Symbol g_game not found in ELF!');

    const ADDR_STATE = syms['g_game'];

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        console.log('1. Booting game into STATE_START...');
        await gba.waitFrames(30);

        // Verify initial state
        const state0 = await gba.readU32(ADDR_STATE);
        console.log(`Initial state: ${state0} (expected 0=STATE_START)`);
        assert.strictEqual(state0, 0, `Expected STATE_START (0), got ${state0}`);

        // Verify Hero Car, Dual Exhaust Nitro Flame, and Neon Ground Underglow sprites in OAM
        const sprites = await getOamEntries(gba, 10);
        console.log('Inspecting active start screen hero sprites:');
        for (const s of sprites.slice(0, 5)) {
            console.log(`  Sprite #${s.index}: tile=${s.tile}, pal=${s.pal}, x=${s.x}, y=${s.y}`);
        }

        // Sprite 0: Underglow (tile 760, 32x16, pal 2)
        const underglow = sprites[0];
        assert.strictEqual(underglow.tile, 760, `Expected Underglow tile 760, got ${underglow.tile}`);
        assert.strictEqual(underglow.pal, 2, `Expected Underglow palette 2, got ${underglow.pal}`);
        assert.strictEqual(underglow.x, 104, `Expected Underglow X=104, got ${underglow.x}`);

        // Sprite 1: Nitro Exhaust Flame (tile 72/248/252/348, pal 2)
        const flame = sprites[1];
        assert([72, 248, 252, 348].includes(flame.tile), `Expected Nitro Flame tile, got ${flame.tile}`);
        assert.strictEqual(flame.pal, 2, `Expected Nitro Flame palette 2, got ${flame.pal}`);
        assert.strictEqual(flame.x, 112, `Expected Nitro Flame X=112, got ${flame.x}`);

        // Sprite 2: Hero Supercar (tile 0=TILE_PLAYER_STRAIGHT, pal 4=RED)
        const car = sprites[2];
        assert.strictEqual(car.tile, 0, `Expected Player Straight tile 0, got ${car.tile}`);
        assert.strictEqual(car.pal, 4, `Expected Player Red Car palette 4, got ${car.pal}`);
        assert.strictEqual(car.x, 104, `Expected Player Car X=104, got ${car.x}`);

        // Take high-resolution screenshot of Start Screen
        const startPngPath = path.resolve(__dirname, '../start_screen.png');
        await gba.screenshot(startPngPath);
        console.log(`Captured Start Screen screenshot -> ${startPngPath}`);

        // 2. Test START RACE from Start Screen (pressing A) -> goes directly to Car Selection (STATE_TITLE)
        console.log('2. Pressing A on Start Screen -> Selecting car color...');
        await gba.tap('A', 4, 10);
        await gba.waitFrames(10);
        const stateCarSelect = await gba.readU32(ADDR_STATE);
        console.log(`State after Start Race: ${stateCarSelect} (expected 1=STATE_TITLE)`);
        assert.strictEqual(stateCarSelect, 1, `Expected STATE_TITLE (1), got ${stateCarSelect}`);

        // Verify car color change works in STATE_TITLE
        await gba.tap('RIGHT', 4, 10);
        await gba.waitFrames(5);

        // Test B in Car Selection returns to Start Screen
        console.log('3. Pressing B in Car Selection -> returning to Start Screen...');
        await gba.tap('B', 4, 10);
        await gba.waitFrames(10);
        const stateBackStart = await gba.readU32(ADDR_STATE);
        console.log(`State after B: ${stateBackStart} (expected 0=STATE_START)`);
        assert.strictEqual(stateBackStart, 0, `Expected STATE_START (0), got ${stateBackStart}`);

        // 4. Test START RACE with START button
        console.log('4. Pressing START on Start Screen -> Selecting car color...');
        await gba.tap('START', 4, 10);
        await gba.waitFrames(10);
        const stateCarSelect2 = await gba.readU32(ADDR_STATE);
        assert.strictEqual(stateCarSelect2, 1, `Expected STATE_TITLE (1), got ${stateCarSelect2}`);

        // 5. Confirm car color (press A) -> transitions to Track Selection (STATE_SELECT_MAP)
        console.log('5. Confirming car color (A) -> Entering Track Selection...');
        await gba.tap('A', 4, 10);
        await gba.waitFrames(10);
        const stateTrackSelect = await gba.readU32(ADDR_STATE);
        console.log(`State after Car Confirmed: ${stateTrackSelect} (expected 5=STATE_SELECT_MAP)`);
        assert.strictEqual(stateTrackSelect, 5, `Expected STATE_SELECT_MAP (5), got ${stateTrackSelect}`);

        // 6. Test B in Track Selection returns to Car Selection
        console.log('6. Pressing B in Track Selection -> returning to Car Selection...');
        await gba.tap('B', 4, 10);
        await gba.waitFrames(10);
        const stateCarSelectBack = await gba.readU32(ADDR_STATE);
        console.log(`State after B: ${stateCarSelectBack} (expected 1=STATE_TITLE)`);
        assert.strictEqual(stateCarSelectBack, 1, `Expected STATE_TITLE (1), got ${stateCarSelectBack}`);

        // 7. Test full path: Car Selection -> Track Selection -> Play Race
        console.log('7. Finalizing selection to start race...');
        await gba.tap('A', 4, 10); // Enter Track Selection
        await gba.waitFrames(10);
        assert.strictEqual(await gba.readU32(ADDR_STATE), 5, 'Expected STATE_SELECT_MAP');

        await gba.tap('A', 4, 10); // Start race
        await gba.waitFrames(15);
        const statePlay = await gba.readU32(ADDR_STATE);
        console.log(`State after starting race: ${statePlay} (expected 2=STATE_PLAY)`);
        assert.strictEqual(statePlay, 2, `Expected STATE_PLAY (2), got ${statePlay}`);

        console.log('=== ALL START SCREEN AND NAVIGATION TESTS PASSED! ===');
    } finally {
        await gba.close();
    }
}

runTest().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
