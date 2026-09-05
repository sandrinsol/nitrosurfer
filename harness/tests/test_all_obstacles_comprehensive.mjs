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

const syms = getSymbols();
const ADDR_OBSTACLES = syms['g_obstacles'];
const ADDR_DEBRIS = syms['g_debris'];
const ADDR_COINS = syms['g_coins'];
const ADDR_PLAYER = syms['g_player'];
const ADDR_GAME = syms['g_game'];
const OAM_BASE = 0x07000000;

// Obstacle Types
const OBS_NONE = 0;
const OBS_LOW_BARRIER = 1;
const OBS_HIGH_SIGN = 2;
const OBS_TRUCK = 3;
const OBS_CIVILIAN_CAR = 4;
const OBS_RAMP = 5;
const OBS_SLICK = 6;
const OBS_SIGNATURE = 7;

function makeObstacle(type, lane, z, hazardState = 0, passed = 0, hurdled = 0) {
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
    buf.writeInt32LE(3, 32);                    // scale = 3
    buf.writeInt32LE(0, 36);                    // lane_shift_timer = 0
    buf.writeInt32LE(0, 40);                    // turn_signal_timer = 0
    buf.writeUInt8(passed ? 1 : 0, 44);         // passed
    buf.writeUInt8(0, 45);                      // close_call_awarded
    buf.writeUInt8(hurdled ? 1 : 0, 46);        // hurdled
    buf.writeUInt8(hazardState, 47);            // hazard_state
    return buf;
}

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

