#include "audio.h"
#include "common.h"

// Channel lock timers and priority arbitration
static int s_ch1_lock_timer = 0;
static int s_ch2_lock_timer = 0;
static int s_ch2_priority = 0; // 0=None, 1=Coin, 2=Hurdle, 3=HighScore
static int s_ch4_lock_timer = 0;
static int s_ch4_priority = 0; // 0=None, 1=Thruster, 2=Shrink/Touchdown, 3=Deflect, 4=Crash

// Sound effect state timers
static int s_coin_chime_step = 0;
static int s_high_score_step = 0;
static int s_engine_timer = 0;

// BGM Synthesizer Sequencer State
static int s_bgm_step = 0;
static int s_bgm_frame_timer = 0;
static bool s_bgm_enabled = false;
static bool s_bgm_overdrive = false;

// 32-step Synthwave Bassline frequencies (Am -> F -> C -> G) on Channel 3
static const u16 s_bgm_bass[32] = {
    // Bar 1: Am (A2 = 1546)
    1546, 1546, 1694, 1546, 1546, 1546, 1732, 1694,
    // Bar 2: F (F2 = 1445)
    1445, 1445, 1650, 1445, 1445, 1445, 1694, 1650,
    // Bar 3: C (C3 = 1650)
    1650, 1650, 1750, 1650, 1650, 1650, 1783, 1750,
    // Bar 4: G (G2 = 1500)
    1500, 1500, 1694, 1500, 1500, 1500, 1750, 1694
};

// 32-step Lead Arpeggio line on Channel 1 (when s_ch1_lock_timer == 0)
static const u16 s_bgm_lead[32] = {
    // Bar 1 (Am)
    1783, 1845, 1897, 1936, 1897, 1845, 1783, 1845,
    // Bar 2 (F)
    1812, 1845, 1897, 1948, 1897, 1845, 1812, 1845,
    // Bar 3 (C)
    1867, 1897, 1936, 1976, 1936, 1897, 1867, 1897,
    // Bar 4 (G)
    1732, 1867, 1912, 1928, 1912, 1867, 1732, 1867
};

void bgm_init(void) {
    // Initialize 32-sample Wave RAM at 0x04000090
    REG_SOUND3CNT_L = 0; // Disable first to unlock RAM
    vu8 *wave_ram = (vu8*)0x04000090;
    static const u8 s_wave_table[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xEE, 0xCC, 0xAA, 0x88, 0x66, 0x44, 0x22, 0x00
    };
    for (int i = 0; i < 16; i++) {
        wave_ram[i] = s_wave_table[i];
    }
    REG_SOUND3CNT_L = 0x80;      // Enable channel 3
    REG_SOUND3CNT_H = (2 << 13); // 50% volume (clean synth bass)

    s_bgm_step = 0;
    s_bgm_frame_timer = 0;
    s_bgm_enabled = true;
    s_bgm_overdrive = false;
}

void bgm_set_overdrive(bool active) {
    s_bgm_overdrive = active;
}

void bgm_update(void) {
    if (!s_bgm_enabled) return;

    s_bgm_frame_timer++;
    int step_period = s_bgm_overdrive ? 4 : 6; // 6 frames = 150 BPM, 4 frames = 225 BPM turbo!
    if (s_bgm_frame_timer >= step_period) {
        s_bgm_frame_timer = 0;
        s_bgm_step = (s_bgm_step + 1) & 31;

        // 1. Pulsing Synth Bass on Channel 3
        u16 bass_freq = s_bgm_bass[s_bgm_step];
        REG_SOUND3CNT_L = 0x80;
        REG_SOUND3CNT_H = (2 << 13);
        REG_SOUND3CNT_X = 0x8000 | bass_freq;

        // 2. Lead synth arpeggio on Channel 1 if no SFX is using it
        if (s_ch1_lock_timer == 0) {
            u16 lead_freq = s_bgm_lead[s_bgm_step];
            REG_SOUND1CNT_L = 0; // No sweep
            REG_SOUND1CNT_H = (1 << 6) | (3 << 12) | (1 << 11); // Duty 25%, Vol 3
            REG_SOUND1CNT_X = 0x8000 | lead_freq;
        }

        // 3. Crisp Hi-hat on off-beats on Channel 4 if noise is free
        if ((s_bgm_step & 1) && s_ch4_priority == 0 && s_ch4_lock_timer == 0) {
            REG_SOUND4CNT_L = (2 << 12) | (1 << 8); // Low volume 2, instant decay
            REG_SOUND4CNT_H = 0x8000 | (1 << 4) | 0; // High frequency tick
        }
    }
}

