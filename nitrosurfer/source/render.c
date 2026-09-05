#include "render.h"
#include "gfx_data.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Shadow OAM buffer for 128 sprites
static OBJ_ATTR s_obj_buffer[128];

void safe_oam_copy(void);

// Tile definitions for BG1 (Skyline) and BG2 (Road)
#define TILE_SKY_TOP        1
#define TILE_SKY_MID        2
#define TILE_SKY_LOW        3
#define TILE_STAR           4
#define TILE_BLDG_TALL      5
#define TILE_BLDG_MID       6
#define TILE_BLDG_LOW       7
#define TILE_HORIZON        8

// Road tiles in CBB 2
#define TILE_GRASS              1
#define TILE_ROAD_DARK          2
#define TILE_ROAD_LIGHT         3
#define TILE_CURB_RED_L         4
#define TILE_CURB_WHITE_L       5
#define TILE_CURB_RED_R         6
#define TILE_CURB_WHITE_R       7
#define TILE_STRIPE_DASH        8
#define TILE_STRIPE_GAP         9
#define TILE_STRIPE_DASH_DARK   10
#define TILE_STRIPE_GAP_DARK    11

// Bottom UI Black Border Box tiles in CBB 2 (3 rows: Rows 17, 18, 19)
#define TILE_HUD_BOX_TL         12
#define TILE_HUD_BOX_TOP        13
#define TILE_HUD_BOX_TR         14
#define TILE_HUD_BOX_L          15
#define TILE_HUD_BOX_BG         16
#define TILE_HUD_BOX_R          17
#define TILE_HUD_BOX_BL         18
#define TILE_HUD_BOX_BTM        19
#define TILE_HUD_BOX_BR         20
#define TILE_HUD_BOX_DIV_TOP    21
#define TILE_HUD_BOX_DIV_MID    22
#define TILE_HUD_BOX_DIV_BTM    23

// Selected Card Box tiles in CBB 2 (Tiles 24..32)
#define TILE_SEL_BOX_TL         24
#define TILE_SEL_BOX_TOP        25
#define TILE_SEL_BOX_TR         26
#define TILE_SEL_BOX_L          27
#define TILE_SEL_BOX_BG         28
#define TILE_SEL_BOX_R          29
#define TILE_SEL_BOX_BL         30
#define TILE_SEL_BOX_BTM        31
#define TILE_SEL_BOX_BR         32

// Beach tiles in CBB 2
#define TILE_SAND               33
#define TILE_OCEAN_L            34
#define TILE_PALM_TOP           35
#define TILE_PALM_TRUNK         36

// Winter tiles in CBB 2
#define TILE_SNOW               37
#define TILE_SNOW_DRIFT         38
#define TILE_PINE_SNOW_TOP      39
#define TILE_PINE_SNOW_BTM      40

// Cape Orbital road tiles in CBB 2
#define TILE_ORBIT_CONCRETE     52
#define TILE_ORBIT_TOWER_TOP    53
#define TILE_ORBIT_TOWER_BTM    54

// Gothic Midnight road tiles in CBB 2
#define TILE_GOTH_COBBLE        55
#define TILE_GOTH_FENCE         56
#define TILE_GOTH_TOMB_TOP      57
#define TILE_GOTH_TOMB_BTM      58

// Maya Temple Trail road tiles in CBB 2
#define TILE_MAYA_JUNGLE        59
#define TILE_MAYA_TOTEM_TOP     60
#define TILE_MAYA_TOTEM_BTM     61
#define TILE_MAYA_STONE         62

// Thumbnail preview tiles in CBB 2 (Tiles 41..51, 63..74)
#define TILE_PREV_CITY_SKY      41
#define TILE_PREV_CITY_BLDG     42
#define TILE_PREV_CITY_ROAD     43

#define TILE_PREV_BEACH_SKY     44
#define TILE_PREV_BEACH_OCEAN   45
#define TILE_PREV_BEACH_SAND    46
#define TILE_PREV_BEACH_PALM    47

#define TILE_PREV_WINT_SKY      48
#define TILE_PREV_WINT_PEAK     49
#define TILE_PREV_WINT_SNOW     50
#define TILE_PREV_WINT_PINE     51

#define TILE_PREV_ORBIT_SKY     63
#define TILE_PREV_ORBIT_TOWER   64
#define TILE_PREV_ORBIT_GROUND  65

#define TILE_PREV_GOTH_SKY      66
#define TILE_PREV_GOTH_SPIRE    67
#define TILE_PREV_GOTH_ROAD     68

#define TILE_PREV_MAYA_CANOPY   69
#define TILE_PREV_MAYA_PYRAMID  70
#define TILE_PREV_MAYA_STONE    71

