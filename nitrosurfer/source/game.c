#include "game.h"
#include "audio.h"
#include "render.h"
#include "gfx_data.h"
#include <stdlib.h>
#include <string.h>

Game g_game;
Player g_player;
Obstacle g_obstacles[MAX_OBSTACLES];
Coin g_coins[MAX_COINS];
WorldPowerup g_world_powerups[MAX_POWERUPS];
DebrisParticle g_debris[MAX_DEBRIS];

// 3D Camera Configuration
const Camera g_camera = {
    .horizon_y = 56,
    .height = 45,
    .dist = 50
};

// Parallel Lane X positions in top-down view (Left=64, Center=120, Right=176)
static const int LANE_X_PLAYER[NUM_LANES] = { LANE_X_LEFT, LANE_X_CENTER, LANE_X_RIGHT };

// Top-Down Linear Highway Projection
// z_fp: distance ahead along highway (96.0 at spawn down to negative values off-screen)
void project_to_screen(int lane, int z_fp, int y_elevation, int *out_x, int *out_y, int *out_scale) {
    int z_int = FP_TO_INT(z_fp);

    // X is strictly centered on the parallel lane
    int x = LANE_X_PLAYER[lane];

    // Linear mapping from top of road down to player base Y
    // At z=96 (spawn), y = ROAD_TOP_Y (16)
    // At z=16 (player), y = PLAYER_BASE_Y (126)
    // Moves downwards at strictly constant linear speed without perspective distortion!
    int y = PLAYER_BASE_Y - ((z_int - PLAYER_BASE_Z) * (PLAYER_BASE_Y - ROAD_TOP_Y)) / (96 - PLAYER_BASE_Z);

    // In top-down view, elevation (e.g. jumping coins or signs) shifts slightly upward
    if (y_elevation > 0) {
        y -= y_elevation;
    }

    if (out_x) *out_x = x;
    if (out_y) *out_y = y;
    if (out_scale) *out_scale = 3; // Constant size: obstacles do not change size!
}

void player_init(void) {
    g_player.lane = LANE_CENTER;
    g_player.x = TO_FP(LANE_X_PLAYER[LANE_CENTER]);
    g_player.target_x = g_player.x;
    g_player.y_offset = 0;
    g_player.vy = 0;
    g_player.is_jumping = false;
    g_player.is_shrinking = false;
    g_player.shrink_timer = 0;
    g_player.tilt = 0;
    g_player.shield_active = 0;
    g_player.nitro_timer = 0;
    g_player.magnet_timer = 0;
    g_player.invuln_timer = 0;
    g_player.is_crashing = false;
    g_player.crash_frame = 0;
    g_player.buffered_key = 0;
    g_player.buffer_timer = 0;
    g_player.landing_squash = 0;
    g_player.close_call_timer = 0;
    g_player.shrink_boost_timer = 0;
    g_player.hurdle_timer = 0;
    g_player.stunt_timer = 0;
    g_player.spinout_timer = 0;
}

void obstacles_init(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        memset(&g_obstacles[i], 0, sizeof(Obstacle));
        g_obstacles[i].active = false;
    }
}

void coins_init(void) {
    for (int i = 0; i < MAX_COINS; i++) {
        g_coins[i].active = false;
        g_coins[i].x_fp = 0;
    }
}

void powerups_init(void) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        g_world_powerups[i].active = false;
    }
}

void debris_init(void) {
    for (int i = 0; i < MAX_DEBRIS; i++) {
        g_debris[i].active = false;
    }
}

void spawn_debris(int sx, int sy, int count) {
    int spawned = 0;
    for (int i = 0; i < MAX_DEBRIS && spawned < count; i++) {
        if (!g_debris[i].active) {
            g_debris[i].active = true;
            g_debris[i].type = DEBRIS_SPARK; // standard spark / crash debris
            g_debris[i].x = TO_FP(sx);
            g_debris[i].y = TO_FP(sy);
            g_debris[i].vx = TO_FP(((rand() % 9) - 4) * 0.75f);
            g_debris[i].vy = -TO_FP(2.2f + (rand() % 25) * 0.1f);
            g_debris[i].life = 16 + (rand() % 10);
            spawned++;
        }
    }
}

// Spawn electric deflection discharge burst from forcefield break
void spawn_shield_discharge(int sx, int sy) {
    for (int i = 0; i < MAX_DEBRIS; i++) {
        if (!g_debris[i].active) {
            g_debris[i].active = true;
            g_debris[i].type = DEBRIS_SHIELD_DISCHARGE;
            g_debris[i].x = TO_FP(sx);
            g_debris[i].y = TO_FP(sy);
            g_debris[i].vx = 0;
            g_debris[i].vy = 0;
            g_debris[i].life = 12; // 12-frame electric discharge
            break;
        }
    }
}

// Spawn arcade clearance feedback: floating score popup + twinkling stars
void spawn_clearance_feedback(int sx, int sy, DebrisType popup_type) {
    // 1. Floating score popup (type: DEBRIS_POPUP_150, DEBRIS_POPUP_100, DEBRIS_POPUP_250)
    for (int i = 0; i < MAX_DEBRIS; i++) {
        if (!g_debris[i].active) {
            g_debris[i].active = true;
            g_debris[i].type = popup_type;
            g_debris[i].x = TO_FP(sx);
            g_debris[i].y = TO_FP(sy);
            g_debris[i].vx = 0;
            g_debris[i].vy = -TO_FP(0.6f); // Gentle steady float upwards
            g_debris[i].life = 28;
            break;
        }
    }
    // 2. Twinkling clearance star sparkles
    int spawned = 0;
    for (int i = 0; i < MAX_DEBRIS && spawned < 4; i++) {
        if (!g_debris[i].active) {
            g_debris[i].active = true;
            g_debris[i].type = DEBRIS_STAR_SPARKLE;
            g_debris[i].x = TO_FP(sx + ((spawned - 2) * 6));
            g_debris[i].y = TO_FP(sy);
            g_debris[i].vx = TO_FP(((spawned & 1) ? 1.0f : -1.0f) * (0.8f + (spawned * 0.3f)));
            g_debris[i].vy = -TO_FP(0.6f + (rand() % 15) * 0.1f);
            g_debris[i].life = 20;
            spawned++;
        }
    }
}

// Spawn falling crystal snowflake for Winter Snow theme
void spawn_snowflake(void) {
    for (int i = 0; i < MAX_DEBRIS; i++) {
        if (!g_debris[i].active) {
            g_debris[i].active = true;
            g_debris[i].type = DEBRIS_SNOWFLAKE;
            g_debris[i].x = TO_FP(rand() % SCREEN_W);
            g_debris[i].y = TO_FP(-4);
            g_debris[i].vx = 0;
            g_debris[i].vy = TO_FP(0.85f);
            g_debris[i].life = 190;
            break;
        }
    }
}

void debris_update(void) {
    for (int i = 0; i < MAX_DEBRIS; i++) {
        if (!g_debris[i].active) continue;
        g_debris[i].x += g_debris[i].vx;
        g_debris[i].y += g_debris[i].vy;

        if (g_debris[i].type == DEBRIS_SPARK) {
            // Normal debris: gravity
            g_debris[i].vy += TO_FP(0.25f);
        } else if (g_debris[i].type == DEBRIS_STAR_SPARKLE) {
            // Star sparkles: slight drag
            g_debris[i].vx = (g_debris[i].vx * 7) / 8;
            g_debris[i].vy += TO_FP(0.02f);
        } else if (g_debris[i].type == DEBRIS_SNOWFLAKE) {
            // Snowflake flutter: gentle sine drift downwards
            int wobble = ((g_game.frame_count + i * 17) & 16) ? 1 : -1;
            g_debris[i].x += TO_FP(wobble * 0.2f);
            g_debris[i].vy = TO_FP(0.8f) + (g_game.speed >> 1);
        }
        // Floating popups (type >= 2): steady upward float, no gravity

        g_debris[i].life--;
        if (g_debris[i].life <= 0 || FP_TO_INT(g_debris[i].y) > 165 || FP_TO_INT(g_debris[i].y) < -10) {
            g_debris[i].active = false;
        }
    }
}

// Cartridge SRAM Persistence (0x0E000000)
#define SRAM_BASE ((volatile u8 *)0x0E000000)
#define SRAM_MAGIC_0 'N'
#define SRAM_MAGIC_1 'I'
#define SRAM_MAGIC_2 'T'
#define SRAM_MAGIC_3 'R'

