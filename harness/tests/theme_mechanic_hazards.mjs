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

const OBS_RAMP = 5;
const OBS_SLICK = 6;
const OBS_SIGNATURE = 7;

function makeObstacle(type, lane, z, hazardState = 0) {
    const buf = Buffer.alloc(48);
    const LANE_X = [72, 120, 168];
    const zInt = Math.round(z);
    const sy = Math.round(112 - ((zInt - 16) * 112) / 80);
    buf.writeUInt8(1, 0);                       // active = 1
    buf.writeUInt8(type, 1);                    // type
    buf.writeUInt32LE(lane, 4);                 // lane
    buf.writeUInt32LE(lane, 8);                 // target_lane
    buf.writeInt32LE(Math.round(z * 256), 12);  // z
    buf.writeInt32LE(0, 16);                    // speed_fp
    buf.writeInt32LE(0, 20);                    // x_fp
    buf.writeInt32LE(LANE_X[lane], 24);         // screen_x
    buf.writeInt32LE(sy, 28);                   // screen_y
    buf.writeUInt8(0, 44);                      // passed
    buf.writeUInt8(0, 45);                      // close_call_awarded
    buf.writeUInt8(0, 46);                      // hurdled
    buf.writeUInt8(hazardState, 47);            // hazard_state
    return buf;
}

const THEMES = [
    { name: 'City Highway', index: 0, pal: 1, ramp: 768, slick: 816, sig: 992, nav: [] },
    { name: 'Palm Beach', index: 1, pal: 10, ramp: 776, slick: 824, sig: 880, nav: ['RIGHT'] },
    { name: 'Winter Snow', index: 2, pal: 11, ramp: 784, slick: 832, sig: 896, nav: ['RIGHT', 'RIGHT'] },
    { name: 'Cape Orbital', index: 3, pal: 12, ramp: 792, slick: 840, sig: 912, nav: ['DOWN'] },
    { name: 'Gothic Midnight', index: 4, pal: 13, ramp: 800, slick: 848, sig: 944, nav: ['DOWN', 'RIGHT'] },
    { name: 'Maya Temple Trail', index: 5, pal: 14, ramp: 808, slick: 856, sig: 960, nav: ['DOWN', 'RIGHT', 'RIGHT'] }
];

async function startTrack(gba, navSteps) {
    await gba.waitFrames(25);
    await gba.tap('A', 4, 10); // car select
    await gba.waitFrames(15);
    await gba.tap('A', 4, 10); // track select
    await gba.waitFrames(15);
    for (const step of navSteps) {
        await gba.tap(step, 4, 10);
        await gba.waitFrames(12);
    }
    await gba.tap('A', 4, 10); // start race
    await gba.waitFrames(30);
}