// Generate background tiles for Skyline (CBB 1) and Road (CBB 2)
static void init_bg_tiles(void) {
    // 1. Skyline tiles in CBB 1
    u32 *cbb1 = (u32 *)tile_mem[1];
    memset32(cbb1, 0, 32 * 8);

    // TILE_SKY_TOP: Deep Indigo (palette index 1)
    for (int i = 0; i < 8; i++) cbb1[TILE_SKY_TOP * 8 + i] = 0x11111111;
    // TILE_SKY_MID: Twilight Dusk (palette index 2)
    for (int i = 0; i < 8; i++) cbb1[TILE_SKY_MID * 8 + i] = 0x22222222;
    // TILE_SKY_LOW: Magenta Sunset Glow (palette index 4)
    for (int i = 0; i < 8; i++) cbb1[TILE_SKY_LOW * 8 + i] = 0x44444444;

    // TILE_STAR: Sky with a twinkle pixel
    for (int i = 0; i < 8; i++) cbb1[TILE_STAR * 8 + i] = 0x11111111;
    cbb1[TILE_STAR * 8 + 3] = 0x111C1111; // Palette 12 (star white)

    // TILE_BLDG_TALL: Skyscraper with gold/cyan lit windows
    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            cbb1[TILE_BLDG_TALL * 8 + i] = 0x8988A888; // Windows
        } else {
            cbb1[TILE_BLDG_TALL * 8 + i] = 0x88888888; // Building body
        }
    }

    // TILE_BLDG_MID
    for (int i = 0; i < 8; i++) {
        if (i == 3) {
            cbb1[TILE_BLDG_MID * 8 + i] = 0x777A7977;
        } else {
            cbb1[TILE_BLDG_MID * 8 + i] = 0x77777777;
        }
    }

    // TILE_HORIZON: Orange sunset gradient
    for (int i = 0; i < 8; i++) {
        u8 c = (i < 4) ? 4 : 5;
        u32 row = c | (c << 4) | (c << 8) | (c << 12) | (c << 16) | (c << 20) | (c << 24) | (c << 28);
        cbb1[TILE_HORIZON * 8 + i] = row;
    }

    // 2. Road tiles in CBB 2
    u32 *cbb2 = (u32 *)tile_mem[2];
    memset32(cbb2, 0, 32 * 8);

    // TILE_GRASS (palette 2, color 1)
    for (int i = 0; i < 8; i++) cbb2[TILE_GRASS * 8 + i] = 0x11111111;
    // TILE_ROAD_DARK (asphalt color 6)
    for (int i = 0; i < 8; i++) cbb2[TILE_ROAD_DARK * 8 + i] = 0x66666666;
    // TILE_ROAD_LIGHT (asphalt color 7)
    for (int i = 0; i < 8; i++) cbb2[TILE_ROAD_LIGHT * 8 + i] = 0x77777777;

    // TILE_CURB_RED_L (red stripe on right edge of grass)
    for (int i = 0; i < 8; i++) cbb2[TILE_CURB_RED_L * 8 + i] = 0x66633111;
    // TILE_CURB_WHITE_L
    for (int i = 0; i < 8; i++) cbb2[TILE_CURB_WHITE_L * 8 + i] = 0x66644111;

    // TILE_CURB_RED_R
    for (int i = 0; i < 8; i++) cbb2[TILE_CURB_RED_R * 8 + i] = 0x11133666;
    // TILE_CURB_WHITE_R
    for (int i = 0; i < 8; i++) cbb2[TILE_CURB_WHITE_R * 8 + i] = 0x11144666;

    // TILE_STRIPE_DASH (light asphalt with dashed white center stripe)
    for (int i = 0; i < 8; i++) {
        if (i >= 1 && i <= 6) {
            cbb2[TILE_STRIPE_DASH * 8 + i] = 0x77799777; // White dashed center
        } else {
            cbb2[TILE_STRIPE_DASH * 8 + i] = 0x77777777;
        }
    }
    // TILE_STRIPE_GAP (light asphalt gap between dashes)
    for (int i = 0; i < 8; i++) cbb2[TILE_STRIPE_GAP * 8 + i] = 0x77777777;

    // TILE_STRIPE_DASH_DARK (dark asphalt with dashed white center stripe)
    for (int i = 0; i < 8; i++) {
        if (i >= 1 && i <= 6) {
            cbb2[TILE_STRIPE_DASH_DARK * 8 + i] = 0x66699666;
        } else {
            cbb2[TILE_STRIPE_DASH_DARK * 8 + i] = 0x66666666;
        }
    }
    // TILE_STRIPE_GAP_DARK (dark asphalt gap between dashes)
    for (int i = 0; i < 8; i++) cbb2[TILE_STRIPE_GAP_DARK * 8 + i] = 0x66666666;

    // Bottom UI Black Border Box tiles (Palette 2: Color 14 = silver border, Color 12 = charcoal bevel, Color 15 = black)
    // TILE_HUD_BOX_TL (top-left corner, Row 17 tx=0)
    cbb2[TILE_HUD_BOX_TL * 8 + 0] = 0xEEEEEEEE;
    cbb2[TILE_HUD_BOX_TL * 8 + 1] = 0xCCCCCCCE;
    for (int i = 2; i < 8; i++) cbb2[TILE_HUD_BOX_TL * 8 + i] = 0xFFFFFFFE;

    // TILE_HUD_BOX_TOP (top border with sleek metallic bevel, Row 17 tx=1..28)
    cbb2[TILE_HUD_BOX_TOP * 8 + 0] = 0xEEEEEEEE;
    cbb2[TILE_HUD_BOX_TOP * 8 + 1] = 0xCCCCCCCC;
    for (int i = 2; i < 8; i++) cbb2[TILE_HUD_BOX_TOP * 8 + i] = 0xFFFFFFFF;

    // TILE_HUD_BOX_TR (top-right corner, Row 17 tx=29)
    cbb2[TILE_HUD_BOX_TR * 8 + 0] = 0xEEEEEEEE;
    cbb2[TILE_HUD_BOX_TR * 8 + 1] = 0xECCCCCCC;
    for (int i = 2; i < 8; i++) cbb2[TILE_HUD_BOX_TR * 8 + i] = 0xEFFFFFFF;

    // TILE_HUD_BOX_L (middle left border, Row 18 tx=0)
    for (int i = 0; i < 8; i++) cbb2[TILE_HUD_BOX_L * 8 + i] = 0xFFFFFFFE;

    // TILE_HUD_BOX_BG (solid black interior, Row 18 tx=1..28)
    for (int i = 0; i < 8; i++) cbb2[TILE_HUD_BOX_BG * 8 + i] = 0xFFFFFFFF;

    // TILE_HUD_BOX_R (middle right border, Row 18 tx=29)
    for (int i = 0; i < 8; i++) cbb2[TILE_HUD_BOX_R * 8 + i] = 0xEFFFFFFF;

    // TILE_HUD_BOX_BL (bottom-left corner, Row 19 tx=0)
    for (int i = 0; i < 7; i++) cbb2[TILE_HUD_BOX_BL * 8 + i] = 0xFFFFFFFE;
    cbb2[TILE_HUD_BOX_BL * 8 + 7] = 0xEEEEEEEE;

    // TILE_HUD_BOX_BTM (bottom border, Row 19 tx=1..28)
    for (int i = 0; i < 7; i++) cbb2[TILE_HUD_BOX_BTM * 8 + i] = 0xFFFFFFFF;
    cbb2[TILE_HUD_BOX_BTM * 8 + 7] = 0xEEEEEEEE;

    // TILE_HUD_BOX_BR (bottom-right corner, Row 19 tx=29)
    for (int i = 0; i < 7; i++) cbb2[TILE_HUD_BOX_BR * 8 + i] = 0xEFFFFFFF;
    cbb2[TILE_HUD_BOX_BR * 8 + 7] = 0xEEEEEEEE;

    // TILE_HUD_BOX_DIV_TOP (divider top: Row 17 tx=19)
    cbb2[TILE_HUD_BOX_DIV_TOP * 8 + 0] = 0xEEEEEEEE;
    cbb2[TILE_HUD_BOX_DIV_TOP * 8 + 1] = 0xCCCCCCCC;
    for (int i = 2; i < 8; i++) cbb2[TILE_HUD_BOX_DIV_TOP * 8 + i] = 0xFFFCFFFF;

    // TILE_HUD_BOX_DIV_MID (divider middle: Row 18 tx=19)
    for (int i = 0; i < 8; i++) cbb2[TILE_HUD_BOX_DIV_MID * 8 + i] = 0xFFFCFFFF;

    // TILE_HUD_BOX_DIV_BTM (divider bottom: Row 19 tx=19)
    for (int i = 0; i < 7; i++) cbb2[TILE_HUD_BOX_DIV_BTM * 8 + i] = 0xFFFCFFFF;
    cbb2[TILE_HUD_BOX_DIV_BTM * 8 + 7] = 0xEEEEEEEE;

    // Selected Box Tiles in CBB 2 (Palette 0: Color 2 = gold border, Color 1 = white highlight, Color 15 = black interior)
    cbb2[TILE_SEL_BOX_TL * 8 + 0] = 0x22222220;
    cbb2[TILE_SEL_BOX_TL * 8 + 1] = 0x11111122;
    for (int i = 2; i < 8; i++) cbb2[TILE_SEL_BOX_TL * 8 + i] = 0xFFFFFF12;

    cbb2[TILE_SEL_BOX_TOP * 8 + 0] = 0x22222222;
    cbb2[TILE_SEL_BOX_TOP * 8 + 1] = 0x11111111;
    for (int i = 2; i < 8; i++) cbb2[TILE_SEL_BOX_TOP * 8 + i] = 0xFFFFFFFF;

    cbb2[TILE_SEL_BOX_TR * 8 + 0] = 0x02222222;
    cbb2[TILE_SEL_BOX_TR * 8 + 1] = 0x22111111;
    for (int i = 2; i < 8; i++) cbb2[TILE_SEL_BOX_TR * 8 + i] = 0x21FFFFFF;

    for (int i = 0; i < 8; i++) cbb2[TILE_SEL_BOX_L * 8 + i] = 0xFFFFFF12;
    for (int i = 0; i < 8; i++) cbb2[TILE_SEL_BOX_BG * 8 + i] = 0xFFFFFFFF;
    for (int i = 0; i < 8; i++) cbb2[TILE_SEL_BOX_R * 8 + i] = 0x21FFFFFF;

    for (int i = 0; i < 6; i++) cbb2[TILE_SEL_BOX_BL * 8 + i] = 0xFFFFFF12;
    cbb2[TILE_SEL_BOX_BL * 8 + 6] = 0x11111122;
    cbb2[TILE_SEL_BOX_BL * 8 + 7] = 0x22222220;

    for (int i = 0; i < 6; i++) cbb2[TILE_SEL_BOX_BTM * 8 + i] = 0xFFFFFFFF;
    cbb2[TILE_SEL_BOX_BTM * 8 + 6] = 0x11111111;
    cbb2[TILE_SEL_BOX_BTM * 8 + 7] = 0x22222222;

    for (int i = 0; i < 6; i++) cbb2[TILE_SEL_BOX_BR * 8 + i] = 0x21FFFFFF;
    cbb2[TILE_SEL_BOX_BR * 8 + 6] = 0x22111111;
    cbb2[TILE_SEL_BOX_BR * 8 + 7] = 0x02222222;

    // Beach Tiles in CBB 2 (33..36)
    for (int i = 0; i < 8; i++) {
        cbb2[TILE_SAND * 8 + i] = ((i & 1) ? 0x22122122 : 0x12221222);
    }
    for (int i = 0; i < 8; i++) {
        cbb2[TILE_OCEAN_L * 8 + i] = 0x22249DDD;
    }
    cbb2[TILE_PALM_TOP * 8 + 0] = 0x22BBBB22;
    cbb2[TILE_PALM_TOP * 8 + 1] = 0x2BBBBBB2;
    cbb2[TILE_PALM_TOP * 8 + 2] = 0xBB2222BB;
    cbb2[TILE_PALM_TOP * 8 + 3] = 0xBB2B22BB;
    cbb2[TILE_PALM_TOP * 8 + 4] = 0x2B2A22B2;
    cbb2[TILE_PALM_TOP * 8 + 5] = 0x222A2222;
    cbb2[TILE_PALM_TOP * 8 + 6] = 0x222A2222;
    cbb2[TILE_PALM_TOP * 8 + 7] = 0x222A2222;

    for (int i = 0; i < 8; i++) {
        cbb2[TILE_PALM_TRUNK * 8 + i] = (i & 1) ? 0x222AA222 : 0x222A1222;
    }

    // Winter Tiles in CBB 2 (37..40)
    for (int i = 0; i < 8; i++) {
        cbb2[TILE_SNOW * 8 + i] = ((i == 2 || i == 6) ? 0x22122212 : 0x22222222);
    }
    cbb2[TILE_SNOW_DRIFT * 8 + 0] = 0x22222211;
    cbb2[TILE_SNOW_DRIFT * 8 + 1] = 0x22222112;
    cbb2[TILE_SNOW_DRIFT * 8 + 2] = 0x22221122;
    cbb2[TILE_SNOW_DRIFT * 8 + 3] = 0x22211222;
    cbb2[TILE_SNOW_DRIFT * 8 + 4] = 0x22112222;
    cbb2[TILE_SNOW_DRIFT * 8 + 5] = 0x21122222;
    cbb2[TILE_SNOW_DRIFT * 8 + 6] = 0x11222222;
    cbb2[TILE_SNOW_DRIFT * 8 + 7] = 0x12222222;

    cbb2[TILE_PINE_SNOW_TOP * 8 + 0] = 0x22222222;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 1] = 0x22222222;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 2] = 0x222BB222;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 3] = 0x22222222;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 4] = 0x22BBBB22;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 5] = 0x22222222;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 6] = 0x2BBBBBB2;
    cbb2[TILE_PINE_SNOW_TOP * 8 + 7] = 0x22222222;

    cbb2[TILE_PINE_SNOW_BTM * 8 + 0] = 0xBBBBBBBB;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 1] = 0x22222222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 2] = 0x222AA222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 3] = 0x222AA222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 4] = 0x222AA222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 5] = 0x222AA222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 6] = 0x22222222;
    cbb2[TILE_PINE_SNOW_BTM * 8 + 7] = 0x22222222;

    // Thumbnail Preview Tiles in CBB 2 (41..51)
    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_CITY_SKY * 8 + i] = 0x66666666;
    cbb2[TILE_PREV_CITY_SKY * 8 + 3] = 0x66696666;

    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) cbb2[TILE_PREV_CITY_BLDG * 8 + i] = 0xCECECECE;
        else cbb2[TILE_PREV_CITY_BLDG * 8 + i] = 0xCCCCCCCC;
    }

    for (int i = 0; i < 8; i++) {
        if (i == 3 || i == 4) cbb2[TILE_PREV_CITY_ROAD * 8 + i] = 0x66699666;
        else cbb2[TILE_PREV_CITY_ROAD * 8 + i] = 0x66666666;
    }

    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_BEACH_SKY * 8 + i] = 0x44444444;
    cbb2[TILE_PREV_BEACH_SKY * 8 + 2] = 0x44422444;
    cbb2[TILE_PREV_BEACH_SKY * 8 + 3] = 0x44222244;

    cbb2[TILE_PREV_BEACH_PALM * 8 + 0] = 0x44BBBB44;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 1] = 0x4BBBBBB4;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 2] = 0xBB4444BB;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 3] = 0x444A4444;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 4] = 0x444A4444;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 5] = 0x444A4444;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 6] = 0x444A4444;
    cbb2[TILE_PREV_BEACH_PALM * 8 + 7] = 0x444A4444;

    for (int i = 0; i < 8; i++) {
        if (i == 1 || i == 5) cbb2[TILE_PREV_BEACH_OCEAN * 8 + i] = 0x9DDD9DDD;
        else cbb2[TILE_PREV_BEACH_OCEAN * 8 + i] = 0xDDDDDDDD;
    }

    for (int i = 0; i < 8; i++) {
        cbb2[TILE_PREV_BEACH_SAND * 8 + i] = ((i & 1) ? 0x22122122 : 0x12221222);
    }

    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_WINT_SKY * 8 + i] = 0x11111111;

    cbb2[TILE_PREV_WINT_PEAK * 8 + 0] = 0x11122111;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 1] = 0x11222211;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 2] = 0x12222221;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 3] = 0x22222222;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 4] = 0x22DDDD22;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 5] = 0x2DDDDDD2;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 6] = 0xDDDDDDDD;
    cbb2[TILE_PREV_WINT_PEAK * 8 + 7] = 0xDDDDDDDD;

    cbb2[TILE_PREV_WINT_PINE * 8 + 0] = 0x11122111;
    cbb2[TILE_PREV_WINT_PINE * 8 + 1] = 0x112BB211;
    cbb2[TILE_PREV_WINT_PINE * 8 + 2] = 0x12222221;
    cbb2[TILE_PREV_WINT_PINE * 8 + 3] = 0x12BBBB21;
    cbb2[TILE_PREV_WINT_PINE * 8 + 4] = 0x22222222;
    cbb2[TILE_PREV_WINT_PINE * 8 + 5] = 0x2BBBBBB2;
    cbb2[TILE_PREV_WINT_PINE * 8 + 6] = 0x111AA111;
    cbb2[TILE_PREV_WINT_PINE * 8 + 7] = 0x111AA111;

    for (int i = 0; i < 8; i++) {
        cbb2[TILE_PREV_WINT_SNOW * 8 + i] = ((i == 3) ? 0x22122122 : 0x22222222);
    }

    // Cape Orbital Road & Scenery Tiles (52..54)
    for (int i = 0; i < 8; i++) cbb2[TILE_ORBIT_CONCRETE * 8 + i] = (i == 7 ? 0x11111111 : 0x22222222);
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 0] = 0x11333111;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 1] = 0x13BBB311;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 2] = 0x3BAAB331;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 3] = 0x3BAAB331;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 4] = 0x3BBBB331;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 5] = 0x3BAAB331;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 6] = 0x3BAAB331;
    cbb2[TILE_ORBIT_TOWER_TOP * 8 + 7] = 0x3BBBB331;

    for (int i = 0; i < 8; i++) {
        if (i == 0 || i == 7) cbb2[TILE_ORBIT_TOWER_BTM * 8 + i] = 0x3BBBB331;
        else if (i == 3 || i == 4) cbb2[TILE_ORBIT_TOWER_BTM * 8 + i] = 0x3B33B331;
        else cbb2[TILE_ORBIT_TOWER_BTM * 8 + i] = 0x3BAAB331;
    }

    // Gothic Midnight Road & Scenery Tiles (55..58)
    for (int i = 0; i < 8; i++) {
        if (i == 0 || i == 4) cbb2[TILE_GOTH_COBBLE * 8 + i] = 0x66566656;
        else if (i == 2 || i == 6) cbb2[TILE_GOTH_COBBLE * 8 + i] = 0x77877787;
        else cbb2[TILE_GOTH_COBBLE * 8 + i] = 0x77777777;
    }

    cbb2[TILE_GOTH_FENCE * 8 + 0] = 0x11161116;
    cbb2[TILE_GOTH_FENCE * 8 + 1] = 0x11666166;
    cbb2[TILE_GOTH_FENCE * 8 + 2] = 0x66666666;
    cbb2[TILE_GOTH_FENCE * 8 + 3] = 0x11161116;
    cbb2[TILE_GOTH_FENCE * 8 + 4] = 0x11161116;
    cbb2[TILE_GOTH_FENCE * 8 + 5] = 0x11161116;
    cbb2[TILE_GOTH_FENCE * 8 + 6] = 0x66666666;
    cbb2[TILE_GOTH_FENCE * 8 + 7] = 0x11161116;

    cbb2[TILE_GOTH_TOMB_TOP * 8 + 0] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 1] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 2] = 0x14444441;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 3] = 0x14444441;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 4] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 5] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 6] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_TOP * 8 + 7] = 0x11144111;

    cbb2[TILE_GOTH_TOMB_BTM * 8 + 0] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 1] = 0x11144111;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 2] = 0x11444411;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 3] = 0x11444411;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 4] = 0x14444441;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 5] = 0x14444441;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 6] = 0x44444444;
    cbb2[TILE_GOTH_TOMB_BTM * 8 + 7] = 0x22222222;

    // Maya Temple Trail Road & Scenery Tiles (59..62)
    for (int i = 0; i < 8; i++) {
        cbb2[TILE_MAYA_JUNGLE * 8 + i] = ((i & 1) ? 0x77B77B77 : 0xB77B77B7);
    }
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 0] = 0x11444411;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 1] = 0x14555541;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 2] = 0x14D22D41;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 3] = 0x14555541;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 4] = 0x14D22D41;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 5] = 0x14555541;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 6] = 0x14444441;
    cbb2[TILE_MAYA_TOTEM_TOP * 8 + 7] = 0x14444441;

    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 0] = 0x14444441;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 1] = 0x14555541;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 2] = 0x14D22D41;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 3] = 0x14555541;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 4] = 0x14444441;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 5] = 0x44444444;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 6] = 0x55555555;
    cbb2[TILE_MAYA_TOTEM_BTM * 8 + 7] = 0x22222222;

    for (int i = 0; i < 8; i++) {
        if (i == 0 || i == 4) cbb2[TILE_MAYA_STONE * 8 + i] = 0x66566656;
        else if (i == 2 || i == 6) cbb2[TILE_MAYA_STONE * 8 + i] = 0x77877787;
        else cbb2[TILE_MAYA_STONE * 8 + i] = 0x77777777;
    }

    // Cape Orbital Thumbnail Preview Tiles (63..65)
    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_ORBIT_SKY * 8 + i] = 0x11111111;
    cbb2[TILE_PREV_ORBIT_SKY * 8 + 2] = 0x11151111;
    cbb2[TILE_PREV_ORBIT_SKY * 8 + 5] = 0x11511111;

    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 0] = 0x113BB311;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 1] = 0x13BAAB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 2] = 0x3BAAAB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 3] = 0x3BAAAB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 4] = 0x3BBBBB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 5] = 0x3BAAAB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 6] = 0x3BAAAB31;
    cbb2[TILE_PREV_ORBIT_TOWER * 8 + 7] = 0x3BBBBB31;

    for (int i = 0; i < 8; i++) {
        if (i == 3 || i == 4) cbb2[TILE_PREV_ORBIT_GROUND * 8 + i] = 0x33443344;
        else cbb2[TILE_PREV_ORBIT_GROUND * 8 + i] = 0x22222222;
    }

    // Gothic Midnight Thumbnail Preview Tiles (66..68)
    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_GOTH_SKY * 8 + i] = 0x11111111;
    cbb2[TILE_PREV_GOTH_SKY * 8 + 2] = 0x11155111;
    cbb2[TILE_PREV_GOTH_SKY * 8 + 3] = 0x11555511;

    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 0] = 0x11144111;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 1] = 0x11444411;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 2] = 0x11444411;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 3] = 0x14444441;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 4] = 0x44444444;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 5] = 0x44444444;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 6] = 0x33333333;
    cbb2[TILE_PREV_GOTH_SPIRE * 8 + 7] = 0x33333333;

    for (int i = 0; i < 8; i++) {
        if (i == 3 || i == 4) cbb2[TILE_PREV_GOTH_ROAD * 8 + i] = 0x77887788;
        else cbb2[TILE_PREV_GOTH_ROAD * 8 + i] = 0x77777777;
    }

    // Maya Temple Trail Thumbnail Preview Tiles (69..71)
    for (int i = 0; i < 8; i++) cbb2[TILE_PREV_MAYA_CANOPY * 8 + i] = 0x22222222;
    cbb2[TILE_PREV_MAYA_CANOPY * 8 + 3] = 0x22B22B22;

    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 0] = 0x22144122;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 1] = 0x21444412;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 2] = 0x214DD412;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 3] = 0x14444441;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 4] = 0x14555541;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 5] = 0x44444444;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 6] = 0x45555554;
    cbb2[TILE_PREV_MAYA_PYRAMID * 8 + 7] = 0x44444444;

    for (int i = 0; i < 8; i++) {
        if (i == 3 || i == 4) cbb2[TILE_PREV_MAYA_STONE * 8 + i] = 0x77887788;
        else cbb2[TILE_PREV_MAYA_STONE * 8 + i] = 0x77777777;
    }

    // Generate Arcade Title Screen Logo in CBB 2 (tiles 72..167)
    gfx_gen_title_logo();
}

