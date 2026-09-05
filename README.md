# 🏎️ Nitro Surfer (Game Boy Advance)

**Nitro Surfer** is a fast-paced 60 FPS top-down arcade racer built from the ground up for the Game Boy Advance. Drift across multi-lane highways, dodge dynamic traffic and construction barriers, collect coins, and blast through courses with high-velocity Nitro boosts.

Includes an offline-ready **web arcade player** and an **AI/headless test harness** powered by EmulatorJS (libretro `mgba`).

---

## 🎮 Play Online

When deployed via **GitHub Pages** or **Netlify**, Nitro Surfer runs directly in any modern desktop or mobile browser with zero installation:

- **GitHub Pages:** `https://sandrinsol.github.io/nitrosurfer/`
- **Playable Web Runner:** Open `index.html` on any static HTTP host.

### Play Locally

Browsers restrict WebAssembly and Web Audio over `file://` URLs, so a lightweight local server is used:

1. **One-Click (macOS):** Double-click [`start.command`](start.command). It opens the game in your default browser at `http://localhost:8000/`.
2. **Terminal:**
   ```bash
   python3 -m http.server 8000
   # Open http://localhost:8000
   ```

---

## 🕹️ Controls

| Action | GBA Button | Keyboard (Web) | Description |
|---|---|---|---|
| **Steer** | D-Pad Left / Right | `◀` `▶` or `A` / `D` | Steer vehicle across lanes |
| **Accelerate** | A Button | `Z` or `▲` | Gas / Confirm menu selection |
| **Brake / Drift** | B Button | `X` or `▼` | Decelerate / Power slide around hazards |
| **Nitro Boost** | L Shoulder | `Space` or `A` | Instant supersonic speed burst |
| **Look Back / Horn** | R Shoulder | `Shift` or `S` | Horn / glance behind |
| **Pause / Menu** | START | `Enter` | Open in-game pause menu |
| **Select** | SELECT | `Shift` / `Backspace` | Toggle UI modes / back |

---

## 🌟 Game Features

- **6 Unique Worlds:**
  - 🏖️ **Palm Beach:** High-speed coastal highway with palm hazards.
  - 🏙️ **City Highway:** Dense urban traffic and speeding semi-trucks.
  - 🏛️ **Maya Temple Trail:** Ancient jungle road with ruins and narrow chicanes.
  - 🌙 **Gothic Midnight:** Low-visibility night raceway with slick patches.
  - 🚀 **Cape Orbital:** Futuristic spaceport runway with high-intensity jumps.
  - ❄️ **Winter Snow:** Slippery snowdrifts and severe ice spinouts.
- **5 Unlockable Hypercars:** Unlock custom machines with distinct acceleration, top speed, and handling in the Showroom.
- **Dynamic Obstacles:** Roadblocks, warning signs, oncoming trucks, and oil slicks.
- **Pure 60 FPS Performance:** Engineered in C for the ARM7TDMI processor with hardware sprite scaling and direct sound.

---

## 🛠️ Building the ROM from Source

To compile [`nitrosurfer.gba`](nitrosurfer.gba) yourself, you need the **devkitARM** toolchain (devkitPro) and **libtonc**:

```bash
cd nitrosurfer
make
```

The compiled binary will be placed at `nitrosurfer/nitrosurfer.gba` and copied to the root directory.

---

## 🤖 Automated Test Harness (`harness/`)

The repository includes a headless Playwright + libretro mgba test harness in [`harness/`](harness/):

- **Deterministic Frame Clock:** Step exact frame numbers, take bit-identical screenshots, and compare against goldens.
- **RAM Inspection & Injection:** Direct memory reading and writing (`g_player`, `g_obstacles`, `g_game`).
- **Run the Test Suite:**
  ```bash
  cd harness
  npm install
  npm test
  ```

---

## 🚀 Hosting & Deployment

### GitHub Pages (Automatic)
This repository includes a GitHub Actions workflow [`.github/workflows/deploy-pages.yml`](.github/workflows/deploy-pages.yml) that automatically deploys the web player to GitHub Pages on every push to `main`.

*(Note: If the repository is private, GitHub Pages requires a GitHub Pro/Enterprise plan or changing visibility to Public in Repo Settings).*

### Netlify (100% Free for Private Repos)
Netlify natively connects to private GitHub repositories for free with no build step required:
1. Go to [Netlify Dashboard](https://app.netlify.com).
2. Click **Add new site** > **Import an existing project** > Select `sandrinsol/nitrosurfer`.
3. Set publish directory to `.` (root). Netlify uses the pre-configured [`netlify.toml`](netlify.toml) for caching and WASM headers.

---

## 📜 License & Credits

- Game Code & Assets: © Sandrino Breshani.
- Bundled Emulator Core: [EmulatorJS](https://emulatorjs.org) (GPL-3.0-or-later). See [`LICENSE`](LICENSE) and [`NOTICE.md`](NOTICE.md).