static int sram_load_high_score(void) {
    if (SRAM_BASE[0] == SRAM_MAGIC_0 &&
        SRAM_BASE[1] == SRAM_MAGIC_1 &&
        SRAM_BASE[2] == SRAM_MAGIC_2 &&
        SRAM_BASE[3] == SRAM_MAGIC_3) {
        int score = (int)SRAM_BASE[4] |
                    ((int)SRAM_BASE[5] << 8) |
                    ((int)SRAM_BASE[6] << 16) |
                    ((int)SRAM_BASE[7] << 24);
        if (score >= 0 && score <= 9999999) {
            return score;
        }
    }
    return 15000;
}

static void sram_save_high_score(int score) {
    if (score > 9999999) score = 9999999;
    SRAM_BASE[0] = SRAM_MAGIC_0;
    SRAM_BASE[1] = SRAM_MAGIC_1;
    SRAM_BASE[2] = SRAM_MAGIC_2;
    SRAM_BASE[3] = SRAM_MAGIC_3;
    SRAM_BASE[4] = (u8)(score & 0xFF);
    SRAM_BASE[5] = (u8)((score >> 8) & 0xFF);
    SRAM_BASE[6] = (u8)((score >> 16) & 0xFF);
    SRAM_BASE[7] = (u8)((score >> 24) & 0xFF);
}

static int sram_load_total_coins(void) {
    if (SRAM_BASE[0] == SRAM_MAGIC_0 &&
        SRAM_BASE[1] == SRAM_MAGIC_1 &&
        SRAM_BASE[2] == SRAM_MAGIC_2 &&
        SRAM_BASE[3] == SRAM_MAGIC_3) {
        int coins = (int)SRAM_BASE[8] |
                    ((int)SRAM_BASE[9] << 8) |
                    ((int)SRAM_BASE[10] << 16) |
                    ((int)SRAM_BASE[11] << 24);
        if (coins >= 0 && coins <= 99999) {
            return coins;
        }
        if (coins > 99999) {
            return 99999;
        }
    }
    return 0;
}

static void sram_save_total_coins(int coins) {
    if (coins < 0) coins = 0;
    if (coins > 99999) coins = 99999;
    SRAM_BASE[0] = SRAM_MAGIC_0;
    SRAM_BASE[1] = SRAM_MAGIC_1;
    SRAM_BASE[2] = SRAM_MAGIC_2;
    SRAM_BASE[3] = SRAM_MAGIC_3;
    SRAM_BASE[8] = (u8)(coins & 0xFF);
    SRAM_BASE[9] = (u8)((coins >> 8) & 0xFF);
    SRAM_BASE[10] = (u8)((coins >> 16) & 0xFF);
    SRAM_BASE[11] = (u8)((coins >> 24) & 0xFF);
}

const int g_map_unlock_costs[MAP_COUNT] = {
    0,     // MAP_CITY (unlocked)
    1000,  // MAP_BEACH (palm)
    2000,  // MAP_WINTER
    3000,  // MAP_ORBITAL
    4000,  // MAP_GOTHIC
    5000   // MAP_MAYA
};

static u8 sram_load_unlocked_cars(void) {
    if (SRAM_BASE[0] == SRAM_MAGIC_0 &&
        SRAM_BASE[1] == SRAM_MAGIC_1 &&
        SRAM_BASE[2] == SRAM_MAGIC_2 &&
        SRAM_BASE[3] == SRAM_MAGIC_3) {
        u8 val = SRAM_BASE[12];
        if ((val & ~0x3F) == 0 && (val & CAR_UNLOCKED_DEFAULT) == CAR_UNLOCKED_DEFAULT) {
            return val;
        }
    }
    return CAR_UNLOCKED_DEFAULT;
}

static void sram_save_unlocked_cars(u8 mask) {
    SRAM_BASE[0] = SRAM_MAGIC_0;
    SRAM_BASE[1] = SRAM_MAGIC_1;
    SRAM_BASE[2] = SRAM_MAGIC_2;
    SRAM_BASE[3] = SRAM_MAGIC_3;
    SRAM_BASE[12] = (mask & 0x3F) | CAR_UNLOCKED_DEFAULT;
}

static u8 sram_load_unlocked_maps(void) {
    if (SRAM_BASE[0] == SRAM_MAGIC_0 &&
        SRAM_BASE[1] == SRAM_MAGIC_1 &&
        SRAM_BASE[2] == SRAM_MAGIC_2 &&
        SRAM_BASE[3] == SRAM_MAGIC_3) {
        u8 val = SRAM_BASE[13];
        if ((val & ~0x3F) == 0 && (val & MAP_UNLOCKED_DEFAULT) == MAP_UNLOCKED_DEFAULT) {
            return val;
        }
    }
    return MAP_UNLOCKED_DEFAULT;
}

static void sram_save_unlocked_maps(u8 mask) {
    SRAM_BASE[0] = SRAM_MAGIC_0;
    SRAM_BASE[1] = SRAM_MAGIC_1;
    SRAM_BASE[2] = SRAM_MAGIC_2;
    SRAM_BASE[3] = SRAM_MAGIC_3;
    SRAM_BASE[13] = (mask & 0x3F) | MAP_UNLOCKED_DEFAULT;
}

static inline void add_score(int points) {
    int mult = (g_game.streak_multiplier > 1) ? g_game.streak_multiplier : 1;
    g_game.score += points * mult;
    if (g_game.score > 9999999) g_game.score = 9999999;
}

void game_trigger_shake(int amp, int frames) {
    if (amp > g_game.shake_amp || g_game.shake_timer == 0) {
        g_game.shake_amp = amp;
    }
    if (frames > g_game.shake_timer) {
        g_game.shake_timer = frames;
    }
}

static void streak_add_action(void) {
    g_game.streak_timer = 180; // 3-second retention window
    g_game.streak_count++;
    if (g_game.streak_count >= 3 && g_game.streak_multiplier < 5) {
        g_game.streak_multiplier++;
        g_game.streak_count = 0;
        sfx_streak_levelup(g_game.streak_multiplier);
    }
}

static int s_start_b_presses = 0;

void game_init(void) {
    s_start_b_presses = 0;
    g_game.state = STATE_START;
    g_game.score = 0;
    g_game.high_score = sram_load_high_score();
    g_game.coins_collected = 0;
    g_game.total_coins = sram_load_total_coins();
    g_game.cars_unlocked = sram_load_unlocked_cars();
    g_game.maps_unlocked = sram_load_unlocked_maps();
    g_game.distance_m = 0;
    g_game.distance_fp = 0;
    g_game.base_speed = TO_FP(0.52f);
    g_game.speed = g_game.base_speed;
    g_game.frame_count = 0;
    g_game.spawn_timer = 90;
    g_game.coin_spawn_timer = 40;
    g_game.powerup_spawn_timer = 360;
    g_game.road_scroll = 0;
    g_game.screen_shake = 0;
    g_game.shake_timer = 0;
    g_game.shake_amp = 0;
    g_game.shake_offset_x = 0;
    g_game.shake_offset_y = 0;
    g_game.streak_multiplier = 1;
    g_game.streak_timer = 0;
    g_game.streak_count = 0;
    g_game.env_theme = ENV_MIDNIGHT;
    g_game.env_transition = 0;
    g_game.env_lightning = 0;
    g_game.flash_timer = 0;
    g_game.is_new_high_score = false;
    g_game.high_score_fanfare_played = false;
    g_game.car_color = CAR_COLOR_RED;
    gfx_set_car_color(g_game.car_color);
    g_game.map_theme = MAP_CITY;
    gfx_apply_map_theme(g_game.map_theme);

    player_init();
    obstacles_init();
    coins_init();
    powerups_init();
    debris_init();
}