static u8 s_logo_bmp[32][192];

static void logo_put(int x, int y, u8 c) {
    if (x >= 0 && x < 192 && y >= 0 && y < 32) {
        s_logo_bmp[y][x] = c;
    }
}

static const u8 s_nitro_grad[13] = {
    1,  // dy = 0 (Y=2): Crisp White highlight
    9,  // dy = 1 (Y=3): Radiant Amber Sun
    9,  // dy = 2 (Y=4): Radiant Amber Sun
    2,  // dy = 3 (Y=5): Pure Gold
    2,  // dy = 4 (Y=6): Pure Gold
    8,  // dy = 5 (Y=7): Flame Orange
    8,  // dy = 6 (Y=8): Flame Orange
    8,  // dy = 7 (Y=9): Flame Orange
    4,  // dy = 8 (Y=10): Vivid Red / Crimson
    4,  // dy = 9 (Y=11): Vivid Red / Crimson
    4,  // dy = 10 (Y=12): Vivid Red / Crimson
    14, // dy = 11 (Y=13): Charcoal bevel
    15  // dy = 12 (Y=14): Deep Void Shadow
};

static void draw_nitro_letters(int ox, int oy, u8 col) {
    // 1. N at ox + 56 (width 14)
    int nx = ox + 56;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_nitro_grad[dy];
        for (int dx = 0; dx < 4; dx++) logo_put(nx + dx, oy + 2 + dy, c);
        for (int dx = 10; dx < 14; dx++) logo_put(nx + dx, oy + 2 + dy, c);
        int cx = nx + 3 + (dy * 7) / 12;
        for (int dx = 0; dx < 3; dx++) logo_put(cx + dx, oy + 2 + dy, c);
    }

    // 2. I at ox + 74 (width 8)
    int ix = ox + 74;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_nitro_grad[dy];
        if (dy <= 1 || dy >= 11) {
            for (int dx = 0; dx < 8; dx++) logo_put(ix + dx, oy + 2 + dy, c);
        } else {
            for (int dx = 2; dx < 6; dx++) logo_put(ix + dx, oy + 2 + dy, c);
        }
    }

    // 3. T at ox + 86 (width 14)
    int tx = ox + 86;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_nitro_grad[dy];
        if (dy <= 2) {
            for (int dx = 0; dx < 14; dx++) logo_put(tx + dx, oy + 2 + dy, c);
        } else {
            for (int dx = 5; dx < 9; dx++) logo_put(tx + dx, oy + 2 + dy, c);
        }
    }

    // 4. R at ox + 104 (width 14)
    int rx = ox + 104;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_nitro_grad[dy];
        for (int dx = 0; dx < 4; dx++) logo_put(rx + dx, oy + 2 + dy, c);
        if (dy <= 2) {
            for (int dx = 4; dx < 13; dx++) logo_put(rx + dx, oy + 2 + dy, c);
        }
        if (dy >= 1 && dy <= 6) {
            for (int dx = 10; dx < 14; dx++) logo_put(rx + dx, oy + 2 + dy, c);
        }
        if (dy >= 5 && dy <= 7) {
            for (int dx = 4; dx < 12; dx++) logo_put(rx + dx, oy + 2 + dy, c);
        }
        if (dy >= 6) {
            int lx = rx + 5 + ((dy - 6) * 5) / 6;
            for (int dx = 0; dx < 4; dx++) logo_put(lx + dx, oy + 2 + dy, c);
        }
    }

    // 5. O at ox + 122 (width 14)
    int ox_pos = ox + 122;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_nitro_grad[dy];
        if (dy <= 2 || dy >= 10) {
            int cut = (dy == 0 || dy == 12) ? 2 : 1;
            for (int dx = cut; dx < 14 - cut; dx++) logo_put(ox_pos + dx, oy + 2 + dy, c);
        } else {
            for (int dx = 0; dx < 4; dx++) logo_put(ox_pos + dx, oy + 2 + dy, c);
            for (int dx = 10; dx < 14; dx++) logo_put(ox_pos + dx, oy + 2 + dy, c);
        }
    }
}

static const u8 s_surfer_grad[13] = {
    1,  // dy = 0 (Y=17): Pure White specular rim
    10, // dy = 1 (Y=18): Brilliant Ice Blue
    10, // dy = 2 (Y=19): Brilliant Ice Blue
    3,  // dy = 3 (Y=20): Neon Cyan
    3,  // dy = 4 (Y=21): Neon Cyan
    11, // dy = 5 (Y=22): Deep Midnight Steel
    1,  // dy = 6 (Y=23): Crisp White chrome horizon flash band!
    12, // dy = 7 (Y=24): Chrome Highlight Silver
    12, // dy = 8 (Y=25): Chrome Highlight Silver
    13, // dy = 9 (Y=26): Chrome Bevel Steel
    13, // dy = 10 (Y=27): Chrome Bevel Steel
    14, // dy = 11 (Y=28): Charcoal shadow bevel
    15  // dy = 12 (Y=29): Dark Void
};

static void draw_surfer_letters(int ox, int oy, u8 col) {
    // 1. S at ox + 56 (width 11)
    int sx = ox + 56;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        if (dy <= 2) {
            for (int dx = 1; dx < 10; dx++) logo_put(sx + dx, oy + 17 + dy, c);
        } else if (dy <= 4) {
            for (int dx = 0; dx < 3; dx++) logo_put(sx + dx, oy + 17 + dy, c);
            if (dy == 3) logo_put(sx + 9, oy + 17 + dy, c);
        } else if (dy <= 7) {
            for (int dx = 1; dx < 10; dx++) logo_put(sx + dx, oy + 17 + dy, c);
        } else if (dy <= 9) {
            for (int dx = 8; dx < 11; dx++) logo_put(sx + dx, oy + 17 + dy, c);
            if (dy == 9) logo_put(sx + 1, oy + 17 + dy, c);
        } else {
            for (int dx = 1; dx < 10; dx++) logo_put(sx + dx, oy + 17 + dy, c);
        }
    }

    // 2. U at ox + 70 (width 11)
    int ux = ox + 70;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        if (dy <= 9) {
            for (int dx = 0; dx < 3; dx++) logo_put(ux + dx, oy + 17 + dy, c);
            for (int dx = 8; dx < 11; dx++) logo_put(ux + dx, oy + 17 + dy, c);
        } else {
            for (int dx = 1; dx < 10; dx++) logo_put(ux + dx, oy + 17 + dy, c);
        }
    }

    // 3. R at ox + 84 (width 11)
    int r1x = ox + 84;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        for (int dx = 0; dx < 3; dx++) logo_put(r1x + dx, oy + 17 + dy, c);
        if (dy <= 2) {
            for (int dx = 3; dx < 10; dx++) logo_put(r1x + dx, oy + 17 + dy, c);
        }
        if (dy >= 1 && dy <= 5) {
            for (int dx = 8; dx < 11; dx++) logo_put(r1x + dx, oy + 17 + dy, c);
        }
        if (dy >= 5 && dy <= 7) {
            for (int dx = 3; dx < 9; dx++) logo_put(r1x + dx, oy + 17 + dy, c);
        }
        if (dy >= 6) {
            int lx = r1x + 4 + ((dy - 6) * 4) / 6;
            for (int dx = 0; dx < 3; dx++) logo_put(lx + dx, oy + 17 + dy, c);
        }
    }

    // 4. F at ox + 98 (width 10)
    int fx = ox + 98;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        for (int dx = 0; dx < 3; dx++) logo_put(fx + dx, oy + 17 + dy, c);
        if (dy <= 2) {
            for (int dx = 3; dx < 10; dx++) logo_put(fx + dx, oy + 17 + dy, c);
        }
        if (dy >= 5 && dy <= 6) {
            for (int dx = 3; dx < 8; dx++) logo_put(fx + dx, oy + 17 + dy, c);
        }
    }

    // 5. E at ox + 111 (width 10)
    int ex = ox + 111;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        for (int dx = 0; dx < 3; dx++) logo_put(ex + dx, oy + 17 + dy, c);
        if (dy <= 2) {
            for (int dx = 3; dx < 10; dx++) logo_put(ex + dx, oy + 17 + dy, c);
        }
        if (dy >= 5 && dy <= 6) {
            for (int dx = 3; dx < 8; dx++) logo_put(ex + dx, oy + 17 + dy, c);
        }
        if (dy >= 10) {
            for (int dx = 3; dx < 10; dx++) logo_put(ex + dx, oy + 17 + dy, c);
        }
    }

    // 6. R at ox + 124 (width 11)
    int r2x = ox + 124;
    for (int dy = 0; dy < 13; dy++) {
        u8 c = col ? col : s_surfer_grad[dy];
        for (int dx = 0; dx < 3; dx++) logo_put(r2x + dx, oy + 17 + dy, c);
        if (dy <= 2) {
            for (int dx = 3; dx < 10; dx++) logo_put(r2x + dx, oy + 17 + dy, c);
        }
        if (dy >= 1 && dy <= 5) {
            for (int dx = 8; dx < 11; dx++) logo_put(r2x + dx, oy + 17 + dy, c);
        }
        if (dy >= 5 && dy <= 7) {
            for (int dx = 3; dx < 9; dx++) logo_put(r2x + dx, oy + 17 + dy, c);
        }
        if (dy >= 6) {
            int lx = r2x + 4 + ((dy - 6) * 4) / 6;
            for (int dx = 0; dx < 3; dx++) logo_put(lx + dx, oy + 17 + dy, c);
        }
    }
}

