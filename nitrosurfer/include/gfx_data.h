#ifndef GFX_DATA_H
#define GFX_DATA_H

#include <tonc.h>
#include "common.h"

// Palettes
extern const COLOR bg_pal_data[48];
extern const COLOR bg_pal_midnight[48];
extern const COLOR bg_pal_dawn[48];
extern const COLOR bg_pal_noon[48];
extern const COLOR bg_pal_storm[48];
extern const COLOR bg_pal_beach[48];
extern const COLOR bg_pal_winter[48];
extern const COLOR bg_pal_orbital[48];
extern const COLOR bg_pal_gothic[48];
extern const COLOR bg_pal_maya[48];
extern const COLOR spr_pal_data[64];
extern const COLOR spr_pal_beach[16];
extern const COLOR spr_pal_winter[16];
extern const COLOR spr_pal_orbital[16];
extern const COLOR spr_pal_gothic[16];
extern const COLOR spr_pal_maya[16];
extern const COLOR g_steel_car_pals[4][16];
extern const COLOR g_car_color_pals[CAR_COLOR_COUNT][16];
extern const char *g_car_color_names[CAR_COLOR_COUNT];
extern const char *g_map_theme_names[MAP_COUNT];
extern const char *g_map_theme_subtitles[MAP_COUNT];

void gfx_apply_env_theme(EnvTheme theme, int transition_progress);
void gfx_apply_map_theme(MapTheme theme);
void gfx_set_car_color(CarColor color);
void gfx_restore_title_palette(void);

// Tile indices for OBJ sprites
#define TILE_PLAYER_STRAIGHT    0       // 32x32 (16 4bpp tiles)
#define TILE_PLAYER_LEFT        16      // 32x32
#define TILE_PLAYER_RIGHT       32      // 32x32
#define TILE_PLAYER_SHRINK_0    48      // 32x32 (16 tiles: 48..63) - Compression transition
#define TILE_PLAYER_SHRINK_1    332     // 32x32 (16 tiles: 332..347) - Compact nano-racer chassis
#define TILE_SHADOW             64      // 32x16 (8 tiles)
#define TILE_SPARK              76      // 8x8 (1 tile)

#define TILE_OBS_BARRIER_LRG    80      // 32x16 (8 tiles: 80..87)
#define TILE_OBS_BARRIER_SML    88      // 16x16 (4 tiles: 88..91)
#define TILE_OBS_SIGN_LRG       92      // 32x32 (16 tiles: 92..107)
#define TILE_OBS_SIGN_SML       108     // 16x16 (4 tiles: 108..111)
#define TILE_OBS_TRUCK_LRG      112     // 32x32 (16 tiles: 112..127)
#define TILE_OBS_TRUCK_SML      128     // 16x16 (4 tiles: 128..131)

#define TILE_COIN_F0            132     // 16x16 (4 tiles)
#define TILE_COIN_F1            136     // 16x16
#define TILE_COIN_F2            140     // 16x16
#define TILE_COIN_F3            144     // 16x16

#define TILE_POW_NITRO          148     // 16x16
#define TILE_POW_MAGNET         152     // 16x16
#define TILE_POW_SHIELD         156     // 16x16

#define TILE_PLAYER_JUMP_UP     176     // 32x32 (16 tiles: 176..191)
#define TILE_PLAYER_JUMP_DOWN   192     // 32x32 (16 tiles: 192..207)
#define TILE_SHADOW_SML         208     // 16x8 (2 tiles: 208..209)

#define TILE_OBS_BARRIER_PASS   214     // 32x16 (8 tiles: 214..221) - Green clearance hazard lights
#define TILE_OBS_SIGN_PASS      222     // 32x32 (16 tiles: 222..237) - Emerald green exit arrows
#define TILE_STAR_SPARKLE       238     // 8x8 (1 tile) - Golden clearance star
#define TILE_POPUP_150          240     // 16x8 (2 tiles: 240..241) - Gold "+150" popup
#define TILE_POPUP_100          242     // 16x8 (2 tiles: 242..243) - Cyan "+100" popup
#define TILE_POPUP_250          244     // 16x8 (2 tiles: 244..245) - Emerald "+250" popup
#define TILE_HUD_COIN           246     // 8x8 (1 tile) - Gold Coin HUD icon
#define TILE_HUD_WHEEL          247     // 8x8 (1 tile) - Sports car wheel HUD icon

