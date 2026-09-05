import { EmuHarness } from './driver.mjs';
import { execSync } from 'child_process';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ELF_PATH = path.resolve(__dirname, '../nitrosurfer/nitrosurfer.elf');
const ROM_PATH = path.resolve(__dirname, '../nitrosurfer.gba');
const ARTIFACT_DIR = '/Users/sandrinobreshani/.gemini/antigravity-ide/brain/505744c3-5cdd-4fcc-9bee-1e246be9ac03';

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

async function freezeWaveSpawner(gba, syms) {
    const timerBuf = Buffer.alloc(4);
    timerBuf.writeInt32LE(99999, 0);
    await gba.writeMemory(syms['g_game'] + 40, timerBuf);
}

async function clearActiveObstacles(gba, syms) {
    await gba.writeMemory(syms['g_obstacles'], Buffer.alloc(8 * 48));
    await gba.writeMemory(syms['g_debris'], Buffer.alloc(16 * 28));
    await gba.writeMemory(syms['g_coins'], Buffer.alloc(16 * 36));
}

async function captureGothicJump(syms) {
    console.log('Capturing Gothic Midnight 3-Gargoyle Jump...');
    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await startTrack(gba, ['DOWN', 'RIGHT']);
        await freezeWaveSpawner(gba, syms);
        await clearActiveObstacles(gba, syms);

        // Place 3 gargoyles across lanes 0, 1, 2 at Z=48
        const threeGargoyles = Buffer.concat([
            makeObstacle(OBS_SIGNATURE, 0, 48),
            makeObstacle(OBS_SIGNATURE, 1, 48),
            makeObstacle(OBS_SIGNATURE, 2, 48)
        ]);
        await gba.writeMemory(syms['g_obstacles'], threeGargoyles);

        // Trigger Jump UP
        await gba.waitFrames(1);
        await gba.tap('UP', 4, 1);
        // Wait until mid-jump elevation right over the gargoyles
        await gba.waitFrames(16);

        const pMem = await gba.readMemory(syms['g_player'], 88);
        console.log(`Gothic player status: crashing=${pMem.readUInt8(36)}, jumping=${pMem.readUInt8(20)}, y_offset=${pMem.readInt32LE(12)/256}`);

        const screenshotPath = path.join(ARTIFACT_DIR, 'gothic_gargoyles_jump.png');
        await gba.screenshot(screenshotPath);
        console.log(`Saved screenshot to ${screenshotPath}`);
    } finally {
        await gba.close();
    }
}

async function captureMayaDuck(syms) {
    console.log('Capturing Maya Temple Trail Dart Trap Duck...');
    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await startTrack(gba, ['DOWN', 'RIGHT', 'RIGHT']);
        await freezeWaveSpawner(gba, syms);
        await clearActiveObstacles(gba, syms);

        // Place dart trap in lane 1 at Z=48
        await gba.writeMemory(syms['g_obstacles'], makeObstacle(OBS_SIGNATURE, 1, 48));

        // Trigger Duck DOWN
        await gba.waitFrames(1);
        await gba.setButton('DOWN', true);
        await gba.waitFrames(16);

        const pMem = await gba.readMemory(syms['g_player'], 88);
        console.log(`Maya player status: crashing=${pMem.readUInt8(36)}, shrinking=${pMem.readUInt8(24)}`);

        const screenshotPath = path.join(ARTIFACT_DIR, 'maya_dart_volley_duck.png');
        await gba.screenshot(screenshotPath);
        await gba.setButton('DOWN', false);
        console.log(`Saved screenshot to ${screenshotPath}`);
    } finally {
        await gba.close();
    }
}

async function main() {
    const syms = getSymbols();
    await captureGothicJump(syms);
    await captureMayaDuck(syms);
    console.log('Screenshots captured successfully!');
}

main().catch(err => {
    console.error('Failed to capture screenshots:', err);
    process.exit(1);
});