void gfx_gen_title_logo(void) {
    memset(s_logo_bmp, 0, sizeof(s_logo_bmp));

    // 1. Draw 3D Drop Shadows (+1, +1) in Void Shadow (Color 15)
    draw_nitro_letters(1, 1, 15);
    draw_surfer_letters(1, 1, 15);

    // 2. Draw Main Stylized Letters with Flame & Chrome Gradient Palettes
    draw_nitro_letters(0, 0, 0);
    draw_surfer_letters(0, 0, 0);

    // 3. Central Glowing Laser Horizon Beam (Y = 15..16)
    for (int x = 12; x < 180; x++) {
        u8 c = 11; // Deep steel blue
        int dist = abs(x - 95);
        if (dist < 10) c = 1;       // White laser core
        else if (dist < 24) c = 10;  // Ice blue aura
        else if (dist < 52) c = 3;   // Cyan beam
        logo_put(x, 15, c);
        logo_put(x, 16, 15);        // Horizon drop shadow
    }
    // Blooming Center Diamond Lens Flare at (95, 15)
    logo_put(95, 13, 10);
    logo_put(95, 14, 1);
    logo_put(95, 15, 1);
    logo_put(95, 16, 1);
    logo_put(95, 17, 10);
    logo_put(94, 15, 1);
    logo_put(96, 15, 1);
    logo_put(93, 15, 10);
    logo_put(97, 15, 10);

    // 4. Aerodynamic Speed Chevrons on Left Wing (Pointing Right '>')
    for (int ch = 0; ch < 3; ch++) {
        int cx = 14 + ch * 12;
        // Upper arm (Flame colors)
        for (int i = 0; i <= 10; i++) {
            int x = cx + (i * 6) / 10;
            int y = 4 + i;
            u8 c = (i < 4) ? 9 : (i < 7 ? 2 : 8);
            for (int w = 0; w < 3; w++) logo_put(x + w, y, c);
            logo_put(x + 3, y, 15);
        }
        // Lower arm (Cyan/Chrome colors)
        for (int i = 0; i <= 10; i++) {
            int x = (cx + 6) - (i * 6) / 10;
            int y = 16 + i;
            u8 c = (i < 4) ? 3 : (i < 7 ? 10 : 12);
            for (int w = 0; w < 3; w++) logo_put(x + w, y, c);
            logo_put(x + 3, y, 15);
        }
    }

    // 5. Aerodynamic Speed Chevrons on Right Wing (Pointing Left '<')
    for (int ch = 0; ch < 3; ch++) {
        int cx = 154 + ch * 12;
        // Upper arm
        for (int i = 0; i <= 10; i++) {
            int x = (cx + 6) - (i * 6) / 10;
            int y = 4 + i;
            u8 c = (i < 4) ? 9 : (i < 7 ? 2 : 8);
            for (int w = 0; w < 3; w++) logo_put(x + w, y, c);
            logo_put(x + 3, y, 15);
        }
        // Lower arm
        for (int i = 0; i <= 10; i++) {
            int x = cx + (i * 6) / 10;
            int y = 16 + i;
            u8 c = (i < 4) ? 3 : (i < 7 ? 10 : 12);
            for (int w = 0; w < 3; w++) logo_put(x + w, y, c);
            logo_put(x + 3, y, 15);
        }
    }

    // 6. Top & Bottom Speed Streak Lines
    for (int x = 20; x < 40; x++) logo_put(x, 1, 9);
    for (int x = 50; x < 80; x++) logo_put(x, 1, 1);
    for (int x = 112; x < 142; x++) logo_put(x, 1, 1);
    for (int x = 152; x < 172; x++) logo_put(x, 1, 9);

    for (int x = 16; x < 44; x++) logo_put(x, 30, 3);
    for (int x = 60; x < 132; x++) logo_put(x, 30, 10);
    for (int x = 148; x < 176; x++) logo_put(x, 30, 3);

    // 7. Pack 192x32 bitmap into CBB 2 (24x4 tiles starting at TILE_LOGO_BASE)
    u32 *cbb2 = (u32 *)tile_mem[2];
    for (int ty = 0; ty < 4; ty++) {
        for (int tx = 0; tx < 24; tx++) {
            int tile_idx = TILE_LOGO_BASE + ty * 24 + tx;
            u32 *tile_ptr = &cbb2[tile_idx * 8];
            for (int py = 0; py < 8; py++) {
                int y = ty * 8 + py;
                u32 row_val = 0;
                for (int px = 0; px < 8; px++) {
                    int x = tx * 8 + px;
                    u8 c = s_logo_bmp[y][x] & 0x0F;
                    row_val |= ((u32)c) << (px * 4);
                }
                tile_ptr[py] = row_val;
            }
        }
    }
}

// Build Bottom HUD Box on BG1 (SBB 30) - Topmost Background Layer (Priority 0)
static void init_hud_map(void) {
    SCR_ENTRY *sbb1 = se_mem[30];
    memset16(sbb1, 0, 32 * 32);

    // Row 17: Top border with sleek metallic bevel & divider top
    sbb1[17 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_TL, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) {
        sbb1[17 * 32 + tx] = SE_BUILD(tx == 19 ? TILE_HUD_BOX_DIV_TOP : TILE_HUD_BOX_TOP, 2, 0, 0);
    }
    sbb1[17 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_TR, 2, 0, 0);

    // Row 18: Middle row with solid black interior & divider mid
    sbb1[18 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_L, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) {
        sbb1[18 * 32 + tx] = SE_BUILD(tx == 19 ? TILE_HUD_BOX_DIV_MID : TILE_HUD_BOX_BG, 2, 0, 0);
    }
    sbb1[18 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_R, 2, 0, 0);

    // Row 19: Bottom border with divider bottom
    sbb1[19 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_BL, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) {
        sbb1[19 * 32 + tx] = SE_BUILD(tx == 19 ? TILE_HUD_BOX_DIV_BTM : TILE_HUD_BOX_BTM, 2, 0, 0);
    }
    sbb1[19 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_BR, 2, 0, 0);
}

static int s_last_title_color = -1;
static int s_last_map_theme = -1;
static u8 s_last_maps_unlocked = 0;
static bool s_start_screen_drawn = false;

// Draw 3x2 Car Selection Boxes on BG1 (SBB 30)
void render_title_boxes(int selected_color) {
    if (selected_color == s_last_title_color) return;
    s_last_title_color = selected_color;

    SCR_ENTRY *sbb1 = se_mem[30];
    const int col_tx[3] = { 1, 11, 21 };
    const int row_ty[2] = { 4, 10 };

    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 3; c++) {
            int idx = r * 3 + c;
            bool sel = (idx == selected_color);
            int pal = sel ? 0 : 2;
            int tl  = sel ? TILE_SEL_BOX_TL  : TILE_HUD_BOX_TL;
            int top = sel ? TILE_SEL_BOX_TOP : TILE_HUD_BOX_TOP;
            int tr  = sel ? TILE_SEL_BOX_TR  : TILE_HUD_BOX_TR;
            int l   = sel ? TILE_SEL_BOX_L   : TILE_HUD_BOX_L;
            int bg  = sel ? TILE_SEL_BOX_BG  : TILE_HUD_BOX_BG;
            int right = sel ? TILE_SEL_BOX_R : TILE_HUD_BOX_R;
            int bl  = sel ? TILE_SEL_BOX_BL  : TILE_HUD_BOX_BL;
            int btm = sel ? TILE_SEL_BOX_BTM : TILE_HUD_BOX_BTM;
            int br  = sel ? TILE_SEL_BOX_BR  : TILE_HUD_BOX_BR;

            int start_x = col_tx[c];
            int start_y = row_ty[r];

            // Top row of box
            sbb1[start_y * 32 + start_x] = SE_BUILD(tl, pal, 0, 0);
            for (int x = 1; x < 7; x++) {
                sbb1[start_y * 32 + start_x + x] = SE_BUILD(top, pal, 0, 0);
            }
            sbb1[start_y * 32 + start_x + 7] = SE_BUILD(tr, pal, 0, 0);

            // Middle 3 rows of box
            for (int y = 1; y < 4; y++) {
                int py = start_y + y;
                sbb1[py * 32 + start_x] = SE_BUILD(l, pal, 0, 0);
                for (int x = 1; x < 7; x++) {
                    sbb1[py * 32 + start_x + x] = SE_BUILD(bg, pal, 0, 0);
                }
                sbb1[py * 32 + start_x + 7] = SE_BUILD(right, pal, 0, 0);
            }

            // Bottom row of box
            int b_y = start_y + 4;
            sbb1[b_y * 32 + start_x] = SE_BUILD(bl, pal, 0, 0);
            for (int x = 1; x < 7; x++) {
                sbb1[b_y * 32 + start_x + x] = SE_BUILD(btm, pal, 0, 0);
            }
            sbb1[b_y * 32 + start_x + 7] = SE_BUILD(br, pal, 0, 0);
        }
    }

    // Car Attributes & Stats Panel Backdrop on BG1 inside Rows 14..16 (tx 1..28, cols 1..28)
    for (int ty = 14; ty <= 16; ty++) {
        for (int tx = 1; tx < 29; tx++) {
            sbb1[ty * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
        }
    }
    // Clear Row 9 backdrop cleanly between Row 0 and Row 1
    for (int tx = 0; tx < 30; tx++) {
        sbb1[9 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
    }
}

// Draw 6 Boxes in 3x2 Grid for Map Selection on BG1 (SBB 30)
void render_map_boxes(int selected_map) {
    if (selected_map == s_last_map_theme && g_game.maps_unlocked == s_last_maps_unlocked) return;
    s_last_map_theme = selected_map;
    s_last_maps_unlocked = g_game.maps_unlocked;

    SCR_ENTRY *sbb1 = se_mem[30];
    const int col_tx[3] = { 1, 11, 21 };
    const int row_ty[2] = { 4, 10 };

    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 3; c++) {
            int idx = r * 3 + c;
            bool sel = (idx == selected_map);
            int pal = sel ? 0 : (2 + idx);
            int tl  = sel ? TILE_SEL_BOX_TL  : TILE_HUD_BOX_TL;
            int top = sel ? TILE_SEL_BOX_TOP : TILE_HUD_BOX_TOP;
            int tr  = sel ? TILE_SEL_BOX_TR  : TILE_HUD_BOX_TR;
            int l   = sel ? TILE_SEL_BOX_L   : TILE_HUD_BOX_L;
            int right = sel ? TILE_SEL_BOX_R : TILE_HUD_BOX_R;
            int bl  = sel ? TILE_SEL_BOX_BL  : TILE_HUD_BOX_BL;
            int btm = sel ? TILE_SEL_BOX_BTM : TILE_HUD_BOX_BTM;
            int br  = sel ? TILE_SEL_BOX_BR  : TILE_HUD_BOX_BR;

            int start_x = col_tx[c];
            int start_y = row_ty[r];

            // Top border (row 4 or 10)
            sbb1[start_y * 32 + start_x] = SE_BUILD(tl, pal, 0, 0);
            for (int x = 1; x < 7; x++) {
                sbb1[start_y * 32 + start_x + x] = SE_BUILD(top, pal, 0, 0);
            }
            sbb1[start_y * 32 + start_x + 7] = SE_BUILD(tr, pal, 0, 0);

            // Interior rows (y = 1..3: rows 5..7 or 11..13)
            for (int y = 1; y < 4; y++) {
                int py = start_y + y;
                sbb1[py * 32 + start_x] = SE_BUILD(l, pal, 0, 0);
                u16 tile_fill;
                if (idx == MAP_CITY) {
                    if (y == 1) tile_fill = TILE_PREV_CITY_SKY;
                    else if (y == 2) tile_fill = TILE_PREV_CITY_BLDG;
                    else tile_fill = TILE_PREV_CITY_ROAD;
                } else if (idx == MAP_BEACH) {
                    if (y == 1) tile_fill = TILE_PREV_BEACH_SKY;
                    else if (y == 2) tile_fill = TILE_PREV_BEACH_PALM;
                    else tile_fill = TILE_PREV_BEACH_SAND;
                } else if (idx == MAP_WINTER) {
                    if (y == 1) tile_fill = TILE_PREV_WINT_SKY;
                    else if (y == 2) tile_fill = TILE_PREV_WINT_PINE;
                    else tile_fill = TILE_PREV_WINT_SNOW;
                } else if (idx == MAP_ORBITAL) {
                    if (y == 1) tile_fill = TILE_PREV_ORBIT_SKY;
                    else if (y == 2) tile_fill = TILE_PREV_ORBIT_TOWER;
                    else tile_fill = TILE_PREV_ORBIT_GROUND;
                } else if (idx == MAP_GOTHIC) {
                    if (y == 1) tile_fill = TILE_PREV_GOTH_SKY;
                    else if (y == 2) tile_fill = TILE_PREV_GOTH_SPIRE;
                    else tile_fill = TILE_PREV_GOTH_ROAD;
                } else { // MAP_MAYA
                    if (y == 1) tile_fill = TILE_PREV_MAYA_CANOPY;
                    else if (y == 2) tile_fill = TILE_PREV_MAYA_PYRAMID;
                    else tile_fill = TILE_PREV_MAYA_STONE;
                }

                for (int x = 1; x < 7; x++) {
                    sbb1[py * 32 + start_x + x] = SE_BUILD(tile_fill, 2 + idx, 0, 0);
                }
                sbb1[py * 32 + start_x + 7] = SE_BUILD(right, pal, 0, 0);
            }

            // Bottom border (row 8 or 14)
            int b_y = start_y + 4;
            sbb1[b_y * 32 + start_x] = SE_BUILD(bl, pal, 0, 0);
            for (int x = 1; x < 7; x++) {
                sbb1[b_y * 32 + start_x + x] = SE_BUILD(btm, pal, 0, 0);
            }
            sbb1[b_y * 32 + start_x + 7] = SE_BUILD(br, pal, 0, 0);
        }
    }

    // Backdrop on BG1 inside Rows 14..16 (tx 1..28, cols 1..28) behind row 1 prices
    for (int ty = 14; ty <= 16; ty++) {
        for (int tx = 1; tx < 29; tx++) {
            sbb1[ty * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
        }
    }
    // Clear Row 9 backdrop cleanly between Row 0 and Row 1
    for (int tx = 0; tx < 30; tx++) {
        sbb1[9 * 32 + tx] = 0;
    }
}

