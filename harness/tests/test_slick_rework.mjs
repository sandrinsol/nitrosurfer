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
const ADDR_PLAYER = syms['g_player'];
const ADDR_GAME = syms['g_game'];
const ADDR_DEBRIS = syms['g_debris'];
const ADDR_COINS = syms['g_coins'];

const OBS_SLICK = 6;
const LANE_X = [72, 120, 168];

function makeObstacle(type, lane, z) {
    const buf = Buffer.alloc(48);
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
    buf.writeUInt8(0, 44);                      // passed = 0
    buf.writeUInt8(0, 45);                      // close_call_awarded = 0
    buf.writeUInt8(0, 46);                      // hurdled = 0
    buf.writeUInt8(0, 47);                      // hazard_state = 0
    return buf;
}

async function startTrack(gba) {
    await gba.waitFrames(35);
    await gba.tap('A', 4, 10);
    await gba.waitFrames(15);
    await gba.tap('A', 4, 10);
    await gba.waitFrames(15);
    await gba.tap('A', 4, 10);
    await gba.waitFrames(30);
}

async function freezeWaveSpawner(gba) {
    const buf = Buffer.alloc(4);
    buf.writeInt32LE(99999, 0);
    await gba.writeMemory(ADDR_GAME + 40, buf);
}

async function clearActiveObstacles(gba) {
    await gba.writeMemory(ADDR_OBSTACLES, Buffer.alloc(8 * 48));
    if (ADDR_DEBRIS) await gba.writeMemory(ADDR_DEBRIS, Buffer.alloc(16 * 28));
    if (ADDR_COINS) await gba.writeMemory(ADDR_COINS, Buffer.alloc(16 * 36));
}

async function getPlayer(gba) {
    const mem = await gba.readMemory(ADDR_PLAYER, 88);
    return {
        lane: mem.readInt32LE(0),
        x: mem.readInt32LE(4) / 256,
        target_x: mem.readInt32LE(8) / 256,
        y_offset: mem.readInt32LE(12) / 256,
        vy: mem.readInt32LE(16) / 256,
        is_jumping: mem.readUInt8(20),
        tilt: mem.readInt32LE(28),
        invuln_timer: mem.readInt32LE(44),
        spinout_timer: mem.readInt32LE(80),
        is_crashing: mem.readUInt8(48)
    };
}

async function getGame(gba) {
    const mem = await gba.readMemory(ADDR_GAME, 120);
    return {
        state: mem.readInt32LE(0),
        speed: mem.readInt32LE(28) / 256,
        base_speed: mem.readInt32LE(32) / 256,
        streak_mult: mem.readInt32LE(76)
    };
}