void audio_init(void) {
    // Turn on sound master
    REG_SOUNDCNT_X = 0x80; // Master sound enable
    REG_SOUNDCNT_L = 0x77; // Max volume Left and Right for all PSG channels
    REG_SOUNDCNT_H = 0x02; // PSG channels 1-4 mixed to output
    s_ch1_lock_timer = 0;
    s_ch2_lock_timer = 0;
    s_ch2_priority = 0;
    s_ch4_lock_timer = 0;
    s_ch4_priority = 0;
    s_coin_chime_step = 0;
    s_high_score_step = 0;
    bgm_init();
}

void audio_update(void) {
    // Decay Channel 1 lock timer
    if (s_ch1_lock_timer > 0) {
        s_ch1_lock_timer--;
    }

    // Decay Channel 2 lock timer
    if (s_ch2_lock_timer > 0) {
        s_ch2_lock_timer--;
        if (s_ch2_lock_timer == 0) {
            s_ch2_priority = 0;
        }
    }

    // Decay Channel 4 lock timer
    if (s_ch4_lock_timer > 0) {
        s_ch4_lock_timer--;
        if (s_ch4_lock_timer == 0) {
            s_ch4_priority = 0;
        }
    }

    // Multi-note coin chime sequencer (B5 -> E6)
    if (s_coin_chime_step > 0) {
        s_coin_chime_step--;
        // Only fire Note 2 if high-priority sound (hurdle/fanfare) has not preempted Channel 2
        if (s_coin_chime_step == 3 && s_ch2_priority <= 1) {
            // Note 2: High E6 (freq = 1912)
            REG_SOUND2CNT_L = (2 << 6) | (10 << 12) | (1 << 11); // Duty 50%, Vol 10, decay
            REG_SOUND2CNT_H = 0x8000 | 1912; // Trigger note
        }
    }

    // Multi-note High Score celebration sequencer (C6 -> E6 -> G6)
    if (s_high_score_step > 0) {
        s_high_score_step--;
        if (s_high_score_step == 12) {
            // Note 2: E6 (freq = 1912)
            REG_SOUND2CNT_L = (2 << 6) | (14 << 12) | (2 << 11);
            REG_SOUND2CNT_H = 0x8000 | 1912;
        } else if (s_high_score_step == 4) {
            // Note 3: G6 (freq = 1976)
            REG_SOUND2CNT_L = (2 << 6) | (15 << 12) | (3 << 11);
            REG_SOUND2CNT_H = 0x8000 | 1976;
        }
    }
}

// Engine hum: modulating pitch based on speed
// Priority 0: Only writes to Channel 1 if no higher-priority effect (jump, powerup) is active!
void sfx_engine_hum(int speed_level) {
    s_engine_timer++;
    if ((s_engine_timer & 7) == 0 && s_ch1_lock_timer == 0) {
        // Base tone frequency between 1300 and 1650
        int freq = 1350 + (speed_level * 25);
        if (freq > 1750) freq = 1750;
        REG_SOUND1CNT_L = 0; // No sweep
        REG_SOUND1CNT_H = (1 << 6) | (2 << 12); // Low volume (2) continuous hum
        REG_SOUND1CNT_X = 0x8000 | freq;
    }
}

// Jump sound: rising whoosh
// Priority 2 on Ch1: Locks Channel 1 for 16 frames so engine hum doesn't clip the sweep
void sfx_jump(void) {
    s_ch1_lock_timer = 16;
    REG_SOUND1CNT_L = (2 << 4) | (0 << 3) | 5; // Sweep time 2, sweep up, shift 5
    REG_SOUND1CNT_H = (2 << 6) | (12 << 12) | (2 << 8); // Duty 50%, Vol 12
    REG_SOUND1CNT_X = 0x8000 | 1400; // Trigger from base pitch
}