void game_start(void) {
    // Seed PRNG using title screen dwell time for varied obstacle waves
    srand(g_game.frame_count ^ 0x5A5A);

    g_game.state = STATE_PLAY;
    g_game.score = 0;
    g_game.coins_collected = 0;
    g_game.distance_m = 0;
    g_game.distance_fp = 0;
    g_game.base_speed = TO_FP(0.52f);
    g_game.speed = g_game.base_speed;
    g_game.frame_count = 0;
    g_game.spawn_timer = 90;
    g_game.coin_spawn_timer = 40;
    g_game.powerup_spawn_timer = 360;
    g_game.road_scroll = 0;
    g_game.screen_shake = 0;
    g_game.shake_timer = 0;
    g_game.shake_amp = 0;
    g_game.shake_offset_x = 0;
    g_game.shake_offset_y = 0;
    g_game.streak_multiplier = 1;
    g_game.streak_timer = 0;
    g_game.streak_count = 0;
    g_game.env_theme = ENV_MIDNIGHT;
    g_game.env_transition = 0;
    g_game.env_lightning = 0;
    g_game.flash_timer = 0;
    g_game.close_call_count = 0;
    g_game.wave_pattern = 0;
    g_game.wave_step = 0;
    g_game.is_new_high_score = false;
    g_game.high_score_fanfare_played = false;

    if (g_game.map_theme == MAP_CITY) {
        gfx_apply_env_theme(ENV_MIDNIGHT, 0);
    } else {
        gfx_apply_map_theme(g_game.map_theme);
    }
    gfx_set_car_color(g_game.car_color);

    // Clear text screenblock cleanly once on start
    memset16(se_mem[31], 0, 32 * 32);
    render_clear_title_boxes();

    player_init();
    obstacles_init();
    coins_init();
    powerups_init();
    debris_init();
}

void player_shift_lane(int dir) {
    if (g_player.is_crashing) return;
    int next_lane = g_player.lane + dir;
    if (next_lane >= LANE_LEFT && next_lane <= LANE_RIGHT) {
        g_player.lane = next_lane;
        g_player.target_x = TO_FP(LANE_X_PLAYER[next_lane]);
        g_player.tilt = dir;
    }
}

void player_jump(void) {
    if (g_player.is_crashing) return;
    // Allow jumping from ground OR cancelling an active shrink into an instant jump!
    if (!g_player.is_jumping || g_player.is_shrinking) {
        g_player.is_jumping = true;
        g_player.vy = TO_FP(5.6f); // Generous high jump impulse (apex ~46px)
        g_player.is_shrinking = false;
        g_player.shrink_timer = 0;
        sfx_jump();
    }
}

void player_shrink(void) {
    if (g_player.is_crashing) return;
    if (g_player.is_jumping) {
        // Fast drop: smash down to road immediately and engage aerodynamic shrink
        g_player.vy = -TO_FP(8.5f);
        g_player.is_shrinking = true;
        g_player.shrink_timer = 50;
        sfx_shrink();
    } else {
        // Aerodynamic nano-shrink + slipstream boost surge
        g_player.is_shrinking = true;
        g_player.shrink_timer = 50; // ~0.83s generous shrink duration
        g_player.shrink_boost_timer = 36; // Slipstream speed boost
        sfx_shrink();
    }
}

void player_update(void) {
    if (g_player.is_crashing) {
        g_player.crash_frame++;

        // Vertical hop and dampening bounce
        if (g_player.crash_frame < 8) {
            g_player.y_offset += TO_FP(1.0f);
        } else if (g_player.y_offset > 0) {
            g_player.y_offset -= TO_FP(1.0f);
            if (g_player.y_offset < 0) g_player.y_offset = 0;
        }

        // Spawn skid sparks while spinning
        if ((g_player.crash_frame & 3) == 0 && g_player.crash_frame < 16) {
            spawn_debris(FP_TO_INT(g_player.x), PLAYER_BASE_Y - 4, 2);
        }

        // Once 20-frame wipeout sequence finishes, settle into STATE_GAMEOVER!
        if (g_player.crash_frame >= 20 && g_game.state != STATE_GAMEOVER) {
            g_game.state = STATE_GAMEOVER;
            game_trigger_shake(4, 12); // Shuddering crash impact shake
            g_game.streak_multiplier = 1;
            g_game.streak_timer = 0;
            g_game.streak_count = 0;
            if (g_game.score > g_game.high_score) {
                g_game.high_score = g_game.score;
                g_game.is_new_high_score = true;
                sram_save_high_score(g_game.high_score);
                if (!g_game.high_score_fanfare_played) {
                    g_game.high_score_fanfare_played = true;
                    sfx_high_score();
                }
            }
            sram_save_total_coins(g_game.total_coins);
        }
        return;
    }

    // Smooth horizontal lane shift interpolation (snappy 4-frame response)
    if (g_player.spinout_timer == 0) {
        int dx = g_player.target_x - g_player.x;
        g_player.x += dx / 4;
        if (abs(dx) < TO_FP(1.0f)) {
            g_player.x = g_player.target_x;
            g_player.tilt = 0;
        }
    }

    // Jump physics
    if (g_player.is_jumping) {
        g_player.y_offset += g_player.vy;
        g_player.vy -= TO_FP(0.34f); // Crisp arcade gravity
        if (g_player.y_offset <= 0) {
            g_player.y_offset = 0;
            g_player.vy = 0;
            g_player.is_jumping = false;
            g_player.landing_squash = 2; // 2-frame suspension squash on touchdown!
            game_trigger_shake(2, 3);    // Tactile vertical touchdown shake!
            sfx_touchdown(); // suspension compression thud
        }
    }

    // Shrink duration
    if (g_player.is_shrinking) {
        g_player.shrink_timer--;
        if (g_player.shrink_timer <= 0) {
            g_player.is_shrinking = false;
        }
    }

    // Feedback and boost timers
    if (g_player.landing_squash > 0) g_player.landing_squash--;
    if (g_player.close_call_timer > 0) g_player.close_call_timer--;
    if (g_player.shrink_boost_timer > 0) g_player.shrink_boost_timer--;
    if (g_player.hurdle_timer > 0) g_player.hurdle_timer--;
    if (g_player.stunt_timer > 0) g_player.stunt_timer--;
    if (g_player.spinout_timer > 0) {
        g_player.spinout_timer--;
        // Smooth uncontrolled lateral slide towards target lane (48px over 24 frames)
        int dx = g_player.target_x - g_player.x;
        int step = TO_FP(2);
        if (abs(dx) <= step) {
            g_player.x = g_player.target_x;
            if (g_player.spinout_timer == 0) {
                g_player.tilt = 0;
            }
        } else {
            g_player.x += (dx > 0) ? step : -step;
        }

        // Heavy screeching smoke and sparks from rear tires
        if ((g_player.spinout_timer & 1) == 0) {
            spawn_debris(FP_TO_INT(g_player.x) - 10, PLAYER_BASE_Y + 4, 1);
            spawn_debris(FP_TO_INT(g_player.x) + 10, PLAYER_BASE_Y + 4, 1);
        }
        // Screeching tire sound
        if ((g_player.spinout_timer % 10) == 0) {
            sfx_skid();
        }
    }

    // Power-up timers
    if (g_player.nitro_timer > 0) {
        g_player.nitro_timer--;
        if ((g_player.nitro_timer & 7) == 0) sfx_nitro();
    }
    if (g_player.magnet_timer > 0) {
        g_player.magnet_timer--;
    }
    if (g_player.invuln_timer > 0) {
        g_player.invuln_timer--;
    }
}

static int get_free_obstacle_slot(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!g_obstacles[i].active) return i;
    }
    return -1;
}

static void init_obstacle(Obstacle *obs, ObstacleType type, int lane, int z_fp) {
    memset(obs, 0, sizeof(Obstacle));
    obs->active = true;
    obs->type = type;
    obs->lane = lane;
    obs->target_lane = lane;
    obs->z = z_fp;
    obs->speed_fp = 0;
    if (type == OBS_SIGNATURE && g_game.map_theme == MAP_CITY) {
        // Heavy Chemical Tanker spans 2 adjacent lanes!
        // If lane == 2, pair is 1 (lanes 1 & 2); otherwise pair is 0 (lanes 0 & 1)
        int l0 = (lane == 2) ? 1 : 0;
        int l1 = l0 + 1;
        obs->lane = l0;
        obs->target_lane = l1;
        int mid_x = (LANE_X_PLAYER[l0] + LANE_X_PLAYER[l1]) / 2;
        obs->x_fp = TO_FP(mid_x);
        obs->screen_x = mid_x;
    } else {
        obs->x_fp = TO_FP(LANE_X_PLAYER[lane]);
        obs->screen_x = LANE_X_PLAYER[lane];
    }
    obs->screen_y = 0;
    obs->scale = 3;
    obs->lane_shift_timer = 0;
    obs->turn_signal_timer = 0;
    obs->passed = false;
    obs->close_call_awarded = false;
    obs->hurdled = false;
}