async function runTest() {
    console.log('================================================================');
    console.log('  OBS_SLICK RANDOM LATERAL LANE SLIDE & FULL SPEED VERIFICATION ');
    console.log('================================================================');

    // Test 1: Center lane hit -> slides randomly to Lane 0 or Lane 2 with NO speed reduction
    {
        console.log('\n[Test 1] Center Lane Hit: Random Lateral Slide & Full Speed Retention');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            const initialPlayer = await getPlayer(gba);
            assert.strictEqual(initialPlayer.lane, 1, 'Player starts in center lane (Lane 1)');
            assert.strictEqual(initialPlayer.spinout_timer, 0, 'Spinout timer starts at 0');

            // Spawn slick in center lane at z=36
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 36));

            let slideTriggered = false;
            let newLane = -1;

            for (let f = 0; f < 45; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                const g = await getGame(gba);
                if (p.spinout_timer > 0 && !slideTriggered) {
                    slideTriggered = true;
                    newLane = p.lane;
                    console.log(`  -> Slide triggered at frame ${f}! New lane: ${newLane}, Timer: ${p.spinout_timer}`);
                    console.log(`  -> Vehicle Speed: ${g.speed.toFixed(3)} (Base: ${g.base_speed.toFixed(3)})`);

                    // Check speed is NOT reduced
                    assert(g.speed >= g.base_speed * 0.99, `Speed MUST NOT be reduced! speed=${g.speed}, base=${g.base_speed}`);
                    // Check new lane is adjacent (Lane 0 or Lane 2)
                    assert(newLane === 0 || newLane === 2, `Expected random slide to Lane 0 or Lane 2, got ${newLane}`);
                    assert.strictEqual(g.streak_mult, 1, `Streak multiplier reset to 1`);
                    // Check tilt direction matches slide direction
                    const expectedTilt = (newLane === 0) ? -1 : 1;
                    assert.strictEqual(p.tilt, expectedTilt, `Tilt (${p.tilt}) should match slide direction (${expectedTilt})`);
                }
            }

            assert(slideTriggered, 'Expected OBS_SLICK contact to trigger slide');

            // Capture screenshot during mid-slide
            const shotPath = path.resolve(__dirname, '../slick_random_slide.png');
            await gba.screenshot(shotPath);
            console.log(`  [OK] Captured mid-slide screenshot -> ${shotPath}`);

            // Wait until slide completes (24 frames total)
            await gba.waitFrames(25);
            const pFinal = await getPlayer(gba);
            const gFinal = await getGame(gba);

            console.log(`  -> Post-slide settled position: x=${pFinal.x.toFixed(1)}, lane=${pFinal.lane}, timer=${pFinal.spinout_timer}, tilt=${pFinal.tilt}`);
            assert.strictEqual(pFinal.spinout_timer, 0, 'Slide timer should have completed');
            assert.strictEqual(pFinal.lane, newLane, 'Lane matches target slide lane');
            assert.strictEqual(pFinal.tilt, 0, 'Tilt resets to 0 upon settling');
            assert(Math.abs(pFinal.x - LANE_X[newLane]) < 1.0, `Player x (${pFinal.x}) matches target lane x (${LANE_X[newLane]})`);
            assert(gFinal.speed >= gFinal.base_speed * 0.99, 'Maintains full base speed throughout');
            console.log('  [PASS] Center lane slick correctly slides to adjacent lane at full speed!');
        } finally {
            await gba.close();
        }
    }

    // Test 2: Left Lane (0) Hit -> Must Slide Inward to Center Lane (1)
    {
        console.log('\n[Test 2] Left Lane (0) Hit: Mandatory Inward Slide to Center Lane (1)');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            // Shift player to Left Lane (0) naturally
            await gba.tap('LEFT', 4, 20);
            const pLeft = await getPlayer(gba);
            assert.strictEqual(pLeft.lane, 0, 'Player positioned in left lane (0)');

            // Spawn slick in lane 0 at z=36
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 0, 36));

            let slideTriggered = false;
            for (let f = 0; f < 45; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                const g = await getGame(gba);
                if (p.spinout_timer > 0) {
                    slideTriggered = true;
                    assert.strictEqual(p.lane, 1, `Left lane hit MUST slide inward to lane 1, got ${p.lane}`);
                    assert.strictEqual(p.tilt, 1, `Slide tilt must be +1 (rightwards towards center)`);
                    assert(g.speed >= g.base_speed * 0.99, 'Speed maintained during left lane slick slide');
                    break;
                }
            }

            assert(slideTriggered, 'Slick contact occurred');
            await gba.waitFrames(25);
            const pFinal = await getPlayer(gba);
            assert.strictEqual(pFinal.lane, 1, 'Player safely arrived in center lane (1)');
            assert.strictEqual(pFinal.tilt, 0, 'Tilt reset to 0 upon reaching center lane');
            assert(Math.abs(pFinal.x - LANE_X[1]) < 1.0, 'Player position aligned with center lane');
            console.log('  [PASS] Left lane slick cleanly slid inward to center lane!');
        } finally {
            await gba.close();
        }
    }

    // Test 3: Right Lane (2) Hit -> Must Slide Inward to Center Lane (1)
    {
        console.log('\n[Test 3] Right Lane (2) Hit: Mandatory Inward Slide to Center Lane (1)');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            // Shift player to Right Lane (2) naturally
            await gba.tap('RIGHT', 4, 20);
            const pRight = await getPlayer(gba);
            assert.strictEqual(pRight.lane, 2, 'Player positioned in right lane (2)');

            // Spawn slick in lane 2 at z=36
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 2, 36));

            let slideTriggered = false;
            for (let f = 0; f < 45; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                const g = await getGame(gba);
                if (p.spinout_timer > 0) {
                    slideTriggered = true;
                    assert.strictEqual(p.lane, 1, `Right lane hit MUST slide inward to lane 1, got ${p.lane}`);
                    assert.strictEqual(p.tilt, -1, `Slide tilt must be -1 (leftwards towards center)`);
                    assert(g.speed >= g.base_speed * 0.99, 'Speed maintained during right lane slick slide');
                    break;
                }
            }

            assert(slideTriggered, 'Slick contact occurred');
            await gba.waitFrames(25);
            const pFinal = await getPlayer(gba);
            assert.strictEqual(pFinal.lane, 1, 'Player safely arrived in center lane (1)');
            assert.strictEqual(pFinal.tilt, 0, 'Tilt reset to 0 upon reaching center lane');
            assert(Math.abs(pFinal.x - LANE_X[1]) < 1.0, 'Player position aligned with center lane');
            console.log('  [PASS] Right lane slick cleanly slid inward to center lane!');
        } finally {
            await gba.close();
        }
    }

    // Test 4: Steering Locked During Lateral Slide
    {
        console.log('\n[Test 4] Controls Locked During Uncontrolled Lateral Slide');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 36));

            let lockedTested = false;
            for (let f = 0; f < 45; f++) {
                await gba.waitFrames(1);
                const p = await getPlayer(gba);
                if (p.spinout_timer > 10) {
                    // Try to steer opposite while slide is active
                    const assignedLane = p.lane;
                    await gba.tap('LEFT', 2, 1);
                    await gba.waitFrames(2);
                    const pCheck = await getPlayer(gba);
                    assert.strictEqual(pCheck.lane, assignedLane, 'Steering input must be locked during slide');
                    lockedTested = true;
                    break;
                }
            }

            assert(lockedTested, 'Steering lock tested during active slide');
            console.log('  [PASS] Player cannot counter-steer during uncontrolled slick slide!');
        } finally {
            await gba.close();
        }
    }

    // Test 5: Clean Jump Over Slick
    {
        console.log('\n[Test 5] Airborne Jump Clears Slick Safely');
        const gba = await EmuHarness.launch(ROM_PATH);
        try {
            await startTrack(gba);
            await freezeWaveSpawner(gba);
            await clearActiveObstacles(gba);

            // Spawn slick ahead at z=48
            await gba.writeMemory(ADDR_OBSTACLES + 0 * 48, makeObstacle(OBS_SLICK, 1, 48));
            await gba.waitFrames(2);
            await gba.tap('UP', 4, 1);
            await gba.waitFrames(35);

            const p = await getPlayer(gba);
            assert.strictEqual(p.is_crashing, 0, 'Jumping over slick must not crash');
            assert.strictEqual(p.spinout_timer, 0, 'Airborne pass must NOT trigger slide');
            assert.strictEqual(p.lane, 1, 'Player lane remains unchanged on clean jump');
            console.log('  [PASS] Airborne jump cleanly avoided slick with zero slide!');
        } finally {
            await gba.close();
        }
    }

    console.log('\n================================================================');
    console.log('  ALL OBS_SLICK RANDOM LATERAL SLIDE TESTS PASSED SUCCESSFULLY!  ');
    console.log('================================================================\n');
}

runTest().catch((err) => {
    console.error('Test failed:', err);
    process.exit(1);
});