// Nitro Animation Tiles (Rear Exhaust Fire Plumes)
#define TILE_NITRO_FIRE_0       72      // 16x16 (4 tiles: 72..75) - Roaring twin supersonic cone
#define TILE_NITRO_FIRE_1       248     // 16x16 (4 tiles: 248..251) - Turbulent fireball expansion
#define TILE_NITRO_FIRE_2       252     // 16x16 (4 tiles: 252..255) - Elongated roaring tongue blowout
#define TILE_NITRO_FIRE_3       348     // 16x16 (4 tiles: 348..351) - High-pressure blast with flying embers
#define TILE_NITRO_BURST        256     // 32x32 (16 tiles: 256..271) - Expanding supersonic shockwave

// Shield Animation Tiles
#define TILE_SHIELD_EXPAND      272     // 16x16 (4 tiles: 272..275) - Initial shield lock-in flash
#define TILE_SHIELD_SPARK       276     // 16x16 (4 tiles: 276..279) - Metal armor shatter deflection sparks

// Moving Civilian Traffic Tiles
#define TILE_OBS_CIVILIAN_LRG   280     // 32x32 (16 tiles: 280..295) - Cruising civilian sedan
#define TILE_OBS_CIVILIAN_SML   296     // 16x16 (4 tiles: 296..299)
#define TILE_TURN_SIGNAL        300     // 8x8 (1 tile) - Blinking amber indicator
#define TILE_COLOR_SWATCH       301     // 8x8 (1 tile) - Glossy chiclet car color swatch
#define TILE_COLOR_SELECTOR     302     // 8x8 (1 tile) - Active color selector chevron/cursor
#define TILE_SNOWFLAKE          303     // 8x8 (1 tile) - Drifting winter snowflake

// Palm Beach Themed Obstacle Tiles (Sprite Palette 10)
#define TILE_OBS_BEACH_DRIFTWOOD_LRG  360 // 32x16 (8 tiles: 360..367) - Tropical driftwood log with coconuts
#define TILE_OBS_BEACH_DRIFTWOOD_PASS 368 // 32x16 (8 tiles: 368..375) - Cleared with green sea glass
#define TILE_OBS_BEACH_TIKI_LRG       376 // 32x32 (16 tiles: 376..391) - Bamboo tiki pier archway
#define TILE_OBS_BEACH_TIKI_PASS      392 // 32x32 (16 tiles: 392..407) - Cleared with emerald lanterns
#define TILE_OBS_BEACH_TRUCK_LRG      408 // 32x32 (16 tiles: 408..423) - Flatbed coastal yacht hauler
#define TILE_OBS_BEACH_CIVILIAN_LRG   424 // 32x32 (16 tiles: 424..439) - Retro surfer woody wagon with surfboard

// Winter Snow Themed Obstacle Tiles (Sprite Palette 11)
#define TILE_OBS_WINT_ICE_LRG         440 // 32x16 (8 tiles: 440..447) - Glacial ice ridge barrier with flags
#define TILE_OBS_WINT_ICE_PASS        448 // 32x16 (8 tiles: 448..455) - Cleared with green crystal sheen
#define TILE_OBS_WINT_CABLE_LRG       456 // 32x32 (16 tiles: 456..471) - Ski gondola cableway gantry
#define TILE_OBS_WINT_CABLE_PASS      472 // 32x32 (16 tiles: 472..487) - Cleared with green beacons
#define TILE_OBS_WINT_TRUCK_LRG       488 // 32x32 (16 tiles: 488..503) - Heavy mountain snowplow truck
#define TILE_OBS_WINT_CIVILIAN_LRG    504 // 32x32 (16 tiles: 504..519) - Alpine 4x4 expedition SUV with rack

// Cape Orbital Themed Obstacle Tiles (Sprite Palette 12)
#define TILE_OBS_ORBIT_PIPE_LRG       520 // 32x16 (8 tiles: 520..527) - Cryogenic fuel line trench
#define TILE_OBS_ORBIT_PIPE_PASS      528 // 32x16 (8 tiles: 528..535) - Cleared with green vapor vent lights
#define TILE_OBS_ORBIT_GANTRY_LRG     536 // 32x32 (16 tiles: 536..551) - Launch umbilical service tower arm
#define TILE_OBS_ORBIT_GANTRY_PASS    552 // 32x32 (16 tiles: 552..567) - Cleared with green launch status LEDs
#define TILE_OBS_ORBIT_CRAWLER_LRG    568 // 32x32 (16 tiles: 568..583) - Crawler rocket booster transporter
#define TILE_OBS_ORBIT_ROVER_LRG      584 // 32x32 (16 tiles: 584..599) - Autonomous 6-wheel security rover