// Choreographed Wave Director:
// Guarantees at least 1 open escape lane and spawns curated wave patterns
void spawn_obstacle_wave(void) {
    // Check which lanes currently have obstacles approaching ahead of player (Z > 20)
    int blocked_mask = 0;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (g_obstacles[i].active && FP_TO_INT(g_obstacles[i].z) > 20) {
            blocked_mask |= (1 << g_obstacles[i].lane);
            blocked_mask |= (1 << g_obstacles[i].target_lane);
        }
    }

    g_game.wave_pattern = (g_game.wave_pattern + 1) % 7;

    if (g_game.wave_pattern == 0) {
        // Pattern 0: Single Hurdle + Gold Coin Jump Arc
        int lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_LOW_BARRIER, lane, TO_FP(96.0f));
                spawn_coin_group(lane, 1); // Arc coins right over roadblock!
            }
        }
    } else if (g_game.wave_pattern == 1) {
        // Pattern 1: Overhead Gantry Slide Challenge
        int lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_HIGH_SIGN, lane, TO_FP(96.0f));
                spawn_coin_group(lane, 0); // Ground coins under gantry
            }
        }
    } else if (g_game.wave_pattern == 2) {
        // Pattern 2: Heavy Semi-Truck Oncoming Traffic with guaranteed escape lane
        int truck_lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << truck_lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_TRUCK, truck_lane, TO_FP(96.0f));

                // Pick guaranteed safe escape lane
                int safe_lane = (truck_lane + 1 + (rand() % 2)) % NUM_LANES;
                int second_blocked = 3 - truck_lane - safe_lane;
                // Only block 2nd lane if BOTH safe_lane and second_blocked are completely clear
                if ((blocked_mask & (1 << safe_lane)) == 0 &&
                    (blocked_mask & (1 << second_blocked)) == 0 &&
                    (rand() % 2 == 0)) {
                    int slot2 = get_free_obstacle_slot();
                    if (slot2 >= 0) {
                        init_obstacle(&g_obstacles[slot2], (rand() % 2 == 0) ? OBS_LOW_BARRIER : OBS_HIGH_SIGN, second_blocked, TO_FP(96.0f));
                    }
                }
            }
        }
    } else if (g_game.wave_pattern == 3) {
        // Pattern 3: Moving Civilian Traffic with Active Lane-Change AI
        int car_lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << car_lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_CIVILIAN_CAR, car_lane, TO_FP(96.0f));
                g_obstacles[slot].speed_fp = TO_FP(0.12f);

                // 40% chance of lane-shift maneuver
                if ((rand() % 10) < 4) {
                    int shift_dir = (car_lane == 0) ? 1 : (car_lane == 2 ? -1 : ((rand() & 1) ? 1 : -1));
                    g_obstacles[slot].target_lane = car_lane + shift_dir;
                    g_obstacles[slot].turn_signal_timer = 50; // Blink turn signal for 50 frames before shifting
                }
            }
        }
    } else if (g_game.wave_pattern == 4) {
        // Pattern 4: Stunt Launch Ramp + Elevated High Coin Arc
        int ramp_lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << ramp_lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_RAMP, ramp_lane, TO_FP(96.0f));
                spawn_coin_group(ramp_lane, 2); // Super-jump elevated coin arc!
            }
        }
    } else if (g_game.wave_pattern == 5) {
        // Pattern 5: Surface Slick Friction Trap
        int slick_lane = rand() % NUM_LANES;
        if ((blocked_mask & (1 << slick_lane)) == 0) {
            int slot = get_free_obstacle_slot();
            if (slot >= 0) {
                init_obstacle(&g_obstacles[slot], OBS_SLICK, slick_lane, TO_FP(96.0f));
            }
        }
    } else {
        // Pattern 6: Track-Specific Signature Hazard
        if (g_game.map_theme == MAP_CITY) {
            // Heavy Industrial Chemical Tanker blocks 2 adjacent lanes:
            // pair 0 = lanes 0 & 1 (safe escape lane = 2)
            // pair 1 = lanes 1 & 2 (safe escape lane = 0)
            int pair = rand() % 2;
            int l0 = pair;
            int l1 = pair + 1;
            if ((blocked_mask & ((1 << l0) | (1 << l1))) == 0) {
                int slot = get_free_obstacle_slot();
                if (slot >= 0) {
                    init_obstacle(&g_obstacles[slot], OBS_SIGNATURE, l0, TO_FP(96.0f));
                    g_obstacles[slot].target_lane = l1;
                    int mid_x = (LANE_X_PLAYER[l0] + LANE_X_PLAYER[l1]) / 2;
                    g_obstacles[slot].x_fp = TO_FP(mid_x);
                    g_obstacles[slot].screen_x = mid_x;
                }
            }
        } else if (g_game.map_theme == MAP_GOTHIC) {
            // Gothic Midnight: 3 Small Stone Gargoyles in every lane (lanes 0, 1, 2)
            // Full-width road hazard row that player must jump (UP) over!
            for (int l = 0; l < NUM_LANES; l++) {
                int slot = get_free_obstacle_slot();
                if (slot >= 0) {
                    init_obstacle(&g_obstacles[slot], OBS_SIGNATURE, l, TO_FP(96.0f));
                }
            }
        } else {
            int sig_lane = rand() % NUM_LANES;
            if ((blocked_mask & (1 << sig_lane)) == 0) {
                int slot = get_free_obstacle_slot();
                if (slot >= 0) {
                    init_obstacle(&g_obstacles[slot], OBS_SIGNATURE, sig_lane, TO_FP(96.0f));
                }
            }
        }
    }
}

void spawn_obstacle(void) {
    spawn_obstacle_wave();
}

