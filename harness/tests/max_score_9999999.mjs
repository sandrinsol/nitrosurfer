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
        if (parts.length === 3) syms[parts[2]] = parseInt(parts[0], 16);
    }
    return syms;
}

const syms = getSymbols();
const ADDR_GAME = syms['g_game'];
const ADDR_PLAYER = syms['g_player'];

async function testMaxScore() {
    console.log('=== Nitro Surfer Max Score (9999999) Verification ===');

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        console.log('1. Booting and checking start screen high score...');
        await gba.waitFrames(40);

        // Check initial high score on boot (default 15000 or loaded from SRAM)
        const highScoreBoot = await gba.readU32(ADDR_GAME + 8);
        console.log(`High score on boot: ${highScoreBoot}`);
        assert(highScoreBoot > 0, `High score should be > 0, got ${highScoreBoot}`);

        // Start race: Start -> Title -> Map -> Race
        await gba.tap('A', 4, 15);
        await gba.tap('A', 4, 15);
        await gba.tap('A', 4, 15);
        await gba.waitFrames(30);

        const stateInRace = await gba.readU32(ADDR_GAME);
        console.log(`State in race: ${stateInRace} (expected 2=STATE_PLAY)`);
        assert.strictEqual(stateInRace, 2, 'Should be in STATE_PLAY');

        // 2. Test score clamping at 9999999 in game state
        console.log('2. Injecting score near max (9999950)...');
        const scoreBuf = Buffer.alloc(4);
        scoreBuf.writeInt32LE(9999950, 0);
        await gba.writeMemory(ADDR_GAME + 4, scoreBuf);

        let score = await gba.readU32(ADDR_GAME + 4);
        assert(score >= 9999950 && score <= 9999999, `Injected score should be near max, got ${score}`);

        console.log('3. Running frames to earn points and verify clamp at 9999999...');
        await gba.waitFrames(60);

        score = await gba.readU32(ADDR_GAME + 4);
        console.log(`Current score after running: ${score}`);
        assert.strictEqual(score, 9999999, `Score must clamp exactly at 9999999, got ${score}`);
        console.log('✓ Score clamped cleanly at 9999999!');

        // Take high-res screenshot of In-Game HUD showing 7-digit score: 9999999
        const hudPng = path.resolve(__dirname, '../hud_9999999.png');
        await gba.screenshot(hudPng);
        console.log(`Captured In-Game HUD screenshot -> ${hudPng}`);

        // 4. Test crash into Game Over with 9999999 score -> high score save
        console.log('4. Triggering game over to verify high score saving...');
        await gba.writeU8(ADDR_PLAYER + 48, 1); // is_crashing = 1
        const crashFrameBuf = Buffer.alloc(4);
        crashFrameBuf.writeInt32LE(20, 0);
        await gba.writeMemory(ADDR_PLAYER + 52, crashFrameBuf); // crash_frame = 20
        await gba.waitFrames(30);

        const state = await gba.readU32(ADDR_GAME);
        const finalHighScore = await gba.readU32(ADDR_GAME + 8);
        console.log(`State: ${state} (expected 4=STATE_GAMEOVER), High Score: ${finalHighScore}`);
        assert.strictEqual(state, 4, 'Should be in STATE_GAMEOVER');
        assert.strictEqual(finalHighScore, 9999999, 'High score should be saved as 9999999');
        console.log('✓ High score 9999999 saved to game state & SRAM!');

        // Take high-res screenshot of Game Over screen showing 7-digit FINAL SCORE: 9999999
        const gameoverPng = path.resolve(__dirname, '../gameover_9999999.png');
        await gba.screenshot(gameoverPng);
        console.log(`Captured Game Over screenshot -> ${gameoverPng}`);

        // 5. Test emulator reset / reboot to verify SRAM persistence
        console.log('5. Restarting emulator core to verify SRAM reload of 9999999 high score...');
        await gba.page.evaluate(() => window.Emu.restart());
        await gba.waitFrames(40);

        const reloadedHighScore = await gba.readU32(ADDR_GAME + 8);
        console.log(`Loaded High Score on restart: ${reloadedHighScore}`);
        assert.strictEqual(reloadedHighScore, 9999999, `Reboot must load 9999999 from SRAM, got ${reloadedHighScore}`);
        console.log('✓ SRAM high score 9999999 persisted and loaded successfully!');

        // Take high-res screenshot of Start screen showing 7-digit HIGH SCORE: 9999999
        const startPng = path.resolve(__dirname, '../start_screen_9999999.png');
        await gba.screenshot(startPng);
        console.log(`Captured Start Screen screenshot -> ${startPng}`);

        // Advance to Title Screen and verify Title Screen 7-digit badge
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        const titleState = await gba.readU32(ADDR_GAME);
        assert.strictEqual(titleState, 1, 'Should be in STATE_TITLE');
        const titlePng = path.resolve(__dirname, '../title_screen_9999999.png');
        await gba.screenshot(titlePng);
        console.log(`Captured Title Screen screenshot -> ${titlePng}`);

    } finally {
        await gba.close();
    }

    console.log('\n=== ALL MAX SCORE 9999999 TESTS PASSED PERFECTLY! ===');
}

testMaxScore().catch(err => {
    console.error('[FATAL] Max score test failed:', err);
    process.exit(1);
});
