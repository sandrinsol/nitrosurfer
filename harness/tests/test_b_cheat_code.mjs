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

async function runTest() {
    console.log('=== Test: B 20 Times Cheat Code & 99999 Max Coins ===');
    const syms = getSymbols();
    assert(syms['g_game'], 'Symbol g_game not found in ELF!');
    const ADDR_GAME = syms['g_game'];

    // Reset save file to start with 0 coins
    const savBuffer = Buffer.alloc(32768, 0xFF);
    savBuffer.write('NITR', 0, 4, 'ascii');
    savBuffer.writeUInt32LE(15000, 4); // high score
    savBuffer.writeUInt32LE(0, 8);     // 0 total coins
    savBuffer[12] = 1;                 // red car unlocked
    fs.writeFileSync(SAV_PATH, savBuffer);

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await gba.waitFrames(35);

        // Verify initial state is STATE_START (0)
        const state0 = await gba.readU32(ADDR_GAME);
        console.log(`Initial State: ${state0} (expected 0=STATE_START)`);
        assert.strictEqual(state0, 0);

        // Tap B 20 times with a few frames between each
        console.log('Tapping B 20 times on the start screen...');
        for (let i = 1; i <= 20; i++) {
            await gba.tap('B', 3, 5);
        }
        await gba.waitFrames(15);

        // Read total_coins from g_game (offset 16)
        const totalCoinsMem = await gba.readU32(ADDR_GAME + 16);
        console.log(`Memory total_coins: ${totalCoinsMem} (expected 99999)`);

        // Capture Start Screen screenshot
        const startScreenshot = path.resolve(__dirname, '../cheat_start_screen.png');
        await gba.screenshot(startScreenshot);
        console.log(`Captured Start Screen screenshot -> ${startScreenshot}`);

        // Advance to Car Select (A)
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        const carSelectScreenshot = path.resolve(__dirname, '../cheat_car_select.png');
        await gba.screenshot(carSelectScreenshot);
        console.log(`Captured Car Select screenshot -> ${carSelectScreenshot}`);

        // Advance to Track Select (A)
        await gba.tap('A', 4, 15);
        await gba.waitFrames(20);
        // Verify SRAM directly from the emulator's save manager
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
        console.log(`SRAM verified via getSaveFile: magic=${sramMagic}, totalCoins=${sramCoins}`);
        assert.strictEqual(sramMagic, 'NITR', 'SRAM magic should be NITR');
        assert.strictEqual(sramCoins, 99999, `Expected 99999 coins in SRAM, got ${sramCoins}`);

        console.log('=== B 20 TIMES CHEAT CODE VERIFIED SUCCESSFULLY! ===');
    } finally {
        await gba.close();
    }
}

runTest().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