// Clear Selection Boxes from BG1 when entering gameplay or switching screens
void render_clear_title_boxes(void) {
    s_last_title_color = -1;
    s_last_map_theme = -1;
    s_last_maps_unlocked = 0;
    s_start_screen_drawn = false;
    SCR_ENTRY *sbb1 = se_mem[30];
    for (int ty = 0; ty < 17; ty++) {
        for (int tx = 0; tx < 30; tx++) {
            sbb1[ty * 32 + tx] = 0;
        }
    }
}

// Build 3-Lane Parallel Top-Down Highway on BG2 (SBB 28)
static void update_road_map(int scroll_phase) {
    SCR_ENTRY *sbb2 = se_mem[28];

    // Parallel highway across all 20 rows (Y: 0 to 159)
    for (int ty = 0; ty < 20; ty++) {
        // Vertical motion: offset rows by scroll_phase
        int phase_y = ty - (scroll_phase >> 2);

        // Alternating curb red/white rhythm moving downwards
        bool curb_white = ((phase_y >> 1) & 1);
        u16 curb_l = curb_white ? TILE_CURB_WHITE_L : TILE_CURB_RED_L;
        u16 curb_r = curb_white ? TILE_CURB_WHITE_R : TILE_CURB_RED_R;

        // Alternating asphalt shade
        bool is_dark = ((phase_y >> 2) & 1);
        u16 road_tile = is_dark ? TILE_ROAD_DARK : TILE_ROAD_LIGHT;

        // Dashed lines rhythm (2 tiles on, 1 tile gap)
        bool dash_on = ((phase_y % 3) != 0);
        u16 stripe_tile;
        if (is_dark) {
            stripe_tile = dash_on ? TILE_STRIPE_DASH_DARK : TILE_STRIPE_GAP_DARK;
        } else {
            stripe_tile = dash_on ? TILE_STRIPE_DASH : TILE_STRIPE_GAP;
        }

        if (g_game.map_theme == MAP_BEACH) {
            // Beach Theme:
            // Left shoulder: Turquoise ocean waves (tx=0) and golden beach sand dunes (tx=1..3)
            // Right shoulder: Golden beach sand (tx=26..29) with roadside tropical palm trees!
            int palm_phase = (phase_y + 1000) % 8;
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx == 0) {
                    tile = TILE_OCEAN_L;
                } else if (tx < 4) {
                    tile = TILE_SAND;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    if (tx == 27 && palm_phase == 0) tile = TILE_PALM_TOP;
                    else if (tx == 27 && palm_phase == 1) tile = TILE_PALM_TRUNK;
                    else tile = TILE_SAND;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        } else if (g_game.map_theme == MAP_WINTER) {
            // Winter Theme:
            // Left and right shoulders: Pure white snowbanks, curved snowdrifts, and snowy pines!
            int pine_phase_l = (phase_y + 1000) % 7;
            int pine_phase_r = (phase_y + 1003) % 7;
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx < 4) {
                    if (tx == 2 && pine_phase_l == 0) tile = TILE_PINE_SNOW_TOP;
                    else if (tx == 2 && pine_phase_l == 1) tile = TILE_PINE_SNOW_BTM;
                    else if (tx == 3 && ((phase_y & 3) == 0)) tile = TILE_SNOW_DRIFT;
                    else tile = TILE_SNOW;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    if (tx == 27 && pine_phase_r == 0) tile = TILE_PINE_SNOW_TOP;
                    else if (tx == 27 && pine_phase_r == 1) tile = TILE_PINE_SNOW_BTM;
                    else if (tx == 26 && ((phase_y & 3) == 2)) tile = TILE_SNOW_DRIFT;
                    else tile = TILE_SNOW;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        } else if (g_game.map_theme == MAP_ORBITAL) {
            // Cape Orbital Theme:
            // Left and right shoulders: Grooved concrete apron with launch service umbilical towers!
            int tower_phase_l = (phase_y + 1000) % 8;
            int tower_phase_r = (phase_y + 1004) % 8;
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx < 4) {
                    if (tx == 2 && tower_phase_l == 0) tile = TILE_ORBIT_TOWER_TOP;
                    else if (tx == 2 && tower_phase_l == 1) tile = TILE_ORBIT_TOWER_BTM;
                    else tile = TILE_ORBIT_CONCRETE;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    if (tx == 27 && tower_phase_r == 0) tile = TILE_ORBIT_TOWER_TOP;
                    else if (tx == 27 && tower_phase_r == 1) tile = TILE_ORBIT_TOWER_BTM;
                    else tile = TILE_ORBIT_CONCRETE;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        } else if (g_game.map_theme == MAP_GOTHIC) {
            // Gothic Midnight Theme:
            // Left and right shoulders: Spiked wrought-iron cemetery fence, stone tombs & cobblestones!
            int tomb_phase_l = (phase_y + 1000) % 7;
            int tomb_phase_r = (phase_y + 1003) % 7;
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx < 4) {
                    if (tx == 3) tile = TILE_GOTH_FENCE;
                    else if (tx == 1 && tomb_phase_l == 0) tile = TILE_GOTH_TOMB_TOP;
                    else if (tx == 1 && tomb_phase_l == 1) tile = TILE_GOTH_TOMB_BTM;
                    else tile = TILE_GOTH_COBBLE;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    if (tx == 26) tile = TILE_GOTH_FENCE;
                    else if (tx == 28 && tomb_phase_r == 0) tile = TILE_GOTH_TOMB_TOP;
                    else if (tx == 28 && tomb_phase_r == 1) tile = TILE_GOTH_TOMB_BTM;
                    else tile = TILE_GOTH_COBBLE;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        } else if (g_game.map_theme == MAP_MAYA) {
            // Maya Temple Trail Theme:
            // Left and right shoulders: Rainforest canopy with carved stone idol totems & ancient stone trail!
            int totem_phase_l = (phase_y + 1000) % 8;
            int totem_phase_r = (phase_y + 1004) % 8;
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx < 4) {
                    if (tx == 3) tile = TILE_MAYA_STONE;
                    else if (tx == 1 && totem_phase_l == 0) tile = TILE_MAYA_TOTEM_TOP;
                    else if (tx == 1 && totem_phase_l == 1) tile = TILE_MAYA_TOTEM_BTM;
                    else tile = TILE_MAYA_JUNGLE;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    if (tx == 26) tile = TILE_MAYA_STONE;
                    else if (tx == 28 && totem_phase_r == 0) tile = TILE_MAYA_TOTEM_TOP;
                    else if (tx == 28 && totem_phase_r == 1) tile = TILE_MAYA_TOTEM_BTM;
                    else tile = TILE_MAYA_JUNGLE;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        } else {
            // City Theme:
            for (int tx = 0; tx < 30; tx++) {
                u16 tile;
                if (tx < 4) {
                    tile = TILE_GRASS;
                } else if (tx == 4) {
                    tile = curb_l;
                } else if (tx == 11 || tx == 18) {
                    tile = stripe_tile;
                } else if (tx == 25) {
                    tile = curb_r;
                } else if (tx > 25) {
                    tile = TILE_GRASS;
                } else {
                    tile = road_tile;
                }
                sbb2[ty * 32 + tx] = SE_BUILD(tile, 2, 0, 0);
            }
        }
    }
}

void render_init(void) {
    // 1. Configure Background Registers
    // BG0: Priority 0 (Top HUD Text overlay), CBB 0, SBB 31
    REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_PRIO(0) | BG_4BPP | BG_REG_32x32;
    // BG1: Priority 0 (Bottom HUD Container Box), CBB 2, SBB 30
    REG_BG1CNT = BG_CBB(2) | BG_SBB(30) | BG_PRIO(0) | BG_4BPP | BG_REG_32x32;
    // BG2: Priority 3 (Road), CBB 2, SBB 28
    REG_BG2CNT = BG_CBB(2) | BG_SBB(28) | BG_PRIO(3) | BG_4BPP | BG_REG_32x32;

    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;

    // Hardware Window 0: Bottom UI Layer (X: 0..240, Y: 136..160)
    REG_WIN0H = (0 << 8) | 240;
    REG_WIN0V = (136 << 8) | 160;
    // Inside WIN0: BG0, BG1, and HUD OBJ allowed; road (BG2) disabled
    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_OBJ, 0);
    // Outside WIN0: BG0, BG1, BG2, OBJ, BLD allowed
    REG_WINOUT = WINOUT_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_OBJ | WIN_BLD, 0);

    // 2. Load Palettes and Generate Sprite & BG Tiles
    gfx_init_all();
    init_bg_tiles();
    init_hud_map();
    update_road_map(0);

    // 3. Initialize Tonc Text Engine (TTE) for BG0
    tte_init_se(0, BG_CBB(0) | BG_SBB(31), 0xF000, CLR_YELLOW, 0, &fwf_default, NULL);
    tte_set_margins(0, 0, SCREEN_W, SCREEN_H);

    // 4. Clear OAM buffer and sync to hardware
    oam_init(s_obj_buffer, 128);
    safe_oam_copy();

    // 5. Enable Display: Mode 0, BG0, BG1, BG2, Sprites in 1D mapping, Window 0
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_WIN0;
}

void render_start_screen(void) {
    SCR_ENTRY *sbb1 = se_mem[30];

    // 1. Draw BG1 Artwork (Logo, Menu Card, & Metallic Box) if not cached
    if (!s_start_screen_drawn) {
        s_start_screen_drawn = true;
        s_last_title_color = -1;
        s_last_map_theme = -1;
        s_last_maps_unlocked = 0;

        // Clear upper BG1 screenblock rows 0..16
        for (int ty = 0; ty < 17; ty++) {
            for (int tx = 0; tx < 30; tx++) {
                sbb1[ty * 32 + tx] = 0;
            }
        }

        // Map 24x4 NITRO SURFER logo at rows 1..4, cols 3..26 (CBB 2 tiles 72..167)
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 24; col++) {
                int tile_idx = TILE_LOGO_BASE + row * 24 + col;
                sbb1[(1 + row) * 32 + (3 + col)] = SE_BUILD(tile_idx, 0, 0, 0);
            }
        }

        // Sleek Dark Backdrop Panel on BG1 for Start Race (Rows 13..15, cols 4..25)
        sbb1[13 * 32 + 4] = SE_BUILD(TILE_HUD_BOX_TL, 2, 0, 0);
        for (int tx = 5; tx < 25; tx++) sbb1[13 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_TOP, 2, 0, 0);
        sbb1[13 * 32 + 25] = SE_BUILD(TILE_HUD_BOX_TR, 2, 0, 0);

        sbb1[14 * 32 + 4] = SE_BUILD(TILE_HUD_BOX_L, 2, 0, 0);
        for (int tx = 5; tx < 25; tx++) sbb1[14 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
        sbb1[14 * 32 + 25] = SE_BUILD(TILE_HUD_BOX_R, 2, 0, 0);

        sbb1[15 * 32 + 4] = SE_BUILD(TILE_HUD_BOX_BL, 2, 0, 0);
        for (int tx = 5; tx < 25; tx++) sbb1[15 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BTM, 2, 0, 0);
        sbb1[15 * 32 + 25] = SE_BUILD(TILE_HUD_BOX_BR, 2, 0, 0);

        // Draw Bottom Metallic HUD Box (Rows 17..19)
        sbb1[17 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_TL, 2, 0, 0);
        for (int tx = 1; tx < 29; tx++) sbb1[17 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_TOP, 2, 0, 0);
        sbb1[17 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_TR, 2, 0, 0);

        sbb1[18 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_L, 2, 0, 0);
        for (int tx = 1; tx < 29; tx++) sbb1[18 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
        sbb1[18 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_R, 2, 0, 0);

        sbb1[19 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_BL, 2, 0, 0);
        for (int tx = 1; tx < 29; tx++) sbb1[19 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BTM, 2, 0, 0);
        sbb1[19 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_BR, 2, 0, 0);
    }

    // 2. Erase text layer and write UI onto BG0
    tte_erase_screen();

    // High Score Badge directly under the Title Logo at Y = 43
    char buf[96];
    sprintf(buf, "#{P:20,43}#{ci:2}-- #{ci:1}HIGH SCORE: %07d #{ci:2}--\n", g_game.high_score);
    tte_write(buf);

    // Single pulsating call to action centered inside its box (Rows 13..15) at Y = 112
    if ((g_game.frame_count & 16) == 0) {
        tte_write("#{P:56,112}#{ci:2}>> #{ci:1}START RACE #{ci:2}<<\n");
    } else {
        tte_write("#{P:56,112}#{ci:2}>  #{ci:1}START RACE  #{ci:2}<\n");
    }

    // Bottom Metallic Box: Accumulated coins display (Rows 17..19, Y in [136, 159])
    sprintf(buf, "#{P:20,144}#{ci:3}COINS ACCUMULATED: #{ci:1}%05d\n", g_game.total_coins);
    tte_write(buf);
}