// Shrink sound: aerodynamic downshift whoosh (Channel 4 noise)
// Priority 2 on Ch4: Only plays if crash or shield deflection is not active
void sfx_shrink(void) {
    if (s_ch4_priority > 2) return;
    s_ch4_priority = 2;
    s_ch4_lock_timer = 6;
    REG_SOUND4CNT_L = (10 << 12) | (1 << 8); // Vol 10, quick decay
    REG_SOUND4CNT_H = 0x8000 | (4 << 4) | 3;  // Aerodynamic compression whoosh
}

// Coin chime: 2-tone bright arpeggio
// Priority 1 on Ch2: Only plays if hurdle or high score is not holding Channel 2
void sfx_coin(void) {
    if (s_ch2_priority > 1) return; // Hurdle or high score active
    s_ch2_priority = 1;
    s_ch2_lock_timer = 8;
    // Note 1: B5 (freq = 1810)
    REG_SOUND2CNT_L = (2 << 6) | (12 << 12) | (1 << 11); // Duty 50%, Vol 12, decay
    REG_SOUND2CNT_H = 0x8000 | 1810;
    s_coin_chime_step = 6; // Trigger Note 2 in 3 frames
}

// Power-up fanfare: bright triumphant blast
// Priority 3 on Ch1: Locks Channel 1 for 24 frames
void sfx_powerup(void) {
    s_ch1_lock_timer = 24;
    REG_SOUND1CNT_L = (1 << 4) | (0 << 3) | 4; // Sweep up
    REG_SOUND1CNT_H = (3 << 6) | (14 << 12) | (1 << 8);
    REG_SOUND1CNT_X = 0x8000 | 1600;
}

// Hurdle clear chime: bright affirmative chirp
// Priority 2 on Ch2: Preempts coins and cancels pending coin step
void sfx_hurdle(void) {
    if (s_ch2_priority > 2) return; // High score active
    s_ch2_priority = 2;
    s_ch2_lock_timer = 16;
    s_coin_chime_step = 0; // Cancel pending coin note 2
    REG_SOUND2CNT_L = (2 << 6) | (14 << 12) | (2 << 11); // Duty 50%, Vol 14, decay 2
    REG_SOUND2CNT_H = 0x8000 | 1976; // B6 high bright chime
}

// High score celebration fanfare: triumphant 3-note arpeggio on Channel 2
// Priority 3 on Ch2: Preempts all other Channel 2 sounds
void sfx_high_score(void) {
    s_ch2_priority = 3;
    s_ch2_lock_timer = 28;
    s_coin_chime_step = 0;
    s_high_score_step = 20;
    // Note 1: C6 (freq = 1869)
    REG_SOUND2CNT_L = (2 << 6) | (14 << 12) | (2 << 11);
    REG_SOUND2CNT_H = 0x8000 | 1869;
}

// Crash explosion: deep heavy noise blast
// Priority 4 on Ch4: Maximum priority, locks Channel 4 for 30 frames
void sfx_crash(void) {
    s_ch4_priority = 4;
    s_ch4_lock_timer = 30;
    REG_SOUND4CNT_L = (15 << 12) | (7 << 8); // Max volume 15, long decay
    REG_SOUND4CNT_H = 0x8000 | (7 << 4) | 7;  // Heavy low-frequency noise rumble
}

// Nitro thruster roar
// Priority 1 on Ch4: Periodic pulse, yields to any higher priority noise effect
void sfx_nitro(void) {
    if (s_ch4_priority > 1) return;
    s_ch4_priority = 1;
    s_ch4_lock_timer = 4;
    REG_SOUND4CNT_L = (10 << 12) | (1 << 8);
    REG_SOUND4CNT_H = 0x8000 | (2 << 4) | 4;
}

// Suspension touchdown thud (replaces incorrect coin chime on jump landing)
// Priority 2 on Ch4: Quick low thump
void sfx_touchdown(void) {
    if (s_ch4_priority > 2) return;
    s_ch4_priority = 2;
    s_ch4_lock_timer = 6;
    REG_SOUND4CNT_L = (5 << 12) | (1 << 8); // Low volume 5, quick decay
    REG_SOUND4CNT_H = 0x8000 | (6 << 4) | 1; // Deep low-frequency suspension thump
}

