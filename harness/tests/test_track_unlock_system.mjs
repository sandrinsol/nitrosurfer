import { EmuHarness } from '../driver.mjs';
import { execSync } from 'child_process';
import assert from 'assert';
import path from 'path';
import { fileURLToPath } from 'url';
import fs from 'fs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ELF_PATH = path.resolve(__dirname, '../../nitrosurfer/nitrosurfer.elf');
const ROM_PATH = path.resolve(__dirname, '../../nitrosurfer.gba');
const SAV_PATH = path.resolve(__dirname, '../../nitrosurfer.sav');

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
const STATE_SELECT_MAP = 5;

async function runTest() {
    console.log('=== Test: Track Unlock System & Progression ===');
    const syms = getSymbols();
    assert(syms['g_game'], 'Symbol g_game not found in ELF!');
    const ADDR_GAME = syms['g_game'];

    // Start with fresh save: 0 coins, only red car unlocked, default maps
    const savBuffer = Buffer.alloc(32768, 0xFF);
    savBuffer.write('NITR', 0, 4, 'ascii');
    savBuffer.writeUInt32LE(15000, 4); // high score
    savBuffer.writeUInt32LE(0, 8);     // 0 total coins
    savBuffer[12] = 1;                 // red car unlocked
    savBuffer[13] = 1;                 // only city unlocked
    fs.writeFileSync(SAV_PATH, savBuffer);

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await gba.waitFrames(35);

        // Advance to Car Select (A)
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);

        // Advance to Track Select (A)
        await gba.tap('A', 4, 15);
        await gba.waitFrames(25);

        let state = await gba.readU32(ADDR_GAME);
        console.log(`Current state: ${state} (expected STATE_SELECT_MAP=${STATE_SELECT_MAP})`);
        assert.strictEqual(state, STATE_SELECT_MAP);

        // Take screenshot of Track Select with locked tracks
        const lockedTracksScreenshot = path.resolve(__dirname, '../track_select_locked.png');
        await gba.screenshot(lockedTracksScreenshot);
        console.log(`Captured Track Select locked screenshot -> ${lockedTracksScreenshot}`);

        // Try navigating to Palm Beach (RIGHT)
        await gba.tap('RIGHT', 4, 15);
        await gba.waitFrames(15);

        // Try pressing A without enough coins
        await gba.tap('A', 4, 15);
        await gba.waitFrames(15);
        state = await gba.readU32(ADDR_GAME);
        console.log(`State after attempting to buy Palm Beach with 0 coins: ${state} (should still be ${STATE_SELECT_MAP})`);
        assert.strictEqual(state, STATE_SELECT_MAP, 'Should not start race when track is locked and unaffordable!');

        // Return to Car Select (B), then Start Screen (B)
        await gba.tap('B', 4, 15);
        await gba.waitFrames(20);
        await gba.tap('B', 4, 15);
        await gba.waitFrames(25);

        state = await gba.readU32(ADDR_GAME);
        console.log(`State back on Start Screen: ${state} (expected STATE_START=${STATE_START})`);
        assert.strictEqual(state, STATE_START);

        // Tap B 20 times to activate cheat code and get 99999 coins
        console.log('Tapping B 20 times on start screen to get 99999 coins...');
        for (let i = 0; i < 20; i++) {
            await gba.tap('B', 3, 5);
        }
        await gba.waitFrames(15);
        let coins = await gba.readU32(ADDR_GAME + 16);
        console.log(`Coins after cheat: ${coins} (expected 99999)`);
        assert.strictEqual(coins, 99999);

        // Go to Car Select (A), then Track Select (A)
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        await gba.tap('A', 4, 15);
        await gba.waitFrames(25);

        // We are already on Palm Beach (1) since map_theme was preserved.
        // Press A to buy Palm Beach (1000 C)!
        console.log('Buying Palm Beach (1000 coins)...');
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);

        coins = await gba.readU32(ADDR_GAME + 16);
        console.log(`Coins after buying Palm Beach: ${coins} (expected 98999)`);
        assert.strictEqual(coins, 98999);

        // Navigate RIGHT to Winter Snow (2, 2000 C)
        await gba.tap('RIGHT', 4, 15);
        await gba.waitFrames(15);
        console.log('Buying Winter Snow (2000 coins)...');
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        coins = await gba.readU32(ADDR_GAME + 16);
        console.log(`Coins after buying Winter Snow: ${coins} (expected 96999)`);
        assert.strictEqual(coins, 96999);

        // Navigate DOWN to Maya Temple (5, 5000 C)
        await gba.tap('DOWN', 4, 15);
        await gba.waitFrames(15);
        console.log('Buying Maya Temple (5000 coins)...');
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        coins = await gba.readU32(ADDR_GAME + 16);
        console.log(`Coins after buying Maya Temple: ${coins} (expected 91999)`);
        assert.strictEqual(coins, 91999);

        // Take screenshot showing unlocked tracks
        const progressTracksScreenshot = path.resolve(__dirname, '../track_select_progression.png');
        await gba.screenshot(progressTracksScreenshot);
        console.log(`Captured Track Select progression screenshot -> ${progressTracksScreenshot}`);

        // Now that Maya is unlocked, pressing A should START THE RACE on Maya!
        console.log('Starting race on Maya Temple...');
        await gba.tap('A', 4, 25);
        await gba.waitFrames(30);

        state = await gba.readU32(ADDR_GAME);
        console.log(`State after starting Maya: ${state} (expected STATE_PLAY=${STATE_PLAY})`);
        assert.strictEqual(state, STATE_PLAY, 'Race should have started!');

        // Take screenshot of Maya gameplay
        const mayaGameplayScreenshot = path.resolve(__dirname, '../gameplay_unlocked_maya.png');
        await gba.screenshot(mayaGameplayScreenshot);
        console.log(`Captured Maya gameplay screenshot -> ${mayaGameplayScreenshot}`);

        // Verify SRAM persistence via getSaveFile
        const savBytes = await gba.page.evaluate(async () => {
            if (window.EJS_emulator && window.EJS_emulator.gameManager) {
                const s = await window.EJS_emulator.gameManager.getSaveFile();
                return Array.from(s.slice(0, 16));
            }
            return null;
        });
        assert(savBytes, 'Failed to retrieve save file from emulator');
        const sramMagic = String.fromCharCode(...savBytes.slice(0, 4));
        const sramCoins = savBytes[8] | (savBytes[9] << 8) | (savBytes[10] << 16) | (savBytes[11] << 24);
        const sramMaps = savBytes[13];
        console.log(`SRAM verified: magic=${sramMagic}, totalCoins=${sramCoins}, mapsUnlocked=0x${sramMaps.toString(16)}`);
        assert.strictEqual(sramMagic, 'NITR');
        assert.strictEqual(sramCoins, 91999);
        // Bit 0 (City), Bit 1 (Beach), Bit 2 (Winter), Bit 5 (Maya) should be unlocked -> 1 + 2 + 4 + 32 = 39 (0x27)
        assert.strictEqual(sramMaps & (1 << 0), 1 << 0, 'City must be unlocked');
        assert.strictEqual(sramMaps & (1 << 1), 1 << 1, 'Palm Beach must be unlocked');
        assert.strictEqual(sramMaps & (1 << 2), 1 << 2, 'Winter Snow must be unlocked');
        assert.strictEqual(sramMaps & (1 << 5), 1 << 5, 'Maya must be unlocked');

        console.log('=== TRACK UNLOCK SYSTEM FULLY VERIFIED! ===');
    } finally {
        await gba.close();
    }
}

runTest().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