void render_title_screen(void) {
    tte_erase_screen();
    tte_write("#{P:48,4}#{ci:2}=== CHOOSE CAR ===\n");

    // Dynamic pulsating chevron banner around color name at Y = 16
    const char *col_name = g_car_color_names[g_game.car_color];
    bool is_locked = !(g_game.cars_unlocked & (1 << g_game.car_color));
    char display_name[64];
    if (is_locked) {
        sprintf(display_name, "%s [2000 C]", col_name);
    } else {
        sprintf(display_name, "%s", col_name);
    }
    int name_len = (int)strlen(display_name);
    char name_buf[128];
    int text_x = 120 - ((name_len + 6) * 8) / 2;
    if (text_x < 8) text_x = 8;
    if ((g_game.frame_count & 16) == 0) {
        sprintf(name_buf, "#{P:%d,16}#{ci:2}<< #{ci:1}%s #{ci:2}>>\n", text_x, display_name);
    } else {
        sprintf(name_buf, "#{P:%d,16}#{ci:2}<  #{ci:1}%s  #{ci:2}>\n", text_x, display_name);
    }
    tte_write(name_buf);

    // Show 2000 C price under locked cars (Pink on Row 0; Black, Yellow, Green on Row 1)
    if (!(g_game.cars_unlocked & (1 << CAR_COLOR_PINK))) {
        tte_write("#{P:176,72}#{ci:1}2000 C\n");
    }
    if (!(g_game.cars_unlocked & (1 << CAR_COLOR_BLACK))) {
        tte_write("#{P:16,123}#{ci:1}2000 C\n");
    }
    if (!(g_game.cars_unlocked & (1 << CAR_COLOR_YELLOW))) {
        tte_write("#{P:96,123}#{ci:1}2000 C\n");
    }
    if (!(g_game.cars_unlocked & (1 << CAR_COLOR_GREEN))) {
        tte_write("#{P:176,123}#{ci:1}2000 C\n");
    }

    // Bottom Metallic Box: Only "COINS COLLECTED: %05d" vertically & horizontally centered (Rows 17..19, Y = 144)
    char coins_buf[64];
    sprintf(coins_buf, "COINS COLLECTED: %05d", g_game.total_coins);
    int c_len = (int)strlen(coins_buf);
    int c_x = 120 - (c_len * 8) / 2;
    char tte_buf[96];
    sprintf(tte_buf, "#{P:%d,144}#{ci:3}COINS COLLECTED: #{ci:1}%05d\n", c_x, g_game.total_coins);
    tte_write(tte_buf);
}

void render_map_select_screen(void) {
    tte_erase_screen();
    tte_write("#{P:48,4}#{ci:2}=== SELECT TRACK ===\n");

    // Dynamic pulsating chevron banner around active map name at Y = 16
    const char *map_name = g_map_theme_names[g_game.map_theme];
    bool is_locked = !(g_game.maps_unlocked & (1 << g_game.map_theme));
    int cost = g_map_unlock_costs[g_game.map_theme];
    char display_name[64];
    if (is_locked) {
        sprintf(display_name, "%s [%d C]", map_name, cost);
    } else {
        sprintf(display_name, "%s", map_name);
    }
    int name_len = (int)strlen(display_name);
    char name_buf[128];
    int text_x = 120 - ((name_len + 6) * 8) / 2;
    if (text_x < 8) text_x = 8;
    if ((g_game.frame_count & 16) == 0) {
        sprintf(name_buf, "#{P:%d,16}#{ci:2}<< #{ci:1}%s #{ci:2}>>\n", text_x, display_name);
    } else {
        sprintf(name_buf, "#{P:%d,16}#{ci:2}<  #{ci:1}%s  #{ci:2}>\n", text_x, display_name);
    }
    tte_write(name_buf);

    // Show price under locked maps (Palm & Winter on Row 0; Orbital, Gothic, Maya on Row 1)
    if (!(g_game.maps_unlocked & (1 << MAP_BEACH))) {
        tte_write("#{P:96,72}#{ci:1}1000 C\n");
    }
    if (!(g_game.maps_unlocked & (1 << MAP_WINTER))) {
        tte_write("#{P:176,72}#{ci:1}2000 C\n");
    }
    if (!(g_game.maps_unlocked & (1 << MAP_ORBITAL))) {
        tte_write("#{P:16,123}#{ci:1}3000 C\n");
    }
    if (!(g_game.maps_unlocked & (1 << MAP_GOTHIC))) {
        tte_write("#{P:96,123}#{ci:1}4000 C\n");
    }
    if (!(g_game.maps_unlocked & (1 << MAP_MAYA))) {
        tte_write("#{P:176,123}#{ci:1}5000 C\n");
    }

    // Bottom Metallic Box: Only "COINS COLLECTED: %05d" vertically & horizontally centered (Rows 17..19, Y = 144)
    char coins_buf[64];
    sprintf(coins_buf, "COINS COLLECTED: %05d", g_game.total_coins);
    int c_len = (int)strlen(coins_buf);
    int c_x = 120 - (c_len * 8) / 2;
    char tte_buf[96];
    sprintf(tte_buf, "#{P:%d,144}#{ci:3}COINS COLLECTED: #{ci:1}%05d\n", c_x, g_game.total_coins);
    tte_write(tte_buf);
}

void render_hud(int *spr_idx_io) {
    // Bottom HUD Bar inside Beveled Box (Rows 17, 18, 19: Y in [136, 159])
    char buf[128];
    // Line 1: SCORE on left + streak multiplier, Coin icon + count on right (cleanly separated)
    if (g_game.streak_multiplier > 1) {
        sprintf(buf, "#{P:10,136}#{ci:2}SCORE:#{ci:1}%07d #{ci:2}x%d", g_game.score, g_game.streak_multiplier);
    } else {
        sprintf(buf, "#{P:10,136}#{ci:2}SCORE:#{ci:1}%07d   ", g_game.score);
    }
    tte_write(buf);
    sprintf(buf, "#{P:162,136}#{ci:1}%05d", g_game.coins_collected);
    tte_write(buf);

    // Line 2: DIST on left, Wheel icon + speed in km/h on right (cleanly separated)
    int speed_kmh = 50 + FP_TO_INT(g_game.speed * 120);
    sprintf(buf, "#{P:10,144}#{ci:3}DIST: #{ci:1}%06dm", g_game.distance_m);
    tte_write(buf);
    sprintf(buf, "#{P:162,144}#{ci:1}%3d km/h", speed_kmh);
    tte_write(buf);

    if (spr_idx_io) {
        // HUD Coin Icon Sprite at X=150, Y=139 (Priority 0, Pal 2, 8x8) - generous margin from score & aligned with text!
        if (*spr_idx_io < 126) {
            OBJ_ATTR *coin_sp = &s_obj_buffer[(*spr_idx_io)++];
            obj_set_attr(coin_sp,
                         ATTR0_SQUARE | ATTR0_4BPP,
                         ATTR1_SIZE_8x8,
                         ATTR2_BUILD(TILE_HUD_COIN, 2, 0));
            obj_set_pos(coin_sp, 150, 139);
        }

        // HUD Wheel Icon Sprite at X=150, Y=147 (Priority 0, Pal 2, 8x8) - generous margin from dist & aligned with text!
        if (*spr_idx_io < 126) {
            OBJ_ATTR *wheel_sp = &s_obj_buffer[(*spr_idx_io)++];
            obj_set_attr(wheel_sp,
                         ATTR0_SQUARE | ATTR0_4BPP,
                         ATTR1_SIZE_8x8,
                         ATTR2_BUILD(TILE_HUD_WHEEL, 2, 0));
            obj_set_pos(wheel_sp, 150, 147);
        }
    }
}

void render_gameover_screen(void) {
    tte_erase_screen();

    // Rebuild unified bottom metallic box on BG1 (remove any divider from gameplay HUD)
    SCR_ENTRY *sbb1 = se_mem[30];
    sbb1[17 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_TL, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) sbb1[17 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_TOP, 2, 0, 0);
    sbb1[17 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_TR, 2, 0, 0);

    sbb1[18 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_L, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) sbb1[18 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BG, 2, 0, 0);
    sbb1[18 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_R, 2, 0, 0);

    sbb1[19 * 32 + 0] = SE_BUILD(TILE_HUD_BOX_BL, 2, 0, 0);
    for (int tx = 1; tx < 29; tx++) sbb1[19 * 32 + tx] = SE_BUILD(TILE_HUD_BOX_BTM, 2, 0, 0);
    sbb1[19 * 32 + 29] = SE_BUILD(TILE_HUD_BOX_BR, 2, 0, 0);

    // Centered Title & Subtitle (Row 2 Y=16, Row 4 Y=32)
    tte_write("#{P:64,16}#{ci:4}** CRASHED! **\n");
    tte_write("#{P:52,32}#{ci:2}--- GAME OVER ---\n");

    // Statistics Table: Exact 22 characters per line starting at X=32 (exact 32px left/right margins)
    char buf[128];
    sprintf(buf, "#{P:32,48}FINAL SCORE:   %07d\n", g_game.score);
    tte_write(buf);
    sprintf(buf, "#{P:32,64}DISTANCE:      %06dm\n", g_game.distance_m);
    tte_write(buf);
    sprintf(buf, "#{P:32,80}COINS ACQUIRED:  +%05d\n", g_game.coins_collected);
    tte_write(buf);

    if (g_game.is_new_high_score || (g_game.score >= g_game.high_score && g_game.score > 0)) {
        // Flashing celebration fanfare text in arcade gold - 23 chars = 184px, centered at X=28, Row 12 Y=96
        if ((g_game.frame_count & 31) < 22) {
            tte_write("#{P:28,96}*** NEW HIGH SCORE! ***\n");
        }
    }

    // Centered prompt text in bottom beveled HUD box (Row 18 Y=144)
    // Left: START: RETRY (12 chars = 96px, X=16..112)
    // Right: B: TITLE (8 chars = 64px, X=148..212)
    tte_write("#{P:16,144}START: RETRY\n");
    tte_write("#{P:148,144}B: TITLE\n");
}

void render_pause_screen(void) {
    tte_write("#{P:84,54}#{ci:2}** PAUSED **\n");
    tte_write("#{P:81,74}#{ci:1}START: RESUME\n");
    tte_write("#{P:84,88}#{ci:1}B: MAIN MENU\n");
}

void render_clear_pause_text(void) {
    tte_erase_rect(0, 48, SCREEN_W, 112);
}

static inline void wait_vblank(void) {
    while (REG_DISPSTAT & 1);
    while (!(REG_DISPSTAT & 1));
}

void safe_oam_copy(void) {
    dma3_cpy(oam_mem, s_obj_buffer, 128 * sizeof(OBJ_ATTR));
}

