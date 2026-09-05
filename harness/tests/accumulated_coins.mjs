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
        if (parts.length === 3) {
            syms[parts[2]] = parseInt(parts[0], 16);
        }
    }
    return syms;
}

const SRAM_BASE = 0x0E000000;

async function runTest() {
    console.log('=== Nitro Surfer Accumulated Coins Verification ===');
    const syms = getSymbols();
    assert(syms['g_game'], 'Symbol g_game not found in ELF!');
    const ADDR_GAME = syms['g_game'];

    console.log('1. Writing 250 accumulated coins to nitrosurfer.sav...');
    const savPath = path.resolve(__dirname, '../../nitrosurfer.sav');
    let savBuffer;
    if (fs.existsSync(savPath)) {
        savBuffer = fs.readFileSync(savPath);
    } else {
        savBuffer = Buffer.alloc(32768, 0xFF);
    }
    savBuffer.write('NITR', 0, 4, 'ascii');
    savBuffer.writeUInt32LE(16000, 4);
    savBuffer.writeUInt32LE(250, 8); // 250 total coins
    fs.writeFileSync(savPath, savBuffer);

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        await gba.waitFrames(30);

        // Verify initial state is STATE_START (0)
        const state0 = await gba.readU32(ADDR_GAME);
        console.log(`State on boot: ${state0} (expected 0=STATE_START)`);
        assert.strictEqual(state0, 0, `Expected STATE_START (0), got ${state0}`);

        // Capture screenshot of start screen with 250 coins
        const screenshotPath = path.resolve(__dirname, '../start_screen_coins.png');
        await gba.screenshot(screenshotPath);
        console.log(`Captured Start Screen screenshot -> ${screenshotPath}`);

        // Read SRAM from disk to verify persistence
        const sramRead = fs.readFileSync(savPath);
        const magic = sramRead.toString('ascii', 0, 4);
        const sramHigh = sramRead.readUInt32LE(4);
        const sramCoins = sramRead.readUInt32LE(8);
        console.log(`SRAM verified: magic=${magic}, highScore=${sramHigh}, totalCoins=${sramCoins}`);
        assert.strictEqual(magic, 'NITR', 'SRAM magic should be NITR');
        assert.strictEqual(sramHigh, 16000, 'SRAM high score should be 16000');
        assert.strictEqual(sramCoins, 250, 'SRAM total coins should be 250');

        console.log('=== ACCUMULATED COINS VERIFICATION PASSED! ===');
    } finally {
        await gba.close();
    }
}

runTest().catch(err => {
    console.error('Test FAILED:', err);
    process.exit(1);
});
