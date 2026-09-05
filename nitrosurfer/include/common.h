#ifndef COMMON_H
#define COMMON_H

#include <tonc.h>

// Screen dimensions
#define SCREEN_W            240
#define SCREEN_H            160

// Top-Down Highway Geometry
#define ROAD_TOP_Y          0       // Top boundary of playable highway (full top view)
#define ROAD_BOTTOM_Y       160     // Bottom edge of screen
#define ROAD_HEIGHT         (ROAD_BOTTOM_Y - ROAD_TOP_Y)

// 3 Parallel Lanes
#define LANE_LEFT           0
#define LANE_CENTER         1
#define LANE_RIGHT          2
#define NUM_LANES           3

#define LANE_X_LEFT         72
#define LANE_X_CENTER       120
#define LANE_X_RIGHT        168

// Player ground position in top-down view (placed cleanly ahead of bottom HUD)
#define PLAYER_BASE_Y       112
#define PLAYER_BASE_Z       16      // Z coordinate where player sits

// Fixed point math (8.8 format)
#define FP_SHIFT            8
#define FP_ONE              (1 << FP_SHIFT)
#define TO_FP(x)            ((int)((x) * FP_ONE))
#define FP_TO_INT(x)        ((x) >> FP_SHIFT)
#define FP_MUL(a, b)        (((a) * (b)) >> FP_SHIFT)
#define FP_DIV(a, b)        (((a) << FP_SHIFT) / (b))

// Game States
typedef enum {
    STATE_START       = 0, // Arcade Start Screen with Logo & Hero Showcase
    STATE_TITLE       = 1, // 6-Car Showroom Grid Selection
    STATE_PLAY        = 2, // In-Game Racing
    STATE_PAUSE       = 3, // Game Paused
    STATE_GAMEOVER    = 4, // Crash / Game Over
    STATE_SELECT_MAP  = 5  // 6-Track Map Grid Selection
} GameStateEnum;

// Map Themes
typedef enum {
    MAP_CITY    = 0,
    MAP_BEACH   = 1,
    MAP_WINTER  = 2,
    MAP_ORBITAL = 3,
    MAP_GOTHIC  = 4,
    MAP_MAYA    = 5,
    MAP_COUNT   = 6
} MapTheme;

// Car Colors for Player Vehicle Selection
typedef enum {
    CAR_COLOR_RED = 0,
    CAR_COLOR_BLUE,
    CAR_COLOR_PINK,
    CAR_COLOR_BLACK,
    CAR_COLOR_YELLOW,
    CAR_COLOR_GREEN,
    CAR_COLOR_COUNT
} CarColor;

// Obstacle Types
typedef enum {
    OBS_NONE = 0,
    OBS_LOW_BARRIER,    // Construction barrier: jump over or dodge
    OBS_HIGH_SIGN,      // Overhead gantry: shrink under or dodge
    OBS_TRUCK,          // Heavy traffic vehicle: must switch lane!
    OBS_CIVILIAN_CAR,   // Moving traffic: slower cruising vehicle
    OBS_RAMP,           // Stunt launch ramp: launches player into super jump!
    OBS_SLICK,          // Surface friction hazard: causes spinout slide if driven over
    OBS_SIGNATURE       // Map-specific signature hazard (Plasma Laser, Dart Trap Volley, Gargoyles, etc.)
} ObstacleType;

// Powerup Types
typedef enum {
    POW_NONE = 0,
    POW_NITRO,          // Super speed + invulnerable
    POW_MAGNET,         // Attracts coins
    POW_SHIELD          // Absorbs 1 collision
} PowerupType;

// Player Structure
typedef struct {
    int lane;               // 0=Left, 1=Center, 2=Right
    int x;                  // Screen X in 8.8 fixed point
    int target_x;           // Target screen X in 8.8 fixed point
    int y_offset;           // Jump elevation in 8.8 fixed point (positive = up)
    int vy;                 // Vertical jump velocity (8.8 fixed point)
    bool is_jumping;
    bool is_shrinking;
    int shrink_timer;
    int tilt;               // -1=left, 0=straight, 1=right
    int shield_active;      // 1 if shield is up
    int nitro_timer;        // frames remaining
    int magnet_timer;       // frames remaining
    int invuln_timer;       // frames remaining (post-hit blink)
    bool is_crashing;
    int crash_frame;
    int buffered_key;       // Buffered action (KEY_A, KEY_LEFT, KEY_RIGHT, KEY_B)
    int buffer_timer;       // Countdown frames for buffered action
    int landing_squash;     // 2-frame suspension compression on touchdown
    int close_call_timer;   // Flash timer for CLOSE CALL bonus
    int shrink_boost_timer; // Speed surge timer from aerodynamic shrinking
    int hurdle_timer;       // Feedback timer for clearing roadblock hurdle
    int spinout_timer;      // Loss of traction / drift timer from surface slicks
    int stunt_timer;        // Super-jump stunt celebration feedback timer
} Player;