void render_frame(void) {
    // 1. Screen Shake & Hardware Flash / Dimming
    // Screen shake affects the highway road on BG2; the bottom HUD on BG1 remains rock-solid!
    if (g_game.shake_timer > 0) {
        REG_BG2HOFS = g_game.shake_offset_x;
        REG_BG2VOFS = g_game.shake_offset_y;
    } else {
        REG_BG2HOFS = 0;
        REG_BG2VOFS = 0;
    }
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;

    // Vertically center HUD text inside the 24px bottom container
    if (g_game.state == STATE_PLAY || g_game.state == STATE_PAUSE) {
        REG_BG0VOFS = 253; // -3px offset centers the two 8px text rows within Y=136..159
    } else if (g_game.state == STATE_START) {
        REG_BG0VOFS = 255; // -1px offset centers text vertically (exact 7px top & 7px bottom padding)
    } else {
        REG_BG0VOFS = 0;
    }

    // Hardware white flash on crash / nitro, or background dimming on Game Over / Pause
    if (g_game.flash_timer > 0) {
        REG_BLDCNT = BLD_BUILD(BLD_ALL, 0, 2); // White brightness fade
        REG_BLDY = BLDY_BUILD(g_game.flash_timer > 16 ? 16 : g_game.flash_timer);
    } else if (g_game.state == STATE_GAMEOVER || g_game.state == STATE_PAUSE) {
        // Arcade Dimming: 50% fade to black (Mode 3 = Brightness Decrease) on BG2 (road)!
        // HUD on BG0 and BG1 stays bright and crisp. In pause state, also dim OBJ so car matches.
        REG_BLDCNT = BLD_BUILD(BLD_BG2 | (g_game.state == STATE_PAUSE ? BLD_OBJ : 0), 0, 3);
        REG_BLDY = BLDY_BUILD(8);
    } else {
        REG_BLDCNT = 0;
        REG_BLDY = 0;
    }

    // 2. Start Sprite List
    int spr_idx = 0;

    // 3. Render State UI
    if (g_game.state == STATE_START) {
        render_start_screen();
        update_road_map((g_game.frame_count >> 1) & 7);

        // Center Hero Supercar at X=104, Y=56 with subtle rumble
        int rumble = ((g_game.frame_count >> 2) & 1);
        int car_y = 56 + rumble;

        // 1. Pulsing Neon Ground Underglow (32x16, Sprite Palette 2, Priority 2)
        OBJ_ATTR *underglow = &s_obj_buffer[spr_idx++];
        obj_set_attr(underglow,
                     ATTR0_WIDE | ATTR0_4BPP,
                     ATTR1_SIZE_32x16,
                     ATTR2_BUILD(TILE_UNDERGLOW, 2, 2));
        obj_set_pos(underglow, 104, car_y + 18);

        // 2. Dual Nitro Exhaust Flames (16x16, Sprite Palette 2, Priority 1)
        int flame_anim = (g_game.frame_count / 3) % 4;
        int fire_tile = (flame_anim == 0) ? TILE_NITRO_FIRE_0 :
                        (flame_anim == 1) ? TILE_NITRO_FIRE_1 :
                        (flame_anim == 2) ? TILE_NITRO_FIRE_2 : TILE_NITRO_FIRE_3;
        OBJ_ATTR *flame = &s_obj_buffer[spr_idx++];
        obj_set_attr(flame,
                     ATTR0_SQUARE | ATTR0_4BPP,
                     ATTR1_SIZE_16x16,
                     ATTR2_BUILD(fire_tile, 2, 1));
        obj_set_pos(flame, 112, car_y + 24);

        // 3. Hero Supercar (32x32, Sprite Palette 4 + car_color, Priority 0)
        OBJ_ATTR *car = &s_obj_buffer[spr_idx++];
        obj_set_attr(car,
                     ATTR0_SQUARE | ATTR0_4BPP,
                     ATTR1_SIZE_32x32,
                     ATTR2_BUILD(TILE_PLAYER_STRAIGHT, 4 + g_game.car_color, 0));
        obj_set_pos(car, 104, car_y);

        for (int i = spr_idx; i < 128; i++) s_obj_buffer[i].attr0 = ATTR0_HIDE;
        safe_oam_copy();
        return;
    } else if (g_game.state == STATE_TITLE) {
        render_title_screen();
        render_title_boxes(g_game.car_color);

        // 6 Supercars in 3x2 Grid
        const int car_pos_x[3] = { 24, 104, 184 };
        const int car_pos_y[2] = { 36, 84 };

        for (int i = 0; i < CAR_COLOR_COUNT; i++) {
            int r = i / 3;
            int c = i % 3;
            int cx = car_pos_x[c];
            int cy = car_pos_y[r];

            bool selected = (i == (int)g_game.car_color);
            int idle = (selected && (g_game.frame_count & 16)) ? 1 : 0;

            OBJ_ATTR *car = &s_obj_buffer[spr_idx++];
            obj_set_attr(car,
                         ATTR0_SQUARE | ATTR0_4BPP,
                         ATTR1_SIZE_32x32,
                         ATTR2_BUILD(TILE_PLAYER_STRAIGHT, 4 + i, 0));
            obj_set_pos(car, cx, cy + idle);
        }

        for (int i = spr_idx; i < 128; i++) s_obj_buffer[i].attr0 = ATTR0_HIDE;
        safe_oam_copy();
        return;
    } else if (g_game.state == STATE_SELECT_MAP) {
        render_map_select_screen();
        render_map_boxes((int)g_game.map_theme);
        for (int i = spr_idx; i < 128; i++) s_obj_buffer[i].attr0 = ATTR0_HIDE;
        safe_oam_copy();
        return;
    } else if (g_game.state == STATE_GAMEOVER) {
        render_gameover_screen();
        // Draw crashed smoking car
        OBJ_ATTR *car = &s_obj_buffer[spr_idx++];
        obj_set_attr(car,
                     ATTR0_SQUARE | ATTR0_4BPP,
                     ATTR1_SIZE_32x32,
                     ATTR2_BUILD(TILE_PLAYER_SHRINK_1, 0, 1));
        obj_set_pos(car, FP_TO_INT(g_player.x) - 16, PLAYER_BASE_Y - 4);

        // Ground shadow below crashed car
        OBJ_ATTR *sh = &s_obj_buffer[spr_idx++];
        obj_set_attr(sh,
                     ATTR0_WIDE | ATTR0_4BPP,
                     ATTR1_SIZE_32x16,
                     ATTR2_BUILD(TILE_SHADOW, 0, 2));
        obj_set_pos(sh, FP_TO_INT(g_player.x) - 16, PLAYER_BASE_Y + 8);
        for (int i = spr_idx; i < 128; i++) s_obj_buffer[i].attr0 = ATTR0_HIDE;
        safe_oam_copy();
        return;
    } else if (g_game.state == STATE_PAUSE) {
        render_pause_screen();
        render_hud(&spr_idx);
        goto render_scene_sprites;
    } else {
        // STATE_PLAY: Render HUD and 3D Scene
        render_hud(&spr_idx);
        update_road_map(g_game.road_scroll);
    render_scene_sprites:

        // A. Foreground: Player Sports Car (OAM index 0)
        // High priority ensures car is ALWAYS drawn in front of roadblocks when leaping over them!
        bool blink = (g_player.invuln_timer > 0) && (g_game.frame_count & 4);
        if (!blink && spr_idx < 125) {
            OBJ_ATTR *car = &s_obj_buffer[spr_idx++];
            int tile = TILE_PLAYER_STRAIGHT;
            if (g_player.is_crashing) {
                // Wipeout spin animation: cycle through 4 angles every 3 frames
                int spin_step = (g_player.crash_frame / 3) % 4;
                if (spin_step == 0) tile = TILE_PLAYER_LEFT;
                else if (spin_step == 1) tile = TILE_PLAYER_SHRINK_1;
                else if (spin_step == 2) tile = TILE_PLAYER_RIGHT;
                else tile = TILE_PLAYER_STRAIGHT;
            } else if (g_player.is_shrinking) {
                // Multi-stage shrinking animation:
                // Initial compression ticks: TILE_PLAYER_SHRINK_0 (narrowing chassis)
                // Sustained compact run: TILE_PLAYER_SHRINK_1 (nano-racer micro chassis)
                tile = (g_player.shrink_timer > 40) ? TILE_PLAYER_SHRINK_0 : TILE_PLAYER_SHRINK_1;
            } else if (g_player.spinout_timer > 0) {
                // Directional spinout rotation cycle based on slide direction!
                int spin_step = (g_player.spinout_timer / 3) % 4;
                if (g_player.tilt >= 0) {
                    // Sliding right: clockwise spin (RIGHT -> SHRINK_1 -> LEFT -> STRAIGHT)
                    if (spin_step == 0) tile = TILE_PLAYER_RIGHT;
                    else if (spin_step == 3) tile = TILE_PLAYER_SHRINK_1;
                    else if (spin_step == 2) tile = TILE_PLAYER_LEFT;
                    else tile = TILE_PLAYER_STRAIGHT;
                } else {
                    // Sliding left: counter-clockwise spin (LEFT -> SHRINK_1 -> RIGHT -> STRAIGHT)
                    if (spin_step == 0) tile = TILE_PLAYER_LEFT;
                    else if (spin_step == 3) tile = TILE_PLAYER_SHRINK_1;
                    else if (spin_step == 2) tile = TILE_PLAYER_RIGHT;
                    else tile = TILE_PLAYER_STRAIGHT;
                }
            } else if (g_player.is_jumping) {
                // Dynamic pitch animation during jump
                if (g_player.vy > 0) {
                    tile = TILE_PLAYER_JUMP_UP;   // Nose up, suspension extended
                } else {
                    tile = TILE_PLAYER_JUMP_DOWN; // Nose down, downforce spoiler
                }
            } else if (g_player.tilt < 0) {
                tile = TILE_PLAYER_LEFT;
            } else if (g_player.tilt > 0) {
                tile = TILE_PLAYER_RIGHT;
            }

            int car_prio = g_player.is_jumping ? 1 : 2;
            int car_attr0 = ATTR0_SQUARE | ATTR0_4BPP | (g_game.state == STATE_PAUSE ? ATTR0_BLEND : 0);
            int car_pal = g_player.shield_active ? 3 : 0; // Turns into animated metallic grey when shield is up!
            obj_set_attr(car,
                         car_attr0,
                         ATTR1_SIZE_32x32,
                         ATTR2_BUILD(tile, car_pal, car_prio));
            int visual_lift = (FP_TO_INT(g_player.y_offset) * 4) / 9; // ~20px balanced arcade jump lift
            int px = FP_TO_INT(g_player.x) - 16;
            int py = PLAYER_BASE_Y - visual_lift - 16;
            if (g_player.landing_squash > 0) py += 2; // suspension compression dip
            if (g_player.is_shrinking) py += 2;      // aerodynamic crouch compression dip
            obj_set_pos(car, px, py);

            // Animate Steel Grey Car Palette while Shield is Active (Palette Bank 3: pal_obj_mem[48..63])
            if (g_player.shield_active) {
                int f = (g_game.frame_count / 4) % 4;
                dma3_cpy(&pal_obj_mem[48], g_steel_car_pals[f], 16 * sizeof(COLOR));
            }

            // Animated Blazing Nitro Exhaust Fire shooting from dual tailpipes behind car (Sprite Palette 2)
            if (g_player.nitro_timer > 0 && spr_idx < 126) {
                int fire_f = (g_game.frame_count / 2) % 4;
                int fire_tile = (fire_f == 0) ? TILE_NITRO_FIRE_0 :
                                (fire_f == 1) ? TILE_NITRO_FIRE_1 :
                                (fire_f == 2) ? TILE_NITRO_FIRE_2 : TILE_NITRO_FIRE_3;
                OBJ_ATTR *fire = &s_obj_buffer[spr_idx++];
                obj_set_attr(fire,
                             ATTR0_SQUARE | ATTR0_4BPP,
                             ATTR1_SIZE_16x16,
                             ATTR2_BUILD(fire_tile, 2, car_prio)); // Sprite Palette 2 (fire)
                obj_set_pos(fire, px + 8, py + 26); // Erupting directly behind the rear chrome exhaust pipes!
            }
        }

        // B. Obstacles (Constant size with true spatial depth & clearance states!)
        int spr_pal = (g_game.map_theme == MAP_BEACH)   ? 10 :
                      (g_game.map_theme == MAP_WINTER)  ? 11 :
                      (g_game.map_theme == MAP_ORBITAL) ? 12 :
                      (g_game.map_theme == MAP_GOTHIC)  ? 13 :
                      (g_game.map_theme == MAP_MAYA)    ? 14 : 1;

        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (!g_obstacles[i].active || spr_idx >= 120) continue;

            int tile;
            int attr0, attr1, ox, oy, prio;

            if (g_obstacles[i].type == OBS_LOW_BARRIER) {
                // Constant 32x16 barrier (turns green upon clearance!)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x16;
                bool cleared = (g_obstacles[i].hurdled || g_obstacles[i].passed);
                if (g_game.map_theme == MAP_BEACH) {
                    tile = cleared ? TILE_OBS_BEACH_DRIFTWOOD_PASS : TILE_OBS_BEACH_DRIFTWOOD_LRG;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = cleared ? TILE_OBS_WINT_ICE_PASS : TILE_OBS_WINT_ICE_LRG;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = cleared ? TILE_OBS_ORBIT_PIPE_PASS : TILE_OBS_ORBIT_PIPE_LRG;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = cleared ? TILE_OBS_GOTH_CRYPT_PASS : TILE_OBS_GOTH_CRYPT_LRG;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = cleared ? TILE_OBS_MAYA_TOTEM_PASS : TILE_OBS_MAYA_TOTEM_LRG;
                } else {
                    tile = cleared ? TILE_OBS_BARRIER_PASS : TILE_OBS_BARRIER_LRG;
                }
                prio = 2; // Ground level (airborne car leaps OVER barrier)
                ox = 16; oy = 8;
            } else if (g_obstacles[i].type == OBS_HIGH_SIGN) {
                // Constant 32x32 overhead gantry (turns green upon shrink clearance!)
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x32;
                bool cleared = g_obstacles[i].passed;
                if (g_game.map_theme == MAP_BEACH) {
                    tile = cleared ? TILE_OBS_BEACH_TIKI_PASS : TILE_OBS_BEACH_TIKI_LRG;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = cleared ? TILE_OBS_WINT_CABLE_PASS : TILE_OBS_WINT_CABLE_LRG;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = cleared ? TILE_OBS_ORBIT_GANTRY_PASS : TILE_OBS_ORBIT_GANTRY_LRG;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = cleared ? TILE_OBS_GOTH_GATE_PASS : TILE_OBS_GOTH_GATE_LRG;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = cleared ? TILE_OBS_MAYA_ARCH_PASS : TILE_OBS_MAYA_ARCH_LRG;
                } else {
                    tile = cleared ? TILE_OBS_SIGN_PASS : TILE_OBS_SIGN_LRG;
                }
                prio = 1; // Overhead suspended in air (sliding car passes UNDER sign)
                ox = 16; oy = 16;
            } else if (g_obstacles[i].type == OBS_CIVILIAN_CAR) {
                // Cruising civilian traffic
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x32;
                if (g_game.map_theme == MAP_BEACH) {
                    tile = TILE_OBS_BEACH_CIVILIAN_LRG;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = TILE_OBS_WINT_CIVILIAN_LRG;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = TILE_OBS_ORBIT_ROVER_LRG;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = TILE_OBS_GOTH_JALOPY_LRG;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = TILE_OBS_MAYA_JEEP_LRG;
                } else {
                    tile = TILE_OBS_CIVILIAN_LRG;
                }
                prio = 2; // Ground level
                ox = 16; oy = 16;
            } else if (g_obstacles[i].type == OBS_TRUCK) {
                // Constant 32x32 heavy truck / hauler
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x32;
                if (g_game.map_theme == MAP_BEACH) {
                    tile = TILE_OBS_BEACH_TRUCK_LRG;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = TILE_OBS_WINT_TRUCK_LRG;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = TILE_OBS_ORBIT_CRAWLER_LRG;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = TILE_OBS_GOTH_HEARSE_LRG;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = TILE_OBS_MAYA_TRUCK_LRG;
                } else {
                    tile = TILE_OBS_TRUCK_LRG;
                }
                prio = 2; // Ground level
                ox = 16; oy = 16;
            } else if (g_obstacles[i].type == OBS_RAMP) {
                // Mechanic-Expanding Stunt Launch Ramp (32x16)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x16;
                if (g_game.map_theme == MAP_BEACH) {
                    tile = TILE_OBS_RAMP_BEACH;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = TILE_OBS_RAMP_WINTER;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = TILE_OBS_RAMP_ORBITAL;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = TILE_OBS_RAMP_GOTHIC;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = TILE_OBS_RAMP_MAYA;
                } else {
                    tile = TILE_OBS_RAMP_CITY;
                }
                prio = 2; // Ground level
                ox = 16; oy = 8;
            } else if (g_obstacles[i].type == OBS_SLICK) {
                // Surface Friction Hazard (32x16, flat on asphalt)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_32x16;
                if (g_game.map_theme == MAP_BEACH) {
                    tile = TILE_OBS_SLICK_BEACH;
                } else if (g_game.map_theme == MAP_WINTER) {
                    tile = TILE_OBS_SLICK_WINTER;
                } else if (g_game.map_theme == MAP_ORBITAL) {
                    tile = TILE_OBS_SLICK_ORBITAL;
                } else if (g_game.map_theme == MAP_GOTHIC) {
                    tile = TILE_OBS_SLICK_GOTHIC;
                } else if (g_game.map_theme == MAP_MAYA) {
                    tile = TILE_OBS_SLICK_MAYA;
                } else {
                    tile = TILE_OBS_SLICK_CITY;
                }
                prio = 3; // Surface asphalt level
                ox = 16; oy = 8;
            } else if (g_obstacles[i].type == OBS_SIGNATURE) {
                if (g_game.map_theme == MAP_CITY) {
                    // Heavy Industrial Chemical Tanker (64x32, 32 tiles, spans 2 lanes!)
                    attr0 = ATTR0_WIDE | ATTR0_4BPP;
                    attr1 = ATTR1_SIZE_64x32;
                    tile = TILE_OBS_SIG_CITY;
                    prio = 2;
                    ox = 32; oy = 16;
                } else {
                    // Track-Specific Signature Hazard (32x32)
                    attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                    attr1 = ATTR1_SIZE_32x32;
                    if (g_game.map_theme == MAP_BEACH) {
                        tile = TILE_OBS_SIG_BEACH;
                        prio = 2;
                    } else if (g_game.map_theme == MAP_WINTER) {
                        tile = TILE_OBS_SIG_WINTER;
                        prio = 2;
                    } else if (g_game.map_theme == MAP_ORBITAL) {
                        // Cycles between 0 (active lethal plasma laser) and 1 (safe pass window)
                        tile = (g_obstacles[i].hazard_state == 0) ? TILE_OBS_SIG_ORBITAL_ON : TILE_OBS_SIG_ORBITAL_OFF;
                        prio = 2;
                    } else if (g_game.map_theme == MAP_GOTHIC) {
                        tile = TILE_OBS_SIG_GOTHIC;
                        prio = 2; // Low stone gargoyle perched on road
                    } else if (g_game.map_theme == MAP_MAYA) {
                        tile = TILE_OBS_SIG_MAYA;
                        prio = 1; // Overhead whistling poison dart stream
                    } else {
                        tile = TILE_OBS_SIG_CITY;
                        prio = 2;
                    }
                    ox = 16; oy = 16;
                }
            } else {
                continue;
            }

            if (g_obstacles[i].screen_y - oy >= 136) continue;

            OBJ_ATTR *sp = &s_obj_buffer[spr_idx++];
            obj_set_attr(sp, attr0, attr1, ATTR2_BUILD(tile, spr_pal, prio));
            obj_set_pos(sp, g_obstacles[i].screen_x - ox, g_obstacles[i].screen_y - oy);

            // Blinking amber turn signal for lane-shifting vehicles
            if (g_obstacles[i].turn_signal_timer > 0 && (g_obstacles[i].turn_signal_timer & 8) && spr_idx < 120) {
                OBJ_ATTR *sig_sp = &s_obj_buffer[spr_idx++];
                int sig_ox = (g_obstacles[i].target_lane > g_obstacles[i].lane) ? 10 : -10;
                obj_set_attr(sig_sp, ATTR0_SQUARE | ATTR0_4BPP, ATTR1_SIZE_8x8, ATTR2_BUILD(TILE_TURN_SIGNAL, 1, 2));
                obj_set_pos(sig_sp, g_obstacles[i].screen_x + sig_ox - 4, g_obstacles[i].screen_y + 4);
            }
        }

        // C. Render Coins
        for (int i = 0; i < MAX_COINS; i++) {
            if (!g_coins[i].active || spr_idx >= 120 || g_coins[i].screen_y - 8 >= 136) continue;
            OBJ_ATTR *sp = &s_obj_buffer[spr_idx++];
            int tile = TILE_COIN_F0 + g_coins[i].anim_frame * 4;
            obj_set_attr(sp,
                         ATTR0_SQUARE | ATTR0_4BPP,
                         ATTR1_SIZE_16x16,
                         ATTR2_BUILD(tile, 2, 2));
            obj_set_pos(sp, g_coins[i].screen_x - 8, g_coins[i].screen_y - 8);
        }

        // D. Render World Power-ups
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!g_world_powerups[i].active || spr_idx >= 120 || g_world_powerups[i].screen_y - 8 >= 136) continue;
            OBJ_ATTR *sp = &s_obj_buffer[spr_idx++];
            int tile = TILE_POW_NITRO;
            if (g_world_powerups[i].type == POW_MAGNET) tile = TILE_POW_MAGNET;
            if (g_world_powerups[i].type == POW_SHIELD) tile = TILE_POW_SHIELD;

            obj_set_attr(sp,
                         ATTR0_SQUARE | ATTR0_4BPP,
                         ATTR1_SIZE_16x16,
                         ATTR2_BUILD(tile, 2, 2));
            obj_set_pos(sp, g_world_powerups[i].screen_x - 8, g_world_powerups[i].screen_y - 8);
        }

        // E. Dynamic Player Drop Shadow on Asphalt
        if (spr_idx < 124) {
            OBJ_ATTR *sh = &s_obj_buffer[spr_idx++];
            int y_off = FP_TO_INT(g_player.y_offset);
            if (y_off >= 10 || g_player.is_shrinking) {
                // Small concentrated oval shadow when high in the air OR when shrunk to nano-chassis
                obj_set_attr(sh,
                             ATTR0_WIDE | ATTR0_4BPP,
                             ATTR1_SIZE_16x8,
                             ATTR2_BUILD(TILE_SHADOW_SML, 0, 3));
                obj_set_pos(sh, FP_TO_INT(g_player.x) - 8, PLAYER_BASE_Y - 2);
            } else {
                // Full shadow on ground or low altitude
                obj_set_attr(sh,
                             ATTR0_WIDE | ATTR0_4BPP,
                             ATTR1_SIZE_32x16,
                             ATTR2_BUILD(TILE_SHADOW, 0, 3));
                obj_set_pos(sh, FP_TO_INT(g_player.x) - 16, PLAYER_BASE_Y - 4);
            }
        }

        // F. Render Flying Debris, Star Sparkles, and Floating Score Popups
        for (int i = 0; i < MAX_DEBRIS; i++) {
            if (!g_debris[i].active || spr_idx >= 126) continue;
            int type = g_debris[i].type;
            int attr0, attr1, tile, pal, ox, oy;

            if (type == DEBRIS_STAR_SPARKLE) {
                // Twinkling Star Sparkle (8x8)
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_8x8;
                tile = TILE_STAR_SPARKLE;
                pal = 1; // Palette 1
                ox = 4; oy = 4;
            } else if (type == DEBRIS_SNOWFLAKE) {
                // Delicate Winter Snowflake (8x8)
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_8x8;
                tile = TILE_SNOWFLAKE;
                pal = 2; // Sprite Palette 2 (pure white & crystal cyan)
                ox = 4; oy = 4;
            } else if (type == DEBRIS_POPUP_150) {
                // +150 Floating Score Popup (16x8)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_16x8;
                tile = TILE_POPUP_150;
                pal = 2; // Palette 2
                ox = 8; oy = 4;
            } else if (type == DEBRIS_POPUP_100) {
                // +100 Floating Score Popup (16x8)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_16x8;
                tile = TILE_POPUP_100;
                pal = 2;
                ox = 8; oy = 4;
            } else if (type == DEBRIS_POPUP_250) {
                // +250 Floating Score Popup (16x8)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_16x8;
                tile = TILE_POPUP_250;
                pal = 2;
                ox = 8; oy = 4;
            } else if (type == DEBRIS_POPUP_STUNT) {
                // +200 STUNT! Floating Score Popup (16x8)
                attr0 = ATTR0_WIDE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_16x8;
                tile = TILE_POPUP_STUNT;
                pal = 2;
                ox = 8; oy = 4;
            } else if (type == DEBRIS_SHIELD_DISCHARGE) {
                // Shield Deflection Shatter Burst (16x16)
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_16x16;
                tile = TILE_SHIELD_SPARK;
                pal = 2;
                ox = 8; oy = 8;
            } else {
                // Standard Friction / Debris Spark (8x8)
                attr0 = ATTR0_SQUARE | ATTR0_4BPP;
                attr1 = ATTR1_SIZE_8x8;
                tile = TILE_SPARK;
                pal = 2;
                ox = 4; oy = 4;
            }

            if (FP_TO_INT(g_debris[i].y) - oy >= 136) continue;
            int deb_prio = ((type >= DEBRIS_POPUP_150 && type <= DEBRIS_POPUP_250) || type == DEBRIS_POPUP_STUNT) ? 1 : 2;
            OBJ_ATTR *sp = &s_obj_buffer[spr_idx++];
            obj_set_attr(sp, attr0, attr1, ATTR2_BUILD(tile, pal, deb_prio));
            obj_set_pos(sp, FP_TO_INT(g_debris[i].x) - ox, FP_TO_INT(g_debris[i].y) - oy);
        }
    }

    // 4. Hide unused sprites
    for (int i = spr_idx; i < 128; i++) {
        s_obj_buffer[i].attr0 = ATTR0_HIDE;
    }
}