void obstacles_update(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!g_obstacles[i].active) continue;

        // Move obstacle toward player based on speed:
        // Oncoming semi-trucks drive actively toward the camera at 1.4x relative speed!
        // Moving civilian cars cruise forward at speed (approached at player_speed - speed_fp)
        // Once an obstacle is hurdled or passed under, it whooshes rapidly underneath and behind at 2.5x speed!
        int speed = g_game.speed;
        if (g_obstacles[i].type == OBS_TRUCK) {
            speed = (g_game.speed * 7) / 5;
        } else if (g_obstacles[i].type == OBS_CIVILIAN_CAR) {
            speed = (g_game.speed * 3) / 5; // Cruising forward: slower approach (overtake)
        } else if (g_obstacles[i].hurdled || g_obstacles[i].passed) {
            speed = (g_game.speed * 5) / 2;
        } else if (g_obstacles[i].type == OBS_SIGNATURE) {
            if (g_game.map_theme == MAP_ORBITAL) {
                // Active laser barrier cycles between ON (0) and OFF (1) in normal gameplay
                if (g_game.spawn_timer < 9000) {
                    g_obstacles[i].hazard_state = ((g_game.frame_count / 40) & 1);
                }
            } else if (g_game.map_theme == MAP_WINTER) {
                // Winter Snow: Rolling glacial ice boulder rolls down toward camera at high velocity
                speed = (g_game.speed * 8) / 5;
            } else if (g_game.map_theme == MAP_BEACH) {
                // Tumbling windsurf rig shifts across lanes in wind
                if (FP_TO_INT(g_obstacles[i].z) > 30 && (g_game.frame_count & 31) == 0 && g_obstacles[i].target_lane == g_obstacles[i].lane) {
                    int shift = (g_obstacles[i].lane == 0) ? 1 : ((g_obstacles[i].lane == 2) ? -1 : ((rand() & 1) ? 1 : -1));
                    g_obstacles[i].target_lane = g_obstacles[i].lane + shift;
                    g_obstacles[i].lane_shift_timer = 24;
                }
            }
        }
        g_obstacles[i].z -= speed;

        // Lane Shifting AI for traffic and windsurf rig:
        if ((g_obstacles[i].type == OBS_CIVILIAN_CAR || g_obstacles[i].type == OBS_TRUCK || g_obstacles[i].type == OBS_SIGNATURE) &&
            !(g_obstacles[i].type == OBS_SIGNATURE && g_game.map_theme == MAP_CITY)) {
            if (g_obstacles[i].turn_signal_timer > 0) {
                g_obstacles[i].turn_signal_timer--;
                if (g_obstacles[i].turn_signal_timer == 0 && g_obstacles[i].target_lane != g_obstacles[i].lane) {
                    g_obstacles[i].lane_shift_timer = 20; // 20-frame lane transition
                }
            }
            if (g_obstacles[i].lane_shift_timer > 0) {
                g_obstacles[i].lane_shift_timer--;
                int target_x_fp = TO_FP(LANE_X_PLAYER[g_obstacles[i].target_lane]);
                int cur_x_fp = (g_obstacles[i].x_fp != 0) ? g_obstacles[i].x_fp : TO_FP(LANE_X_PLAYER[g_obstacles[i].lane]);
                int diff = target_x_fp - cur_x_fp;
                g_obstacles[i].x_fp = cur_x_fp + diff / 4;
                if (g_obstacles[i].lane_shift_timer == 0 || abs(diff) < TO_FP(1.0f)) {
                    g_obstacles[i].lane = g_obstacles[i].target_lane;
                    g_obstacles[i].x_fp = target_x_fp;
                }
            }
        }

        // Calculate screen projection (all obstacles rest firmly on road surface)
        int sx, sy, sc;
        project_to_screen(g_obstacles[i].lane, g_obstacles[i].z, 0, &sx, &sy, &sc);
        if (g_obstacles[i].type == OBS_SIGNATURE && g_game.map_theme == MAP_CITY) {
            // Ensure 2-lane chemical tanker has correct 2-lane span and midpoint X
            if (g_obstacles[i].lane == g_obstacles[i].target_lane) {
                if (g_obstacles[i].lane == 0) {
                    g_obstacles[i].target_lane = 1;
                } else if (g_obstacles[i].lane == 2) {
                    g_obstacles[i].lane = 1;
                    g_obstacles[i].target_lane = 2;
                } else { // lane 1 defaults to lanes 0 & 1
                    g_obstacles[i].lane = 0;
                    g_obstacles[i].target_lane = 1;
                }
            }
            int mid_x = (LANE_X_PLAYER[g_obstacles[i].lane] + LANE_X_PLAYER[g_obstacles[i].target_lane]) / 2;
            g_obstacles[i].x_fp = TO_FP(mid_x);
            sx = mid_x;
        } else if (g_obstacles[i].x_fp != 0) {
            sx = FP_TO_INT(g_obstacles[i].x_fp);
        }
        g_obstacles[i].screen_x = sx;
        g_obstacles[i].screen_y = sy;
        g_obstacles[i].scale = sc;

        // Past camera cleanup: only deactivate when smoothly rolled off-screen bottom!
        if (g_obstacles[i].screen_y >= 185 || FP_TO_INT(g_obstacles[i].z) <= -20) {
            memset(&g_obstacles[i], 0, sizeof(Obstacle));
            g_obstacles[i].active = false;
        }
    }
}

void spawn_coin_group(int lane, int pattern) {
    // Pattern 0: Straight 4-coin line
    // Pattern 1: Aerial jump arc
    // Pattern 2: High stunt super-jump arc
    for (int step = 0; step < 4; step++) {
        for (int i = 0; i < MAX_COINS; i++) {
            if (!g_coins[i].active) {
                g_coins[i].active = true;
                g_coins[i].lane = lane;
                g_coins[i].z = TO_FP(94.0f + step * 8.0f);
                if (pattern == 2) {
                    g_coins[i].y_offset = (step == 1 || step == 2) ? 26 : 14;
                } else if (pattern == 1) {
                    g_coins[i].y_offset = (step == 1 || step == 2) ? 18 : 8;
                } else {
                    g_coins[i].y_offset = 0;
                }
                g_coins[i].anim_frame = (step * 2) % 4;
                g_coins[i].collected = false;
                g_coins[i].x_fp = 0;
                break;
            }
        }
    }
}

void coins_update(void) {
    bool magnet = (g_player.magnet_timer > 0);
    int player_x = FP_TO_INT(g_player.x);
    int player_y = FP_TO_INT(g_player.y_offset);

    for (int i = 0; i < MAX_COINS; i++) {
        if (!g_coins[i].active) continue;

        // Advance towards player
        g_coins[i].z -= g_game.speed;
        int z_int = FP_TO_INT(g_coins[i].z);

        // Animate spin
        if ((g_game.frame_count & 7) == 0) {
            g_coins[i].anim_frame = (g_coins[i].anim_frame + 1) % 4;
        }

        // Standard perspective coordinates
        int sx, sy, sc;
        project_to_screen(g_coins[i].lane, g_coins[i].z, g_coins[i].y_offset, &sx, &sy, &sc);

        int lane_world_x = TO_FP(LANE_X_PLAYER[g_coins[i].lane]);
        if (g_coins[i].x_fp == 0) {
            g_coins[i].x_fp = lane_world_x;
        }

        // Smooth Vector Magnet Attraction in unified world coordinates
        if (magnet && z_int <= 55) {
            int dx_fp = g_player.x - g_coins[i].x_fp;
            int pull_speed = TO_FP(3.5f) + (55 - z_int) * 24; // Accelerates toward hood!
            if (abs(dx_fp) <= pull_speed) {
                g_coins[i].x_fp = g_player.x;
            } else if (dx_fp > 0) {
                g_coins[i].x_fp += pull_speed;
            } else {
                g_coins[i].x_fp -= pull_speed;
            }
            int offset_px = FP_TO_INT(g_coins[i].x_fp - lane_world_x);
            g_coins[i].screen_x = sx + offset_px;
        } else {
            g_coins[i].x_fp = lane_world_x;
            g_coins[i].screen_x = sx;
        }
        g_coins[i].screen_y = sy;

        // Coin pickup detection: covers full 32px vehicle body length (Z=8..24) and generous lateral contact
        if (z_int >= 8 && z_int <= 24) {
            int dx = abs(player_x - g_coins[i].screen_x);
            int dy = abs(player_y - g_coins[i].y_offset);
            bool is_moving_lateral = (g_player.tilt != 0 || g_player.x != g_player.target_x);
            int pickup_rad = magnet ? 36 : (is_moving_lateral ? 26 : 24);
            int pickup_dy = magnet ? 36 : 22;

            if (dx < pickup_rad && dy < pickup_dy) {
                g_coins[i].active = false;
                g_game.coins_collected++;
                if (g_game.coins_collected > 99999) g_game.coins_collected = 99999;
                g_game.total_coins++;
                if (g_game.total_coins > 99999) g_game.total_coins = 99999;
                sram_save_total_coins(g_game.total_coins);
                add_score(50);
                sfx_coin();
                continue;
            }
        }

        // Clean up past camera smoothly once off-screen
        if (g_coins[i].screen_y >= 185 || z_int <= -20) {
            g_coins[i].active = false;
        }
    }
}

void spawn_powerup(void) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!g_world_powerups[i].active) {
            g_world_powerups[i].active = true;
            int r = rand() % 3;
            g_world_powerups[i].type = (r == 0) ? POW_NITRO : ((r == 1) ? POW_MAGNET : POW_SHIELD);
            g_world_powerups[i].lane = rand() % NUM_LANES;
            g_world_powerups[i].z = TO_FP(95.0f);
            break;
        }
    }
}

void powerups_update(void) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!g_world_powerups[i].active) continue;
        g_world_powerups[i].z -= g_game.speed;

        int sx, sy, sc;
        project_to_screen(g_world_powerups[i].lane, g_world_powerups[i].z, 4, &sx, &sy, &sc);
        g_world_powerups[i].screen_x = sx;
        g_world_powerups[i].screen_y = sy;

        int z_int = FP_TO_INT(g_world_powerups[i].z);
        int p_world_x = LANE_X_PLAYER[g_world_powerups[i].lane];
        int dx = abs(FP_TO_INT(g_player.x) - p_world_x);
        bool is_moving_lateral = (g_player.tilt != 0 || g_player.x != g_player.target_x);
        int pickup_rad = is_moving_lateral ? 26 : 24;
        if (z_int >= 8 && z_int <= 24 && dx < pickup_rad) {
            // Picked up!
            PowerupType t = g_world_powerups[i].type;
            if (t == POW_NITRO) {
                g_player.nitro_timer = 240; // 4 seconds of nitro
                g_game.flash_timer = 6;
                sfx_nitro();
            } else if (t == POW_MAGNET) {
                g_player.magnet_timer = 360; // 6 seconds magnet
                sfx_powerup();
            } else if (t == POW_SHIELD) {
                g_player.shield_active = 1;
                sfx_shield_activate();
            }
            add_score(200);
            g_world_powerups[i].active = false;
            continue;
        }

        if (g_world_powerups[i].screen_y >= 185 || z_int <= -20) g_world_powerups[i].active = false;
    }
}

