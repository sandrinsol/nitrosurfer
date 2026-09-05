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

const STATE_START = 0;
const STATE_TITLE = 1;
const STATE_PLAY = 2;
const STATE_PAUSE = 3;
const STATE_GAMEOVER = 4;
const STATE_SELECT_MAP = 5;

async function runTest() {
    console.log('=== Test: Press B in Pause Menu to Return to Main Menu ===');
    const syms = getSymbols();
    assert(syms['g_game'], 'Symbol g_game not found in ELF!');
    const ADDR_GAME = syms['g_game'];

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await gba.waitFrames(35);

        // 1. Check initial state is STATE_START
        let state = await gba.readU32(ADDR_GAME);
        console.log(`[1] Boot State: ${state} (expected STATE_START=${STATE_START})`);
        assert.strictEqual(state, STATE_START, `Expected STATE_START, got ${state}`);

        // 2. Press A to go to Car Select (STATE_TITLE)
        await gba.tap('A', 4, 10);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        console.log(`[2] After A: State ${state} (expected STATE_TITLE=${STATE_TITLE})`);
        assert.strictEqual(state, STATE_TITLE, `Expected STATE_TITLE, got ${state}`);

        // 3. Press A to go to Map Select (STATE_SELECT_MAP)
        await gba.tap('A', 4, 10);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        console.log(`[3] After A: State ${state} (expected STATE_SELECT_MAP=${STATE_SELECT_MAP})`);
        assert.strictEqual(state, STATE_SELECT_MAP, `Expected STATE_SELECT_MAP, got ${state}`);

        // 4. Press A to start race (STATE_PLAY)
        await gba.tap('A', 4, 20);
        await gba.waitFrames(30);
        state = await gba.readU32(ADDR_GAME);
        console.log(`[4] After A: State ${state} (expected STATE_PLAY=${STATE_PLAY})`);
        assert.strictEqual(state, STATE_PLAY, `Expected STATE_PLAY, got ${state}`);

        // 5. Press START to pause game
        console.log('[5] Pressing START to pause...');
        await gba.tap('START', 4, 15);
        await gba.waitFrames(10);
        state = await gba.readU32(ADDR_GAME);
        console.log(`State when paused: ${state} (expected STATE_PAUSE=${STATE_PAUSE})`);
        assert.strictEqual(state, STATE_PAUSE, `Expected STATE_PAUSE, got ${state}`);

        // Capture screenshot of pause screen showing START: RESUME and B: MAIN MENU
        const pauseScreenshot = path.resolve(__dirname, '../pause_menu_screen.png');
        await gba.screenshot(pauseScreenshot);
        console.log(`Captured pause screen -> ${pauseScreenshot}`);

        // 6. Test that pressing START unpauses
        console.log('[6] Testing unpause with START...');
        await gba.tap('START', 4, 15);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        console.log(`State after unpausing: ${state} (expected STATE_PLAY=${STATE_PLAY})`);
        assert.strictEqual(state, STATE_PLAY, `Expected STATE_PLAY after unpausing, got ${state}`);

        // 7. Pause again
        console.log('[7] Pausing again with START...');
        await gba.tap('START', 4, 15);
        await gba.waitFrames(10);
        state = await gba.readU32(ADDR_GAME);
        console.log(`State when paused again: ${state} (expected STATE_PAUSE=${STATE_PAUSE})`);
        assert.strictEqual(state, STATE_PAUSE, `Expected STATE_PAUSE, got ${state}`);

        // 8. Press B to return to Main Menu (STATE_START)
        console.log('[8] Pressing B to return to Main Menu...');
        await gba.tap('B', 4, 20);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        console.log(`State after pressing B: ${state} (expected STATE_START=${STATE_START})`);
        assert.strictEqual(state, STATE_START, `Expected STATE_START after pressing B while paused, got ${state}`);

        // Capture screenshot of main menu after returning from pause
        const mainMenuScreenshot = path.resolve(__dirname, '../main_menu_after_pause_b.png');
        await gba.screenshot(mainMenuScreenshot);
        console.log(`Captured Main Menu screenshot -> ${mainMenuScreenshot}`);

        // 9. Verify game can be started again from main menu
        console.log('[9] Starting a new game from main menu after pause exit...');
        await gba.tap('A', 4, 10);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, STATE_TITLE, 'Should reach STATE_TITLE');

        await gba.tap('A', 4, 10);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, STATE_SELECT_MAP, 'Should reach STATE_SELECT_MAP');

        await gba.tap('A', 4, 20);
        await gba.waitFrames(30);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, STATE_PLAY, 'Should reach STATE_PLAY again');

        console.log('=== TEST PASSED: B button cleanly returns from pause to main menu! ===');
    } finally {
        await gba.close();
    }
}

runTest().catch((err) => {
    console.error('Test failed:', err);
    process.exit(1);
});