// Gothic Midnight Themed Obstacle Tiles (Sprite Palette 13)
#define TILE_OBS_GOTH_CRYPT_LRG       600 // 32x16 (8 tiles: 600..607) - Crumbling tombstone and crypt slab
#define TILE_OBS_GOTH_CRYPT_PASS      608 // 32x16 (8 tiles: 608..615) - Cleared with eerie green soul flame
#define TILE_OBS_GOTH_GATE_LRG        616 // 32x32 (16 tiles: 616..631) - Wrought-iron cemetery gateway arch
#define TILE_OBS_GOTH_GATE_PASS       632 // 32x32 (16 tiles: 632..647) - Cleared with green spectral lanterns
#define TILE_OBS_GOTH_HEARSE_LRG      648 // 32x32 (16 tiles: 648..663) - Black gothic funeral hearse with silver trim
#define TILE_OBS_GOTH_JALOPY_LRG      664 // 32x32 (16 tiles: 664..679) - Battered 1950s phantom hot-rod jalopy

// Maya Temple Trail Themed Obstacle Tiles (Sprite Palette 14)
#define TILE_OBS_MAYA_TOTEM_LRG       680 // 32x16 (8 tiles: 680..687) - Ancient carved stone stela totem
#define TILE_OBS_MAYA_TOTEM_PASS      688 // 32x32 (16 tiles: 688..695) - Cleared with glowing jade runes
#define TILE_OBS_MAYA_ARCH_LRG        696 // 32x32 (16 tiles: 696..711) - Ruined stone aqueduct with hanging vines
#define TILE_OBS_MAYA_ARCH_PASS       712 // 32x32 (16 tiles: 712..727) - Cleared with glowing jade portal runes
#define TILE_OBS_MAYA_TRUCK_LRG       728 // 32x32 (16 tiles: 728..743) - 6x6 canvas safari expedition hauler
#define TILE_OBS_MAYA_JEEP_LRG        744 // 32x32 (16 tiles: 744..759) - Rugged jungle patrol 4x4 with winch & snorkel
// Start Screen Artwork & UI Tiles
#define TILE_LOGO_BASE          72      // In CBB 2: 24x4 grid (96 tiles: 72..167) - Stylized NITRO SURFER Logo
#define TILE_UNDERGLOW          760     // In OBJ VRAM: 32x16 (8 tiles: 760..767) - Pulsing Neon Ground Underglow

// Mechanic-Expanding Stunt Ramps (32x16, 8 tiles each: 768..815)
#define TILE_OBS_RAMP_CITY      768     // Steel chevron ramp with hazard chevrons
#define TILE_OBS_RAMP_BEACH     776     // Bamboo & driftwood surf kicker ramp
#define TILE_OBS_RAMP_WINTER    784     // Glacial snowpack jump with guide poles
#define TILE_OBS_RAMP_ORBITAL   792     // Magnetic levitation launch strip with cyan LEDs
#define TILE_OBS_RAMP_GOTHIC    800     // Weathered stone crypt sarcophagus ramp
#define TILE_OBS_RAMP_MAYA      808     // Carved stone stepped temple ramp

// Mechanic-Expanding Surface Slicks (32x16, 8 tiles each: 816..863)
#define TILE_OBS_SLICK_CITY     816     // Dark motor oil puddle with iridescent sheen
#define TILE_OBS_SLICK_BEACH    824     // Wet tidal surge & sea foam patch
#define TILE_OBS_SLICK_WINTER   832     // Crystal black ice patch
#define TILE_OBS_SLICK_ORBITAL  840     // Glowing hyper-coolant leak pool
#define TILE_OBS_SLICK_GOTHIC   848     // Bubbling eerie green ectoplasm slime
#define TILE_OBS_SLICK_MAYA     856     // Wet jungle moss & red clay mud trap

// Track Signature Hazards (32x32, 16 tiles each: 880..975; City Tanker is 64x32, 32 tiles: 992..1023)
#define TILE_OBS_SIG_CITY       992     // Heavy Industrial Chemical Tanker (64x32, 32 tiles: 992..1023)
#define TILE_OBS_SIG_BEACH      880     // Windblown Windsurf Rig tumbling across road
#define TILE_OBS_SIG_WINTER     896     // Craggy Glacial Snowball / Ice Boulder
#define TILE_OBS_SIG_ORBITAL_ON 912     // Active Plasma Laser Barrier (lethal discharge)
#define TILE_OBS_SIG_ORBITAL_OFF 928    // Charging Laser Pylons (safe pass window)
#define TILE_OBS_SIG_GOTHIC     944     // Crouching Stone Gargoyle road barrier (jump UP over)
#define TILE_OBS_SIG_MAYA       960     // Ancient Poison Dart Trap Volley (duck DOWN under)

// Stunt Super-Jump Popup Debris
#define TILE_POPUP_STUNT        976     // 16x8 (2 tiles: 976..977) - Gold "+200 STUNT!"

void gfx_gen_title_logo(void);
void gfx_init_all(void);

#endif // GFX_DATA_H