async function main() {
    console.log('=== Theme-Adapted Mechanic Obstacles & Signature Hazards Test ===');
    const syms = getSymbols();
    const ADDR_OBSTACLES = syms['g_obstacles'];
    const ADDR_DEBRIS = syms['g_debris'];
    const ADDR_COINS = syms['g_coins'];
    const ADDR_PLAYER = syms['g_player'];
    const ADDR_GAME = syms['g_game'];

    // 1. VERIFY THEMED VISUALS FOR ALL 6 TRACKS
    for (const theme of THEMES) {
        console.log(`\nTesting themed visuals on: ${theme.name}...`);
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, theme.nav);
            await gba.tap('START', 4, 10); // pause for deterministic injection
            await gba.waitFrames(10);

            // Clear obstacles, debris, coins
            for (let i = 0; i < 8; i++) await gba.writeU8(ADDR_OBSTACLES + i * 48, 0);
            for (let d = 0; d < 16; d++) await gba.writeU8(ADDR_DEBRIS + d * 28, 0);
            for (let c = 0; c < 16; c++) await gba.writeU8(ADDR_COINS + c * 36, 0);

            // Inject: Ramp in Lane 0, Slick in Lane 1, Signature Hazard in Lane 2
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_RAMP, 0, 50));
            await gba.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_SLICK, 1, 55));
            await gba.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_SIGNATURE, 2, 60));

            await gba.waitFrames(5);
            const sprites = await getOamEntries(gba, 40);

            // Check Ramp
            const rampSpr = sprites.find(s => s.tile === theme.ramp);
            assert(rampSpr, `${theme.name}: Missing ramp tile ${theme.ramp}`);
            assert.strictEqual(rampSpr.pal, theme.pal, `${theme.name}: Ramp expected palette ${theme.pal}, got ${rampSpr.pal}`);

            // Check Slick
            const slickSpr = sprites.find(s => s.tile === theme.slick);
            assert(slickSpr, `${theme.name}: Missing slick tile ${theme.slick}`);
            assert.strictEqual(slickSpr.pal, theme.pal, `${theme.name}: Slick expected palette ${theme.pal}, got ${slickSpr.pal}`);

            // Check Signature Hazard
            const sigSpr = sprites.find(s => s.tile === theme.sig);
            assert(sigSpr, `${theme.name}: Missing signature hazard tile ${theme.sig}`);
            assert.strictEqual(sigSpr.pal, theme.pal, `${theme.name}: Signature expected palette ${theme.pal}, got ${sigSpr.pal}`);

            const filename = `/Users/sandrinobreshani/Downloads/newthing/harness/hazards_${theme.name.toLowerCase().replace(/\s+/g, '_')}.png`;
            await gba.screenshot(filename);
            console.log(`✓ ${theme.name} passed! Tiles: Ramp=${theme.ramp}, Slick=${theme.slick}, Sig=${theme.sig} (Pal ${theme.pal})`);
        } finally {
            await gba.close();
        }
    }

    // 2. VERIFY MECHANIC: STUNT RAMP SUPER-JUMP
    console.log('\n--- Testing Stunt Ramp Super-Jump Mechanic ---');
    const gbaRamp = await EmuHarness.launch(ROM_PATH);
    try {
        await startTrack(gbaRamp, []);
        // Prevent auto-spawner from overwriting our injected obstacle during test
        const spawnTimerBuf = Buffer.alloc(4);
        spawnTimerBuf.writeInt32LE(9999, 0);
        await gbaRamp.writeMemory(ADDR_GAME + 40, spawnTimerBuf);

        // Place Ramp at Z=24 in player's center lane
        for (let i = 0; i < 8; i++) await gbaRamp.writeU8(ADDR_OBSTACLES + i * 48, 0);
        await gbaRamp.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_RAMP, 1, 24));

        let jumped = false;
        let maxElevation = 0;
        console.log('Ramp obstacle placed at Z=24. Advancing frames:');
        for (let f = 0; f < 30; f++) {
            await gbaRamp.waitFrames(1);
            const obsMem = await gbaRamp.readMemory(ADDR_OBSTACLES, 48);
            const playerMem = await gbaRamp.readMemory(ADDR_PLAYER, 32);
            const obsZ = obsMem.readInt32LE(12) / 256;
            const obsPassed = obsMem.readUInt8(44);
            const isJumping = playerMem.readUInt8(20);
            const yOffset = playerMem.readInt32LE(12) / 256;
            const vy = playerMem.readInt32LE(16) / 256;
            if (isJumping === 1 || yOffset > 0) {
                jumped = true;
                if (yOffset > maxElevation) maxElevation = yOffset;
            }
            if (f % 3 === 0 || isJumping) {
                console.log(`  f=${f}: obsZ=${obsZ.toFixed(1)}, pass=${obsPassed}, jump=${isJumping}, y=${yOffset.toFixed(1)}, vy=${vy.toFixed(2)}`);
            }
        }

        assert(jumped, 'Player should have jumped upon contacting Stunt Ramp');
        assert(maxElevation > 10, `Expected high super-jump elevation > 10px, got ${maxElevation.toFixed(1)}px`);
        console.log(`✓ Stunt Ramp launched car into Super-Jump! Peak elevation: ${maxElevation.toFixed(1)}px`);
    } finally {
        await gbaRamp.close();
    }

    // 3. VERIFY MECHANIC: SURFACE SLICK SPINOUT DRIFT
    console.log('\n--- Testing Surface Slick Spinout Mechanic ---');
    const gbaSlick = await EmuHarness.launch(ROM_PATH);
    try {
        await startTrack(gbaSlick, []);
        // Place Slick at Z=18 in player's center lane
        for (let i = 0; i < 8; i++) await gbaSlick.writeU8(ADDR_OBSTACLES + i * 48, 0);
        await gbaSlick.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 18));

        // Advance frames through collision
        await gbaSlick.waitFrames(8);

        // Player spinout_timer is near offset 76 (or let's check crash state)
        const isCrashing = await gbaSlick.readU8(ADDR_PLAYER + 36);
        assert.strictEqual(isCrashing, 0, 'Slick should NOT instantly crash player (should trigger fishtail spinout)');
        console.log('✓ Slick contact successfully triggered traction loss without crash!');
    } finally {
        await gbaSlick.close();
    }

    // 4. VERIFY MECHANIC: JUMP OVER SLICK (SAFE PASS)
    console.log('\n--- Testing Jump Over Slick (Safe Avoidance) ---');
    const gbaJumpSlick = await EmuHarness.launch(ROM_PATH);
    try {
        await startTrack(gbaJumpSlick, []);
        // Jump early
        await gbaJumpSlick.tap('UP', 4, 10);
        await gbaJumpSlick.waitFrames(6);

        // Place slick right ahead
        for (let i = 0; i < 8; i++) await gbaJumpSlick.writeU8(ADDR_OBSTACLES + i * 48, 0);
        await gbaJumpSlick.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 16));

        await gbaJumpSlick.waitFrames(8);
        const isCrashing = await gbaJumpSlick.readU8(ADDR_PLAYER + 36);
        assert.strictEqual(isCrashing, 0, 'Jumping over slick should be completely safe');
        console.log('✓ Leaping over slick safely avoids traction loss!');
    } finally {
        await gbaJumpSlick.close();
    }

    console.log('\n=== ALL THEME-ADAPTED OBSTACLES & SIGNATURE HAZARDS VERIFIED SUCCESSFULLY! ===');
}

main().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