#define MAX_OBSTACLES       8
typedef struct {
    u8 active;
    u8 type;
    u8 pad[2];
    int lane;
    int target_lane;        // Target lane for active lane shifting
    int z;                  // Distance from horizon (e.g. 100 down to 0) in 8.8
    int speed_fp;           // Forward speed along road in 8.8 (0 = static, >0 = moving traffic)
    int x_fp;               // Smooth horizontal coordinate in 8.8
    int screen_x;
    int screen_y;
    int scale;              // 0=tiny, 1=small, 2=medium, 3=large
    int lane_shift_timer;   // Timer for lane transition
    int turn_signal_timer;  // Blinking timer before lane shift
    u8 passed;
    u8 close_call_awarded;
    u8 hurdled;             // Set when player leaps over barrier
    u8 hazard_state;        // Cycle / animation / orientation state for dynamic hazards
} Obstacle;

#define MAX_COINS           16
typedef struct {
    bool active;
    int lane;
    int z;                  // 8.8 fixed point
    int y_offset;           // 0=ground, >0=elevated for jump arcs
    int screen_x;
    int screen_y;
    int x_fp;               // Smooth horizontal position (8.8 fixed point)
    int anim_frame;
    bool collected;
} Coin;

#define MAX_POWERUPS        2
typedef struct {
    bool active;
    PowerupType type;
    int lane;
    int z;
    int screen_x;
    int screen_y;
} WorldPowerup;

typedef enum {
    DEBRIS_SPARK = 0,
    DEBRIS_STAR_SPARKLE = 1,
    DEBRIS_POPUP_150 = 2,
    DEBRIS_POPUP_100 = 3,
    DEBRIS_POPUP_250 = 4,
    DEBRIS_SHIELD_DISCHARGE = 5,
    DEBRIS_SNOWFLAKE = 6,
    DEBRIS_POPUP_STUNT = 7,
} DebrisType;

#define MAX_DEBRIS          16
typedef struct {
    bool active;
    DebrisType type;
    int x;                  // 8.8 fixed point screen X
    int y;                  // 8.8 fixed point screen Y
    int vx;                 // 8.8 fixed point velocity X
    int vy;                 // 8.8 fixed point velocity Y
    int life;               // Lifetime in frames
} DebrisParticle;

// 3D Camera Configuration
typedef struct {
    int horizon_y;          // Pixel Y where road converges
    int height;             // Virtual camera elevation
    int dist;               // Virtual camera focal distance
} Camera;

// Environment Themes
typedef enum {
    ENV_MIDNIGHT = 0,       // Neon Midnight (twilight indigo skyline)
    ENV_DAWN = 1,           // Cyber Dawn / Sunset (amber & gold)
    ENV_NOON = 2,           // High Noon (crisp blue & glass towers)
    ENV_STORM = 3           // Thunderstorm (charcoal & lightning)
} EnvTheme;

// Overall Game State
typedef struct {
    GameStateEnum state;
    int score;
    int high_score;
    int coins_collected;
    int total_coins;        // Total coins accumulated across games (persisted in SRAM)
    int distance_m;         // Meters traveled
    int distance_fp;        // Sub-meter 8.8 fixed-point distance accumulator
    int speed;              // Speed in 8.8 fixed point
    int base_speed;         // Base speed before multipliers
    int frame_count;
    int spawn_timer;
    int coin_spawn_timer;
    int powerup_spawn_timer;
    int road_scroll;        // Scrolling offset for dashed lines
    int screen_shake;       // Screen shake intensity
    int shake_timer;        // Active shake frames remaining
    int shake_amp;          // Current shake amplitude in pixels
    int shake_offset_x;     // Render-time X scroll offset
    int shake_offset_y;     // Render-time Y scroll offset
    int streak_multiplier;  // Surf Streak combo: 1, 2, 3, 4, 5
    int streak_timer;       // Decay countdown for combo streak (180 frames = 3s)
    int streak_count;       // Actions toward next multiplier tier (0..2)
    EnvTheme env_theme;     // Current environment theme
    int env_transition;     // Transition crossfade timer (0..30)
    int env_lightning;      // Lightning flash timer during storm (0..2)
    int flash_timer;        // Screen white-flash timer on crash/nitro
    int close_call_count;   // Total near misses in run
    int wave_pattern;       // Current choreographed obstacle wave
    int wave_step;          // Step within wave
    bool is_new_high_score; // True if run broke personal best
    bool high_score_fanfare_played; // True once celebration fanfare has sounded
    u8 cars_unlocked;       // Bitmask of unlocked cars (bits 0..5, Red & Blue default)
    u8 maps_unlocked;       // Bitmask of unlocked maps (bits 0..5, City default)
    CarColor car_color;     // Selected player sports car color (0..CAR_COLOR_COUNT-1)
    MapTheme map_theme;     // Selected track map theme (0..MAP_COUNT-1)
} Game;

#define CAR_UNLOCK_COST      2000
#define CAR_UNLOCKED_DEFAULT ((1 << CAR_COLOR_RED) | (1 << CAR_COLOR_BLUE))

#define MAP_UNLOCKED_DEFAULT (1 << MAP_CITY)
extern const int g_map_unlock_costs[MAP_COUNT];

extern const Camera g_camera;
extern Game g_game;
extern Player g_player;
extern Obstacle g_obstacles[MAX_OBSTACLES];
extern Coin g_coins[MAX_COINS];
extern WorldPowerup g_world_powerups[MAX_POWERUPS];
extern DebrisParticle g_debris[MAX_DEBRIS];

#endif // COMMON_H