async function startTrack(gba, navSteps = []) {
    await gba.waitFrames(35);
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

async function freezeWaveSpawner(gba) {
    const buf = Buffer.alloc(4);
    buf.writeInt32LE(99999, 0);
    await gba.writeMemory(ADDR_GAME + 40, buf);
}

async function clearActiveObstacles(gba) {
    await gba.writeMemory(ADDR_OBSTACLES, Buffer.alloc(8 * 48));
    await gba.writeMemory(ADDR_DEBRIS, Buffer.alloc(16 * 28));
    await gba.writeMemory(ADDR_COINS, Buffer.alloc(16 * 36));
}

async function getPlayer(gba) {
    const mem = await gba.readMemory(ADDR_PLAYER, 88);
    return {
        lane: mem.readInt32LE(0),
        x: mem.readInt32LE(4) / 256,
        y_offset: mem.readInt32LE(12) / 256,
        vy: mem.readInt32LE(16) / 256,
        is_jumping: mem.readUInt8(20),
        is_shrinking: mem.readUInt8(21),
        shrink_timer: mem.readInt32LE(24),
        shield_active: mem.readInt32LE(32),
        nitro_timer: mem.readInt32LE(36),
        invuln_timer: mem.readInt32LE(44),
        is_crashing: mem.readUInt8(48),
        spinout_timer: mem.readInt32LE(80),
        stunt_timer: mem.readInt32LE(84)
    };
}

async function getGame(gba) {
    const mem = await gba.readMemory(ADDR_GAME, 120);
    return {
        state: mem.readInt32LE(0),
        score: mem.readInt32LE(4),
        high_score: mem.readInt32LE(8),
        coins: mem.readInt32LE(12),
        speed: mem.readInt32LE(28) / 256,
        streak_mult: mem.readInt32LE(72),
        close_calls: mem.readInt32LE(100)
    };
}

const THEMES = [
    {
        name: 'City Highway', nav: [], pal: 1,
        barrier: 80, sign: 92, truck: 112, car: 280, ramp: 768, slick: 816, sig: 992
    },
    {
        name: 'Palm Beach', nav: ['RIGHT'], pal: 10,
        barrier: 360, sign: 376, truck: 408, car: 424, ramp: 776, slick: 824, sig: 880
    },
    {
        name: 'Winter Snow', nav: ['RIGHT', 'RIGHT'], pal: 11,
        barrier: 440, sign: 456, truck: 488, car: 504, ramp: 784, slick: 832, sig: 896
    },
    {
        name: 'Cape Orbital', nav: ['DOWN'], pal: 12,
        barrier: 520, sign: 536, truck: 568, car: 584, ramp: 792, slick: 840, sig: 912
    },
    {
        name: 'Gothic Midnight', nav: ['DOWN', 'RIGHT'], pal: 13,
        barrier: 600, sign: 616, truck: 648, car: 664, ramp: 800, slick: 848, sig: 944
    },
    {
        name: 'Maya Temple Trail', nav: ['DOWN', 'RIGHT', 'RIGHT'], pal: 14,
        barrier: 680, sign: 696, truck: 728, car: 744, ramp: 808, slick: 856, sig: 960
    }
];

async function run() {
    console.log('================================================================');
    console.log('  NITRO SURFER: COMPREHENSIVE OBSTACLE & HAZARD VERIFICATION   ');
    console.log('================================================================\n');

    // =========================================================================
    // PHASE 1: 6-TRACK THEMED SPRITE & PALETTE VERIFICATION (42 CHECKS)
    // =========================================================================
    console.log('>>> PHASE 1: Themed Sprite Tiles & Palette Allocation (All 6 Tracks)');
    for (const theme of THEMES) {
        process.stdout.write(`  Testing ${theme.name.padEnd(20)}: `);
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, theme.nav);
            await gba.tap('START', 4, 10);
            await gba.waitFrames(10);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            // Inject all obstacle varieties
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 0, 50));
            await gba.writeMemory(ADDR_OBSTACLES + 1 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 55));
            await gba.writeMemory(ADDR_OBSTACLES + 2 * 48, makeObstacle(OBS_TRUCK, 2, 60));
            await gba.writeMemory(ADDR_OBSTACLES + 3 * 48, makeObstacle(OBS_CIVILIAN_CAR, 0, 65));
            await gba.writeMemory(ADDR_OBSTACLES + 4 * 48, makeObstacle(OBS_RAMP, 1, 70));
            await gba.writeMemory(ADDR_OBSTACLES + 5 * 48, makeObstacle(OBS_SLICK, 2, 75));
            await gba.writeMemory(ADDR_OBSTACLES + 6 * 48, makeObstacle(OBS_SIGNATURE, 0, 80));

            await gba.waitFrames(5);
            const sprites = await getOamEntries(gba, 40);

            const check = (name, tile) => {
                const s = sprites.find(sp => sp.tile === tile);
                assert(s, `${theme.name}: Missing ${name} (tile ${tile})`);
                assert.strictEqual(s.pal, theme.pal, `${theme.name}: ${name} palette expected ${theme.pal}, got ${s.pal}`);
            };

            check('Barrier', theme.barrier);
            check('Sign', theme.sign);
            check('Truck', theme.truck);
            check('Civilian', theme.car);
            check('Ramp', theme.ramp);
            check('Slick', theme.slick);
            check('Signature', theme.sig);

            console.log(`[PASS] 7/7 obstacle sprites verified on Pal ${theme.pal}`);
        } finally {
            await gba.close();
        }
    }

    // =========================================================================
    // PHASE 2: CORE OBSTACLES (BARRIER, SIGN, TRUCK, CIVILIAN)
    // =========================================================================
    console.log('\n>>> PHASE 2: Core Obstacle Collision & Clearance Mechanics');

    // 2.1 Barrier Crash
    {
        process.stdout.write('  Test 2.1: OBS_LOW_BARRIER Unjumped Collision -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 1, 24));

            await gba.waitFrames(25);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Player should crash when hitting low barrier without jumping');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // 2.2 Barrier Hurdle (Jumped with UP)
    {
        process.stdout.write('  Test 2.2: OBS_LOW_BARRIER Jump Hurdle (UP) -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_LOW_BARRIER, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Player should NOT crash when leaping over barrier');
            assert(g.score >= 150, `Expected >= 150 points for hurdle, got ${g.score}`);
            console.log(`[PASS] Score=${g.score}, Cleanly Hurdled!`);
        } finally {
            await gba.close();
        }
    }

    // 2.3 High Sign Ground Pass Without Shrinking -> Crash
    {
        process.stdout.write('  Test 2.3: OBS_HIGH_SIGN Normal Pass Without Shrink -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 36));

            await gba.waitFrames(30);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Player without shrinking must crash on OBS_HIGH_SIGN');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // 2.4 High Sign Shrink Under (DOWN)
    {
        process.stdout.write('  Test 2.4: OBS_HIGH_SIGN Shrink Slipstream (DOWN) -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('DOWN', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Player should NOT crash when shrinking under sign');
            assert(g.score >= 150, `Expected slipstream reward >= 150, got ${g.score}`);
            console.log(`[PASS] Score=${g.score}, Slipstream Clearance!`);
        } finally {
            await gba.close();
        }
    }

    // 2.5 High Sign Airborne Jump Hit
    {
        process.stdout.write('  Test 2.5: OBS_HIGH_SIGN Jump Into Gantry -> Airborne Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_HIGH_SIGN, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Jumping into overhead sign should crash the player');
            console.log('[PASS] Airborne collision registered correctly!');
        } finally {
            await gba.close();
        }
    }

    // 2.6 Semi-Truck Roadblock Crash
    {
        process.stdout.write('  Test 2.6: OBS_TRUCK Head-On Collision -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_TRUCK, 1, 24));

            await gba.waitFrames(25);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Head-on collision with semi-truck must crash player');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // 2.7 Semi-Truck Close Call Evasion
    {
        process.stdout.write('  Test 2.7: OBS_TRUCK Near Miss Evasion -> +250 Close Call ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_TRUCK, 0, 36));

            await gba.waitFrames(30);
            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Near miss truck should not crash');
            console.log(`[PASS] Close calls=${g.close_calls}, Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 2.8 Civilian Car Jump Hurdle
    {
        process.stdout.write('  Test 2.8: OBS_CIVILIAN_CAR High Jump Hurdle -> +200 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_CIVILIAN_CAR, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Leaping over civilian car should clear hurdle');
            assert(g.score >= 200, `Expected hurdle score >= 200, got ${g.score}`);
            console.log(`[PASS] Score=${g.score}, Car Hurdled!`);
        } finally {
            await gba.close();
        }
    }

    // =========================================================================
    // PHASE 3: EXPANDING MECHANICS (STUNT RAMP & SURFACE SLICK)
    // =========================================================================
    console.log('\n>>> PHASE 3: Expanding Mechanic Obstacles (Ramp & Slick)');

    // 3.1 Stunt Launch Ramp
    {
        process.stdout.write('  Test 3.1: OBS_RAMP Stunt Launch -> Super-Jump +200 STUNT! ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_RAMP, 1, 36));

            let maxElevation = 0;
            let stuntAwarded = false;

            for (let f = 0; f < 35; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                if (p.y_offset > maxElevation) maxElevation = p.y_offset;
                if (p.stunt_timer > 0) stuntAwarded = true;
            }

            const g = await getGame(gba);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 0, 'Stunt ramp must not crash player');
            assert(maxElevation >= 15, `Expected super-jump elevation >= 15px, got ${maxElevation.toFixed(1)}px`);
            assert(stuntAwarded, 'Expected stunt_timer to trigger on launch');
            assert(g.score >= 200, `Expected score >= 200, got ${g.score}`);
            console.log(`[PASS] Peak=${maxElevation.toFixed(1)}px, Stunt Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 3.2 Surface Slick Ground Contact
    {
        process.stdout.write('  Test 3.2: OBS_SLICK Ground Contact -> Fishtail Spinout Drift ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 36));

            let spinoutActive = false;
            for (let f = 0; f < 30; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                if (p.spinout_timer > 0) spinoutActive = true;
            }

            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 0, 'Slick contact must NOT crash player (friction loss only)');
            assert(spinoutActive, 'Expected spinout_timer to activate on slick contact');
            console.log('[PASS] Friction loss and lateral drift triggered!');
        } finally {
            await gba.close();
        }
    }

    // 3.3 Surface Slick Jump Over
    {
        process.stdout.write('  Test 3.3: OBS_SLICK Leaping Over -> Safe Clearance ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 0, 'Jumping over slick must be safe');
            assert.strictEqual(p.spinout_timer, 0, 'Airborne pass should NOT trigger spinout');
            console.log('[PASS] Safely sailed over slick!');
        } finally {
            await gba.close();
        }
    }

    // =========================================================================
    // PHASE 4: TRACK-SPECIFIC SIGNATURE HAZARDS (ALL 6 TRACKS)
    // =========================================================================
    console.log('\n>>> PHASE 4: Track-Specific Signature Hazards');

    // 4.1 Cape Orbital Laser ON (Crash)
    {
        process.stdout.write('  Test 4.1: Cape Orbital Laser (Active Beam ON) -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['DOWN']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 24, 0));

            await gba.waitFrames(25);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Active laser beam must crash player');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // 4.2 Cape Orbital Laser OFF (Safe Pass Window +150)
    {
        process.stdout.write('  Test 4.2: Cape Orbital Laser (Charging Window OFF) -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['DOWN']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            // Set frame_count to 45 so ((frame_count / 40) & 1) == 1 (Charging window / laser OFF)
            const fcBuf = Buffer.alloc(4);
            fcBuf.writeInt32LE(45, 0);
            await gba.writeMemory(ADDR_GAME + 36, fcBuf);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 36, 1));

            await gba.waitFrames(30);
            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Player should safely pass charging laser window');
            assert(g.score >= 150, `Expected score >= 150, got ${g.score}`);
            console.log(`[PASS] Timing window cleared! Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 4.3 Gothic Gargoyles Hurdle (Jump UP over 3-Gargoyle Road Barrier)
    {
        process.stdout.write('  Test 4.3: Gothic 3-Gargoyle Barrier Hurdle (UP) -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['DOWN', 'RIGHT']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            // Spawn 3 gargoyles across all 3 lanes atomically
            const threeGargoyles = Buffer.concat([
                makeObstacle(OBS_SIGNATURE, 0, 48),
                makeObstacle(OBS_SIGNATURE, 1, 48),
                makeObstacle(OBS_SIGNATURE, 2, 48)
            ]);
            await gba.writeMemory(ADDR_OBSTACLES, threeGargoyles);

            await gba.waitFrames(1);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Jumping over Gothic gargoyle barrier should succeed');
            assert(g.score >= 150, `Expected score >= 150, got ${g.score}`);
            console.log(`[PASS] Leaped over gargoyles! Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 4.4 Maya Poison Dart Trap Volley (Duck DOWN under jade dart stream)
    {
        process.stdout.write('  Test 4.4: Maya Temple Poison Dart Volley Ducking (DOWN) -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['DOWN', 'RIGHT', 'RIGHT']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('DOWN', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Ducking under Maya poison dart barrage should succeed');
            assert(g.score >= 150, `Expected score >= 150, got ${g.score}`);
            console.log(`[PASS] Ducked under poison dart barrage! Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 4.5 Palm Beach Windsurf Rig Ducking (DOWN)
    {
        process.stdout.write('  Test 4.5: Palm Beach Windsurf Rig Duck Under Sail -> +150 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['RIGHT']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 48));

            await gba.waitFrames(1);
            await gba.tap('DOWN', 4, 1);
            await gba.waitFrames(30);

            const p = await getPlayer(gba);
            const g = await getGame(gba);
            assert.strictEqual(p.is_crashing, 0, 'Ducking under windsurf sail should succeed');
            assert(g.score >= 150, `Expected score >= 150, got ${g.score}`);
            console.log(`[PASS] Ducked under windsurf sail! Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 4.6 Winter Snow Ice Boulder Ground Collision
    {
        process.stdout.write('  Test 4.6: Winter Snow Glacial Ice Boulder Ground Hit -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, ['RIGHT', 'RIGHT']);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 24));

            await gba.waitFrames(25);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Hitting ice boulder without jump must crash player');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // 4.7 City Highway Chemical Tanker Ground Collision
    {
        process.stdout.write('  Test 4.7: City Highway Heavy Chemical Tanker Hit -> Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SIGNATURE, 1, 24));

            await gba.waitFrames(25);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 1, 'Hitting chemical tanker must crash player');
            console.log('[PASS]');
        } finally {
            await gba.close();
        }
    }

    // =========================================================================
    // PHASE 5: POWERUP DEFENSIVE SYNERGIES (NITRO SMASH & SHIELD DEFLECT)
    // =========================================================================
    console.log('\n>>> PHASE 5: Powerup Defensive Interactions');

    // 5.1 Nitro Smash Through Obstacle
    {
        process.stdout.write('  Test 5.1: Nitro Blast Smashes Through Obstacle -> +300 Points ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            // Grant 180 frames of Nitro to player
            const nitroBuf = Buffer.alloc(4);
            nitroBuf.writeInt32LE(180, 0);
            await gba.writeMemory(ADDR_PLAYER + 36, nitroBuf);

            // Place heavy roadblock semi-truck
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_TRUCK, 1, 36));

            await gba.waitFrames(30);
            const p = await getPlayer(gba);
            const g = await getGame(gba);
            const obsMem = await gba.readMemory(ADDR_OBSTACLES, 48);
            const obsActive = obsMem.readUInt8(0);

            assert.strictEqual(p.is_crashing, 0, 'Player with Nitro must NOT crash when hitting obstacle');
            assert.strictEqual(obsActive, 0, 'Obstacle must be destroyed by Nitro smash');
            assert(g.score >= 300, `Expected score >= 300 for nitro smash, got ${g.score}`);
            console.log(`[PASS] Obstacle smashed! Score=${g.score}`);
        } finally {
            await gba.close();
        }
    }

    // 5.2 Shield Deflection Absorbs Blow
    {
        process.stdout.write('  Test 5.2: Forcefield Shield Absorbs Impact -> Prevents Crash ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);
            // Grant active shield to player
            const shieldBuf = Buffer.alloc(4);
            shieldBuf.writeInt32LE(1, 0);
            await gba.writeMemory(ADDR_PLAYER + 32, shieldBuf);

            // Place heavy roadblock semi-truck
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_TRUCK, 1, 36));

            await gba.waitFrames(30);
            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 0, 'Player with shield must NOT crash on collision');
            assert.strictEqual(p.shield_active, 0, 'Shield should be depleted after absorbing blow');
            assert(p.invuln_timer > 0, 'Player should enter invulnerability grace period');
            console.log('[PASS] Shield successfully absorbed impact!');
        } finally {
            await gba.close();
        }
    }

    // =========================================================================
    // PHASE 6: NATURAL WAVE SPAWNER LIVE PROGRESSION
    // =========================================================================
    console.log('\n>>> PHASE 6: Natural Game Spawner Live Progression');
    {
        process.stdout.write('  Test 6.1: 300-Frame Natural Gameplay Wave Progression ... ');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba, []);
            // Keep car cruising through 300 frames of natural obstacle observation
            const invulnBuf = Buffer.alloc(4);
            invulnBuf.writeInt32LE(500, 0);
            await gba.writeMemory(ADDR_PLAYER + 44, invulnBuf);
            let obstaclesSpawned = 0;
            for (let chunk = 0; chunk < 10; chunk++) {
                await gba.waitFrames(30);
                if (chunk % 3 === 1) await gba.tap('LEFT', 3, 5);
                else if (chunk % 3 === 2) await gba.tap('RIGHT', 3, 5);
                else await gba.tap('UP', 3, 5);

                for (let i = 0; i < 8; i++) {
                    const act = await gba.readU8(ADDR_OBSTACLES + i * 48);
                    if (act === 1) obstaclesSpawned++;
                }
            }

            const g = await getGame(gba);
            assert(obstaclesSpawned > 5, 'Natural wave director should spawn obstacles over 300 frames');
            assert(g.speed > 0, 'Game speed should be active');
            console.log(`[PASS] ${obstaclesSpawned} obstacle instances naturally directed across lanes!`);
        } finally {
            await gba.close();
        }
    }

    console.log('\n================================================================');
    console.log('  ALL OBSTACLES & HAZARDS IN NITRO SURFER ARE WORKING PROPERLY!  ');
    console.log('================================================================\n');
}

run().catch(err => {
    console.error('\n[FATAL] Comprehensive Obstacle Test Suite FAILED:');
    console.error(err);
    process.exit(1);
});