// Pause toggle chime (clean menu blip on Channel 2)
void sfx_pause(void) {
    REG_SOUND2CNT_L = (2 << 6) | (10 << 12) | (1 << 11); // Vol 10, quick decay
    REG_SOUND2CNT_H = 0x8000 | 1700;
}

// Close-call near-miss Doppler horn & wind whoosh
void sfx_close_call(void) {
    REG_SOUND1CNT_L = (1 << 4) | (2 << 0);                // Fast downward sweep
    REG_SOUND1CNT_H = (2 << 6) | (12 << 12) | (1 << 11);  // High volume 12, quick decay
    REG_SOUND1CNT_X = 0x8000 | 1880;                      // High horn pitch
    s_ch1_lock_timer = 14;
}

// Stunt Launch Ramp supersonic launch chime
void sfx_stunt_ramp(void) {
    s_ch1_lock_timer = 22;
    REG_SOUND1CNT_L = (1 << 4) | (0 << 3) | 5;            // Rapid upward sweep
    REG_SOUND1CNT_H = (3 << 6) | (14 << 12) | (2 << 11);  // High volume 14, 50% duty
    REG_SOUND1CNT_X = 0x8000 | 1550;                      // Soaring rising tone
}

// Tire skid / surface slick spinout screech
void sfx_skid(void) {
    if (s_ch4_priority > 2) return;
    s_ch4_priority = 2;
    s_ch4_lock_timer = 18;
    REG_SOUND4CNT_L = (11 << 12) | (2 << 8);              // Vol 11, medium decay
    REG_SOUND4CNT_H = 0x8000 | (3 << 4) | 2;              // Grating tire friction noise
}

// Shield activation: crystalline rising harmonic chime
void sfx_shield_activate(void) {
    s_ch1_lock_timer = 20;
    REG_SOUND1CNT_L = (1 << 4) | (0 << 3) | 3;                // Sweep up
    REG_SOUND1CNT_H = (2 << 6) | (13 << 12) | (1 << 11);       // Vol 13
    REG_SOUND1CNT_X = 0x8000 | 1750;                          // High harmonic
}

// Shield deflection: crisp electric barrier discharge
// Priority 3 on Ch4: High-energy electric discharge
void sfx_shield_deflect(void) {
    if (s_ch4_priority > 3) return;
    s_ch4_priority = 3;
    s_ch4_lock_timer = 14;
    REG_SOUND4CNT_L = (14 << 12) | (2 << 8);                  // Vol 14, quick decay
    REG_SOUND4CNT_H = 0x8000 | (1 << 4) | 3;                  // High-frequency deflector pop
}

// Lateral metal-on-guardrail friction scrape screech on Channel 4
void sfx_scrape(void) {
    if (s_ch4_priority > 2) return;
    s_ch4_priority = 2;
    s_ch4_lock_timer = 10;
    REG_SOUND4CNT_L = (11 << 12) | (1 << 8);                  // Vol 11, quick decay
    REG_SOUND4CNT_H = 0x8000 | (2 << 4) | 0;                  // High-frequency friction screech
}

// Rising golden chord chime on Channel 2 when streak multiplier levels up
void sfx_streak_levelup(int level) {
    s_ch2_priority = 2;
    s_ch2_lock_timer = 16;
    int base_freq = 1750 + (level * 50);
    if (base_freq > 1950) base_freq = 1950;
    REG_SOUND2CNT_L = (2 << 6) | (14 << 12) | (2 << 11);     // Vol 14, smooth decay
    REG_SOUND2CNT_H = 0x8000 | base_freq;
}

// Silence continuous engine hum and BGM when entering pause
void audio_pause(void) {
    REG_SOUND1CNT_H = 0; // Cut Channel 1 volume to 0
    s_ch1_lock_timer = 0;
    REG_SOUND3CNT_H = 0; // Cut Channel 3 bass volume
    s_bgm_enabled = false;
    REG_SOUND4CNT_L = 0; // Cut Channel 4 noise volume
    s_ch4_lock_timer = 0;
    s_ch4_priority = 0;
}

// Restore engine audio and BGM when unpausing
void audio_resume(void) {
    REG_SOUND3CNT_H = (2 << 13); // Restore Channel 3 bass volume
    s_bgm_enabled = true;
    sfx_engine_hum(0);
}
