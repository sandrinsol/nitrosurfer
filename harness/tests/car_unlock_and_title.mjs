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
        if (parts.length === 3) syms[parts[2]] = parseInt(parts[0], 16);
    }
    return syms;
}

const syms = getSymbols();
const ADDR_GAME = syms['g_game'];

async function testCarUnlockAndTitle() {
    console.log('=== Nitro Surfer Car Select & 2000G Unlock Verification ===');

    const gba = await EmuHarness.launch(ROM_PATH);
    try {
        console.log('1. Booting game and navigating to Car Select screen...');
        await gba.waitFrames(35);
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);

        let state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 1, 'Should be in STATE_TITLE (1)');

        // Check initial unlocked cars mask: Red(1) and Blue(2) must be unlocked (mask = 3)
        // In Game struct:
        // offset 0: state (4)
        // offset 4: score (4)
        // offset 8: high_score (4)
        // offset 12: coins_collected (4)
        // offset 16: total_coins (4)
        // Let's verify total_coins offset and cars_unlocked
        const memGame = await gba.readMemory(ADDR_GAME, 120);
        console.log('2. Verifying default unlocked cars: Red & Blue are free...');

        // Verify selecting Red (0) advances to Track Selection
        console.log('Testing Red car selection...');
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 5, 'Red car should be unlocked and advance to STATE_SELECT_MAP (5)');

        // Return to Car Select with B
        await gba.tap('B', 4, 10);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 1, 'Should return to STATE_TITLE (1)');

        // Verify selecting Blue (1) advances to Track Selection
        console.log('Testing Blue car selection...');
        await gba.tap('RIGHT', 4, 10);
        await gba.waitFrames(15);
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 5, 'Blue car should be unlocked and advance to STATE_SELECT_MAP (5)');

        // Return to Car Select with B
        await gba.tap('B', 4, 10);
        await gba.waitFrames(20);

        // Navigate to Pink car (Col 2, Row 0)
        console.log('3. Testing locked Pink car without enough coins...');
        await gba.tap('RIGHT', 4, 10); // currently Blue (1), move to Pink (2)
        await gba.waitFrames(15);

        // Ensure coins are 0 initially
        const zeroBuf = Buffer.alloc(4);
        zeroBuf.writeInt32LE(0, 0);
        await gba.writeMemory(ADDR_GAME + 16, zeroBuf);
        await gba.waitFrames(5);

        // Try pressing A on locked Pink car with 0 coins
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 1, 'Locked Pink car must NOT advance to track select with 0 coins');
        console.log('✓ Locked car blocked when player cannot afford 2000 gold!');

        // 4. Inject 2500 coins and purchase Pink car for 2000 gold
        console.log('4. Injecting 2500 total coins and purchasing Pink car for 2000 gold...');
        const coinsBuf = Buffer.alloc(4);
        coinsBuf.writeInt32LE(2500, 0);
        await gba.writeMemory(ADDR_GAME + 16, coinsBuf);
        await gba.waitFrames(10);

        // Press A to buy
        await gba.tap('A', 4, 10);
        await gba.waitFrames(25);

        const coinsAfter = (await gba.readMemory(ADDR_GAME + 16, 4)).readInt32LE(0);
        console.log(`Coins after purchasing Pink car: ${coinsAfter} (expected 500)`);
        assert.strictEqual(coinsAfter, 500, `Coins should decrease by 2000, expected 500, got ${coinsAfter}`);
        console.log('✓ 2000 gold deducted successfully upon purchase!');

        // Press A again on newly unlocked Pink car -> should advance to track select!
        console.log('5. Selecting newly unlocked Pink car...');
        await gba.tap('A', 4, 10);
        await gba.waitFrames(20);
        state = await gba.readU32(ADDR_GAME);
        assert.strictEqual(state, 5, 'Newly unlocked Pink car must now advance to STATE_SELECT_MAP (5)');
        console.log('✓ Unlocked Pink car successfully selected!');

        // Capture screenshot of Car Select screen with Pink car owned
        await gba.tap('B', 4, 10);
        await gba.waitFrames(20);
        const carSelectPng = path.resolve(__dirname, '../car_select_verified.png');
        await gba.screenshot(carSelectPng);
        console.log(`Captured Car Select screenshot -> ${carSelectPng}`);

    } finally {
        await gba.close();
    }

    console.log('\n=== ALL CAR SELECT & 2000G UNLOCK TESTS PASSED PERFECTLY! ===');
}

testCarUnlockAndTitle().catch(err => {
    console.error('[FATAL] Car select test failed:', err);
    process.exit(1);
});