void check_collisions(void) {
    if (g_player.is_crashing || g_player.invuln_timer > 0) return;

    int player_x_int = FP_TO_INT(g_player.x);

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!g_obstacles[i].active || g_obstacles[i].passed) continue;

        int z_int = FP_TO_INT(g_obstacles[i].z);
        // If the obstacle is already behind the player base plane (Z < 12), mark passed!
        if (z_int < 12) {
            g_obstacles[i].passed = true;
            continue;
        }

        // Spatial early exit: skip any obstacle outside the collision threat zone (12..21)
        if (z_int > 21) continue;

        // Collision window near player base plane (Z in 12..18)
        if (z_int >= 12 && z_int <= 18) {
            // Pure world-space lateral distance: guarantees 100% sync with on-screen sprite!
            int obs_world_x = (g_obstacles[i].x_fp != 0) ? g_obstacles[i].x_fp : TO_FP(LANE_X_PLAYER[g_obstacles[i].lane]);
            int dx = FP_TO_INT(abs(g_player.x - obs_world_x));

            // Dynamic Dodge Hitbox:
            // If shrunk to nano-chassis, actively steering, transitioning lanes, or in another lane, give graceful forgiveness!
            bool is_evading = (g_player.tilt != 0 || g_player.x != g_player.target_x || (g_player.lane != g_obstacles[i].lane && g_player.lane != g_obstacles[i].target_lane));
            int hit_width = 18;
            if (g_obstacles[i].type == OBS_SIGNATURE && g_game.map_theme == MAP_CITY) {
                hit_width = 30; // Tanker spans 2 lanes; center-to-midpoint distance is 24px
            } else if (g_player.is_shrinking) {
                hit_width = 10; // Nano shrunk car is compact and agile
            } else if (g_obstacles[i].type == OBS_HIGH_SIGN) {
                hit_width = 13;
            } else if (is_evading) {
                hit_width = 12; // Evasion grazing forgiveness
            }

            if (dx < hit_width) {
                bool hit = false;
                if (g_obstacles[i].type == OBS_LOW_BARRIER) {
                    // Jump clearance: elevation >= 8px OR actively ascending in jump impulse
                    if (FP_TO_INT(g_player.y_offset) < 8 && !(g_player.is_jumping && g_player.vy > TO_FP(2.0f))) {
                        hit = true;
                    } else if (!g_obstacles[i].hurdled) {
                        // Successfully cleared the roadblock hurdle!
                        g_obstacles[i].hurdled = true;
                        g_obstacles[i].passed = true; // Mark passed so landing later inside window doesn't crash!
                        g_player.hurdle_timer = 28; // ~0.45s visual feedback
                        add_score(150);
                        streak_add_action();
                        sfx_hurdle();
                        spawn_clearance_feedback(g_obstacles[i].screen_x, PLAYER_BASE_Y + 12, DEBRIS_POPUP_150);
                    }
                } else if (g_obstacles[i].type == OBS_HIGH_SIGN) {
                    // Overhead gantry: must shrink to slide under!
                    if (g_player.is_shrinking) {
                        // Crouch/shrink under gantry earns slipstream reward
                        if (!g_obstacles[i].passed) {
                            add_score(150);
                            streak_add_action();
                            sfx_hurdle();
                            spawn_clearance_feedback(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 4, DEBRIS_POPUP_150);
                        }
                        g_obstacles[i].passed = true;
                    } else {
                        // Normal height (or jumping) hits overhead gantry!
                        hit = true;
                    }
                } else if (g_obstacles[i].type == OBS_CIVILIAN_CAR) {
                    // Slower cruising car: can jump over if high enough, otherwise must dodge!
                    if (FP_TO_INT(g_player.y_offset) >= 12) {
                        if (!g_obstacles[i].hurdled) {
                            g_obstacles[i].hurdled = true;
                            g_obstacles[i].passed = true;
                            g_player.hurdle_timer = 28;
                            add_score(200);
                            streak_add_action();
                            sfx_hurdle();
                            spawn_clearance_feedback(g_obstacles[i].screen_x, PLAYER_BASE_Y + 12, DEBRIS_POPUP_150);
                        }
                    } else {
                        hit = true;
                    }
                } else if (g_obstacles[i].type == OBS_TRUCK) {
                    // Semi-truck cannot be jumped or shrunk under! Must dodge lane
                    hit = true;
                } else if (g_obstacles[i].type == OBS_RAMP) {
                    // Mechanic-Expanding Stunt Launch Ramp: Launches player into a thrilling Super-Jump!
                    g_player.is_jumping = true;
                    g_player.vy = TO_FP(6.2f); // Generous super-jump impulse (apex ~55px)
                    g_player.is_shrinking = false;
                    g_player.shrink_timer = 0;
                    g_player.stunt_timer = 30;
                    g_obstacles[i].passed = true;
                    g_obstacles[i].hurdled = true;
                    add_score(200);
                    streak_add_action();
                    game_trigger_shake(2, 4);
                    sfx_stunt_ramp();
                    spawn_clearance_feedback(g_obstacles[i].screen_x, PLAYER_BASE_Y - 8, DEBRIS_POPUP_STUNT);
                } else if (g_obstacles[i].type == OBS_SLICK) {
                    // Mechanic-Expanding Surface Slick:
                    if (FP_TO_INT(g_player.y_offset) >= 4) {
                        // Airborne jump sails cleanly over the slick!
                        g_obstacles[i].passed = true;
                        g_obstacles[i].hurdled = true;
                    } else if (g_player.nitro_timer > 0) {
                        // Nitro blazes through friction loss
                        g_obstacles[i].passed = true;
                        spawn_debris(g_obstacles[i].screen_x, g_obstacles[i].screen_y, 4);
                    } else {
                        // Ground contact triggers lateral slide to adjacent lane randomly!
                        int slide_dir;
                        if (g_player.lane <= LANE_LEFT) {
                            slide_dir = 1;
                        } else if (g_player.lane >= LANE_RIGHT) {
                            slide_dir = -1;
                        } else {
                            slide_dir = (rand() & 1) ? 1 : -1;
                        }
                        int next_lane = g_player.lane + slide_dir;
                        g_player.lane = next_lane;
                        g_player.target_x = TO_FP(LANE_X_PLAYER[next_lane]);
                        g_player.tilt = slide_dir;
                        g_player.spinout_timer = 24;
                        g_obstacles[i].passed = true;
                        g_game.streak_multiplier = 1;
                        g_game.streak_count = 0;
                        g_game.streak_timer = 0;
                        sfx_skid();
                        game_trigger_shake(4, 12);
                        for (int s = 0; s < 10; s++) {
                            spawn_debris(player_x_int + ((rand() % 24) - 12), PLAYER_BASE_Y + 4, 1);
                        }
                    }
                } else if (g_obstacles[i].type == OBS_SIGNATURE) {
                    if (g_game.map_theme == MAP_ORBITAL) {
                        // Cape Orbital: Pulsing plasma laser (hazard_state == 0 is active lethal laser; 1 is charging/safe window)
                        if (g_obstacles[i].hazard_state == 0) {
                            hit = true;
                        } else {
                            if (!g_obstacles[i].passed) {
                                add_score(150);
                                streak_add_action();
                                sfx_hurdle();
                                spawn_clearance_feedback(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 4, DEBRIS_POPUP_150);
                            }
                            g_obstacles[i].passed = true;
                        }
                    } else if (g_game.map_theme == MAP_GOTHIC) {
                        // Gothic Midnight: 3 Small Stone Gargoyles in every lane (jump over them to clear)
                        if (FP_TO_INT(g_player.y_offset) >= 6 || (g_player.is_jumping && g_player.vy > TO_FP(1.0f))) {
                            if (!g_obstacles[i].hurdled) {
                                g_obstacles[i].hurdled = true;
                                g_obstacles[i].passed = true;
                                g_player.hurdle_timer = 28;
                                add_score(150);
                                streak_add_action();
                                sfx_hurdle();
                                spawn_clearance_feedback(g_obstacles[i].screen_x, PLAYER_BASE_Y + 12, DEBRIS_POPUP_150);
                            }
                        } else {
                            hit = true;
                        }
                    } else if (g_game.map_theme == MAP_BEACH) {
                        // Palm Beach: Tumbling windsurf rig with overhead boom/sail (duck under or dodge)
                        if (g_player.is_shrinking) {
                            if (!g_obstacles[i].passed) {
                                add_score(150);
                                streak_add_action();
                                sfx_hurdle();
                                spawn_clearance_feedback(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 4, DEBRIS_POPUP_150);
                            }
                            g_obstacles[i].passed = true;
                        } else {
                            hit = true;
                        }
                    } else if (g_game.map_theme == MAP_MAYA) {
                        // Maya Temple Trail: Ancient Poison Dart Trap Volley (duck/shrink under low dart stream)
                        if (g_player.is_shrinking) {
                            if (!g_obstacles[i].passed) {
                                add_score(150);
                                streak_add_action();
                                sfx_hurdle();
                                spawn_clearance_feedback(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 4, DEBRIS_POPUP_150);
                            }
                            g_obstacles[i].passed = true;
                        } else {
                            hit = true;
                        }
                    } else if (g_game.map_theme == MAP_WINTER) {
                        // Winter Snow: Glacial Ice Boulder (jump over if airborne >= 12px or ascending)
                        if (FP_TO_INT(g_player.y_offset) >= 12 || (g_player.is_jumping && g_player.vy > TO_FP(2.0f))) {
                            if (!g_obstacles[i].hurdled) {
                                g_obstacles[i].hurdled = true;
                                g_obstacles[i].passed = true;
                                add_score(200);
                                streak_add_action();
                                sfx_hurdle();
                                spawn_clearance_feedback(g_obstacles[i].screen_x, PLAYER_BASE_Y + 12, DEBRIS_POPUP_150);
                            }
                        } else {
                            hit = true;
                        }
                    } else { // MAP_CITY: Heavy Chemical Tanker
                        hit = true;
                    }
                }

                if (hit) {
                    if (g_player.nitro_timer > 0) {
                        // Nitro smashes right through obstacle with flying debris!
                        g_obstacles[i].active = false;
                        add_score(300);
                        streak_add_action();
                        game_trigger_shake(3, 6); // Nitro smash screen shake
                        spawn_debris(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 8, 8);
                        sfx_crash();
                    } else if (g_player.shield_active) {
                        // Shield absorbs blow
                        g_player.shield_active = 0;
                        g_player.invuln_timer = 60; // 1 second blink
                        g_obstacles[i].passed = true;
                        g_game.streak_multiplier = 1; // Shield hit resets streak
                        g_game.streak_count = 0;
                        g_game.streak_timer = 0;
                        game_trigger_shake(2, 4);
                        spawn_shield_discharge(player_x_int - 8, PLAYER_BASE_Y - 8);
                        spawn_debris(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 8, 8);
                        sfx_shield_deflect();
                    } else {
                        // CRASH! Enter 20-frame arcade wipeout spin sequence
                        g_player.is_crashing = true;
                        g_player.crash_frame = 0;
                        g_game.streak_multiplier = 1;
                        g_game.streak_count = 0;
                        g_game.streak_timer = 0;
                        game_trigger_shake(4, 12);
                        g_game.flash_timer = 8;
                        spawn_debris(player_x_int, PLAYER_BASE_Y - 8, 8);
                        sfx_crash();
                        return;
                    }
                }
            } else {
                if (dx <= 24 && !g_obstacles[i].close_call_awarded) {
                    // CLOSE CALL! Grazed past obstacle at close proximity!
                    g_obstacles[i].close_call_awarded = true;
                    g_player.close_call_timer = 30;
                    add_score(250);
                    streak_add_action();
                    g_game.close_call_count++;

                    // Spawn lateral friction sparks along contact flank
                    int spark_x = (g_obstacles[i].screen_x < player_x_int) ? (player_x_int - 8) : (player_x_int + 18);
                    for (int s = 0; s < 4; s++) {
                        int sx = spark_x + ((rand() % 5) - 2);
                        int sy = PLAYER_BASE_Y - 4 + ((rand() % 5) - 2);
                        spawn_debris(sx, sy, 1);
                    }
                    sfx_scrape(); // Metal-on-guardrail friction screech
                    sfx_close_call();
                }

                if (!g_obstacles[i].passed && z_int <= 14) {
                    // Safely dodged obstacle in another lane or passed alongside!
                    g_obstacles[i].passed = true;
                    if (g_obstacles[i].type == OBS_TRUCK || g_obstacles[i].type == OBS_CIVILIAN_CAR) {
                        add_score(100);
                        streak_add_action();
                        sfx_coin();
                        spawn_clearance_feedback(g_obstacles[i].screen_x, g_obstacles[i].screen_y - 6, DEBRIS_POPUP_100);
                    }
                }
            }
        }
    }
}

void game_update(void) {
    g_game.frame_count++;

    // Shake and Flash decay
    if (g_game.screen_shake > 0) g_game.screen_shake--;
    if (g_game.flash_timer > 0) g_game.flash_timer--;

    debris_update();

    if (g_game.state == STATE_START) {
        if (key_hit(KEY_B)) {
            s_start_b_presses++;
            if (s_start_b_presses == 20) {
                g_game.total_coins = 99999;
                sram_save_total_coins(g_game.total_coins);
                sfx_high_score();
            } else if (s_start_b_presses > 20) {
                g_game.total_coins = 99999;
                sram_save_total_coins(g_game.total_coins);
                sfx_coin();
            } else {
                sfx_coin();
            }
        }
        if (key_hit(KEY_START) || key_hit(KEY_A)) {
            s_start_b_presses = 0;
            render_clear_title_boxes();
            memset16(se_mem[31], 0, 32 * 32);
            g_game.state = STATE_TITLE;
            sfx_nitro();
        }
        return;
    }

    if (g_game.state == STATE_TITLE) {
        if (key_hit(KEY_LEFT) || key_hit(KEY_L)) {
            g_game.car_color = (CarColor)((g_game.car_color + CAR_COLOR_COUNT - 1) % CAR_COLOR_COUNT);
            gfx_set_car_color(g_game.car_color);
            sfx_coin();
        } else if (key_hit(KEY_RIGHT) || key_hit(KEY_R)) {
            g_game.car_color = (CarColor)((g_game.car_color + 1) % CAR_COLOR_COUNT);
            gfx_set_car_color(g_game.car_color);
            sfx_coin();
        } else if (key_hit(KEY_UP) || key_hit(KEY_DOWN)) {
            g_game.car_color = (CarColor)((g_game.car_color + 3) % CAR_COLOR_COUNT);
            gfx_set_car_color(g_game.car_color);
            sfx_coin();
        } else if (key_hit(KEY_B)) {
            render_clear_title_boxes();
            memset16(se_mem[31], 0, 32 * 32);
            g_game.state = STATE_START;
            s_start_b_presses = 0;
            sfx_pause();
            return;
        }

        if (key_hit(KEY_START) || key_hit(KEY_A)) {
            bool is_unlocked = (g_game.cars_unlocked & (1 << g_game.car_color)) != 0;
            if (is_unlocked) {
                render_clear_title_boxes();
                memset16(se_mem[31], 0, 32 * 32);
                g_game.state = STATE_SELECT_MAP;
                sfx_coin();
            } else {
                if (g_game.total_coins >= CAR_UNLOCK_COST) {
                    g_game.total_coins -= CAR_UNLOCK_COST;
                    g_game.cars_unlocked |= (1 << g_game.car_color);
                    sram_save_total_coins(g_game.total_coins);
                    sram_save_unlocked_cars(g_game.cars_unlocked);
                    sfx_high_score();
                } else {
                    sfx_skid();
                }
            }
        }
        return;
    }

    if (g_game.state == STATE_SELECT_MAP) {
        if (key_hit(KEY_LEFT) || key_hit(KEY_L)) {
            g_game.map_theme = (MapTheme)((g_game.map_theme + MAP_COUNT - 1) % MAP_COUNT);
            sfx_coin();
        } else if (key_hit(KEY_RIGHT) || key_hit(KEY_R)) {
            g_game.map_theme = (MapTheme)((g_game.map_theme + 1) % MAP_COUNT);
            sfx_coin();
        } else if (key_hit(KEY_UP) || key_hit(KEY_DOWN)) {
            g_game.map_theme = (MapTheme)((g_game.map_theme + 3) % MAP_COUNT);
            sfx_coin();
        } else if (key_hit(KEY_B)) {
            // Return back to car color selection
            render_clear_title_boxes();
            memset16(se_mem[31], 0, 32 * 32);
            gfx_restore_title_palette();
            g_game.state = STATE_TITLE;
            sfx_pause();
        } else if (key_hit(KEY_START) || key_hit(KEY_A)) {
            bool is_unlocked = (g_game.maps_unlocked & (1 << g_game.map_theme)) != 0;
            if (is_unlocked) {
                game_start();
            } else {
                int cost = g_map_unlock_costs[g_game.map_theme];
                if (g_game.total_coins >= cost) {
                    g_game.total_coins -= cost;
                    g_game.maps_unlocked |= (1 << g_game.map_theme);
                    sram_save_total_coins(g_game.total_coins);
                    sram_save_unlocked_maps(g_game.maps_unlocked);
                    sfx_high_score();
                } else {
                    sfx_skid();
                }
            }
        }
        return;
    }

    if (g_game.state == STATE_GAMEOVER) {
        g_player.crash_frame++;
        if (g_player.crash_frame > 30) {
            if (key_hit(KEY_START) || key_hit(KEY_A)) {
                game_start();
            } else if (key_hit(KEY_B)) {
                render_clear_title_boxes();
                memset16(se_mem[31], 0, 32 * 32);
                CarColor col = g_game.car_color;
                MapTheme map = g_game.map_theme;
                game_init();
                g_game.car_color = col;
                g_game.map_theme = map;
                gfx_set_car_color(col);
                gfx_restore_title_palette();
                bgm_init();
                sfx_pause();
            }
        }
        return;
    }

    if (g_game.state == STATE_PAUSE) {
        if (key_hit(KEY_START)) {
            g_game.state = STATE_PLAY;
            render_clear_pause_text();
            audio_resume();
            sfx_pause();
        } else if (key_hit(KEY_B)) {
            render_clear_pause_text();
            render_clear_title_boxes();
            memset16(se_mem[31], 0, 32 * 32);
            CarColor col = g_game.car_color;
            MapTheme map = g_game.map_theme;
            game_init();
            g_game.car_color = col;
            g_game.map_theme = map;
            gfx_set_car_color(col);
            gfx_restore_title_palette();
            bgm_init();
            sfx_pause();
            return;
        }
        return;
    }

    // STATE_PLAY
    if (g_player.is_crashing) {
        g_game.speed = 0;
        audio_pause(); // Silence engine hum during wipeout crash
        player_update();
        debris_update();
        return;
    }

    if (key_hit(KEY_START)) {
        g_game.state = STATE_PAUSE;
        audio_pause();
        sfx_pause();
        return;
    }

    // Speed control: Nitro grants 2x speed, Shrink boost grants +15% surge (Slick slide retains full speed)
    int current_base_speed = g_game.base_speed;
    if (g_player.nitro_timer > 0) {
        g_game.speed = current_base_speed * 2;
    } else if (g_player.shrink_boost_timer > 0) {
        g_game.speed = current_base_speed + TO_FP(0.12f);
    } else {
        g_game.speed = current_base_speed;
    }

    // Gradually accelerate over time (every ~6 seconds, from 0.42 up to 1.30)
    if ((g_game.frame_count % 360) == 0 && g_game.base_speed < TO_FP(1.30f)) {
        g_game.base_speed += TO_FP(0.025f);
    }

    // Update distance and score (sub-meter fixed-point accumulation)
    g_game.distance_fp += g_game.speed;
    g_game.distance_m = FP_TO_INT(g_game.distance_fp);
    if (g_game.distance_m > 999999) g_game.distance_m = 999999;

    add_score((g_player.nitro_timer > 0) ? 4 : 2);

    // Road scroll animation
    g_game.road_scroll = (g_game.road_scroll + FP_TO_INT(g_game.speed * 4)) & 63;

    // Sound engine hum
    int speed_lvl = FP_TO_INT((g_game.speed - TO_FP(0.35f)) * 4);
    if (speed_lvl < 0) speed_lvl = 0;
    sfx_engine_hum(speed_lvl);

    // Input buffering system
    int hit_key = 0;
    if (key_hit(KEY_LEFT))                          hit_key = KEY_LEFT;
    else if (key_hit(KEY_RIGHT))                     hit_key = KEY_RIGHT;
    else if (key_hit(KEY_UP) || key_hit(KEY_A))    hit_key = KEY_A;
    else if (key_hit(KEY_DOWN) || key_hit(KEY_B))  hit_key = KEY_B;

    if (hit_key) {
        g_player.buffered_key = hit_key;
        g_player.buffer_timer = 6;
    }

    if (g_player.buffer_timer > 0) {
        bool consumed = false;
        if (g_player.spinout_timer > 0) {
            // Steering and maneuvers are locked during severe spinout!
            consumed = true;
        } else if (g_player.buffered_key == KEY_LEFT) {
            player_shift_lane(-1);
            consumed = true;
        } else if (g_player.buffered_key == KEY_RIGHT) {
            player_shift_lane(1);
            consumed = true;
        } else if (g_player.buffered_key == KEY_A) {
            // Shrink-cancel jump or ground jump
            if (!g_player.is_jumping || g_player.is_shrinking) {
                player_jump();
                consumed = true;
            }
        } else if (g_player.buffered_key == KEY_B) {
            player_shrink();
            consumed = true;
        }

        if (consumed) {
            g_player.buffered_key = 0;
            g_player.buffer_timer = 0;
        } else {
            g_player.buffer_timer--;
        }
    }

    player_update();
    obstacles_update();
    coins_update();
    powerups_update();
    check_collisions();

    // Spawners (Choreographed Waves)
    g_game.spawn_timer--;
    if (g_game.spawn_timer <= 0) {
        spawn_obstacle_wave();
        g_game.spawn_timer = 85 + (rand() % 40);
    }

    g_game.powerup_spawn_timer--;
    if (g_game.powerup_spawn_timer <= 0) {
        spawn_powerup();
        g_game.powerup_spawn_timer = 450 + (rand() % 250); // Every ~8-12s
    }

    // Screen shake update
    if (g_game.shake_timer > 0) {
        g_game.shake_timer--;
        int sign = (g_game.shake_timer & 1) ? 1 : -1;
        g_game.shake_offset_x = sign * g_game.shake_amp;
        g_game.shake_offset_y = (g_game.shake_timer & 2) ? 1 : -1;
    } else {
        g_game.shake_offset_x = 0;
        g_game.shake_offset_y = 0;
        g_game.shake_amp = 0;
    }

    // Surf streak combo timer decay
    if (g_game.streak_timer > 0) {
        g_game.streak_timer--;
        if (g_game.streak_timer == 0) {
            g_game.streak_multiplier = 1;
            g_game.streak_count = 0;
        }
    }

    // Environment milestone progression (every 600m) for City Highway
    if (g_game.map_theme == MAP_CITY) {
        EnvTheme target_theme = (g_game.distance_m / 600) % 4;
        if (target_theme != g_game.env_theme && g_game.env_transition == 0) {
            g_game.env_theme = target_theme;
            g_game.env_transition = 30; // 30-frame smooth palette crossfade
        }
        if (g_game.env_transition > 0) {
            g_game.env_transition--;
            gfx_apply_env_theme(g_game.env_theme, 30 - g_game.env_transition);
        }

        // Thunderstorm ambient lightning flash
        if (g_game.env_theme == ENV_STORM) {
            if ((g_game.frame_count % 360) == 180) {
                g_game.env_lightning = 2; // 2-frame lightning flash
            }
            if (g_game.env_lightning > 0) {
                g_game.env_lightning--;
                if (g_game.env_lightning > 0) {
                    // Flash storm horizon to bright white
                    pal_bg_mem[17] = RGB15(31, 31, 31);
                    pal_bg_mem[18] = RGB15(31, 31, 31);
                    pal_bg_mem[20] = RGB15(31, 31, 31);
                } else {
                    gfx_apply_env_theme(ENV_STORM, 0);
                }
            }
        }
    }

    // BGM sequencer update & Nitro overdrive
    bgm_update();
    bgm_set_overdrive(g_player.nitro_timer > 0);
}
