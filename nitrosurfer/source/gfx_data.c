#include "gfx_data.h"
#include "common.h"
#include <stdlib.h>

#define C_RGB(r, g, b) ((COLOR)((r) | ((g) << 5) | ((b) << 10)))

// 15-bit BGR colors: C_RGB(r, g, b) where r,g,b in 0..31
const COLOR bg_pal_data[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(31, 31, 31),     // 1: Crisp White
    C_RGB(31, 28, 0),      // 2: Gold / Yellow
    C_RGB(0, 30, 28),      // 3: Neon Cyan
    C_RGB(31, 5, 5),       // 4: Vivid Red
    C_RGB(10, 10, 14),     // 5: Dark Panel BG
    C_RGB(18, 18, 22),     // 6: Panel Border
    C_RGB(20, 26, 31),     // 7: Muted Silver
    C_RGB(31, 14, 0),      // 8: Flame Orange
    C_RGB(31, 22, 4),      // 9: Radiant Amber Sun
    C_RGB(14, 28, 31),     // 10: Brilliant Electric Ice Blue
    C_RGB(4, 16, 26),      // 11: Deep Midnight Blue
    C_RGB(28, 29, 31),     // 12: Chrome Highlight Silver
    C_RGB(16, 18, 22),     // 13: Chrome Bevel Steel
    C_RGB(6, 6, 8),        // 14: Dark Shadow Charcoal
    C_RGB(1, 1, 2),        // 15: Deep Void Black

    // Palette 1: Distant Skyline & Sunset
    C_RGB(0, 0, 0),        // 0: Transparent
    C_RGB(2, 6, 20),       // 1: Deep Indigo Night Sky
    C_RGB(8, 6, 24),       // 2: Twilight Dusk
    C_RGB(16, 5, 22),      // 3: Sunset Purple
    C_RGB(26, 6, 20),      // 4: Magenta Horizon Glow
    C_RGB(31, 14, 8),      // 5: Sunset Orange
    C_RGB(6, 8, 16),       // 6: Far Skyscraper Silhouette
    C_RGB(10, 14, 22),     // 7: Mid Skyscraper Body
    C_RGB(14, 18, 26),     // 8: Near Building Body
    C_RGB(31, 28, 8),      // 9: Glowing Window Gold
    C_RGB(6, 28, 31),      // 10: Glowing Window Cyan
    C_RGB(31, 6, 8),       // 11: Radio Tower Red Light
    C_RGB(31, 31, 31),     // 12: Distant Star White
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Road Surface & Shoulders
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(3, 8, 4),        // 1: Road Shoulder Grass Dark
    C_RGB(6, 12, 6),       // 2: Road Shoulder Grass Light
    C_RGB(28, 4, 4),       // 3: Curb Red Stripe
    C_RGB(30, 30, 30),     // 4: Curb White Stripe
    C_RGB(4, 4, 5),        // 5: Dark Road Edge
    C_RGB(6, 6, 8),        // 6: Dark Asphalt
    C_RGB(9, 9, 12),       // 7: Main Asphalt
    C_RGB(12, 12, 16),     // 8: Light Asphalt / Horizon
    C_RGB(31, 31, 31),     // 9: Lane Divider Dashed White
    C_RGB(24, 22, 10),     // 10: Center Yellow Divider
    C_RGB(16, 14, 6),      // 11: Dark Yellow Divider
    C_RGB(10, 12, 16),     // 12: Border Bevel Charcoal
    C_RGB(0, 24, 26),      // 13: Border Cyan Accent Line
    C_RGB(24, 26, 28),     // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)         // 15: Solid Pitch Black Box Background
};

// Environment Theme 0: Neon Midnight (matches bg_pal_data)
const COLOR bg_pal_midnight[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(31, 14, 0), C_RGB(31, 22, 4), C_RGB(14, 28, 31), C_RGB(4, 16, 26),
    C_RGB(28, 29, 31), C_RGB(16, 18, 22), C_RGB(6, 6, 8), C_RGB(1, 1, 2),

    // Palette 1: Distant Skyline & Indigo Dusk
    C_RGB(0, 0, 0), C_RGB(2, 6, 20), C_RGB(8, 6, 24), C_RGB(16, 5, 22),
    C_RGB(26, 6, 20), C_RGB(31, 14, 8), C_RGB(6, 8, 16), C_RGB(10, 14, 22),
    C_RGB(14, 18, 26), C_RGB(31, 28, 8), C_RGB(6, 28, 31), C_RGB(31, 6, 8),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Road Surface & Shoulders
    C_RGB(0, 0, 0), C_RGB(3, 8, 4), C_RGB(6, 12, 6), C_RGB(28, 4, 4),
    C_RGB(30, 30, 30), C_RGB(4, 4, 5), C_RGB(6, 6, 8), C_RGB(9, 9, 12),
    C_RGB(12, 12, 16), C_RGB(31, 31, 31), C_RGB(24, 22, 10), C_RGB(16, 14, 6),
    C_RGB(10, 12, 16), C_RGB(0, 24, 26), C_RGB(24, 26, 28), C_RGB(1, 1, 2)
};

// Environment Theme 1: Cyber Dawn / Golden Sunset
const COLOR bg_pal_dawn[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Golden Amber Sky & Terracotta Towers
    C_RGB(0, 0, 0), C_RGB(16, 6, 2), C_RGB(24, 12, 4), C_RGB(30, 18, 6),
    C_RGB(31, 26, 12), C_RGB(31, 14, 4), C_RGB(10, 4, 12), C_RGB(16, 8, 14),
    C_RGB(20, 10, 12), C_RGB(31, 28, 10), C_RGB(6, 28, 30), C_RGB(31, 4, 4),
    C_RGB(31, 31, 28), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Warm Asphalt with Sunset Glow
    C_RGB(0, 0, 0), C_RGB(6, 8, 3), C_RGB(10, 12, 4), C_RGB(28, 6, 2),
    C_RGB(31, 28, 24), C_RGB(6, 5, 5), C_RGB(9, 7, 8), C_RGB(13, 10, 11),
    C_RGB(17, 14, 15), C_RGB(31, 31, 28), C_RGB(28, 24, 8), C_RGB(18, 14, 4),
    C_RGB(14, 12, 14), C_RGB(30, 16, 4), C_RGB(28, 26, 24), C_RGB(2, 1, 1)
};

// Environment Theme 2: High Noon Metropolis
const COLOR bg_pal_noon[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Cerulean Blue Sky & Glass Towers
    C_RGB(0, 0, 0), C_RGB(6, 16, 28), C_RGB(12, 22, 31), C_RGB(18, 26, 31),
    C_RGB(28, 30, 31), C_RGB(31, 31, 26), C_RGB(12, 14, 18), C_RGB(16, 20, 24),
    C_RGB(22, 26, 28), C_RGB(28, 31, 31), C_RGB(10, 28, 31), C_RGB(31, 4, 4),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Bright Crisp Asphalt
    C_RGB(0, 0, 0), C_RGB(4, 10, 4), C_RGB(8, 16, 6), C_RGB(26, 4, 4),
    C_RGB(31, 31, 31), C_RGB(5, 5, 6), C_RGB(8, 9, 11), C_RGB(12, 13, 16),
    C_RGB(16, 17, 20), C_RGB(31, 31, 31), C_RGB(26, 24, 6), C_RGB(18, 16, 4),
    C_RGB(12, 14, 16), C_RGB(0, 26, 30), C_RGB(26, 28, 30), C_RGB(1, 1, 2)
};

// Environment Theme 3: Thunderstorm Highway
const COLOR bg_pal_storm[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Dark Tempest & Electric Clouds
    C_RGB(0, 0, 0), C_RGB(3, 4, 8), C_RGB(6, 7, 11), C_RGB(8, 10, 14),
    C_RGB(14, 20, 24), C_RGB(24, 28, 31), C_RGB(4, 5, 8), C_RGB(7, 8, 12),
    C_RGB(10, 12, 16), C_RGB(20, 28, 31), C_RGB(24, 31, 31), C_RGB(31, 2, 4),
    C_RGB(30, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Wet Reflective Asphalt
    C_RGB(0, 0, 0), C_RGB(2, 6, 4), C_RGB(4, 9, 5), C_RGB(24, 2, 4),
    C_RGB(26, 28, 30), C_RGB(3, 4, 6), C_RGB(5, 6, 9), C_RGB(7, 9, 13),
    C_RGB(11, 13, 18), C_RGB(28, 30, 31), C_RGB(20, 18, 8), C_RGB(14, 12, 6),
    C_RGB(8, 10, 14), C_RGB(0, 20, 26), C_RGB(20, 22, 26), C_RGB(1, 1, 2)
};

// Environment Theme: Palm Beach (Tropical Coastline)
const COLOR bg_pal_beach[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Sunny Tropical Coastal Sky & Distant Ocean
    C_RGB(0, 0, 0), C_RGB(10, 20, 31), C_RGB(14, 24, 31), C_RGB(20, 27, 31),
    C_RGB(31, 28, 20), C_RGB(31, 22, 10), C_RGB(0, 20, 24), C_RGB(0, 24, 26),
    C_RGB(0, 28, 28), C_RGB(31, 31, 24), C_RGB(6, 28, 30), C_RGB(31, 6, 6),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Golden Sand, Turquoise Waves, Tropical Asphalt
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(26, 20, 8),       // 1: Golden Sand Dune Dark
    C_RGB(31, 26, 12),      // 2: Golden Sand Dune Light
    C_RGB(28, 8, 6),        // 3: Coral Red Curb
    C_RGB(31, 29, 24),      // 4: Warm Sandstone Curb Stripe
    C_RGB(20, 16, 8),       // 5: Wet Sand Road Edge
    C_RGB(7, 6, 8),         // 6: Dark Sunbaked Coastal Asphalt
    C_RGB(10, 9, 12),       // 7: Main Coastal Asphalt
    C_RGB(13, 12, 15),      // 8: Light Sunlit Asphalt
    C_RGB(31, 31, 31),      // 9: Lane Divider Dashed White
    C_RGB(16, 10, 4),       // 10: Palm Tree Trunk Dark Bark
    C_RGB(2, 22, 6),        // 11: Palm Frond Emerald Green
    C_RGB(10, 12, 16),      // 12: Border Bevel Charcoal
    C_RGB(0, 26, 28),       // 13: Ocean Turquoise Wave Water
    C_RGB(24, 26, 28),      // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)          // 15: Solid Pitch Black Box Background
};

// Environment Theme: Winter Snow (Alpine Frost & Powder)
const COLOR bg_pal_winter[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Alpine Twilight & Snowy Peaks
    C_RGB(0, 0, 0), C_RGB(12, 18, 26), C_RGB(16, 22, 29), C_RGB(20, 25, 31),
    C_RGB(26, 29, 31), C_RGB(31, 31, 31), C_RGB(8, 12, 18), C_RGB(12, 16, 22),
    C_RGB(16, 20, 26), C_RGB(28, 30, 31), C_RGB(18, 28, 31), C_RGB(31, 4, 6),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Crisp Powder Snowbanks, Ice Curbs, Frosted Asphalt
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(22, 25, 29),      // 1: Snowbank Soft Blue Shadow
    C_RGB(31, 31, 31),      // 2: Pure Powder White Snow
    C_RGB(8, 20, 28),       // 3: Frost Ice Blue Curb
    C_RGB(30, 31, 31),      // 4: Crisp White Curb Stripe
    C_RGB(4, 5, 7),         // 5: Frozen Road Edge
    C_RGB(5, 6, 9),         // 6: Wet Cold Dark Asphalt
    C_RGB(8, 9, 13),        // 7: Frosted Asphalt with Ice Sheen
    C_RGB(13, 16, 21),      // 8: Ice Shimmer Patch
    C_RGB(30, 31, 31),      // 9: Lane Divider Dashed White
    C_RGB(8, 6, 4),         // 10: Snowy Pine Tree Dark Bark
    C_RGB(1, 12, 5),        // 11: Snowy Pine Tree Dark Evergreen
    C_RGB(10, 12, 16),      // 12: Border Bevel Charcoal
    C_RGB(14, 28, 31),      // 13: Ice Crystal Cyan
    C_RGB(24, 26, 28),      // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)          // 15: Solid Pitch Black Box Background
};

// Environment Theme: Cape Orbital (Spaceport Launch Apron)
const COLOR bg_pal_orbital[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Twilight Spaceport Sky & Rocket Exhaust
    C_RGB(0, 0, 0), C_RGB(2, 4, 12), C_RGB(4, 8, 20), C_RGB(8, 14, 28),
    C_RGB(16, 22, 31), C_RGB(31, 31, 31), C_RGB(31, 16, 4), C_RGB(31, 24, 8),
    C_RGB(12, 16, 24), C_RGB(28, 30, 31), C_RGB(0, 26, 31), C_RGB(31, 4, 4),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Concrete Slabs, Caution Chevrons, Industrial Tarmac
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(15, 17, 20),      // 1: Grooved Concrete Dark Shadow
    C_RGB(25, 27, 29),      // 2: Clean Aerospace Concrete Apron
    C_RGB(31, 26, 0),       // 3: Hazard Caution Yellow Curb
    C_RGB(3, 3, 4),         // 4: Hazard Black Stripe Curb
    C_RGB(8, 10, 13),       // 5: Expansion Joint Gutter Edge
    C_RGB(7, 8, 10),        // 6: Dark Runway Asphalt
    C_RGB(11, 13, 16),      // 7: Main Spaceport Tarmac
    C_RGB(16, 18, 22),      // 8: Light Reflective Surface
    C_RGB(31, 31, 31),      // 9: White Taxiway Dashed Centerlines
    C_RGB(6, 12, 18),       // 10: Launch Service Tower Steel Blue
    C_RGB(31, 14, 2),       // 11: Launch Gantry Safety Orange
    C_RGB(10, 12, 16),      // 12: Border Bevel Charcoal
    C_RGB(4, 28, 31),       // 13: Liquid Oxygen Cyan Glow
    C_RGB(24, 26, 28),      // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)          // 15: Solid Pitch Black Box Background
};

// Environment Theme: Gothic Midnight (Transylvania Mist)
const COLOR bg_pal_gothic[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Transylvanian Night Mist & Full Blood Moon
    C_RGB(0, 0, 0), C_RGB(4, 2, 8), C_RGB(8, 3, 14), C_RGB(14, 5, 20),
    C_RGB(24, 8, 22), C_RGB(30, 26, 28), C_RGB(6, 4, 10), C_RGB(10, 6, 14),
    C_RGB(15, 8, 18), C_RGB(28, 24, 29), C_RGB(20, 6, 26), C_RGB(26, 2, 4),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Wet Cobblestone Highway, Blood Curbs, Graveyard Moss
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(5, 7, 5),         // 1: Graveyard Moss Dark Shadow
    C_RGB(9, 14, 9),        // 2: Wet Mossy Stone Patch
    C_RGB(20, 2, 4),        // 3: Gothic Blood Crimson Curb
    C_RGB(26, 25, 24),      // 4: Bone Ivory Curb Stripe
    C_RGB(4, 3, 5),         // 5: Cobblestone Gutter Edge
    C_RGB(6, 4, 8),         // 6: Wet Dark Violet Cobblestone
    C_RGB(9, 7, 12),        // 7: Main Purple Cobblestone Road
    C_RGB(14, 11, 18),      // 8: Moonlit Cobblestone Sheen
    C_RGB(22, 22, 25),      // 9: Ghostly Silver Dashed Centerline
    C_RGB(3, 2, 4),         // 10: Spiked Iron Fence Dark
    C_RGB(7, 5, 3),         // 11: Withered Bare Tree Bark
    C_RGB(10, 12, 16),      // 12: Border Bevel Charcoal
    C_RGB(16, 6, 26),       // 13: Eerie Phantom Mist Violet
    C_RGB(24, 26, 28),      // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)          // 15: Solid Pitch Black Box Background
};

// Environment Theme: Maya Temple Trail (Rainforest Ruins)
const COLOR bg_pal_maya[48] = {
    // Palette 0: Text HUD & UI
    C_RGB(0, 0, 0), C_RGB(31, 31, 31), C_RGB(31, 28, 0), C_RGB(0, 30, 28),
    C_RGB(31, 5, 5), C_RGB(10, 10, 14), C_RGB(18, 18, 22), C_RGB(20, 26, 31),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),
    C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 1: Jungle Canopy Mist & Ancient Stepped Pyramid
    C_RGB(0, 0, 0), C_RGB(3, 8, 5), C_RGB(6, 14, 8), C_RGB(12, 22, 14),
    C_RGB(20, 28, 18), C_RGB(31, 30, 24), C_RGB(8, 12, 7), C_RGB(14, 18, 10),
    C_RGB(18, 22, 14), C_RGB(28, 30, 24), C_RGB(4, 26, 16), C_RGB(28, 6, 4),
    C_RGB(31, 31, 31), C_RGB(0, 0, 0), C_RGB(0, 0, 0), C_RGB(0, 0, 0),

    // Palette 2: Carved Flagstone Highway, Terracotta Curbs, Emerald Rainforest
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(7, 6, 4),         // 1: Rainforest Mud Shadow
    C_RGB(2, 16, 5),        // 2: Lush Jungle Moss Green
    C_RGB(22, 6, 2),        // 3: Terracotta Clay Curb
    C_RGB(28, 25, 18),      // 4: Ancient Sandstone Curb Stripe
    C_RGB(9, 7, 5),         // 5: Overgrown Stone Edge
    C_RGB(8, 9, 8),         // 6: Dark Weathered Flagstone
    C_RGB(12, 13, 11),      // 7: Main Ancient Flagstone Road
    C_RGB(17, 18, 15),      // 8: Sunlit Mossy Flagstone
    C_RGB(29, 27, 20),      // 9: Ancient Carved Glyph Road Markers
    C_RGB(9, 5, 2),         // 10: Strangler Vine Dark Wood
    C_RGB(1, 24, 6),        // 11: Rainforest Emerald Leaves
    C_RGB(10, 12, 16),      // 12: Border Bevel Charcoal
    C_RGB(0, 26, 16),       // 13: Sacred Temple Jade Green
    C_RGB(24, 26, 28),      // 14: Border Silver/Chrome
    C_RGB(1, 1, 2)          // 15: Solid Pitch Black Box Background
};

// Map Theme Names and Subtitles
const char *g_map_theme_names[MAP_COUNT] = {
    "CITY HIGHWAY",
    "PALM BEACH",
    "WINTER SNOW",
    "CAPE ORBITAL",
    "GOTHIC MIDNIGHT",
    "MAYA TEMPLE TRAIL"
};

const char *g_map_theme_subtitles[MAP_COUNT] = {
    "NEON METROPOLIS ROADWAY",
    "COASTAL DUNES & PALMS",
    "FROZEN PASS & SNOW BANKS",
    "SPACEPORT LAUNCHPAD & CRAWLER",
    "HAUNTED MIST & GOTHIC CRYPT",
    "JUNGLE RUINS & STONE TRAIL"
};

void gfx_apply_map_theme(MapTheme theme) {
    const COLOR *target_pal = (theme == MAP_BEACH)   ? bg_pal_beach :
                              (theme == MAP_WINTER)  ? bg_pal_winter :
                              (theme == MAP_ORBITAL) ? bg_pal_orbital :
                              (theme == MAP_GOTHIC)  ? bg_pal_gothic :
                              (theme == MAP_MAYA)    ? bg_pal_maya : bg_pal_midnight;
    dma3_cpy(&pal_bg_mem[16], &target_pal[16], 32 * sizeof(COLOR));
}

void gfx_restore_title_palette(void) {
    // Restore default midnight/city palette for Banks 1 & 2 (road & City thumbnail)
    dma3_cpy(&pal_bg_mem[16], &bg_pal_midnight[16], 32 * sizeof(COLOR));
    // Restore preview palettes for all 5 additional maps in Banks 3..7:
    dma3_cpy(&pal_bg_mem[48],  &bg_pal_beach[32],   16 * sizeof(COLOR)); // Bank 3
    dma3_cpy(&pal_bg_mem[64],  &bg_pal_winter[32],  16 * sizeof(COLOR)); // Bank 4
    dma3_cpy(&pal_bg_mem[80],  &bg_pal_orbital[32], 16 * sizeof(COLOR)); // Bank 5
    dma3_cpy(&pal_bg_mem[96],  &bg_pal_gothic[32],  16 * sizeof(COLOR)); // Bank 6
    dma3_cpy(&pal_bg_mem[112], &bg_pal_maya[32],    16 * sizeof(COLOR)); // Bank 7
}

static COLOR lerp_color(COLOR c1, COLOR c2, int t, int max_t) {
    int r1 = c1 & 0x1F, g1 = (c1 >> 5) & 0x1F, b1 = (c1 >> 10) & 0x1F;
    int r2 = c2 & 0x1F, g2 = (c2 >> 5) & 0x1F, b2 = (c2 >> 10) & 0x1F;
    int r = r1 + ((r2 - r1) * t) / max_t;
    int g = g1 + ((g2 - g1) * t) / max_t;
    int b = b1 + ((b2 - b1) * t) / max_t;
    return C_RGB(r, g, b);
}

void gfx_apply_env_theme(EnvTheme theme, int transition_progress) {
    const COLOR *target_pal = (theme == ENV_MIDNIGHT) ? bg_pal_midnight :
                              (theme == ENV_DAWN) ? bg_pal_dawn :
                              (theme == ENV_NOON) ? bg_pal_noon : bg_pal_storm;

    if (transition_progress <= 0 || transition_progress >= 30) {
        dma3_cpy(&pal_bg_mem[16], &target_pal[16], 32 * sizeof(COLOR));
        return;
    }

    EnvTheme prev_theme = (theme == 0) ? 3 : (theme - 1);
    const COLOR *prev_pal = (prev_theme == ENV_MIDNIGHT) ? bg_pal_midnight :
                            (prev_theme == ENV_DAWN) ? bg_pal_dawn :
                            (prev_theme == ENV_NOON) ? bg_pal_noon : bg_pal_storm;

    for (int i = 16; i < 48; i++) {
        if ((i & 15) == 0) continue; // Keep transparent 0 intact
        pal_bg_mem[i] = lerp_color(prev_pal[i], target_pal[i], transition_progress, 30);
    }
}

const COLOR spr_pal_data[64] = {
    // Sprite Palette 0: Player Sports Car
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(28, 2, 8),       // 1: Cherry Red Chassis
    C_RGB(18, 1, 4),       // 2: Deep Crimson Shadow
    C_RGB(31, 8, 14),      // 3: Vermillion Highlight
    C_RGB(0, 28, 26),      // 4: Windshield Neon Cyan
    C_RGB(24, 31, 31),     // 5: Windshield Specular White
    C_RGB(1, 10, 14),      // 6: Deep Tint Glass
    C_RGB(31, 0, 8),       // 7: Taillight Glowing Red
    C_RGB(31, 18, 0),      // 8: Turn Signal Amber
    C_RGB(3, 3, 4),        // 9: Tire Charcoal
    C_RGB(22, 22, 24),     // 10: Wheel Rim Chrome
    C_RGB(16, 16, 18),     // 11: Metallic Spoiler / Exhaust
    C_RGB(0, 22, 31),      // 12: Nitro Blue Flame
    C_RGB(31, 31, 31),     // 13: Core White Flame
    C_RGB(2, 2, 3),        // 14: Shadow Dark
    C_RGB(0, 0, 0),        // 15: Black Outline

    // Sprite Palette 1: Obstacles (Barriers, Signs, Semi-Trucks)
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(31, 26, 6),      // 1: Hazard Warning Yellow
    C_RGB(28, 14, 0),      // 2: Warning Amber / Shadow
    C_RGB(2, 3, 5),        // 3: Hazard Black Stripe
    C_RGB(31, 31, 31),     // 4: Barrier White Stripe
    C_RGB(26, 3, 3),       // 5: Barrier Red Stripe
    C_RGB(0, 14, 24),      // 6: Semi-Truck Blue Body
    C_RGB(0, 6, 14),       // 7: Truck Deep Blue Shadow
    C_RGB(14, 26, 30),     // 8: Truck Windshield Cyan
    C_RGB(31, 2, 4),       // 9: Truck Taillight Red
    C_RGB(26, 28, 30),     // 10: Chrome Bumpers / Door Locks
    C_RGB(4, 18, 14),      // 11: Highway Sign Green
    C_RGB(2, 10, 8),       // 12: Highway Sign Dark Green
    C_RGB(31, 28, 8),      // 13: Sign Warning Text Gold
    C_RGB(12, 13, 15),     // 14: Steel Truss Grey
    C_RGB(0, 0, 0),        // 15: Outline

    // Sprite Palette 2: Coins & Power-ups
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(31, 27, 0),      // 1: Bright Gold
    C_RGB(26, 18, 0),      // 2: Deep Amber Gold
    C_RGB(31, 31, 20),     // 3: Coin Shimmer White
    C_RGB(16, 10, 0),      // 4: Coin Dark Edge
    C_RGB(0, 16, 28),      // 5: Nitro Canister Blue
    C_RGB(28, 10, 4),      // 6: Nitro Flame Orange
    C_RGB(26, 28, 30),     // 7: Metal Silver
    C_RGB(28, 4, 6),       // 8: Magnet Red
    C_RGB(28, 28, 28),     // 9: Magnet Tip Silver
    C_RGB(0, 26, 30),      // 10: Shield Cyan Glow
    C_RGB(20, 31, 31),     // 11: Shield Inner White
    C_RGB(31, 30, 12),     // 12: Spark Light Yellow
    C_RGB(31, 16, 0),      // 13: Spark Orange
    C_RGB(14, 14, 16),     // 14: Smoke Grey
    C_RGB(1, 1, 2),        // 15: Outline

    // Sprite Palette 3: Steel Coat Armor Car (Animated Grey Base)
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(18, 19, 21),     // 1: Gunmetal Grey Chassis
    C_RGB(11, 12, 14),     // 2: Slate Charcoal Shadow
    C_RGB(25, 26, 28),     // 3: Brushed Silver Highlight
    C_RGB(0, 28, 26),      // 4: Windshield Neon Cyan
    C_RGB(24, 31, 31),     // 5: Windshield Specular White
    C_RGB(1, 10, 14),      // 6: Deep Tint Glass
    C_RGB(31, 0, 8),       // 7: Taillight Glowing Red
    C_RGB(31, 18, 0),      // 8: Turn Signal Amber
    C_RGB(3, 3, 4),        // 9: Tire Charcoal
    C_RGB(22, 22, 24),     // 10: Wheel Rim Chrome
    C_RGB(20, 21, 23),     // 11: Metallic Spoiler / Exhaust
    C_RGB(0, 24, 28),      // 12: Kinetic Seam Cyan
    C_RGB(31, 31, 31),     // 13: Core White
    C_RGB(2, 2, 3),        // 14: Shadow Dark
    C_RGB(0, 0, 0)         // 15: Black Outline
};

// 6 Player Sports Car Paint Colors
const char *g_car_color_names[CAR_COLOR_COUNT] = {
    "CHERRY RED",
    "COBALT BLUE",
    "NEON PINK",
    "STEALTH BLACK",
    "RACING YELLOW",
    "VIPER GREEN"
};

#define BASE_CAR_PAL(c_main, c_shadow, c_highlight) { \
    C_RGB(0, 0, 0),         /* 0: Transparent */ \
    (c_main),               /* 1: Chassis Main Color */ \
    (c_shadow),             /* 2: Deep Shadow / Underbody */ \
    (c_highlight),          /* 3: Highlight / Flare */ \
    C_RGB(0, 28, 26),       /* 4: Windshield Neon Cyan */ \
    C_RGB(24, 31, 31),      /* 5: Windshield Specular White */ \
    C_RGB(1, 10, 14),       /* 6: Deep Tint Glass */ \
    C_RGB(31, 0, 8),        /* 7: Taillight Glowing Red */ \
    C_RGB(31, 18, 0),       /* 8: Turn Signal Amber */ \
    C_RGB(3, 3, 4),         /* 9: Tire Charcoal */ \
    C_RGB(22, 22, 24),      /* 10: Wheel Rim Chrome */ \
    C_RGB(16, 16, 18),      /* 11: Metallic Spoiler / Exhaust */ \
    C_RGB(0, 22, 31),       /* 12: Nitro Blue Flame */ \
    C_RGB(31, 31, 31),      /* 13: Core White Flame */ \
    C_RGB(2, 2, 3),         /* 14: Shadow Dark */ \
    C_RGB(0, 0, 0)          /* 15: Black Outline */ \
}

const COLOR g_car_color_pals[CAR_COLOR_COUNT][16] = {
    [CAR_COLOR_RED]    = BASE_CAR_PAL(C_RGB(28, 2, 8), C_RGB(18, 1, 4), C_RGB(31, 8, 14)),
    [CAR_COLOR_BLUE]   = BASE_CAR_PAL(C_RGB(2, 14, 28), C_RGB(1, 6, 17), C_RGB(10, 22, 31)),
    [CAR_COLOR_PINK]   = BASE_CAR_PAL(C_RGB(30, 6, 22), C_RGB(18, 2, 12), C_RGB(31, 16, 28)),
    [CAR_COLOR_BLACK]  = BASE_CAR_PAL(C_RGB(6, 6, 8), C_RGB(2, 2, 3), C_RGB(14, 15, 18)),
    [CAR_COLOR_YELLOW] = BASE_CAR_PAL(C_RGB(31, 26, 0), C_RGB(22, 16, 0), C_RGB(31, 31, 14)),
    [CAR_COLOR_GREEN]  = BASE_CAR_PAL(C_RGB(2, 24, 8), C_RGB(1, 13, 4), C_RGB(10, 31, 16))
};

void gfx_set_car_color(CarColor color) {
    if (color >= CAR_COLOR_COUNT) color = CAR_COLOR_RED;
    dma3_cpy(&pal_obj_mem[0], g_car_color_pals[color], 16 * sizeof(COLOR));
}

// Animated Grey Metallic Steel Coat Palettes (4-frame shimmer cycle for Shield)
const COLOR g_steel_car_pals[4][16] = {
    // Frame 0: Deep Gunmetal Steel
    {
        C_RGB(0, 0, 0),         // 0: Transparent
        C_RGB(18, 19, 21),      // 1: Gunmetal Grey Chassis
        C_RGB(11, 12, 14),      // 2: Slate Charcoal Shadow
        C_RGB(25, 26, 28),      // 3: Brushed Silver Highlight
        C_RGB(0, 28, 26),       // 4: Windshield Neon Cyan
        C_RGB(24, 31, 31),      // 5: Specular White
        C_RGB(1, 10, 14),       // 6: Deep Tint Glass
        C_RGB(31, 0, 8),        // 7: Taillight Red
        C_RGB(31, 18, 0),       // 8: Turn Signal Amber
        C_RGB(3, 3, 4),         // 9: Tire Charcoal
        C_RGB(22, 22, 24),      // 10: Wheel Rim Chrome
        C_RGB(20, 21, 23),      // 11: Steel Spoiler
        C_RGB(0, 24, 28),       // 12: Kinetic Seam Cyan
        C_RGB(31, 31, 31),      // 13: Core White
        C_RGB(2, 2, 3),         // 14: Shadow Dark
        C_RGB(0, 0, 0)          // 15: Black Outline
    },
    // Frame 1: Rising Chrome Sheen
    {
        C_RGB(0, 0, 0),
        C_RGB(21, 22, 24),
        C_RGB(14, 15, 17),
        C_RGB(28, 29, 31),
        C_RGB(0, 28, 26),
        C_RGB(24, 31, 31),
        C_RGB(1, 10, 14),
        C_RGB(31, 0, 8),
        C_RGB(31, 18, 0),
        C_RGB(3, 3, 4),
        C_RGB(24, 24, 26),
        C_RGB(23, 24, 26),
        C_RGB(10, 28, 31),
        C_RGB(31, 31, 31),
        C_RGB(2, 2, 3),
        C_RGB(0, 0, 0)
    },
    // Frame 2: Peak Specular Titanium Glint
    {
        C_RGB(0, 0, 0),
        C_RGB(25, 26, 28),
        C_RGB(17, 18, 20),
        C_RGB(31, 31, 31),      // Pure white metallic glint
        C_RGB(0, 28, 26),
        C_RGB(24, 31, 31),
        C_RGB(1, 10, 14),
        C_RGB(31, 0, 8),
        C_RGB(31, 18, 0),
        C_RGB(3, 3, 4),
        C_RGB(26, 26, 28),
        C_RGB(26, 27, 29),
        C_RGB(20, 31, 31),
        C_RGB(31, 31, 31),
        C_RGB(2, 2, 3),
        C_RGB(0, 0, 0)
    },
    // Frame 3: Cooling Platinum Sheen
    {
        C_RGB(0, 0, 0),
        C_RGB(21, 22, 24),
        C_RGB(14, 15, 17),
        C_RGB(28, 29, 31),
        C_RGB(0, 28, 26),
        C_RGB(24, 31, 31),
        C_RGB(1, 10, 14),
        C_RGB(31, 0, 8),
        C_RGB(31, 18, 0),
        C_RGB(3, 3, 4),
        C_RGB(24, 24, 26),
        C_RGB(23, 24, 26),
        C_RGB(10, 28, 31),
        C_RGB(31, 31, 31),
        C_RGB(2, 2, 3),
        C_RGB(0, 0, 0)
    }
};

// Sprite Palette 10: Palm Beach Themed Obstacles
const COLOR spr_pal_beach[16] = {
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(28, 23, 12),      // 1: Bamboo Light Gold / Tan Straw
    C_RGB(16, 10, 5),       // 2: Driftwood / Bamboo Shadow Brown
    C_RGB(8, 4, 2),         // 3: Dark Bark / Tiki Mahogany Brown
    C_RGB(31, 31, 30),      // 4: Crisp White Gelcoat / Coconut Flesh
    C_RGB(28, 6, 6),        // 5: Coral Red / Surfboard Stripe / Taillight
    C_RGB(0, 22, 24),       // 6: Ocean Turquoise / Surfer Wagon Aqua Body
    C_RGB(0, 12, 16),       // 7: Deep Turquoise Shadow
    C_RGB(14, 26, 28),      // 8: Tinted Marine Windshield Cyan Glass
    C_RGB(24, 16, 6),       // 9: Warm Woodgrain / Honey Oak Siding
    C_RGB(26, 27, 29),      // 10: Chrome Bumpers / Hubcaps / Metal Trailer
    C_RGB(4, 26, 12),       // 11: Emerald Sea-Glass / Tiki Clearance Green
    C_RGB(2, 14, 6),        // 12: Dark Emerald Green / Tropical Foliage
    C_RGB(31, 18, 2),       // 13: Tiki Torch Flame Orange / Surfboard Yellow
    C_RGB(4, 4, 6),         // 14: Trailer Frame Charcoal / Tire Rubber
    C_RGB(0, 0, 0)          // 15: Deep Outline Black
};

// Sprite Palette 11: Winter Snow Themed Obstacles
const COLOR spr_pal_winter[16] = {
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(16, 26, 31),      // 1: Glacial Ice Light Cyan
    C_RGB(6, 14, 22),       // 2: Deep Glacial Blue Shadow
    C_RGB(10, 6, 4),        // 3: Alpine Timber Dark Cedar Brown
    C_RGB(31, 31, 31),      // 4: Pure Powder Snow White
    C_RGB(28, 4, 4),        // 5: Safety / Ski Flag Red / Taillight
    C_RGB(31, 14, 0),       // 6: High-Vis Snowplow Orange
    C_RGB(18, 6, 0),        // 7: Dark Orange / Rust Shadow
    C_RGB(12, 22, 26),      // 8: Tinted / Frosted Windshield Cyan
    C_RGB(31, 28, 4),       // 9: Alpine High-Vis Yellow / Strobe Lamp
    C_RGB(20, 22, 24),      // 10: Steel Plow Blade / Gunmetal Silver
    C_RGB(4, 26, 12),       // 11: Clearance Auroral Green / Ski Trail Open
    C_RGB(2, 14, 8),        // 12: Dark Evergreen / Alpine SUV Green
    C_RGB(18, 12, 6),       // 13: Warm Timber Pine Tan
    C_RGB(4, 4, 6),         // 14: Knobby Snow Tire Charcoal / Chassis
    C_RGB(0, 0, 0)          // 15: Deep Outline Black
};

// Sprite Palette 12: Cape Orbital Themed Obstacles
const COLOR spr_pal_orbital[16] = {
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(2, 2, 4),         // 1: Deep Carbon Black / Tire Rubber
    C_RGB(6, 8, 13),        // 2: Dark Aerospace Steel Navy
    C_RGB(12, 15, 19),      // 3: Structural Titanium Grey
    C_RGB(20, 23, 26),      // 4: Specular Chrome / Heat Shield Plate
    C_RGB(30, 31, 31),      // 5: Clean Aerospace White
    C_RGB(31, 12, 2),       // 6: Industrial Safety Orange
    C_RGB(31, 26, 0),       // 7: Caution Hazard Yellow
    C_RGB(12, 26, 31),      // 8: Cryo Frost Pipe Cyan
    C_RGB(0, 20, 31),       // 9: Plasma Thruster Blue
    C_RGB(31, 4, 4),        // 10: Warning Beacon Red
    C_RGB(2, 31, 10),       // 11: Launch Clearance Green LED
    C_RGB(14, 28, 30),      // 12: Rover Glass Canopy Cyan
    C_RGB(22, 16, 8),       // 13: Antenna Bronze / Thruster Bell
    C_RGB(31, 31, 31),      // 14: Specular White Reflection
    C_RGB(0, 0, 0)          // 15: Deep Outline Black
};

// Sprite Palette 13: Gothic Midnight Themed Obstacles
const COLOR spr_pal_gothic[16] = {
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(2, 1, 3),         // 1: Pitch Gothic Black / Hearse Body
    C_RGB(5, 5, 7),         // 2: Crypt Stone Shadow Charcoal
    C_RGB(11, 12, 15),      // 3: Weathered Grave Slate Grey
    C_RGB(18, 19, 21),      // 4: Aged Stone Carving Light Grey
    C_RGB(26, 27, 28),      // 5: Pale Tombstone Marble / Skeleton Bone
    C_RGB(7, 7, 9),         // 6: Wrought Iron Fence Filigree Dark
    C_RGB(24, 25, 27),      // 7: Silver Hearse Trim / Chrome Grille
    C_RGB(14, 4, 20),       // 8: Velvet Casket Curtain Royal Purple
    C_RGB(17, 8, 4),        // 9: Rusty Hot-Rod Jalopy Primer Brown
    C_RGB(31, 20, 4),       // 10: Flickering Amber Gas Lamp / Headlight
    C_RGB(4, 31, 12),       // 11: Spectral Soul Flame / Pass Green
    C_RGB(22, 8, 30),       // 12: Eerie Ghostly Violet Mist Glow
    C_RGB(28, 2, 3),        // 13: Blood Red Hearse Taillight
    C_RGB(31, 31, 31),      // 14: Specular Reflection White
    C_RGB(0, 0, 0)          // 15: Deep Outline Black
};

// Sprite Palette 14: Maya Temple Trail Themed Obstacles
const COLOR spr_pal_maya[16] = {
    C_RGB(0, 0, 0),         // 0: Transparent
    C_RGB(2, 3, 2),         // 1: Deep Rainforest Jungle Shadow
    C_RGB(6, 8, 6),         // 2: Weathered Stela Basalt Dark Stone
    C_RGB(12, 14, 11),      // 3: Ancient Carved Stela Grey
    C_RGB(20, 19, 15),      // 4: Ancient Sandstone Pyramid Block
    C_RGB(27, 26, 22),      // 5: Sunlit Stone Relief Highlight
    C_RGB(11, 6, 2),        // 6: Strangler Vine Dark Root Brown
    C_RGB(2, 19, 5),        // 7: Jungle Canopy Emerald Leaf Green
    C_RGB(0, 27, 14),       // 8: Sacred Jade Gemstone Dark
    C_RGB(6, 31, 16),       // 9: Glowing Jade Clearance Portal Green
    C_RGB(21, 18, 11),      // 10: Safari Truck Khaki Canvas Cover
    C_RGB(4, 13, 5),        // 11: Expedition Army Green 4x4 Chassis
    C_RGB(27, 4, 3),        // 12: Fuel Jerrycan Safety Red / Taillight
    C_RGB(31, 22, 2),       // 13: Roof Spotlight Amber Beam
    C_RGB(31, 31, 31),      // 14: Specular Sunlight Gleam
    C_RGB(0, 0, 0)          // 15: Deep Outline Black
};

// Helper to set pixel in 4bpp 1D mapped sprite tiles
static void put_pix(u32 *tile_base, int spr_w_tiles, int x, int y, u8 color) {
    if (x < 0 || x >= spr_w_tiles * 8 || y < 0) return;
    int tx = x / 8;
    int ty = y / 8;
    int px = x % 8;
    int py = y % 8;
    int tile_idx = ty * spr_w_tiles + tx;
    u32 *tile = tile_base + tile_idx * 8;
    int shift = px * 4;
    tile[py] = (tile[py] & ~(0xFu << shift)) | (((u32)color & 0xFu) << shift);
}

// Draw a filled rectangle in sprite coordinates
static void fill_rect(u32 *tile_base, int spr_w_tiles, int x0, int y0, int w, int h, u8 color) {
    for (int y = y0; y < y0 + h; y++) {
        for (int x = x0; x < x0 + w; x++) {
            put_pix(tile_base, spr_w_tiles, x, y, color);
        }
    }
}

// Generate Player Sports Car (32x32 sprites, 4x4 tiles = 16 tiles each)
static void gen_player_car(u32 *base, int tilt_type) {
    // Clean to transparent
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    int ox = 0;
    if (tilt_type == -1) ox = -1; // Tilting left
    if (tilt_type == 1)  ox = 1;  // Tilting right

    // Car dimensions: 26 wide x 20 high, placed at bottom of 32x32 box
    int cx = 16 + ox;
    int cy = 20;

    // Chassis base / lower body
    fill_rect(base, 4, cx - 11, cy + 2, 22, 6, 1);      // Main red
    fill_rect(base, 4, cx - 10, cy + 5, 20, 3, 2);      // Dark red shadow
    fill_rect(base, 4, cx - 11, cy + 1, 22, 1, 3);      // Top highlight line

    // Rear bumper & diffuser
    fill_rect(base, 4, cx - 10, cy + 7, 20, 2, 15);     // Black bottom trim
    fill_rect(base, 4, cx - 3, cy + 7, 6, 2, 11);       // Chrome dual exhaust center

    // Wide rear tires
    fill_rect(base, 4, cx - 13, cy + 3, 3, 6, 9);       // Left tire
    fill_rect(base, 4, cx + 10, cy + 3, 3, 6, 9);       // Right tire
    put_pix(base, 4, cx - 12, cy + 6, 10);              // Left rim chrome
    put_pix(base, 4, cx + 11, cy + 6, 10);              // Right rim chrome

    // Cabin / Cockpit
    fill_rect(base, 4, cx - 8, cy - 6, 16, 8, 1);       // Red roof base
    fill_rect(base, 4, cx - 7, cy - 8, 14, 2, 3);       // Roof highlight
    fill_rect(base, 4, cx - 7, cy - 5, 14, 6, 6);       // Dark rear windshield
    fill_rect(base, 4, cx - 6, cy - 4, 12, 4, 4);       // Cyan tint glass
    put_pix(base, 4, cx - 5, cy - 3, 5);                // Specular glint
    put_pix(base, 4, cx - 4, cy - 3, 5);

    // Aerodynamic Rear Wing / Spoiler
    fill_rect(base, 4, cx - 12, cy - 2, 24, 2, 11);     // Chrome wing
    fill_rect(base, 4, cx - 11, cy - 1, 22, 1, 15);     // Wing bottom shadow
    put_pix(base, 4, cx - 8, cy, 11);                   // Wing strut left
    put_pix(base, 4, cx + 7, cy, 11);                   // Wing strut right

    // Glowing Neon Taillights
    fill_rect(base, 4, cx - 9, cy + 3, 4, 2, 7);        // Left taillight red
    fill_rect(base, 4, cx + 5, cy + 3, 4, 2, 7);        // Right taillight red
    put_pix(base, 4, cx - 9, cy + 3, 8);                // Left amber indicator
    put_pix(base, 4, cx + 8, cy + 3, 8);                // Right amber indicator
    put_pix(base, 4, cx - 8, cy + 3, 13);               // Left inner core
    put_pix(base, 4, cx + 6, cy + 3, 13);               // Right inner core

    // Subtle outline
    for (int x = cx - 11; x <= cx + 10; x++) {
        put_pix(base, 4, x, cy + 9, 15);
    }
}

// Generate Stage 1 Shrinking Compression Transition (32x32 bounding box)
// Wheels begin tucking inwards, chassis narrows to 18px with neon compression lines
static void gen_player_shrink_0(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    int cx = 16;
    int cy = 20;

    // Narrowed Chassis (18 wide x 6 high)
    fill_rect(base, 4, cx - 9, cy + 1, 18, 6, 1);      // Cherry red body
    fill_rect(base, 4, cx - 8, cy + 4, 16, 3, 2);      // Crimson shadow
    fill_rect(base, 4, cx - 9, cy, 18, 1, 3);          // Highlight line

    // Rear bumper
    fill_rect(base, 4, cx - 8, cy + 6, 16, 2, 15);     // Black bottom trim
    fill_rect(base, 4, cx - 3, cy + 6, 6, 2, 11);      // Chrome exhaust center

    // Tucking rear tires (22px total width)
    fill_rect(base, 4, cx - 11, cy + 2, 2, 6, 9);      // Left tire
    fill_rect(base, 4, cx + 9, cy + 2, 2, 6, 9);       // Right tire
    put_pix(base, 4, cx - 10, cy + 5, 10);             // Left rim chrome
    put_pix(base, 4, cx + 9, cy + 5, 10);              // Right rim chrome

    // Cockpit
    fill_rect(base, 4, cx - 7, cy - 5, 14, 6, 1);      // Red roof
    fill_rect(base, 4, cx - 6, cy - 6, 12, 1, 3);
    fill_rect(base, 4, cx - 6, cy - 4, 12, 4, 6);      // Dark glass
    fill_rect(base, 4, cx - 5, cy - 3, 10, 3, 4);      // Cyan windshield
    put_pix(base, 4, cx - 3, cy - 2, 5);

    // Rear wing
    fill_rect(base, 4, cx - 10, cy - 3, 20, 1, 11);
    put_pix(base, 4, cx - 6, cy - 2, 11);
    put_pix(base, 4, cx + 5, cy - 2, 11);

    // Taillights
    fill_rect(base, 4, cx - 8, cy + 2, 3, 2, 7);
    fill_rect(base, 4, cx + 5, cy + 2, 3, 2, 7);
    put_pix(base, 4, cx - 7, cy + 2, 13);
    put_pix(base, 4, cx + 6, cy + 2, 13);

    // Cyan nano-compression energy sparks along flanks
    put_pix(base, 4, cx - 12, cy + 3, 4);
    put_pix(base, 4, cx + 11, cy + 3, 4);
}

// Generate Stage 2 Compact Nano-Racer Sports Car (32x32 bounding box, ~60% scale)
// Low-profile, ultra-compact cyber nano-chassis designed to shoot under overhead gantries!
static void gen_player_shrink_1(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    int cx = 16;
    int cy = 21; // Low crouch ride height

    // Compact Chassis (14 wide x 5 high)
    fill_rect(base, 4, cx - 7, cy + 1, 14, 5, 1);      // Cherry red body
    fill_rect(base, 4, cx - 6, cy + 4, 12, 2, 2);      // Crimson shadow
    fill_rect(base, 4, cx - 7, cy, 14, 1, 3);          // Top highlight line

    // Rear bumper & diffuser
    fill_rect(base, 4, cx - 6, cy + 5, 12, 2, 15);     // Black bottom trim
    fill_rect(base, 4, cx - 2, cy + 5, 4, 2, 11);      // Chrome dual exhaust center

    // Tucked micro-tires (18px total width)
    fill_rect(base, 4, cx - 9, cy + 2, 2, 5, 9);       // Left tire
    fill_rect(base, 4, cx + 7, cy + 2, 2, 5, 9);       // Right tire
    put_pix(base, 4, cx - 8, cy + 4, 10);              // Left rim chrome
    put_pix(base, 4, cx + 7, cy + 4, 10);              // Right rim chrome

    // Compact Cockpit / Cabin
    fill_rect(base, 4, cx - 5, cy - 5, 10, 6, 1);      // Red roof base
    fill_rect(base, 4, cx - 4, cy - 6, 8, 1, 3);       // Roof highlight
    fill_rect(base, 4, cx - 4, cy - 4, 8, 4, 6);       // Dark rear glass
    fill_rect(base, 4, cx - 3, cy - 3, 6, 3, 4);       // Cyan windshield
    put_pix(base, 4, cx - 2, cy - 2, 5);               // Specular glint

    // Low-drag Compact Rear Wing
    fill_rect(base, 4, cx - 8, cy - 2, 16, 1, 11);     // Chrome wing
    put_pix(base, 4, cx - 5, cy - 1, 11);              // Left strut
    put_pix(base, 4, cx + 4, cy - 1, 11);              // Right strut

    // Glowing Taillights
    fill_rect(base, 4, cx - 6, cy + 2, 3, 2, 7);       // Left taillight red
    fill_rect(base, 4, cx + 3, cy + 2, 3, 2, 7);       // Right taillight red
    put_pix(base, 4, cx - 6, cy + 2, 8);               // Amber accent
    put_pix(base, 4, cx + 5, cy + 2, 8);
    put_pix(base, 4, cx - 5, cy + 2, 13);              // White core
    put_pix(base, 4, cx + 4, cy + 2, 13);

    // Bottom chassis outline
    for (int x = cx - 7; x <= cx + 6; x++) {
        put_pix(base, 4, x, cy + 7, 15);
    }

    // Twin cyan nano-exhaust micro-jets
    put_pix(base, 4, cx - 2, cy + 7, 4);
    put_pix(base, 4, cx + 1, cy + 7, 4);
}

// Generate Drop Shadow (32x16 sprite = 4x2 = 8 tiles)
static void gen_shadow(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;
    // Dark rounded oval
    for (int y = 4; y < 12; y++) {
        int w = (y == 4 || y == 11) ? 14 : ((y == 5 || y == 10) ? 20 : 24);
        int x0 = 16 - w / 2;
        for (int x = x0; x < x0 + w; x++) {
            put_pix(base, 4, x, y, 14);
        }
    }
}

// Generate Small Airborne Shadow (16x8 sprite = 2x1 = 2 tiles)
static void gen_shadow_sml(u32 *base) {
    for (int i = 0; i < 2 * 8; i++) base[i] = 0;
    // Tight dark ellipse
    for (int y = 2; y < 6; y++) {
        int w = (y == 2 || y == 5) ? 8 : 12;
        int x0 = 8 - w / 2;
        for (int x = x0; x < x0 + w; x++) {
            put_pix(base, 2, x, y, 14);
        }
    }
}

// Generate Pulsing Neon Ground Underglow (32x16 sprite = 4x2 = 8 tiles, Sprite Palette 2)
static void gen_underglow(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;
    // Ground neon reflection oval (Palette 2: 11 inner core, 10 vibrant cyan, 5 deep blue edge)
    for (int y = 3; y < 13; y++) {
        int w = (y == 3 || y == 12) ? 14 : ((y == 4 || y == 11) ? 22 : ((y == 5 || y == 10) ? 26 : 28));
        int x0 = 16 - w / 2;
        for (int x = x0; x < x0 + w; x++) {
            if (y >= 6 && y <= 9 && x >= 10 && x <= 21) {
                put_pix(base, 4, x, y, 11); // White / light cyan core
            } else if (y >= 5 && y <= 10 && x >= 6 && x <= 25) {
                put_pix(base, 4, x, y, 10); // Neon cyan glow
            } else {
                put_pix(base, 4, x, y, 5);  // Deep electric blue perimeter
            }
        }
    }
}

// Generate Airborne Ascending Sports Car (32x32, nose tilted up, suspension drooped)
static void gen_player_jump_up(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 18; // Nose higher

    // Chassis angled upwards
    fill_rect(base, 4, cx - 11, cy + 1, 22, 6, 1);
    fill_rect(base, 4, cx - 10, cy + 4, 20, 3, 2);
    fill_rect(base, 4, cx - 11, cy, 22, 1, 3);

    // Rear bumper & exhaust
    fill_rect(base, 4, cx - 10, cy + 7, 20, 2, 15);
    fill_rect(base, 4, cx - 3, cy + 7, 6, 2, 11);

    // Drooped suspension: wheels hang lower in the air!
    fill_rect(base, 4, cx - 13, cy + 4, 3, 7, 9);
    fill_rect(base, 4, cx + 10, cy + 4, 3, 7, 9);
    put_pix(base, 4, cx - 12, cy + 3, 11); // Chrome suspension coil left
    put_pix(base, 4, cx + 11, cy + 3, 11); // Chrome suspension coil right
    put_pix(base, 4, cx - 12, cy + 7, 10); // Rim
    put_pix(base, 4, cx + 11, cy + 7, 10);

    // Cockpit pitched back
    fill_rect(base, 4, cx - 8, cy - 7, 16, 7, 1);
    fill_rect(base, 4, cx - 7, cy - 9, 14, 2, 3);
    fill_rect(base, 4, cx - 7, cy - 6, 14, 5, 6);
    fill_rect(base, 4, cx - 6, cy - 5, 12, 3, 4);

    // Angled Rear Wing
    fill_rect(base, 4, cx - 12, cy - 3, 24, 2, 11);
    put_pix(base, 4, cx - 8, cy - 1, 11);
    put_pix(base, 4, cx + 7, cy - 1, 11);

    // Taillights
    fill_rect(base, 4, cx - 9, cy + 2, 4, 2, 7);
    fill_rect(base, 4, cx + 5, cy + 2, 4, 2, 7);
}

// Generate Airborne Descending Sports Car (32x32, nose tilted down preparing for touchdown)
static void gen_player_jump_down(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 21; // Nose lower

    // Chassis angled slightly down
    fill_rect(base, 4, cx - 11, cy + 2, 22, 6, 1);
    fill_rect(base, 4, cx - 10, cy + 5, 20, 3, 2);
    fill_rect(base, 4, cx - 11, cy + 1, 22, 1, 3);

    // Rear bumper
    fill_rect(base, 4, cx - 10, cy + 7, 20, 2, 15);
    fill_rect(base, 4, cx - 3, cy + 7, 6, 2, 11);

    // Wheels tucked up into arches
    fill_rect(base, 4, cx - 13, cy + 2, 3, 5, 9);
    fill_rect(base, 4, cx + 10, cy + 2, 3, 5, 9);
    put_pix(base, 4, cx - 12, cy + 5, 10);
    put_pix(base, 4, cx + 11, cy + 5, 10);

    // Cockpit pitched forward
    fill_rect(base, 4, cx - 8, cy - 5, 16, 7, 1);
    fill_rect(base, 4, cx - 7, cy - 7, 14, 2, 3);
    fill_rect(base, 4, cx - 7, cy - 4, 14, 5, 6);
    fill_rect(base, 4, cx - 6, cy - 3, 12, 3, 4);

    // High Downforce Rear Wing
    fill_rect(base, 4, cx - 12, cy - 6, 24, 2, 11);
    put_pix(base, 4, cx - 8, cy - 4, 11);
    put_pix(base, 4, cx + 7, cy - 4, 11);

    // Taillights
    fill_rect(base, 4, cx - 9, cy + 3, 4, 2, 7);
    fill_rect(base, 4, cx + 5, cy + 3, 4, 2, 7);
}

// Generate Nitro Roaring Exhaust Fire Animation (16x16 = 2x2 = 4 tiles each, Sprite Palette 2)
// Palette 2: Color 11=White Core, Color 12=Yellow Flame, Color 13=Orange Body, Color 6=Red Tip, Color 14=Smoke
static void gen_nitro_fire_0(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Frame 0: Roaring twin supersonic shock cones shooting directly downwards behind exhaust pipes
    // Left Exhaust Jet (centered at x=4)
    fill_rect(base, 2, 3, 0, 3, 11, 13); // Orange flame envelope
    fill_rect(base, 2, 3, 1, 3, 8, 12);  // Yellow flame body
    fill_rect(base, 2, 4, 0, 1, 6, 11);  // White-hot core
    put_pix(base, 2, 4, 11, 13);
    put_pix(base, 2, 4, 12, 6);          // Crimson tip
    put_pix(base, 2, 2, 4, 6);           // Side lick
    put_pix(base, 2, 5, 7, 6);

    // Right Exhaust Jet (centered at x=11)
    fill_rect(base, 2, 10, 0, 3, 11, 13);
    fill_rect(base, 2, 10, 1, 3, 8, 12);
    fill_rect(base, 2, 11, 0, 1, 6, 11);
    put_pix(base, 2, 11, 11, 13);
    put_pix(base, 2, 11, 12, 6);
    put_pix(base, 2, 9, 7, 6);
    put_pix(base, 2, 12, 4, 6);
}

static void gen_nitro_fire_1(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Frame 1: Turbulent expanding fireball blast
    // Left Jet
    fill_rect(base, 2, 2, 0, 5, 13, 13); // Wide orange pulse
    fill_rect(base, 2, 3, 1, 3, 9, 12);  // Wide yellow pulse
    fill_rect(base, 2, 3, 1, 2, 5, 11);  // Core white
    put_pix(base, 2, 4, 13, 6);
    put_pix(base, 2, 1, 5, 6);
    put_pix(base, 2, 2, 9, 6);

    // Right Jet
    fill_rect(base, 2, 9, 0, 5, 13, 13);
    fill_rect(base, 2, 10, 1, 3, 9, 12);
    fill_rect(base, 2, 11, 1, 2, 5, 11);
    put_pix(base, 2, 11, 13, 6);
    put_pix(base, 2, 14, 5, 6);
    put_pix(base, 2, 13, 9, 6);

    // Center cross-pipe turbulent heat wash
    fill_rect(base, 2, 6, 1, 4, 3, 13);
    put_pix(base, 2, 7, 2, 12);
    put_pix(base, 2, 8, 2, 12);
}

static void gen_nitro_fire_2(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Frame 2: Elongated roaring blowout with crackling sparks
    // Left Jet reaches Y=15
    fill_rect(base, 2, 3, 0, 3, 14, 13);
    fill_rect(base, 2, 3, 0, 3, 9, 12);
    fill_rect(base, 2, 4, 0, 1, 7, 11);
    put_pix(base, 2, 4, 14, 6);
    put_pix(base, 2, 3, 15, 6);
    put_pix(base, 2, 1, 8, 13); // Flying spark

    // Right Jet reaches Y=14
    fill_rect(base, 2, 10, 0, 3, 13, 13);
    fill_rect(base, 2, 10, 0, 3, 8, 12);
    fill_rect(base, 2, 11, 0, 1, 6, 11);
    put_pix(base, 2, 11, 13, 6);
    put_pix(base, 2, 12, 14, 6);
    put_pix(base, 2, 14, 7, 13); // Flying spark
}

static void gen_nitro_fire_3(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Frame 3: Concentrated high-velocity needle thrust plume
    // Left Jet
    fill_rect(base, 2, 3, 0, 3, 12, 13);
    fill_rect(base, 2, 4, 0, 2, 10, 12);
    fill_rect(base, 2, 4, 1, 1, 7, 11);
    put_pix(base, 2, 4, 12, 6);
    put_pix(base, 2, 5, 13, 13);

    // Right Jet
    fill_rect(base, 2, 10, 0, 3, 12, 13);
    fill_rect(base, 2, 10, 0, 2, 10, 12);
    fill_rect(base, 2, 11, 1, 1, 7, 11);
    put_pix(base, 2, 11, 12, 6);
    put_pix(base, 2, 10, 13, 13);
}

// Generate Nitro Supersonic Shockwave Burst (32x32 = 4x4 = 16 tiles)
static void gen_nitro_burst(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    // Expanding circular shockwave ring centered at (15.5, 15.5)
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            int dx = x * 2 - 31;
            int dy = y * 2 - 31;
            int dist2 = dx * dx + dy * dy;
            if (dist2 >= 550 && dist2 <= 760) {
                put_pix(base, 4, x, y, 12); // Blue plasma ring
                if (dist2 >= 610 && dist2 <= 710) {
                    put_pix(base, 4, x, y, 13); // Core white ring
                }
            }
        }
    }
    // 8 Supersonic expansion spikes
    fill_rect(base, 4, 15, 0, 2, 5, 13);   // Top
    fill_rect(base, 4, 15, 27, 2, 5, 13);  // Bottom
    fill_rect(base, 4, 0, 15, 5, 2, 13);   // Left
    fill_rect(base, 4, 27, 15, 5, 2, 13);  // Right
    for (int d = 1; d <= 4; d++) {
        put_pix(base, 4, 3 + d, 3 + d, 13);
        put_pix(base, 4, 28 - d, 3 + d, 13);
        put_pix(base, 4, 3 + d, 28 - d, 13);
        put_pix(base, 4, 28 - d, 28 - d, 13);
    }
}

// Generate Shield Lock-in Lattice Flash (16x16 = 2x2 = 4 tiles, Sprite Palette 2)
static void gen_shield_expand(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Expanding metallic armor locking lattice
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int dx = abs(x * 2 - 15);
            int dy = abs(y * 2 - 15);
            int m = dx + dy;
            if (m >= 11 && m <= 15) {
                put_pix(base, 2, x, y, 7); // Metal silver
                if (m >= 12 && m <= 14) {
                    put_pix(base, 2, x, y, 9); // Bright chrome
                }
            }
        }
    }
    put_pix(base, 2, 7, 0, 11);
    put_pix(base, 2, 8, 0, 11);
    put_pix(base, 2, 7, 15, 11);
    put_pix(base, 2, 8, 15, 11);
    put_pix(base, 2, 0, 7, 11);
    put_pix(base, 2, 0, 8, 11);
    put_pix(base, 2, 15, 7, 11);
    put_pix(base, 2, 15, 8, 11);
}

// Generate Shield Deflection Sparks (16x16 = 2x2 = 4 tiles)
static void gen_shield_spark(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // 4 Electric zig-zag deflection arcs radiating outward from center
    for (int d = 1; d <= 6; d++) {
        put_pix(base, 2, 7 - d, 7 - d, 11);
        put_pix(base, 2, 7 - d + 1, 7 - d, 10);
        put_pix(base, 2, 8 + d, 7 - d, 11);
        put_pix(base, 2, 8 + d - 1, 7 - d, 10);
        put_pix(base, 2, 7 - d, 8 + d, 11);
        put_pix(base, 2, 7 - d + 1, 8 + d, 10);
        put_pix(base, 2, 8 + d, 8 + d, 11);
        put_pix(base, 2, 8 + d - 1, 8 + d, 10);
    }
}

// Generate Crash & Skid Sparks (8x8 = 1 tile)
// Palette 2: 11=White Core, 12=Diamond Yellow, 13=Spark Orange
static void gen_spark(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // Bright incandescent multi-pixel star spark centered at (3, 3)
    put_pix(base, 1, 3, 3, 11); // Center white core
    put_pix(base, 1, 4, 3, 11);
    put_pix(base, 1, 3, 4, 11);
    put_pix(base, 1, 4, 4, 11);

    // Yellow inner cross
    put_pix(base, 1, 2, 3, 12);
    put_pix(base, 1, 5, 3, 12);
    put_pix(base, 1, 3, 2, 12);
    put_pix(base, 1, 4, 2, 12);
    put_pix(base, 1, 3, 5, 12);
    put_pix(base, 1, 4, 5, 12);

    // Orange glowing ember tips
    put_pix(base, 1, 1, 3, 13);
    put_pix(base, 1, 6, 4, 13);
    put_pix(base, 1, 3, 1, 13);
    put_pix(base, 1, 4, 6, 13);
    put_pix(base, 1, 2, 2, 13);
    put_pix(base, 1, 5, 5, 13);
}

// Generate Low Barrier Roadblock Large (32x16, 4x2 = 8 tiles)
static void gen_obstacle_barrier_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;
    // Amber flashing hazard lamps on top corners
    fill_rect(base, 4, 3, 0, 3, 2, 2);   // Amber lamp base
    fill_rect(base, 4, 4, 0, 1, 1, 1);   // Bright yellow core
    fill_rect(base, 4, 26, 0, 3, 2, 2);
    fill_rect(base, 4, 27, 0, 1, 1, 1);

    // Crossboard with alternating Red and White diagonal hazard stripes
    fill_rect(base, 4, 1, 2, 30, 8, 4); // White base board
    for (int x = 1; x < 31; x++) {
        for (int y = 2; y < 10; y++) {
            if ((x + y) % 8 < 4) {
                put_pix(base, 4, x, y, 5); // Red diagonal stripe
            }
        }
    }
    // Board top/bottom outline
    for (int x = 1; x < 31; x++) {
        put_pix(base, 4, x, 2, 15);
        put_pix(base, 4, x, 9, 15);
    }
    put_pix(base, 4, 1, 2, 15);
    put_pix(base, 4, 30, 2, 15);

    // Sturdy steel triangular support legs standing firmly on road (Y=10..15)
    // Left leg
    fill_rect(base, 4, 3, 10, 3, 5, 14); // Steel grey leg
    fill_rect(base, 4, 1, 14, 7, 2, 3);   // Wide rubber/charcoal base foot
    // Right leg
    fill_rect(base, 4, 26, 10, 3, 5, 14);
    fill_rect(base, 4, 24, 14, 7, 2, 3);
}

// Generate Low Barrier Roadblock Small (16x16, 2x2 = 4 tiles)
static void gen_obstacle_barrier_sml(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Amber lamp dots
    put_pix(base, 2, 2, 4, 1);
    put_pix(base, 2, 13, 4, 1);

    // White/Red striped crossboard (X: 1..14, Y: 5..10)
    fill_rect(base, 2, 1, 5, 14, 6, 4); // White base
    for (int x = 1; x < 15; x++) {
        for (int y = 5; y < 11; y++) {
            if ((x + y) % 6 < 3) {
                put_pix(base, 2, x, y, 5); // Red stripe
            }
        }
    }
    for (int x = 1; x < 15; x++) {
        put_pix(base, 2, x, 5, 15);
        put_pix(base, 2, x, 10, 15);
    }

    // Legs and feet on ground (Y=11..15)
    fill_rect(base, 2, 3, 11, 2, 4, 3);  // Left leg
    fill_rect(base, 2, 2, 14, 4, 2, 3);  // Left foot
    fill_rect(base, 2, 11, 11, 2, 4, 3); // Right leg
    fill_rect(base, 2, 10, 14, 4, 2, 3); // Right foot
}

// Generate High Clearance Overhead Sign / Gantry Large (32x32, 4x4 = 16 tiles)
static void gen_obstacle_sign_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    // Side Steel Truss Pillars (X: 0..3 and 28..31, Y: 6..31)
    fill_rect(base, 4, 0, 6, 4, 26, 14); // Left steel pillar
    fill_rect(base, 4, 28, 6, 4, 26, 14); // Right steel pillar
    // Lattice cross-hatch detail on pillars
    for (int y = 8; y < 31; y += 4) {
        put_pix(base, 4, 1, y, 10);
        put_pix(base, 4, 2, y + 2, 10);
        put_pix(base, 4, 29, y, 10);
        put_pix(base, 4, 30, y + 2, 10);
    }

    // Overhead Highway Green Sign (X: 2..29, Y: 0..10)
    fill_rect(base, 4, 2, 0, 28, 11, 11); // Green sign
    fill_rect(base, 4, 3, 1, 26, 9, 12);  // Dark green inner panel
    fill_rect(base, 4, 2, 0, 28, 1, 4);   // White top border
    fill_rect(base, 4, 2, 10, 28, 1, 4);  // White bottom border
    fill_rect(base, 4, 2, 0, 1, 11, 4);   // White left border
    fill_rect(base, 4, 29, 0, 1, 11, 4);  // White right border

    // Highway Exit Arrow Chevrons (Yellow Gold, color 13)
    fill_rect(base, 4, 8, 4, 3, 3, 13);
    put_pix(base, 4, 11, 5, 13);
    fill_rect(base, 4, 15, 4, 3, 3, 13);
    put_pix(base, 4, 18, 5, 13);
    fill_rect(base, 4, 22, 4, 3, 3, 13);
    put_pix(base, 4, 25, 5, 13);

    // Yellow/Black diagonal striped clearance warning bar (Y: 11..13)
    fill_rect(base, 4, 2, 11, 28, 3, 1); // Yellow clearance bar
    for (int x = 2; x < 30; x++) {
        for (int y = 11; y <= 13; y++) {
            if ((x + y) % 4 < 2) {
                put_pix(base, 4, x, y, 3); // Black hazard stripe
            }
        }
    }
    // Note: Y: 14..31 from X: 4..27 is completely transparent for car to slide under!
}

// Generate Overhead Sign Small (16x16, 2x2 = 4 tiles)
static void gen_obstacle_sign_sml(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Side posts
    fill_rect(base, 2, 0, 3, 2, 13, 14); // Left post
    fill_rect(base, 2, 14, 3, 2, 13, 14); // Right post

    // Green overhead sign (Y: 1..6)
    fill_rect(base, 2, 0, 1, 16, 6, 11);
    fill_rect(base, 2, 1, 2, 14, 4, 12);
    fill_rect(base, 2, 4, 3, 8, 2, 13); // Yellow arrow line

    // Yellow clearance strip (Y: 7)
    fill_rect(base, 2, 0, 7, 16, 1, 1);
    // Y: 8..15 in middle is open clearance!
}

// Generate Heavy Traffic Semi-Truck Large (32x32, 4x4 = 16 tiles)
static void gen_obstacle_truck_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    // Container box (X: 2..29, Y: 0..24)
    fill_rect(base, 4, 2, 0, 28, 25, 6);  // Blue container body
    fill_rect(base, 4, 4, 1, 24, 23, 7);  // Corrugated inner darker blue

    // Vertical corrugation grooves on trailer doors
    for (int x = 6; x <= 26; x += 3) {
        if (x == 15) continue; // Skip center door split
        for (int y = 2; y < 24; y++) {
            put_pix(base, 4, x, y, 6);
        }
    }

    // Chrome top roof rail
    fill_rect(base, 4, 2, 0, 28, 1, 10);
    // Center door split line and vertical chrome lock rods
    for (int y = 1; y < 25; y++) {
        put_pix(base, 4, 15, y, 15); // Black door gap
        put_pix(base, 4, 14, y, 10); // Left chrome rod
        put_pix(base, 4, 17, y, 10); // Right chrome rod
    }
    // Heavy locking latches
    fill_rect(base, 4, 12, 15, 3, 3, 10);
    fill_rect(base, 4, 17, 15, 3, 3, 10);

    // Big high-intensity oncoming halogen headlights (Y: 20..23)
    fill_rect(base, 4, 3, 20, 5, 4, 4);   // Left bright halogen white
    put_pix(base, 4, 3, 20, 1);           // Amber corner
    put_pix(base, 4, 5, 21, 10);          // Chrome projector ring
    fill_rect(base, 4, 24, 20, 5, 4, 4);  // Right bright halogen white
    put_pix(base, 4, 28, 20, 1);          // Amber corner
    put_pix(base, 4, 26, 21, 10);         // Chrome projector ring

    // Amber cab roof clearance marker lights (5 lights across trailer roof)
    put_pix(base, 4, 5, 0, 1);
    put_pix(base, 4, 10, 0, 1);
    put_pix(base, 4, 15, 0, 1);
    put_pix(base, 4, 21, 0, 1);
    put_pix(base, 4, 26, 0, 1);

    // Heavy rear underride bumper bar (Y: 25..26)
    fill_rect(base, 4, 2, 25, 28, 2, 10); // Chrome bumper
    for (int x = 2; x < 30; x += 4) {
        put_pix(base, 4, x, 25, 5);       // Red reflectors on bumper
        put_pix(base, 4, x + 1, 25, 4);   // White reflectors
    }

    // Mudflaps (Y: 27..28)
    fill_rect(base, 4, 3, 27, 6, 2, 3);
    fill_rect(base, 4, 23, 27, 6, 2, 3);

    // Dual wide rubber tires planted on road (Y: 28..31)
    fill_rect(base, 4, 3, 28, 6, 4, 3);   // Left wheels
    fill_rect(base, 4, 5, 29, 2, 2, 10);  // Left hub rim
    fill_rect(base, 4, 23, 28, 6, 4, 3);  // Right wheels
    fill_rect(base, 4, 25, 29, 2, 2, 10); // Right hub rim
}

// Generate Semi-Truck Small (16x16, 2x2 = 4 tiles)
static void gen_obstacle_truck_sml(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    // Blue container box (X: 1..14, Y: 1..11)
    fill_rect(base, 2, 1, 1, 14, 11, 6);
    fill_rect(base, 2, 2, 2, 12, 9, 7);
    fill_rect(base, 2, 1, 1, 14, 1, 10); // Top chrome rail

    // Amber roof clearance marker lights (3 lights across roof row 0)
    // Instantly identifies tall freight truck even from far horizon!
    put_pix(base, 2, 3, 0, 1);
    put_pix(base, 2, 4, 0, 1);
    put_pix(base, 2, 7, 0, 1);
    put_pix(base, 2, 8, 0, 1);
    put_pix(base, 2, 11, 0, 1);
    put_pix(base, 2, 12, 0, 1);

    // Center door line
    for (int y = 2; y < 11; y++) {
        put_pix(base, 2, 7, y, 15);
    }
    // Front oncoming halogen headlights (Y: 9..10)
    fill_rect(base, 2, 2, 9, 3, 2, 4);
    fill_rect(base, 2, 11, 9, 3, 2, 4);
    put_pix(base, 2, 2, 9, 1);
    put_pix(base, 2, 13, 9, 1);

    // Chrome bumper (Y: 11)
    fill_rect(base, 2, 1, 11, 14, 1, 10);

    // Tires on road (Y: 12..15)
    fill_rect(base, 2, 2, 12, 3, 4, 3);
    fill_rect(base, 2, 11, 12, 3, 4, 3);
}

// Generate Moving Civilian Sedan (32x32 = 16 tiles, Sprite Palette 1)
static void gen_obstacle_civilian_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Body chassis (Cruising blue sedan)
    fill_rect(base, 4, cx - 10, cy - 8, 20, 18, 6);   // Blue body
    fill_rect(base, 4, cx - 9, cy - 9, 18, 2, 7);     // Dark blue top
    fill_rect(base, 4, cx - 9, cy + 8, 18, 2, 7);     // Lower bumper shadow

    // Rear windshield (Dark cyan glass)
    fill_rect(base, 4, cx - 7, cy - 5, 14, 6, 8);
    fill_rect(base, 4, cx - 6, cy - 4, 12, 4, 7);

    // Roof panel
    fill_rect(base, 4, cx - 7, cy - 8, 14, 3, 6);

    // Taillights (Glowing red)
    fill_rect(base, 4, cx - 9, cy + 6, 4, 2, 9);
    fill_rect(base, 4, cx + 5, cy + 6, 4, 2, 9);

    // Chrome exhaust pipes
    put_pix(base, 4, cx - 7, cy + 9, 10);
    put_pix(base, 4, cx + 6, cy + 9, 10);

    // Tires
    fill_rect(base, 4, cx - 12, cy - 4, 2, 6, 3);
    fill_rect(base, 4, cx + 10, cy - 4, 2, 6, 3);
    fill_rect(base, 4, cx - 12, cy + 4, 2, 6, 3);
    fill_rect(base, 4, cx + 10, cy + 4, 2, 6, 3);

    // Chrome side mirrors
    put_pix(base, 4, cx - 11, cy - 7, 10);
    put_pix(base, 4, cx + 10, cy - 7, 10);
}

// Generate Distant Moving Civilian Sedan (16x16 = 4 tiles)
static void gen_obstacle_civilian_sml(u32 *base) {
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    int cx = 8;
    int cy = 8;

    fill_rect(base, 2, cx - 5, cy - 4, 10, 9, 6); // Blue body
    fill_rect(base, 2, cx - 4, cy - 2, 8, 3, 8);  // Cyan glass
    fill_rect(base, 2, cx - 5, cy + 4, 3, 1, 9);  // Red taillight left
    fill_rect(base, 2, cx + 2, cy + 4, 3, 1, 9);  // Red taillight right
    put_pix(base, 2, cx - 6, cy, 3);              // Tires
    put_pix(base, 2, cx + 5, cy, 3);
}

// Generate Blinking Turn Signal Indicator (8x8 = 1 tile, Sprite Palette 1)
static void gen_turn_signal(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // Bright glowing amber flashing chevron / arrow
    put_pix(base, 1, 3, 1, 13);
    put_pix(base, 1, 4, 1, 13);
    put_pix(base, 1, 2, 2, 13);
    put_pix(base, 1, 5, 2, 13);
    put_pix(base, 1, 1, 3, 13);
    put_pix(base, 1, 6, 3, 13);
    put_pix(base, 1, 0, 4, 13);
    put_pix(base, 1, 7, 4, 13);
    put_pix(base, 1, 1, 5, 2);
    put_pix(base, 1, 6, 5, 2);
    put_pix(base, 1, 2, 6, 2);
    put_pix(base, 1, 5, 6, 2);
    put_pix(base, 1, 3, 7, 2);
    put_pix(base, 1, 4, 7, 2);
}

// Generate 4 Spinning Coin Animation Frames (16x16 each, 4 tiles each)
static void gen_coins(u32 *base) {
    // Frame 0: Full circular face
    for (int i = 0; i < 4 * 8; i++) base[i] = 0;
    int cx = 8, cy = 8;
    for (int y = -5; y <= 5; y++) {
        for (int x = -5; x <= 5; x++) {
            if (x * x + y * y <= 26) {
                u8 c = (x < -1 || y < -1) ? 3 : (x > 1 || y > 1 ? 2 : 1);
                put_pix(base, 2, cx + x, cy + y, c);
            }
        }
    }
    // Inner star/dollar emboss
    put_pix(base, 2, cx, cy - 2, 4);
    put_pix(base, 2, cx, cy + 2, 4);
    put_pix(base, 2, cx - 2, cy, 4);
    put_pix(base, 2, cx + 2, cy, 4);

    // Frame 1: Three-quarters turn (elliptical)
    u32 *f1 = base + 4 * 8;
    for (int i = 0; i < 4 * 8; i++) f1[i] = 0;
    for (int y = -5; y <= 5; y++) {
        for (int x = -3; x <= 3; x++) {
            if ((x * x * 2) + (y * y) <= 26) {
                u8 c = (x < 0) ? 3 : (x > 0 ? 2 : 1);
                put_pix(f1, 2, cx + x, cy + y, c);
            }
        }
    }

    // Frame 2: Edge view (thin vertical bar with ridge)
    u32 *f2 = base + 8 * 8;
    for (int i = 0; i < 4 * 8; i++) f2[i] = 0;
    fill_rect(f2, 2, cx - 1, cy - 5, 3, 11, 1);
    fill_rect(f2, 2, cx - 1, cy - 4, 1, 9, 3);
    fill_rect(f2, 2, cx + 1, cy - 4, 1, 9, 4);

    // Frame 3: Reversed three-quarters turn
    u32 *f3 = base + 12 * 8;
    for (int i = 0; i < 4 * 8; i++) f3[i] = 0;
    for (int y = -5; y <= 5; y++) {
        for (int x = -3; x <= 3; x++) {
            if ((x * x * 2) + (y * y) <= 26) {
                u8 c = (x > 0) ? 3 : (x < 0 ? 2 : 1);
                put_pix(f3, 2, cx + x, cy + y, c);
            }
        }
    }
}

// Generate Power-up Icons (Nitro, Magnet, Shield) - 16x16 each
static void gen_powerups(u32 *base) {
    // Nitro Canister
    u32 *nitro = base;
    for (int i = 0; i < 4 * 8; i++) nitro[i] = 0;
    fill_rect(nitro, 2, 5, 4, 6, 9, 5);      // Blue bottle
    fill_rect(nitro, 2, 6, 2, 4, 2, 7);      // Silver valve
    fill_rect(nitro, 2, 6, 6, 4, 4, 6);      // Orange flame emblem
    put_pix(nitro, 2, 7, 7, 1);              // Gold flame center

    // Magnet (Red horseshoe)
    u32 *mag = base + 4 * 8;
    for (int i = 0; i < 4 * 8; i++) mag[i] = 0;
    fill_rect(mag, 2, 3, 3, 3, 10, 8);      // Left arm red
    fill_rect(mag, 2, 10, 3, 3, 10, 8);     // Right arm red
    fill_rect(mag, 2, 3, 10, 10, 3, 8);     // Bottom arch red
    fill_rect(mag, 2, 3, 2, 3, 2, 9);       // Left pole silver
    fill_rect(mag, 2, 10, 2, 3, 2, 9);      // Right pole silver

    // Shield Icon
    u32 *shield = base + 8 * 8;
    for (int i = 0; i < 4 * 8; i++) shield[i] = 0;
    for (int y = 2; y <= 13; y++) {
        int w = (y < 8) ? (y + 1) : (16 - y);
        if (w > 6) w = 6;
        fill_rect(shield, 2, 8 - w, y, w * 2, 1, 10);
    }
    fill_rect(shield, 2, 6, 4, 4, 5, 11);   // Inner white glow
}

// Generate Cleared Barrier (32x16, emerald green clearance lamps & stripes)
static void gen_obstacle_barrier_pass(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;
    // Glowing Green Clearance Lamps on top corners
    fill_rect(base, 4, 3, 0, 3, 2, 12);  // Dark green base
    fill_rect(base, 4, 4, 0, 1, 1, 11);  // Bright emerald green lamp
    fill_rect(base, 4, 26, 0, 3, 2, 12);
    fill_rect(base, 4, 27, 0, 1, 1, 11);

    // Crossboard with Green and White clearance stripes (signal "GO / CLEAR!")
    fill_rect(base, 4, 1, 2, 30, 8, 4); // White board base
    for (int x = 1; x < 31; x++) {
        for (int y = 2; y < 10; y++) {
            if ((x + y) % 8 < 4) {
                put_pix(base, 4, x, y, 11); // Green clearance stripe
            }
        }
    }
    for (int x = 1; x < 31; x++) {
        put_pix(base, 4, x, 2, 15);
        put_pix(base, 4, x, 9, 15);
    }
    put_pix(base, 4, 1, 2, 15);
    put_pix(base, 4, 30, 2, 15);

    // Steel support legs
    fill_rect(base, 4, 3, 10, 3, 5, 14);
    fill_rect(base, 4, 1, 14, 7, 2, 3);
    fill_rect(base, 4, 26, 10, 3, 5, 14);
    fill_rect(base, 4, 24, 14, 7, 2, 3);
}

// Generate Cleared Overhead Sign (32x32, glowing green chevrons & green clearance bar)
static void gen_obstacle_sign_pass(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    // Steel Pillars
    fill_rect(base, 4, 0, 6, 4, 26, 14);
    fill_rect(base, 4, 28, 6, 4, 26, 14);
    for (int y = 8; y < 31; y += 4) {
        put_pix(base, 4, 1, y, 10);
        put_pix(base, 4, 2, y + 2, 10);
        put_pix(base, 4, 29, y, 10);
        put_pix(base, 4, 30, y + 2, 10);
    }

    // Overhead Sign Panel
    fill_rect(base, 4, 2, 0, 28, 11, 11); // Vibrant green
    fill_rect(base, 4, 3, 1, 26, 9, 12);
    fill_rect(base, 4, 2, 0, 28, 1, 4);
    fill_rect(base, 4, 2, 10, 28, 1, 4);
    fill_rect(base, 4, 2, 0, 1, 11, 4);
    fill_rect(base, 4, 29, 0, 1, 11, 4);

    // Glowing White/Green Cleared Exit Chevrons
    fill_rect(base, 4, 8, 4, 3, 3, 4);  // White core
    put_pix(base, 4, 11, 5, 4);
    fill_rect(base, 4, 15, 4, 3, 3, 4);
    put_pix(base, 4, 18, 5, 4);
    fill_rect(base, 4, 22, 4, 3, 3, 4);
    put_pix(base, 4, 25, 5, 4);

    // Green/White Clearance Bar (Y: 11..13)
    fill_rect(base, 4, 2, 11, 28, 3, 11);
    for (int x = 2; x < 30; x++) {
        for (int y = 11; y <= 13; y++) {
            if ((x + y) % 4 < 2) {
                put_pix(base, 4, x, y, 4); // White stripe
            }
        }
    }
}

// =========================================================================
// PALM BEACH THEMED OBSTACLES (Sprite Palette 10)
// =========================================================================

// Generate Beach Driftwood Log Barrier Large (32x16, 4x2 = 8 tiles)
static void gen_obs_beach_driftwood_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;

    // Sandy pebble / driftwood support feet resting on road (Y: 12..15)
    fill_rect(base, 4, 3, 12, 5, 3, 2);   // Left driftwood prop
    fill_rect(base, 4, 4, 13, 3, 2, 14);  // Charcoal rock
    fill_rect(base, 4, 24, 12, 5, 3, 2);  // Right driftwood prop
    fill_rect(base, 4, 25, 13, 3, 2, 14); // Charcoal rock

    // Main Weathered Driftwood Trunk (X: 1..30, Y: 4..12)
    fill_rect(base, 4, 2, 5, 28, 7, 2);   // Deep brown wood body
    fill_rect(base, 4, 4, 6, 24, 5, 9);   // Warm honey wood grain core
    fill_rect(base, 4, 3, 5, 26, 1, 1);   // Sun-bleached top rim highlight

    // Bark furrows and knots
    for (int x = 6; x <= 26; x += 5) {
        put_pix(base, 4, x, 7, 3);
        put_pix(base, 4, x + 1, 8, 3);
        put_pix(base, 4, x + 2, 7, 2);
    }

    // Cut End Rings (Weathered log ends showing growth rings)
    fill_rect(base, 4, 1, 6, 2, 5, 9);
    put_pix(base, 4, 2, 8, 3); // Left center heartwood knot
    put_pix(base, 4, 1, 5, 15);
    put_pix(base, 4, 1, 11, 15);

    fill_rect(base, 4, 29, 6, 2, 5, 9);
    put_pix(base, 4, 29, 8, 3); // Right center knot
    put_pix(base, 4, 30, 5, 15);
    put_pix(base, 4, 30, 11, 15);

    // Tropical Coconuts perched on top (X: 13..18, Y: 2..5)
    fill_rect(base, 4, 13, 2, 3, 3, 3);
    put_pix(base, 4, 14, 2, 2);
    put_pix(base, 4, 14, 3, 4); // White husk fiber speck
    fill_rect(base, 4, 16, 3, 3, 3, 3);
    put_pix(base, 4, 17, 3, 2);

    // Wrapped Tropical Palm Vines & Fronds
    put_pix(base, 4, 8, 5, 11);
    put_pix(base, 4, 9, 6, 12);
    put_pix(base, 4, 10, 7, 11);
    put_pix(base, 4, 11, 8, 12);
    put_pix(base, 4, 10, 9, 11);
    put_pix(base, 4, 9, 10, 12);

    put_pix(base, 4, 20, 5, 11);
    put_pix(base, 4, 21, 6, 12);
    put_pix(base, 4, 22, 7, 11);
    put_pix(base, 4, 23, 8, 12);
    put_pix(base, 4, 22, 9, 11);
    put_pix(base, 4, 21, 10, 12);

    // Dark log outlines
    for (int x = 2; x < 30; x++) {
        put_pix(base, 4, x, 4, 15);
        put_pix(base, 4, x, 12, 15);
    }
}

// Generate Cleared Beach Driftwood Log (32x16, emerald sea-glass & clearance glow)
static void gen_obs_beach_driftwood_pass(u32 *base) {
    gen_obs_beach_driftwood_lrg(base);

    // Transform coconuts into glowing emerald sea-glass gems
    fill_rect(base, 4, 13, 2, 3, 3, 11);
    put_pix(base, 4, 14, 2, 4); // Bright white shimmer core
    fill_rect(base, 4, 16, 3, 3, 3, 11);
    put_pix(base, 4, 17, 3, 4);

    // Glowing sea-glass crystal inlays along the log top (Y: 5)
    for (int x = 4; x < 28; x += 3) {
        put_pix(base, 4, x, 5, 11);
        put_pix(base, 4, x + 1, 5, 4);
    }

    // Emerald glowing log ends
    put_pix(base, 4, 2, 8, 11);
    put_pix(base, 4, 29, 8, 11);
}

// Generate Beach Bamboo Tiki Pier Archway / Gantry Large (32x32, 4x4 = 16 tiles)
static void gen_obs_beach_tiki_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // Side Bamboo Pillars (Left: X: 1..3, Right: X: 28..30, Y: 6..31)
    fill_rect(base, 4, 1, 6, 3, 25, 1);   // Left bamboo stalk (gold/tan)
    fill_rect(base, 4, 28, 6, 3, 25, 1);  // Right bamboo stalk (gold/tan)
    for (int y = 6; y < 31; y++) {
        put_pix(base, 4, 3, y, 2);
        put_pix(base, 4, 28, y, 2);
    }
    // Bamboo segmented node rings
    for (int y = 8; y < 31; y += 5) {
        fill_rect(base, 4, 1, y, 3, 1, 3);
        fill_rect(base, 4, 28, y, 3, 1, 3);
    }
    // Sandstone / driftwood base pedestals
    fill_rect(base, 4, 0, 28, 5, 4, 2);
    fill_rect(base, 4, 27, 28, 5, 4, 2);
    fill_rect(base, 4, 1, 29, 3, 2, 9);
    fill_rect(base, 4, 28, 29, 3, 2, 9);

    // Overhead Thatched Roof & Lintel Beam (Y: 0..10, X: 1..30)
    fill_rect(base, 4, 2, 1, 28, 4, 1);  // Straw tan base
    fill_rect(base, 4, 4, 0, 24, 2, 9);  // Upper thatch roof peak
    for (int x = 2; x < 30; x += 2) {
        put_pix(base, 4, x, 5, 1);
        put_pix(base, 4, x + 1, 4, 9);
    }

    // Heavy Horizontal Bamboo Crossbar (Y: 6..8, X: 1..30)
    fill_rect(base, 4, 1, 6, 30, 3, 1);
    fill_rect(base, 4, 1, 8, 30, 1, 2);
    for (int x = 5; x < 28; x += 6) {
        fill_rect(base, 4, x, 6, 1, 3, 3);
    }

    // Carved Polynesian Tiki Mask in Center (X: 12..19, Y: 2..10)
    fill_rect(base, 4, 12, 2, 8, 8, 3);  // Dark mahogany carved wood
    fill_rect(base, 4, 13, 3, 6, 6, 2);  // Inner face contour
    put_pix(base, 4, 14, 4, 13);         // Glowing amber eyes
    put_pix(base, 4, 17, 4, 13);
    fill_rect(base, 4, 13, 7, 6, 2, 15); // Fierce grinning mouth
    put_pix(base, 4, 14, 7, 4);          // White carved teeth
    put_pix(base, 4, 15, 7, 4);
    put_pix(base, 4, 16, 7, 4);
    put_pix(base, 4, 17, 7, 4);

    // Blazing Tiki Torches on Outer Posts (X: 0..4 and 27..31, Y: 0..6)
    fill_rect(base, 4, 1, 4, 3, 2, 3);   // Left torch basket
    put_pix(base, 4, 2, 1, 13);
    fill_rect(base, 4, 1, 2, 3, 2, 13);  // Left flame
    put_pix(base, 4, 2, 2, 4);
    put_pix(base, 4, 1, 3, 5);
    put_pix(base, 4, 3, 3, 5);

    fill_rect(base, 4, 28, 4, 3, 2, 3);  // Right torch basket
    put_pix(base, 4, 29, 1, 13);
    fill_rect(base, 4, 28, 2, 3, 2, 13); // Right flame
    put_pix(base, 4, 29, 2, 4);
    put_pix(base, 4, 28, 3, 5);
    put_pix(base, 4, 30, 3, 5);

    // Hanging Bamboo Clearance Wind Chimes (Y: 10..13)
    fill_rect(base, 4, 2, 10, 28, 1, 3);
    for (int x = 4; x < 28; x += 4) {
        fill_rect(base, 4, x, 10, 2, 4, 1);
        put_pix(base, 4, x, 13, 2);
    }
}

// Generate Cleared Beach Tiki Archway (32x32, emerald lanterns & green clearance chimes)
static void gen_obs_beach_tiki_pass(u32 *base) {
    gen_obs_beach_tiki_lrg(base);

    // Torches become brilliant emerald sea-glass beacons
    fill_rect(base, 4, 1, 1, 3, 4, 11);
    put_pix(base, 4, 2, 2, 4);
    fill_rect(base, 4, 28, 1, 3, 4, 11);
    put_pix(base, 4, 29, 2, 4);

    // Tiki mask eyes glow friendly emerald green
    put_pix(base, 4, 14, 4, 11);
    put_pix(base, 4, 17, 4, 11);

    // Hanging chimes glow emerald green & white clearance signal
    for (int x = 4; x < 28; x += 4) {
        fill_rect(base, 4, x, 10, 2, 4, 11);
        put_pix(base, 4, x, 11, 4);
    }
}

// Generate Coastal Yacht Hauler Trailer Large (32x32, 4x4 = 16 tiles)
static void gen_obs_beach_truck_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // 1. Heavy Flatbed Trailer Chassis (Y: 22..29)
    fill_rect(base, 4, 2, 22, 28, 5, 14); // Dark charcoal trailer frame
    fill_rect(base, 4, 4, 23, 24, 2, 10); // Chrome structural frame rails

    // Trailer Dual Wheels (X: 1..4 and 27..30, Y: 22..29)
    fill_rect(base, 4, 1, 23, 4, 7, 14);
    fill_rect(base, 4, 2, 24, 2, 5, 10);  // Wheel chrome rim
    fill_rect(base, 4, 27, 23, 4, 7, 14);
    fill_rect(base, 4, 28, 24, 2, 5, 10);

    // Trailer Rear LED Tail Lamps & Clearance Markers (Y: 25..26)
    fill_rect(base, 4, 5, 25, 4, 2, 5);   // Left brake lamps (red)
    put_pix(base, 4, 5, 25, 13);          // Left amber indicator
    fill_rect(base, 4, 23, 25, 4, 2, 5);  // Right brake lamps (red)
    put_pix(base, 4, 26, 25, 13);         // Right amber indicator

    // 2. Coastal Motor Yacht Hull Transom (X: 4..27, Y: 4..22)
    fill_rect(base, 4, 5, 5, 22, 16, 4);  // Pure white gelcoat
    fill_rect(base, 4, 6, 6, 20, 14, 4);
    for (int y = 5; y <= 21; y++) {
        put_pix(base, 4, 4, y, 15);       // Left hull outline
        put_pix(base, 4, 27, y, 15);      // Right hull outline
    }

    // Ocean Turquoise Racing Hull Stripe (Y: 10..12, X: 5..26)
    fill_rect(base, 4, 5, 10, 22, 3, 6);  // Turquoise racing stripe
    fill_rect(base, 4, 5, 12, 22, 1, 7);  // Lower stripe dark shadow

    // Teak Wood Swim Platform at Transom Base (Y: 19..21, X: 6..25)
    fill_rect(base, 4, 6, 19, 20, 3, 9);  // Teak wood decking
    for (int x = 8; x <= 23; x += 3) {
        for (int y = 19; y <= 21; y++) {
            put_pix(base, 4, x, y, 3);    // Teak plank caulking grooves
        }
    }

    // Twin High-Performance Outboard Racing Motors (Y: 15..24)
    fill_rect(base, 4, 9, 15, 4, 9, 14);  // Left cowl body
    fill_rect(base, 4, 10, 16, 2, 6, 10); // Chrome cowling stripe
    put_pix(base, 4, 10, 23, 10);         // Propeller shaft
    put_pix(base, 4, 11, 23, 10);

    fill_rect(base, 4, 19, 15, 4, 9, 14); // Right cowl body
    fill_rect(base, 4, 20, 16, 2, 6, 10);
    put_pix(base, 4, 20, 23, 10);
    put_pix(base, 4, 21, 23, 10);

    // Smoked Marine Windscreen & Rail (Y: 4..8, X: 7..24)
    fill_rect(base, 4, 8, 5, 16, 4, 8);   // Tinted cyan windshield glass
    fill_rect(base, 4, 7, 4, 18, 1, 10);  // Chrome windscreen rail
    put_pix(base, 4, 7, 5, 10);
    put_pix(base, 4, 24, 5, 10);

    // Yacht Stern Mooring Cleats & Flagstaff
    put_pix(base, 4, 6, 18, 10);
    put_pix(base, 4, 25, 18, 10);
    put_pix(base, 4, 16, 3, 5);           // Red ensign pennant at top
}

// Generate Beach Woody Surfer Wagon Large (32x32, 4x4 = 16 tiles)
static void gen_obs_beach_civilian_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Classic Wagon Body Chassis (Aqua / Turquoise: col 6, col 7)
    fill_rect(base, 4, cx - 10, cy - 8, 20, 18, 6);   // Main aqua body
    fill_rect(base, 4, cx - 9, cy - 9, 18, 2, 7);     // Roof shadow
    fill_rect(base, 4, cx - 9, cy + 8, 18, 2, 7);     // Lower chassis shadow

    // Real Wood-Paneled Tailgate & Quarter Panels (Y: cy..cy+7, X: cx-8..cx+7)
    fill_rect(base, 4, cx - 8, cy, 16, 7, 9);         // Warm honey oak siding
    fill_rect(base, 4, cx - 8, cy, 16, 1, 3);         // Mahogany top border
    fill_rect(base, 4, cx - 8, cy + 6, 16, 1, 3);     // Mahogany bottom border
    fill_rect(base, 4, cx - 8, cy, 1, 7, 3);          // Mahogany left border
    fill_rect(base, 4, cx + 7, cy, 1, 7, 3);          // Mahogany right border
    fill_rect(base, 4, cx, cy, 1, 7, 3);              // Center split trim
    put_pix(base, 4, cx - 1, cy + 3, 10);             // Chrome tailgate handle

    // Rear Wrap-around Window (Tinted Marine Cyan: col 8)
    fill_rect(base, 4, cx - 7, cy - 6, 14, 5, 8);
    fill_rect(base, 4, cx - 6, cy - 5, 12, 3, 7);
    fill_rect(base, 4, cx - 7, cy - 7, 14, 1, 10);    // Chrome window trim

    // Roof Rack
    fill_rect(base, 4, cx - 8, cy - 9, 16, 1, 10);

    // Surfboard Mounted on Roof Rack (X: cx - 2..cx + 2, Y: cy - 14..cy - 2)
    fill_rect(base, 4, cx - 2, cy - 14, 5, 12, 13);   // Bright yellow surfboard
    put_pix(base, 4, cx, cy - 15, 4);                 // White nose tip
    fill_rect(base, 4, cx, cy - 14, 1, 12, 5);        // Coral red center stringer stripe
    put_pix(base, 4, cx - 2, cy - 14, 15);
    put_pix(base, 4, cx + 2, cy - 14, 15);

    // Classic Round Chrome-Bezeled Taillights (col 5 red, col 10 chrome)
    fill_rect(base, 4, cx - 10, cy + 5, 2, 3, 10);
    put_pix(base, 4, cx - 10, cy + 6, 5);
    fill_rect(base, 4, cx + 8, cy + 5, 2, 3, 10);
    put_pix(base, 4, cx + 9, cy + 6, 5);

    // Wide Retro Chrome Rear Bumper (Y: cy + 8, col 10)
    fill_rect(base, 4, cx - 11, cy + 8, 22, 2, 10);
    put_pix(base, 4, cx - 11, cy + 7, 10);
    put_pix(base, 4, cx + 10, cy + 7, 10);

    // Dual Chrome Exhaust Pipes
    put_pix(base, 4, cx - 7, cy + 10, 10);
    put_pix(base, 4, cx + 6, cy + 10, 10);

    // Whitewall Retro Tires
    fill_rect(base, 4, cx - 12, cy - 4, 2, 6, 14); // Left tire tread
    fill_rect(base, 4, cx - 11, cy - 3, 1, 4, 4);  // Whitewall ring
    fill_rect(base, 4, cx + 10, cy - 4, 2, 6, 14); // Right tire tread
    fill_rect(base, 4, cx + 10, cy - 3, 1, 4, 4);
    fill_rect(base, 4, cx - 12, cy + 4, 2, 5, 14);
    fill_rect(base, 4, cx + 10, cy + 4, 2, 5, 14);

    // Chrome Side Mirrors
    put_pix(base, 4, cx - 12, cy - 7, 10);
    put_pix(base, 4, cx + 11, cy - 7, 10);
}

// =========================================================================
// WINTER SNOW THEMED OBSTACLES (Sprite Palette 11)
// =========================================================================

// Generate Winter Ice Ridge Barrier Large (32x16, 4x2 = 8 tiles)
static void gen_obs_wint_ice_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;

    // Snowbank spreading on road surface (Y: 12..15)
    fill_rect(base, 4, 1, 13, 30, 3, 4);  // Pure powder white snow
    fill_rect(base, 4, 2, 12, 28, 2, 2);  // Blue shadow under snowbank
    fill_rect(base, 4, 3, 14, 26, 2, 4);

    // Jagged Glacial Ice Ridge Spires (X: 2..29, Y: 3..12)
    fill_rect(base, 4, 3, 6, 26, 6, 1);   // Glacial cyan translucent ice
    fill_rect(base, 4, 4, 8, 24, 4, 2);   // Deep glacial blue core shadow

    // Spire 1 (peak at X=6, Y=3)
    put_pix(base, 4, 6, 3, 4);
    fill_rect(base, 4, 5, 4, 3, 2, 1);
    // Spire 2 (peak at X=13, Y=4)
    put_pix(base, 4, 13, 4, 4);
    fill_rect(base, 4, 12, 5, 3, 2, 1);
    // Spire 3 (peak at X=19, Y=3)
    put_pix(base, 4, 19, 3, 4);
    fill_rect(base, 4, 18, 4, 3, 2, 1);
    // Spire 4 (peak at X=25, Y=4)
    put_pix(base, 4, 25, 4, 4);
    fill_rect(base, 4, 24, 5, 3, 2, 1);

    // Frost crystalline surface sparkle
    put_pix(base, 4, 8, 7, 4);
    put_pix(base, 4, 16, 6, 4);
    put_pix(base, 4, 22, 7, 4);

    // Two Alpine Ski Hazard Flags planted firmly in ice
    fill_rect(base, 4, 3, 0, 1, 7, 10);  // Left steel pole
    fill_rect(base, 4, 4, 0, 3, 2, 5);   // Left red pennant
    put_pix(base, 4, 4, 1, 6);
    put_pix(base, 4, 6, 1, 5);

    fill_rect(base, 4, 28, 0, 1, 7, 10); // Right steel pole
    fill_rect(base, 4, 25, 0, 3, 2, 5);  // Right red pennant
    put_pix(base, 4, 26, 1, 6);
    put_pix(base, 4, 25, 1, 5);

    // Clean outline
    for (int x = 2; x < 30; x++) {
        put_pix(base, 4, x, 15, 15);
    }
}

// Generate Cleared Winter Ice Ridge (32x16, auroral green crystal glow)
static void gen_obs_wint_ice_pass(u32 *base) {
    gen_obs_wint_ice_lrg(base);

    // Flags turn into glowing green open-trail pennants
    fill_rect(base, 4, 4, 0, 3, 2, 11);
    put_pix(base, 4, 5, 0, 4);
    fill_rect(base, 4, 25, 0, 3, 2, 11);
    put_pix(base, 4, 26, 0, 4);

    // Crystalline ice spires glow with vibrant auroral green
    put_pix(base, 4, 6, 3, 11);
    put_pix(base, 4, 13, 4, 11);
    put_pix(base, 4, 19, 3, 11);
    put_pix(base, 4, 25, 4, 11);
    for (int x = 4; x < 28; x += 3) {
        put_pix(base, 4, x, 6, 11);
        put_pix(base, 4, x + 1, 6, 4);
    }
}

// Generate Winter Ski Gondola Cableway Gantry Large (32x32, 4x4 = 16 tiles)
static void gen_obs_wint_cable_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // Heavy Alpine Timber Posts (Left: X: 1..4, Right: X: 27..30, Y: 5..31)
    fill_rect(base, 4, 1, 5, 4, 26, 3);  // Dark cedar brown
    fill_rect(base, 4, 2, 6, 2, 25, 13); // Warm pine grain inner strip
    fill_rect(base, 4, 27, 5, 4, 26, 3);
    fill_rect(base, 4, 28, 6, 2, 25, 13);

    // Timber metal support brackets & snow drift bases
    fill_rect(base, 4, 0, 28, 6, 4, 4);  // White snowdrifts at base
    fill_rect(base, 4, 26, 28, 6, 4, 4);
    fill_rect(base, 4, 1, 27, 4, 1, 10); // Steel mounting foot
    fill_rect(base, 4, 27, 27, 4, 1, 10);

    // Diagonal Timber Bracing Struts (Corners)
    put_pix(base, 4, 5, 8, 3); put_pix(base, 4, 6, 9, 3); put_pix(base, 4, 7, 10, 3);
    put_pix(base, 4, 26, 8, 3); put_pix(base, 4, 25, 9, 3); put_pix(base, 4, 24, 10, 3);

    // Heavy Snow Blanket on Roof Peak (Y: 0..3, X: 0..31)
    fill_rect(base, 4, 1, 0, 30, 3, 4);  // Pure powder white snow
    fill_rect(base, 4, 2, 3, 28, 1, 2);  // Blue shadow under snow lip
    for (int x = 2; x < 30; x += 3) {
        put_pix(base, 4, x, 3, 4);
    }

    // Heavy Timber Crossbeam (Y: 4..7, X: 1..30)
    fill_rect(base, 4, 1, 4, 30, 4, 3);
    fill_rect(base, 4, 2, 5, 28, 2, 13);

    // Steel Gondola Cable Track & Pulleys (Y: 7..9, X: 8..23)
    fill_rect(base, 4, 8, 7, 16, 1, 10); // Steel cable
    fill_rect(base, 4, 14, 7, 4, 2, 10); // Center pulley housing
    put_pix(base, 4, 15, 8, 14);

    // Dangling Alpine Warning Sign (X: 11..20, Y: 8..13)
    fill_rect(base, 4, 11, 8, 10, 5, 6); // High-vis orange outer frame
    fill_rect(base, 4, 12, 9, 8, 3, 9);  // Bright yellow inner sign panel
    put_pix(base, 4, 15, 9, 15);
    put_pix(base, 4, 16, 9, 15);
    put_pix(base, 4, 15, 10, 15);
    put_pix(base, 4, 16, 10, 15);
    put_pix(base, 4, 15, 11, 15);
    put_pix(base, 4, 16, 11, 15);

    // Clearance Warning Stripe Bar (Y: 12..13, X: 2..29)
    fill_rect(base, 4, 2, 12, 28, 2, 9); // Yellow bar
    for (int x = 2; x < 30; x++) {
        if ((x % 4) < 2) {
            put_pix(base, 4, x, 12, 6);      // Orange hazard stripes
            put_pix(base, 4, x, 13, 6);
        }
    }
}

// Generate Cleared Winter Cableway Gantry (32x32, trail open green beacons & bar)
static void gen_obs_wint_cable_pass(u32 *base) {
    gen_obs_wint_cable_lrg(base);

    // Sign turns into glowing green trail open board
    fill_rect(base, 4, 11, 8, 10, 5, 11);
    fill_rect(base, 4, 12, 9, 8, 3, 11);
    put_pix(base, 4, 14, 10, 4);
    put_pix(base, 4, 15, 11, 4);
    put_pix(base, 4, 16, 10, 4);
    put_pix(base, 4, 17, 9, 4);

    // Clearance bar becomes green and white
    fill_rect(base, 4, 2, 12, 28, 2, 11);
    for (int x = 2; x < 30; x++) {
        if ((x % 4) < 2) {
            put_pix(base, 4, x, 12, 4);
            put_pix(base, 4, x, 13, 4);
        }
    }
}

// Generate Mountain Snowplow Truck Large (32x32, 4x4 = 16 tiles)
static void gen_obs_wint_truck_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // 1. High-Mounted Emergency Warning Strobe Bar on Cab Roof (Y: 1..3, X: 9..22)
    fill_rect(base, 4, 9, 2, 14, 2, 10);  // Chrome mounting rack
    fill_rect(base, 4, 10, 1, 4, 2, 9);   // Left bright yellow strobe
    put_pix(base, 4, 11, 1, 4);           // White strobe flash core
    fill_rect(base, 4, 18, 1, 4, 2, 9);   // Right bright yellow strobe
    put_pix(base, 4, 19, 1, 4);
    fill_rect(base, 4, 14, 1, 4, 2, 6);   // Center safety orange beacon

    // 2. High-Vis Municipal Orange Truck Cab & Hopper Body (X: 3..28, Y: 4..22)
    fill_rect(base, 4, 4, 4, 24, 18, 6);  // Safety orange body
    fill_rect(base, 4, 5, 5, 22, 16, 7);  // Inner orange shadow

    // Open V-Hopper Filled with Granular White Rock Salt Grit (X: 6..25, Y: 5..15)
    fill_rect(base, 4, 6, 5, 20, 11, 10); // Steel hopper interior wall
    fill_rect(base, 4, 7, 6, 18, 9, 4);   // Granular white rock salt
    for (int x = 7; x <= 24; x += 2) {
        for (int y = 7; y <= 14; y += 2) {
            put_pix(base, 4, x, y, 2);    // Blue salt shadow crystals
        }
    }

    // Heavy Steel Rear Underrun Bumper & Red Taillights (Y: 20..23)
    fill_rect(base, 4, 3, 20, 26, 3, 10); // Heavy steel bumper bar
    fill_rect(base, 4, 4, 20, 5, 3, 5);   // Left commercial red taillights
    put_pix(base, 4, 4, 21, 9);           // Left amber corner indicator
    fill_rect(base, 4, 23, 20, 5, 3, 5);  // Right commercial red taillights
    put_pix(base, 4, 27, 21, 9);          // Right amber corner indicator

    // Circular Salt Spreader Mechanism (Bottom center, X: 12..19, Y: 23..28)
    fill_rect(base, 4, 13, 23, 6, 5, 14); // Spreader housing
    fill_rect(base, 4, 12, 26, 8, 2, 10); // Rotating steel spinner disc
    put_pix(base, 4, 15, 27, 4);          // Salt spray particles
    put_pix(base, 4, 16, 27, 4);

    // Studded Heavy Snow Tires (X: 1..5 and 26..30, Y: 21..29)
    fill_rect(base, 4, 1, 21, 4, 9, 14);  // Left tire rubber
    fill_rect(base, 4, 26, 21, 4, 9, 14); // Right tire rubber
    for (int y = 22; y <= 28; y += 2) {
        put_pix(base, 4, 2, y, 10);       // Steel studs
        put_pix(base, 4, 27, y, 10);
    }
}

// Generate Alpine 4x4 Expedition SUV Large (32x32, 4x4 = 16 tiles)
static void gen_obs_wint_civilian_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Rugged Boxy SUV Chassis in Alpine Dark Forest Green (col 12, col 15)
    fill_rect(base, 4, cx - 10, cy - 7, 20, 17, 12);  // Alpine forest green body
    fill_rect(base, 4, cx - 9, cy - 8, 18, 2, 15);    // Dark roof rail contour
    fill_rect(base, 4, cx - 9, cy + 8, 18, 2, 14);    // Lower bumper shadow

    // Frosted Rear Tailgate Window (Tinted glass col 8 with ice frost col 4)
    fill_rect(base, 4, cx - 7, cy - 5, 14, 6, 8);     // Cyan tinted glass
    fill_rect(base, 4, cx - 6, cy - 4, 12, 4, 2);     // Deep glass reflection
    put_pix(base, 4, cx - 7, cy - 5, 4);              // Ice frost edging
    put_pix(base, 4, cx + 6, cy - 5, 4);
    put_pix(base, 4, cx - 7, cy, 4);
    put_pix(base, 4, cx + 6, cy, 4);

    // Aluminum Heavy-Duty Roof Cargo Rack (Y: cy - 9..cy - 8, col 10)
    fill_rect(base, 4, cx - 8, cy - 9, 16, 2, 10);

    // Skis and Snowboards Mounted on Roof Rack (Y: cy - 14..cy - 8)
    fill_rect(base, 4, cx - 6, cy - 14, 3, 8, 6);     // Left orange skis
    put_pix(base, 4, cx - 5, cy - 15, 4);             // Ski tip
    put_pix(base, 4, cx - 5, cy - 11, 14);            // Black binding
    fill_rect(base, 4, cx + 3, cy - 14, 3, 8, 9);     // Right yellow skis
    put_pix(base, 4, cx + 4, cy - 15, 4);
    put_pix(base, 4, cx + 4, cy - 11, 14);

    // Roof-Mounted Ultra-Bright LED Light Bar (Y: cy - 7, X: cx - 5..cx + 4)
    fill_rect(base, 4, cx - 5, cy - 7, 10, 1, 4);     // Pure white LED array
    put_pix(base, 4, cx - 6, cy - 7, 10);
    put_pix(base, 4, cx + 5, cy - 7, 10);

    // Modern Vertical LED Taillights (Y: cy - 3..cy + 6, X: cx - 10, cx + 9)
    fill_rect(base, 4, cx - 10, cy - 3, 2, 9, 5);     // Left red LED light tube
    put_pix(base, 4, cx - 10, cy + 2, 4);             // White reverse lamp
    fill_rect(base, 4, cx + 8, cy - 3, 2, 9, 5);      // Right red LED light tube
    put_pix(base, 4, cx + 9, cy + 2, 4);

    // Heavy Aluminum Off-Road Skid Plate & Dual Exhaust
    fill_rect(base, 4, cx - 7, cy + 7, 14, 2, 10);    // Silver skid plate
    put_pix(base, 4, cx - 8, cy + 8, 10);             // Left exhaust tip
    put_pix(base, 4, cx + 7, cy + 8, 10);             // Right exhaust tip

    // Massive Studded Off-Road Snow Tires (High clearance)
    fill_rect(base, 4, cx - 12, cy - 3, 3, 6, 14);
    fill_rect(base, 4, cx + 9, cy - 3, 3, 6, 14);
    fill_rect(base, 4, cx - 12, cy + 4, 3, 5, 14);
    fill_rect(base, 4, cx + 9, cy + 4, 3, 5, 14);
    put_pix(base, 4, cx - 11, cy, 10);                // Steel rim bolts
    put_pix(base, 4, cx + 10, cy, 10);
}

// =========================================================================
// CAPE ORBITAL THEMED OBSTACLES (Sprite Palette 12)
// =========================================================================

// Generate Cryogenic Frosted Fuel Pipe Barrier (32x16, 4x2 = 8 tiles)
static void gen_obs_orbit_pipe_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;

    // Steel trench mounting base on concrete (Y: 13..15)
    fill_rect(base, 4, 2, 13, 28, 3, 2);   // Dark aerospace steel trench
    fill_rect(base, 4, 3, 12, 26, 2, 3);   // Titanium mounting plate

    // Twin Cryogenic Insulated Fuel Pipes (X: 3..28, Y: 4..11)
    // Lower Pipe (Y: 8..11)
    fill_rect(base, 4, 3, 8, 26, 4, 8);    // Cryo cyan frost insulation
    fill_rect(base, 4, 4, 9, 24, 2, 5);    // White frost core
    fill_rect(base, 4, 3, 11, 26, 1, 2);   // Bottom shadow
    // Upper Pipe (Y: 4..7)
    fill_rect(base, 4, 4, 4, 24, 4, 8);
    fill_rect(base, 4, 5, 5, 22, 2, 5);
    fill_rect(base, 4, 4, 7, 24, 1, 2);

    // Expansion Couplings & Heavy Flange Rings (X: 6, 15, 24)
    for (int fx = 6; fx <= 24; fx += 9) {
        fill_rect(base, 4, fx, 3, 3, 9, 4);   // Specular chrome flange ring
        put_pix(base, 4, fx + 1, 3, 14);      // Chrome highlight
        put_pix(base, 4, fx + 1, 7, 1);       // Joint bolt
        put_pix(base, 4, fx + 1, 11, 1);
    }

    // High-Pressure Cryogenic Relief Valves & Red Warning Beacons
    fill_rect(base, 4, 10, 1, 2, 3, 6);       // Left safety orange valve neck
    fill_rect(base, 4, 9, 0, 4, 2, 10);       // Left red shutoff wheel
    fill_rect(base, 4, 20, 1, 2, 3, 6);       // Right valve neck
    fill_rect(base, 4, 19, 0, 4, 2, 10);      // Right red wheel

    // Vented frost vapor wisps
    put_pix(base, 4, 8, 1, 8);
    put_pix(base, 4, 23, 1, 8);

    // Dark outline
    for (int x = 2; x < 30; x++) put_pix(base, 4, x, 15, 15);
}

// Generate Cleared Cryogenic Fuel Pipe (32x16, clearance green vents)
static void gen_obs_orbit_pipe_pass(u32 *base) {
    gen_obs_orbit_pipe_lrg(base);

    // Pressure valves turn to vibrant launch green
    fill_rect(base, 4, 9, 0, 4, 2, 11);
    put_pix(base, 4, 10, 0, 14);
    fill_rect(base, 4, 19, 0, 4, 2, 11);
    put_pix(base, 4, 20, 0, 14);

    // Flange status indicators illuminate with green pass LEDs
    for (int fx = 7; fx <= 25; fx += 9) {
        put_pix(base, 4, fx, 5, 11);
        put_pix(base, 4, fx, 9, 11);
    }
}

// Generate Launch Umbilical Service Gantry Large (32x32, 4x4 = 16 tiles)
static void gen_obs_orbit_gantry_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // Heavy Lattice Service Tower Support Columns (Left: X: 1..5, Right: X: 26..30)
    fill_rect(base, 4, 1, 5, 5, 26, 6);   // Industrial safety orange tower struts
    fill_rect(base, 4, 2, 6, 3, 25, 3);   // Titanium grey inner frame
    fill_rect(base, 4, 26, 5, 5, 26, 6);
    fill_rect(base, 4, 27, 6, 3, 25, 3);

    // Diagonal Cross-Bracing on Tower Pillars
    for (int y = 7; y < 27; y += 4) {
        put_pix(base, 4, 2, y, 6);     put_pix(base, 4, 3, y + 1, 6); put_pix(base, 4, 4, y + 2, 6);
        put_pix(base, 4, 27, y, 6);    put_pix(base, 4, 28, y + 1, 6); put_pix(base, 4, 29, y + 2, 6);
    }

    // Heavy Reinforced Concrete Launch Apron Footings (Y: 28..31)
    fill_rect(base, 4, 0, 28, 7, 4, 2);   // Concrete base
    fill_rect(base, 4, 25, 28, 7, 4, 2);
    fill_rect(base, 4, 1, 28, 5, 1, 7);   // Hazard yellow stripe on base
    fill_rect(base, 4, 26, 28, 5, 1, 7);

    // Overhead Umbilical Service Bridge Truss (Y: 5..14, X: 4..27)
    fill_rect(base, 4, 4, 5, 24, 9, 3);   // Main titanium bridge truss
    fill_rect(base, 4, 5, 6, 22, 7, 2);   // Inner service conduit cavity
    fill_rect(base, 4, 4, 4, 24, 2, 6);   // Safety orange upper rail
    fill_rect(base, 4, 4, 13, 24, 2, 6);  // Safety orange lower clearance rail

    // Bundled Fuel & Telemetry Umbilical Hoses (col 8, col 9, col 13)
    fill_rect(base, 4, 6, 7, 20, 2, 8);   // Liquid oxygen cryo cyan line
    fill_rect(base, 4, 6, 10, 20, 2, 13); // High-pressure helium bronze line

    // Digital Launch Status / Clearance Board in Center (X: 11..20, Y: 7..11)
    fill_rect(base, 4, 11, 7, 10, 5, 1);  // Dark display housing
    fill_rect(base, 4, 12, 8, 8, 3, 10);  // Red "STANDBY" status warning LED block
    put_pix(base, 4, 13, 9, 14);          // Core LED highlight
    put_pix(base, 4, 18, 9, 14);

    // Warning Strobe Beacons on Top of Gantry (X: 4, 27, Y: 2..4)
    fill_rect(base, 4, 4, 2, 3, 2, 10);   // Left red strobe
    put_pix(base, 4, 5, 1, 14);
    fill_rect(base, 4, 25, 2, 3, 2, 10);  // Right red strobe
    put_pix(base, 4, 26, 1, 14);
}

// Generate Cleared Launch Umbilical Gantry (32x32, green launch status)
static void gen_obs_orbit_gantry_pass(u32 *base) {
    gen_obs_orbit_gantry_lrg(base);

    // Center display flips to high-illumination Launch Green LEDs
    fill_rect(base, 4, 12, 8, 8, 3, 11);
    put_pix(base, 4, 13, 9, 14);          // Emerald pass core
    put_pix(base, 4, 15, 9, 14);
    put_pix(base, 4, 17, 9, 14);

    // Strobes turn to pulsing green clearance lights
    fill_rect(base, 4, 4, 2, 3, 2, 11);
    fill_rect(base, 4, 25, 2, 3, 2, 11);

    // Under-bridge clearance lamps illuminate green
    for (int x = 7; x < 25; x += 3) {
        put_pix(base, 4, x, 14, 11);
    }
}

// Generate Crawler Rocket Booster Transporter Large (32x32, 4x4 = 16 tiles)
static void gen_obs_orbit_crawler_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Massive Multi-Axle Crawler Transporter Deck (X: cx - 12..cx + 11, Y: cy - 2..cy + 11)
    fill_rect(base, 4, cx - 12, cy - 2, 24, 12, 3);   // Heavy titanium chassis deck
    fill_rect(base, 4, cx - 11, cy - 1, 22, 10, 2);   // Structural deck plate
    fill_rect(base, 4, cx - 12, cy + 9, 24, 2, 7);    // Hazard yellow chevron lower bumper
    for (int x = cx - 12; x < cx + 12; x += 3) put_pix(base, 4, x, cy + 9, 1); // Hazard striping

    // Massive Continuous Crawler Track Bogies (Left & Right tread units)
    fill_rect(base, 4, cx - 14, cy + 2, 3, 11, 1);    // Left heavy track assembly
    fill_rect(base, 4, cx + 11, cy + 2, 3, 11, 1);    // Right heavy track assembly
    for (int y = cy + 3; y < cy + 12; y += 2) {
        put_pix(base, 4, cx - 13, y, 4);              // Track roller wheels
        put_pix(base, 4, cx + 12, y, 4);
    }

    // Aerospace Rocket Booster Stage Cylindrical Hull (Mounted Horizontally: Y: cy - 14..cy - 3)
    fill_rect(base, 4, cx - 9, cy - 13, 18, 11, 5);   // Clean aerospace white rocket skin
    fill_rect(base, 4, cx - 8, cy - 12, 16, 9, 4);    // Specular hull curvature shadow
    fill_rect(base, 4, cx - 5, cy - 11, 10, 7, 5);    // White core reflection
    fill_rect(base, 4, cx - 9, cy - 8, 18, 2, 6);     // Safety orange agency logo stripe
    fill_rect(base, 4, cx - 9, cy - 14, 18, 2, 2);    // Forward stage separation ring

    // High-Expansion Rocket Exhaust Rocket Engine Bell (Y: cy - 2..cy + 2, centered)
    fill_rect(base, 4, cx - 5, cy - 3, 10, 5, 13);    // Titanium/bronze engine bell
    fill_rect(base, 4, cx - 4, cy - 2, 8, 3, 2);      // Inner dark combustion chamber
    put_pix(base, 4, cx - 1, cy - 1, 9);              // Blue glow pilot igniter
    put_pix(base, 4, cx, cy - 1, 9);

    // Heavy Cradle Clamps securing rocket to flatbed
    fill_rect(base, 4, cx - 11, cy - 7, 2, 6, 6);     // Left orange clamp arm
    fill_rect(base, 4, cx + 9, cy - 7, 2, 6, 6);      // Right orange clamp arm

    // Dual Flashing Amber Beacons on Transporter Rear Corners
    fill_rect(base, 4, cx - 11, cy + 1, 2, 2, 7);
    put_pix(base, 4, cx - 10, cy + 1, 14);
    fill_rect(base, 4, cx + 9, cy + 1, 2, 2, 7);
    put_pix(base, 4, cx + 10, cy + 1, 14);
}

// Generate Autonomous 6-Wheel Spaceport Security Rover (32x32, 4x4 = 16 tiles)
static void gen_obs_orbit_rover_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Futuristic Aerodynamic Rover Body in Aerospace White (col 5)
    fill_rect(base, 4, cx - 8, cy - 7, 16, 17, 5);    // White carbon composite body
    fill_rect(base, 4, cx - 9, cy - 6, 18, 14, 3);    // Titanium side sponsons
    fill_rect(base, 4, cx - 7, cy - 8, 14, 2, 4);     // Front aero nose contour
    fill_rect(base, 4, cx - 8, cy + 8, 16, 2, 2);     // Rear diffusers

    // Panoramic Hexagonal Sensor Canopy Dome (Y: cy - 6..cy + 1, col 12 cyan glass)
    fill_rect(base, 4, cx - 5, cy - 6, 10, 7, 12);    // Cyan tinted panoramic canopy
    fill_rect(base, 4, cx - 4, cy - 5, 8, 5, 2);      // Dark interior electronics cavity
    put_pix(base, 4, cx - 5, cy - 6, 14);             // Specular glare reflection
    put_pix(base, 4, cx + 4, cy - 6, 14);

    // Roof-Mounted Satellite Uplink Dish & High-Gain Antenna (Y: cy - 13..cy - 7)
    fill_rect(base, 4, cx - 3, cy - 11, 6, 3, 4);     // Curved circular dish reflector
    fill_rect(base, 4, cx - 2, cy - 10, 4, 2, 13);    // Bronze feed horn
    fill_rect(base, 4, cx - 1, cy - 8, 2, 2, 3);      // Gimbal mount base
    put_pix(base, 4, cx + 5, cy - 12, 14);            // High-gain antenna whip tip
    fill_rect(base, 4, cx + 5, cy - 11, 1, 5, 3);     // Antenna mast

    // 6 Rugged All-Terrain Independent Drive Wheels (3 on each side)
    // Left side wheels (X: cx - 12..cx - 10)
    fill_rect(base, 4, cx - 12, cy - 6, 3, 5, 1);     // Front-left tire
    fill_rect(base, 4, cx - 12, cy - 1, 3, 5, 1);     // Mid-left tire
    fill_rect(base, 4, cx - 12, cy + 4, 3, 5, 1);     // Rear-left tire
    put_pix(base, 4, cx - 11, cy - 4, 4);             // Wheel hubcaps
    put_pix(base, 4, cx - 11, cy + 1, 4);
    put_pix(base, 4, cx - 11, cy + 6, 4);
    // Right side wheels (X: cx + 9..cx + 11)
    fill_rect(base, 4, cx + 9, cy - 6, 3, 5, 1);      // Front-right tire
    fill_rect(base, 4, cx + 9, cy - 1, 3, 5, 1);      // Mid-right tire
    fill_rect(base, 4, cx + 9, cy + 4, 3, 5, 1);      // Rear-right tire
    put_pix(base, 4, cx + 10, cy - 4, 4);
    put_pix(base, 4, cx + 10, cy + 1, 4);
    put_pix(base, 4, cx + 10, cy + 6, 4);

    // High-Intensity Security Patrol Light Bar (Y: cy - 7, X: cx - 4..cx + 3)
    fill_rect(base, 4, cx - 4, cy - 7, 3, 1, 10);     // Left red strobe
    fill_rect(base, 4, cx + 1, cy - 7, 3, 1, 9);      // Right blue strobe

    // Dual Rear LED Safety Lights (Y: cy + 7)
    fill_rect(base, 4, cx - 7, cy + 7, 3, 1, 10);
    fill_rect(base, 4, cx + 4, cy + 7, 3, 1, 10);
}

// =========================================================================
// GOTHIC MIDNIGHT THEMED OBSTACLES (Sprite Palette 13)
// =========================================================================

// Generate Gothic Crypt & Tombstone Slab Barrier (32x16, 4x2 = 8 tiles)
static void gen_obs_goth_crypt_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;

    // Overgrown Cemetery Earth & Damp Slate Base (Y: 12..15)
    fill_rect(base, 4, 1, 13, 30, 3, 2);   // Dark grave soil / charcoal shadow
    fill_rect(base, 4, 2, 11, 28, 2, 3);   // Weathered slate base slab

    // Carved Stone Sarcophagus / Crypt Slab (X: 5..26, Y: 5..12)
    fill_rect(base, 4, 5, 5, 22, 7, 4);    // Aged stone carving grey
    fill_rect(base, 4, 6, 6, 20, 5, 3);    // Sarcophagus lid recessed face
    fill_rect(base, 4, 5, 4, 22, 2, 5);    // Marble top rim highlight
    fill_rect(base, 4, 5, 11, 22, 2, 2);   // Crypt lid under-shadow

    // Carved Gothic Cross Relief on Sarcophagus Face (X: 15..16, Y: 6..10)
    fill_rect(base, 4, 15, 6, 2, 5, 5);    // Vertical cross beam
    fill_rect(base, 4, 13, 7, 6, 2, 5);    // Horizontal cross beam

    // Tilting Weathered Tombstones flanking the Crypt
    // Left Tombstone (tilted arched gravestone: X: 2..5, Y: 1..10)
    fill_rect(base, 4, 2, 3, 3, 8, 4);
    put_pix(base, 4, 3, 2, 5);             // Rounded headstone arch
    put_pix(base, 4, 3, 5, 1);             // "R.I.P." carving chiseled groove
    put_pix(base, 4, 3, 7, 1);
    // Right Broken Cross Tombstone (X: 26..29, Y: 2..10)
    fill_rect(base, 4, 27, 4, 2, 7, 4);
    fill_rect(base, 4, 26, 5, 4, 2, 4);
    put_pix(base, 4, 27, 3, 5);            // Broken cross top

    // Creeping Night Ivy & Cobwebs across stones
    put_pix(base, 4, 4, 10, 6);
    put_pix(base, 4, 8, 7, 6);
    put_pix(base, 4, 23, 8, 6);

    // Clean outline
    for (int x = 1; x < 31; x++) put_pix(base, 4, x, 15, 15);
}

// Generate Cleared Crypt Slab (32x16, eerie green spectral soul flame)
static void gen_obs_goth_crypt_pass(u32 *base) {
    gen_obs_goth_crypt_lrg(base);

    // Carved cross illuminates with ethereal soul flame green
    fill_rect(base, 4, 15, 6, 2, 5, 11);
    fill_rect(base, 4, 13, 7, 6, 2, 11);
    put_pix(base, 4, 15, 7, 14);          // Core spiritual light

    // Tombstone fractures glow with spectral energy
    put_pix(base, 4, 3, 2, 11);
    put_pix(base, 4, 27, 3, 11);
    for (int x = 6; x < 26; x += 4) {
        put_pix(base, 4, x, 5, 11);
    }
}

// Generate Wrought-Iron Cemetery Gateway Arch Large (32x32, 4x4 = 16 tiles)
static void gen_obs_goth_gate_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // Massive Victorian Granite Gate Pillars (Left: X: 1..5, Right: X: 26..30, Y: 4..31)
    fill_rect(base, 4, 1, 4, 5, 27, 3);    // Weathered stone pillar body
    fill_rect(base, 4, 2, 5, 3, 25, 4);    // Light stone face
    fill_rect(base, 4, 26, 4, 5, 27, 3);
    fill_rect(base, 4, 27, 5, 3, 25, 4);

    // Pillar Cornices & Base Steps (Y: 28..31, Y: 4..6)
    fill_rect(base, 4, 0, 28, 7, 4, 2);    // Stepped base
    fill_rect(base, 4, 25, 28, 7, 4, 2);
    fill_rect(base, 4, 0, 4, 7, 2, 5);     // Top capstone
    fill_rect(base, 4, 25, 4, 7, 2, 5);

    // Carved Gargoyles perched atop each pillar (X: 1..5, 26..30, Y: 0..4)
    fill_rect(base, 4, 2, 1, 3, 3, 4);     // Left gargoyle body
    put_pix(base, 4, 1, 1, 5);             // Wing tip
    put_pix(base, 4, 3, 2, 13);            // Glowing red demonic eye
    fill_rect(base, 4, 27, 1, 3, 3, 4);    // Right gargoyle body
    put_pix(base, 4, 30, 1, 5);            // Wing tip
    put_pix(base, 4, 28, 2, 13);           // Glowing red eye

    // Wrought Iron Spiked Overhead Gothic Arch (Y: 6..14, X: 4..27)
    fill_rect(base, 4, 4, 6, 24, 2, 6);    // Upper iron arch bar
    fill_rect(base, 4, 5, 12, 22, 2, 6);   // Lower clearance arch bar

    // Gothic Iron Vertical Spikes with Fleur-de-lis Spearpoints
    for (int x = 6; x <= 25; x += 3) {
        fill_rect(base, 4, x, 7, 1, 6, 7); // Iron picket rod
        put_pix(base, 4, x, 5, 7);         // Spear tip
        put_pix(base, 4, x - 1, 6, 6);     // Spear barb
        put_pix(base, 4, x + 1, 6, 6);
    }

    // Centered Cemetery Coat-of-Arms / Skull Crest (X: 13..18, Y: 7..12)
    fill_rect(base, 4, 13, 7, 6, 5, 2);    // Iron crest shield
    fill_rect(base, 4, 14, 8, 4, 3, 5);    // White carved skull face
    put_pix(base, 4, 14, 9, 1);            // Eye socket
    put_pix(base, 4, 17, 9, 1);

    // Hanging Violet Gas Lanterns suspended beneath gate (X: 7, 24, Y: 13..17)
    fill_rect(base, 4, 7, 13, 1, 2, 6);    // Left chain
    fill_rect(base, 4, 6, 15, 3, 3, 12);   // Violet glass lantern
    put_pix(base, 4, 7, 16, 10);           // Amber gas flame core
    fill_rect(base, 4, 24, 13, 1, 2, 6);   // Right chain
    fill_rect(base, 4, 23, 15, 3, 3, 12);
    put_pix(base, 4, 24, 16, 10);
}

// Generate Cleared Gothic Cemetery Gate (32x32, spectral green lanterns)
static void gen_obs_goth_gate_pass(u32 *base) {
    gen_obs_goth_gate_lrg(base);

    // Lanterns illuminate with brilliant spectral green flame
    fill_rect(base, 4, 6, 15, 3, 3, 11);
    put_pix(base, 4, 7, 16, 14);
    fill_rect(base, 4, 23, 15, 3, 3, 11);
    put_pix(base, 4, 24, 16, 14);

    // Skull crest eyes glow with spectral green soul fire
    put_pix(base, 4, 14, 9, 11);
    put_pix(base, 4, 17, 9, 11);

    // Archway clearance runes ignite across lower bar
    for (int x = 6; x < 26; x += 3) {
        put_pix(base, 4, x, 13, 11);
    }
}

// Generate Black Gothic Funeral Hearse Large (32x32, 4x4 = 16 tiles)
static void gen_obs_goth_hearse_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Elongated Heavy Funeral Coach Body in Pitch Gothic Black (col 1)
    fill_rect(base, 4, cx - 11, cy - 7, 22, 17, 1);   // Mirror black lacquer chassis
    fill_rect(base, 4, cx - 10, cy - 8, 20, 2, 7);    // Polished silver roof rails
    fill_rect(base, 4, cx - 11, cy + 9, 22, 2, 7);    // Massive chrome rear bumper

    // Ornate Silver Landau S-Bars on Rear Quarter Panels
    put_pix(base, 4, cx - 10, cy - 4, 7);
    put_pix(base, 4, cx - 9, cy - 3, 7);
    put_pix(base, 4, cx - 10, cy - 2, 7);
    put_pix(base, 4, cx + 9, cy - 4, 7);
    put_pix(base, 4, cx + 8, cy - 3, 7);
    put_pix(base, 4, cx + 9, cy - 2, 7);

    // Rear Casket Viewing Window (Y: cy - 6..cy + 1, X: cx - 7..cx + 6)
    fill_rect(base, 4, cx - 7, cy - 6, 14, 7, 2);     // Dark tinted glass
    // Draped Royal Purple Velvet Casket Curtains (col 8)
    fill_rect(base, 4, cx - 7, cy - 6, 3, 7, 8);      // Left velvet drape
    fill_rect(base, 4, cx + 4, cy - 6, 3, 7, 8);      // Right velvet drape
    put_pix(base, 4, cx - 5, cy - 3, 10);             // Gold tassel cord
    put_pix(base, 4, cx + 4, cy - 3, 10);
    // Silver filigree casket visible inside center window
    fill_rect(base, 4, cx - 3, cy - 3, 6, 4, 4);
    fill_rect(base, 4, cx - 2, cy - 2, 4, 2, 7);      // Silver casket handles

    // Slender Gothic Chrome Tailfins with Vertical Blade Taillights
    fill_rect(base, 4, cx - 11, cy + 1, 2, 8, 13);    // Left blood red light blade
    put_pix(base, 4, cx - 11, cy + 8, 7);             // Chrome tailfin tip
    fill_rect(base, 4, cx + 9, cy + 1, 2, 8, 13);     // Right blood red light blade
    put_pix(base, 4, cx + 10, cy + 8, 7);

    // Whitewall Luxury Tires (4 tires visible)
    fill_rect(base, 4, cx - 13, cy - 4, 2, 6, 1);
    fill_rect(base, 4, cx - 12, cy - 3, 1, 4, 5);     // Whitewall ring
    fill_rect(base, 4, cx + 11, cy - 4, 2, 6, 1);
    fill_rect(base, 4, cx + 11, cy - 3, 1, 4, 5);
    fill_rect(base, 4, cx - 13, cy + 4, 2, 5, 1);
    fill_rect(base, 4, cx + 11, cy + 4, 2, 5, 1);

    // Polished Silver Chrome Grille & Filigree Crest on trunk
    fill_rect(base, 4, cx - 4, cy + 5, 8, 3, 7);
    put_pix(base, 4, cx, cy + 4, 14);                 // Silver emblem
}

// Generate Battered Phantom Hot-Rod Jalopy Large (32x32, 4x4 = 16 tiles)
static void gen_obs_goth_jalopy_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Chopped-Top 1930s Coupe Body in Primer Rust Brown (col 9) & Black (col 1)
    fill_rect(base, 4, cx - 9, cy - 6, 18, 16, 9);    // Primer rust body
    fill_rect(base, 4, cx - 8, cy - 7, 16, 2, 1);     // Chopped black roof
    fill_rect(base, 4, cx - 10, cy + 8, 20, 2, 6);    // Battered iron bumper

    // Chopped Windshield & Cabin (Narrow slit windows)
    fill_rect(base, 4, cx - 6, cy - 5, 12, 4, 2);     // Dark tinted glass
    put_pix(base, 4, cx - 6, cy - 5, 5);              // Cracked glass hairline
    put_pix(base, 4, cx - 5, cy - 4, 5);
    put_pix(base, 4, cx - 4, cy - 3, 5);

    // Exposed Chrome V8 Engine Blower protruding through hood (Y: cy - 13..cy - 7)
    fill_rect(base, 4, cx - 4, cy - 12, 8, 5, 7);     // Triple chrome intake scoops
    fill_rect(base, 4, cx - 3, cy - 11, 6, 3, 1);     // Dark butterfly valves
    fill_rect(base, 4, cx - 2, cy - 7, 4, 2, 7);      // Blower belt pulley
    put_pix(base, 4, cx - 4, cy - 12, 14);            // Chrome gleam

    // Open Exhaust Zoomie Headers spitting embers (flanking engine)
    fill_rect(base, 4, cx - 7, cy - 9, 2, 4, 6);      // Left iron header pipes
    put_pix(base, 4, cx - 8, cy - 10, 10);            // Glowing ember spark
    fill_rect(base, 4, cx + 5, cy - 9, 2, 4, 6);      // Right header pipes
    put_pix(base, 4, cx + 7, cy - 10, 10);

    // Single Working Headlamp & One Broken Smashed Lamp
    fill_rect(base, 4, cx - 8, cy - 2, 3, 3, 10);     // Working left amber headlight
    put_pix(base, 4, cx - 7, cy - 1, 14);             // High-intensity filament
    fill_rect(base, 4, cx + 5, cy - 2, 3, 3, 1);      // Smashed dark right headlight socket
    put_pix(base, 4, cx + 6, cy - 2, 7);              // Broken glass shard

    // Oversized Rear Drag Slicks
    fill_rect(base, 4, cx - 12, cy - 2, 3, 6, 1);
    fill_rect(base, 4, cx + 9, cy - 2, 3, 6, 1);
    fill_rect(base, 4, cx - 12, cy + 4, 3, 5, 1);
    fill_rect(base, 4, cx + 9, cy + 4, 3, 5, 1);
    put_pix(base, 4, cx - 11, cy + 1, 7);             // Rusty steel rims
    put_pix(base, 4, cx + 10, cy + 1, 7);

    // Flickering Single Red Taillight
    fill_rect(base, 4, cx - 8, cy + 7, 2, 2, 13);
}

// =========================================================================
// MAYA TEMPLE TRAIL THEMED OBSTACLES (Sprite Palette 14)
// =========================================================================

// Generate Ancient Carved Stone Stela Totem Barrier (32x16, 4x2 = 8 tiles)
static void gen_obs_maya_totem_lrg(u32 *base) {
    for (int i = 0; i < 8 * 8; i++) base[i] = 0;

    // Overgrown Jungle Soil & Tangled Roots (Y: 12..15)
    fill_rect(base, 4, 1, 13, 30, 3, 1);   // Deep rainforest mud shadow
    fill_rect(base, 4, 2, 11, 28, 2, 6);   // Thick strangler vine roots
    fill_rect(base, 4, 4, 13, 24, 2, 7);   // Emerald jungle moss

    // Massive Carved Basalt Stela Monolith Slab (X: 4..27, Y: 4..12)
    fill_rect(base, 4, 4, 4, 24, 8, 3);    // Ancient carved stela grey stone
    fill_rect(base, 4, 5, 5, 22, 6, 2);    // Recessed carved glyph field
    fill_rect(base, 4, 4, 3, 24, 2, 5);    // Sunlit top stone edge highlight
    fill_rect(base, 4, 4, 11, 24, 2, 1);   // Bottom heavy shadow

    // Intricate Tribal Geometric Glyph Reliefs chiseled across face
    // Glyph Block 1 (X: 7..10)
    fill_rect(base, 4, 7, 6, 4, 4, 4);
    put_pix(base, 4, 8, 7, 2); put_pix(base, 4, 9, 8, 2);
    // Glyph Block 2 (Central Sun God Mask: X: 14..17)
    fill_rect(base, 4, 14, 5, 4, 6, 5);   // Central mask
    put_pix(base, 4, 14, 6, 8);           // Dark jade eye
    put_pix(base, 4, 17, 6, 8);           // Dark jade eye
    fill_rect(base, 4, 15, 9, 2, 1, 2);   // Carved stone mouth
    // Glyph Block 3 (X: 21..24)
    fill_rect(base, 4, 21, 6, 4, 4, 4);
    put_pix(base, 4, 22, 8, 2); put_pix(base, 4, 23, 7, 2);

    // Thick Strangler Vines draping across stone
    fill_rect(base, 4, 6, 2, 2, 7, 6);    // Left creeping vine
    put_pix(base, 4, 5, 4, 7);            // Emerald leaf
    fill_rect(base, 4, 24, 2, 2, 7, 6);   // Right vine
    put_pix(base, 4, 26, 3, 7);

    // Clean outline
    for (int x = 2; x < 30; x++) put_pix(base, 4, x, 15, 15);
}

// Generate Cleared Stela Monolith (32x16, glowing jade runes)
static void gen_obs_maya_totem_pass(u32 *base) {
    gen_obs_maya_totem_lrg(base);

    // Central Sun God Mask eyes ignite with radiant sacred jade fire
    fill_rect(base, 4, 14, 6, 2, 2, 9);
    fill_rect(base, 4, 16, 6, 2, 2, 9);
    put_pix(base, 4, 15, 6, 14);          // Core gemstone gleam

    // Tribal glyphs pulse with bright jade green
    put_pix(base, 4, 8, 7, 9);
    put_pix(base, 4, 9, 8, 9);
    put_pix(base, 4, 22, 8, 9);
    put_pix(base, 4, 23, 7, 9);

    // Monolith rim illuminates with green ancient power
    for (int x = 5; x < 27; x += 3) {
        put_pix(base, 4, x, 4, 9);
    }
}

// Generate Ancient Stone Aqueduct Arch Large (32x32, 4x4 = 16 tiles)
static void gen_obs_maya_arch_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;

    // Massive Carved Stepped Stone Pillars (Left: X: 1..5, Right: X: 26..30, Y: 4..31)
    fill_rect(base, 4, 1, 4, 5, 27, 2);    // Weathered temple basalt
    fill_rect(base, 4, 2, 5, 3, 25, 4);    // Ancient sandstone face
    fill_rect(base, 4, 26, 4, 5, 27, 2);
    fill_rect(base, 4, 27, 5, 3, 25, 4);

    // Stepped Pyramid Base Footings (Y: 28..31, X: 0..6, 25..31)
    fill_rect(base, 4, 0, 28, 7, 4, 3);
    fill_rect(base, 4, 25, 28, 7, 4, 3);
    fill_rect(base, 4, 1, 28, 5, 1, 7);    // Emerald moss on steps
    fill_rect(base, 4, 26, 28, 5, 1, 7);

    // Heavy Stone Corbel Arch Bridge Span (Y: 5..14, X: 4..27)
    fill_rect(base, 4, 4, 5, 24, 9, 3);    // Carved masonry stone block span
    fill_rect(base, 4, 5, 6, 22, 7, 2);    // Inner recessed water conduit cavity
    fill_rect(base, 4, 4, 4, 24, 2, 5);    // Sunlit top cornice
    fill_rect(base, 4, 4, 13, 24, 2, 1);   // Lower clearance lintel shadow

    // Intricate Carved Serpent Stone Cornice along top
    for (int x = 6; x < 26; x += 4) {
        fill_rect(base, 4, x, 6, 2, 4, 4); // Carved glyph blocks
        put_pix(base, 4, x, 8, 8);         // Dark jade inset
    }

    // Sacred Jade Portal Keystone in Center (X: 13..18, Y: 7..12)
    fill_rect(base, 4, 13, 7, 6, 5, 8);    // Dark polished jade medallion
    fill_rect(base, 4, 14, 8, 4, 3, 2);    // Carved jaguar totem face
    put_pix(base, 4, 14, 9, 9);            // Jade eye
    put_pix(base, 4, 17, 9, 9);

    // Hanging Jungle Lianas / Strangler Vines hanging down from bridge
    fill_rect(base, 4, 7, 13, 1, 8, 6);    // Left hanging vine
    put_pix(base, 4, 6, 17, 7);            // Emerald leaf
    fill_rect(base, 4, 11, 13, 1, 5, 6);
    fill_rect(base, 4, 20, 13, 1, 6, 6);
    fill_rect(base, 4, 24, 13, 1, 9, 6);   // Right hanging vine
    put_pix(base, 4, 25, 18, 7);
}

// Generate Cleared Ancient Aqueduct (32x32, glowing jade runes & portal fire)
static void gen_obs_maya_arch_pass(u32 *base) {
    gen_obs_maya_arch_lrg(base);

    // Central Jaguar Keystone ignites with mystical radiant jade green
    fill_rect(base, 4, 13, 7, 6, 5, 9);
    fill_rect(base, 4, 14, 8, 4, 3, 14);  // White solar core
    put_pix(base, 4, 15, 9, 9);
    put_pix(base, 4, 16, 9, 9);

    // Inset jade stones across the lintel pulse with brilliant light
    for (int x = 6; x < 26; x += 4) {
        fill_rect(base, 4, x, 7, 2, 2, 9);
    }

    // Under-bridge corbel archway glows with glowing clearance energy
    for (int x = 5; x < 27; x += 3) {
        put_pix(base, 4, x, 13, 9);
    }
}

// Generate 6x6 Canvas Safari Expedition Hauler Large (32x32, 4x4 = 16 tiles)
static void gen_obs_maya_truck_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Heavy 6x6 Off-Road Military Chassis in Expedition Army Green (col 11)
    fill_rect(base, 4, cx - 11, cy - 4, 22, 14, 11);  // Army green chassis
    fill_rect(base, 4, cx - 11, cy + 9, 22, 2, 1);    // Heavy steel bumper & mudflaps
    fill_rect(base, 4, cx - 10, cy - 5, 20, 2, 1);    // Cargo bed rim

    // Arched Khaki Expedition Canvas Cargo Canopy (Y: cy - 14..cy - 4, col 10)
    fill_rect(base, 4, cx - 9, cy - 14, 18, 10, 10);  // Khaki canvas cover
    fill_rect(base, 4, cx - 8, cy - 13, 16, 8, 4);    // Sunlit canvas highlight
    fill_rect(base, 4, cx - 7, cy - 12, 14, 6, 10);
    // Canvas Tie-Down Ropes & Steel Bow Ribs
    for (int y = cy - 13; y <= cy - 5; y += 4) {
        fill_rect(base, 4, cx - 9, y, 18, 1, 6);      // Brown tie-down strap
    }

    // Rear Canvas Flap Opening (showing equipment cargo inside)
    fill_rect(base, 4, cx - 5, cy - 7, 10, 5, 1);     // Dark cargo interior
    fill_rect(base, 4, cx - 3, cy - 6, 6, 3, 4);      // Expedition wooden crate

    // Heavy Steel Dual Jerrycans Mounted on Rear Sides (Red fuel cans)
    fill_rect(base, 4, cx - 10, cy + 1, 3, 6, 12);    // Left red fuel can
    put_pix(base, 4, cx - 9, cy + 1, 14);             // Filler cap
    fill_rect(base, 4, cx + 7, cy + 1, 3, 6, 12);     // Right red fuel can
    put_pix(base, 4, cx + 8, cy + 1, 14);

    // 6 Massive High-Flotation Mud Tires (3 on each side)
    fill_rect(base, 4, cx - 13, cy - 4, 2, 4, 1);     // Front-left tire
    fill_rect(base, 4, cx - 13, cy + 1, 2, 4, 1);     // Mid-left tire
    fill_rect(base, 4, cx - 13, cy + 6, 2, 4, 1);     // Rear-left tire
    fill_rect(base, 4, cx + 11, cy - 4, 2, 4, 1);     // Front-right tire
    fill_rect(base, 4, cx + 11, cy + 1, 2, 4, 1);     // Mid-right tire
    fill_rect(base, 4, cx + 11, cy + 6, 2, 4, 1);     // Rear-right tire

    // Dual Rear Round Hazard Taillights (Y: cy + 8)
    fill_rect(base, 4, cx - 8, cy + 8, 2, 2, 12);
    fill_rect(base, 4, cx + 6, cy + 8, 2, 2, 12);
}

// Generate Open-Top Safari 4x4 with Winch & Snorkel (32x32, 4x4 = 16 tiles)
static void gen_obs_maya_jeep_lrg(u32 *base) {
    for (int i = 0; i < 16 * 8; i++) base[i] = 0;
    int cx = 16;
    int cy = 16;

    // Rugged Open-Top Safari 4x4 Chassis in Khaki/Army Green (col 11, col 10)
    fill_rect(base, 4, cx - 9, cy - 6, 18, 15, 11);   // Army green body
    fill_rect(base, 4, cx - 8, cy - 7, 16, 2, 10);    // Khaki cowl contour
    fill_rect(base, 4, cx - 10, cy + 8, 20, 2, 1);    // Tubular rear bumper

    // Heavy Tubular Roll Cage (Y: cy - 13..cy - 5, X: cx - 7..cx + 6)
    fill_rect(base, 4, cx - 7, cy - 12, 14, 2, 1);    // Overhead roll bar
    fill_rect(base, 4, cx - 7, cy - 12, 2, 8, 1);     // Left roll bar pillar
    fill_rect(base, 4, cx + 5, cy - 12, 2, 8, 1);     // Right roll bar pillar

    // Overhead Quad Safari Fog Lights mounted on roll bar (col 13 amber beams)
    fill_rect(base, 4, cx - 6, cy - 14, 2, 2, 13);
    put_pix(base, 4, cx - 5, cy - 14, 14);
    fill_rect(base, 4, cx - 3, cy - 14, 2, 2, 13);
    put_pix(base, 4, cx - 2, cy - 14, 14);
    fill_rect(base, 4, cx, cy - 14, 2, 2, 13);
    put_pix(base, 4, cx + 1, cy - 14, 14);
    fill_rect(base, 4, cx + 3, cy - 14, 2, 2, 13);
    put_pix(base, 4, cx + 4, cy - 14, 14);

    // Open Cabin Interior (High-backed bucket seats & spare gear)
    fill_rect(base, 4, cx - 5, cy - 5, 10, 6, 1);     // Dark cabin shadow
    fill_rect(base, 4, cx - 4, cy - 4, 3, 4, 10);     // Left driver seat (khaki)
    fill_rect(base, 4, cx + 1, cy - 4, 3, 4, 10);     // Right passenger seat

    // Right-Mounted Raised Snorkel Air Intake (along right pillar: X: cx + 7..cx + 8, Y: cy - 14..cy - 4)
    fill_rect(base, 4, cx + 7, cy - 13, 2, 10, 1);    // Black snorkel tube
    fill_rect(base, 4, cx + 7, cy - 14, 3, 2, 1);     // Snorkel ram-air intake scoop

    // Rear Mounted Spare Mud Tire on Tailgate (Centered: X: cx - 4..cx + 3, Y: cy + 1..cy + 8)
    fill_rect(base, 4, cx - 4, cy + 1, 8, 7, 1);      // Black spare tire
    fill_rect(base, 4, cx - 2, cy + 3, 4, 3, 4);      // Steel rim center
    put_pix(base, 4, cx, cy + 4, 14);                 // Lug nuts

    // Mud-Terrain Tires (High clearance)
    fill_rect(base, 4, cx - 12, cy - 3, 3, 6, 1);
    fill_rect(base, 4, cx + 9, cy - 3, 3, 6, 1);
    fill_rect(base, 4, cx - 12, cy + 4, 3, 5, 1);
    fill_rect(base, 4, cx + 9, cy + 4, 3, 5, 1);

    // Dual Red LED Taillights
    fill_rect(base, 4, cx - 8, cy + 6, 2, 2, 12);
    fill_rect(base, 4, cx + 6, cy + 6, 2, 2, 12);
}

// Generate Twinkling Clearance Star Sparkle (8x8, 1 tile)
static void gen_star_sparkle(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // Four-pointed star
    put_pix(base, 1, 3, 0, 1);
    put_pix(base, 1, 3, 1, 1);
    put_pix(base, 1, 3, 2, 3);
    put_pix(base, 1, 3, 3, 3); // White center core
    put_pix(base, 1, 4, 3, 3);
    put_pix(base, 1, 3, 4, 3);
    put_pix(base, 1, 3, 5, 1);
    put_pix(base, 1, 3, 6, 1);

    put_pix(base, 1, 0, 3, 1);
    put_pix(base, 1, 1, 3, 1);
    put_pix(base, 1, 2, 3, 3);
    put_pix(base, 1, 4, 3, 3);
    put_pix(base, 1, 5, 3, 1);
    put_pix(base, 1, 6, 3, 1);

    // Diagonal accents
    put_pix(base, 1, 2, 2, 12);
    put_pix(base, 1, 4, 2, 12);
    put_pix(base, 1, 2, 4, 12);
    put_pix(base, 1, 4, 4, 12);
}

// Generate Floating Score Popups (16x8, 2 tiles)
// "+150" in Gold
static void gen_popup_150(u32 *base) {
    for (int i = 0; i < 2 * 8; i++) base[i] = 0;
    // '+' at x=1..3, y=2..4
    put_pix(base, 2, 2, 2, 1); put_pix(base, 2, 1, 3, 1); put_pix(base, 2, 2, 3, 3); put_pix(base, 2, 3, 3, 1); put_pix(base, 2, 2, 4, 1);
    // '1' at x=5..6, y=1..5
    put_pix(base, 2, 5, 1, 3); put_pix(base, 2, 6, 1, 1);
    put_pix(base, 2, 6, 2, 1); put_pix(base, 2, 6, 3, 1); put_pix(base, 2, 6, 4, 1);
    put_pix(base, 2, 5, 5, 1); put_pix(base, 2, 6, 5, 1); put_pix(base, 2, 7, 5, 1);
    // '5' at x=8..11, y=1..5
    put_pix(base, 2, 8, 1, 1); put_pix(base, 2, 9, 1, 1); put_pix(base, 2, 10, 1, 1);
    put_pix(base, 2, 8, 2, 1); put_pix(base, 2, 9, 3, 1); put_pix(base, 2, 10, 3, 1);
    put_pix(base, 2, 11, 4, 1); put_pix(base, 2, 8, 5, 1); put_pix(base, 2, 9, 5, 1); put_pix(base, 2, 10, 5, 1);
    // '0' at x=12..15, y=1..5
    put_pix(base, 2, 13, 1, 1); put_pix(base, 2, 14, 1, 1);
    put_pix(base, 2, 12, 2, 1); put_pix(base, 2, 15, 2, 1);
    put_pix(base, 2, 12, 3, 1); put_pix(base, 2, 15, 3, 1);
    put_pix(base, 2, 12, 4, 1); put_pix(base, 2, 15, 4, 1);
    put_pix(base, 2, 13, 5, 1); put_pix(base, 2, 14, 5, 1);
}

// "+100" in Cyan
static void gen_popup_100(u32 *base) {
    for (int i = 0; i < 2 * 8; i++) base[i] = 0;
    // '+' at x=1..3
    put_pix(base, 2, 2, 2, 10); put_pix(base, 2, 1, 3, 10); put_pix(base, 2, 2, 3, 11); put_pix(base, 2, 3, 3, 10); put_pix(base, 2, 2, 4, 10);
    // '1' at x=5..6
    put_pix(base, 2, 5, 1, 11); put_pix(base, 2, 6, 1, 10);
    put_pix(base, 2, 6, 2, 10); put_pix(base, 2, 6, 3, 10); put_pix(base, 2, 6, 4, 10);
    put_pix(base, 2, 5, 5, 10); put_pix(base, 2, 6, 5, 10); put_pix(base, 2, 7, 5, 10);
    // '0' at x=8..11
    put_pix(base, 2, 9, 1, 10); put_pix(base, 2, 10, 1, 10);
    put_pix(base, 2, 8, 2, 10); put_pix(base, 2, 11, 2, 10);
    put_pix(base, 2, 8, 3, 10); put_pix(base, 2, 11, 3, 10);
    put_pix(base, 2, 8, 4, 10); put_pix(base, 2, 11, 4, 10);
    put_pix(base, 2, 9, 5, 10); put_pix(base, 2, 10, 5, 10);
    // '0' at x=12..15
    put_pix(base, 2, 13, 1, 10); put_pix(base, 2, 14, 1, 10);
    put_pix(base, 2, 12, 2, 10); put_pix(base, 2, 15, 2, 10);
    put_pix(base, 2, 12, 3, 10); put_pix(base, 2, 15, 3, 10);
    put_pix(base, 2, 12, 4, 10); put_pix(base, 2, 15, 4, 10);
    put_pix(base, 2, 13, 5, 10); put_pix(base, 2, 14, 5, 10);
}

// "+250" in Emerald Green / Gold
static void gen_popup_250(u32 *base) {
    for (int i = 0; i < 2 * 8; i++) base[i] = 0;
    // '+' at x=1..3
    put_pix(base, 2, 2, 2, 1); put_pix(base, 2, 1, 3, 1); put_pix(base, 2, 2, 3, 3); put_pix(base, 2, 3, 3, 1); put_pix(base, 2, 2, 4, 1);
    // '2' at x=5..7
    put_pix(base, 2, 5, 1, 1); put_pix(base, 2, 6, 1, 3); put_pix(base, 2, 7, 1, 1);
    put_pix(base, 2, 7, 2, 1); put_pix(base, 2, 6, 3, 1); put_pix(base, 2, 5, 4, 1);
    put_pix(base, 2, 5, 5, 1); put_pix(base, 2, 6, 5, 1); put_pix(base, 2, 7, 5, 1);
    // '5' at x=9..11
    put_pix(base, 2, 9, 1, 1); put_pix(base, 2, 10, 1, 1); put_pix(base, 2, 11, 1, 1);
    put_pix(base, 2, 9, 2, 1); put_pix(base, 2, 10, 3, 1); put_pix(base, 2, 11, 3, 1);
    put_pix(base, 2, 12, 4, 1); put_pix(base, 2, 9, 5, 1); put_pix(base, 2, 10, 5, 1); put_pix(base, 2, 11, 5, 1);
    // '0' at x=13..15
    put_pix(base, 2, 14, 1, 1); put_pix(base, 2, 13, 2, 1); put_pix(base, 2, 15, 2, 1);
    put_pix(base, 2, 13, 3, 1); put_pix(base, 2, 15, 3, 1);
    put_pix(base, 2, 13, 4, 1); put_pix(base, 2, 15, 4, 1);
    put_pix(base, 2, 14, 5, 1);
}

// Generate 8x8 HUD Gold Coin Icon (Sprite Palette 2)
static void gen_hud_coin(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // 8x8 round golden coin icon in Sprite Palette 2
    // Color 1: Bright Gold, Color 2: Amber, Color 3: White shimmer, Color 4: Dark Gold Edge
    base[0] = 0x00444400;
    base[1] = 0x04311240;
    base[2] = 0x43311124;
    base[3] = 0x43111124;
    base[4] = 0x41111124;
    base[5] = 0x42111244;
    base[6] = 0x04222240;
    base[7] = 0x00444400;
}

// Generate 8x8 HUD Wheel Icon (Sprite Palette 2)
static void gen_hud_wheel(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // 8x8 sports car racing alloy wheel in Sprite Palette 2
    // Color 7: Metal Silver tread outline, Color 14: Smoke Grey rubber,
    // Color 9: Bright Silver rim/spokes, Color 11: Specular White shine
    base[0] = 0x00777700;
    base[1] = 0x07E99E70;
    base[2] = 0x7E9BB9E7;
    base[3] = 0x79900997;
    base[4] = 0x79900997;
    base[5] = 0x7E9999E7;
    base[6] = 0x07E99E70;
    base[7] = 0x00777700;
}

// Generate 8x8 Glossy Color Swatch Chiclet
static void gen_color_swatch(u32 *base) {
    base[0] = 0x02222220;
    base[1] = 0x21111332;
    base[2] = 0x21111132;
    base[3] = 0x21111112;
    base[4] = 0x22111112;
    base[5] = 0x22221112;
    base[6] = 0x22222222;
    base[7] = 0x02222220;
}

// Generate 8x8 Active Color Selector Chevron (Sprite Palette 2)
static void gen_color_selector(u32 *base) {
    base[0] = 0x00000000;
    base[1] = 0x00011000;
    base[2] = 0x00133100;
    base[3] = 0x01333310;
    base[4] = 0x13300331;
    base[5] = 0x11000011;
    base[6] = 0x00000000;
    base[7] = 0x00000000;
}

// Generate 8x8 Drifting Winter Snowflake (Sprite Palette 2)
static void gen_snowflake(u32 *base) {
    for (int i = 0; i < 8; i++) base[i] = 0;
    // Delicate 6-axis crystal snowflake pattern centered at (3, 3)
    put_pix(base, 1, 3, 3, 11); // Center white crystal
    put_pix(base, 1, 4, 3, 11);
    put_pix(base, 1, 3, 4, 11);
    put_pix(base, 1, 4, 4, 11);

    // Cross arms
    put_pix(base, 1, 3, 1, 11);
    put_pix(base, 1, 4, 1, 11);
    put_pix(base, 1, 3, 6, 11);
    put_pix(base, 1, 4, 6, 11);
    put_pix(base, 1, 1, 3, 11);
    put_pix(base, 1, 1, 4, 11);
    put_pix(base, 1, 6, 3, 11);
    put_pix(base, 1, 6, 4, 11);

    // Cyan crystal tips
    put_pix(base, 1, 2, 2, 10);
    put_pix(base, 1, 5, 2, 10);
    put_pix(base, 1, 2, 5, 10);
    put_pix(base, 1, 5, 5, 10);
}

// === Mechanic-Expanding Stunt Ramps (32x16, 8 tiles, 4 tiles wide) ===
static void gen_obs_ramp_city(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Sloped metal construction ramp (Y: 2..15, X: 2..29)
    fill_rect(base, 4, 2, 8, 3, 7, 14); // Steel grey left truss
    fill_rect(base, 4, 27, 8, 3, 7, 14); // Steel grey right truss
    for (int y = 4; y < 15; y++) {
        u8 col = ((y & 2) == 0) ? 1 : 2; // Alternating yellow / amber hazard deck
        fill_rect(base, 4, 5, y, 22, 1, col);
    }
    // High-visibility forward launch chevrons "^"
    for (int ch = 0; ch < 3; ch++) {
        int cy = 6 + ch * 3;
        put_pix(base, 4, 15, cy - 1, 4); put_pix(base, 4, 16, cy - 1, 4); // White apex
        put_pix(base, 4, 14, cy, 4);     put_pix(base, 4, 17, cy, 4);
        put_pix(base, 4, 13, cy + 1, 4); put_pix(base, 4, 18, cy + 1, 4);
    }
    fill_rect(base, 4, 5, 3, 22, 2, 10); // Chrome lip
}

static void gen_obs_ramp_beach(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Bamboo kicker ramp
    fill_rect(base, 4, 3, 7, 3, 8, 3); // Dark mahogany struts
    fill_rect(base, 4, 26, 7, 3, 8, 3);
    for (int y = 5; y < 15; y++) {
        u8 c = (y % 3 == 0) ? 1 : (y % 3 == 1 ? 9 : 2); // Bamboo gold / honey oak planks
        fill_rect(base, 4, 6, y, 20, 1, c);
    }
    fill_rect(base, 4, 6, 4, 20, 2, 4); // White gelcoat lip
    for (int ch = 0; ch < 2; ch++) {
        int cy = 7 + ch * 4;
        put_pix(base, 4, 15, cy - 1, 13); put_pix(base, 4, 16, cy - 1, 13);
        put_pix(base, 4, 14, cy, 13);     put_pix(base, 4, 17, cy, 13);
    }
}

static void gen_obs_ramp_winter(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Glacial snowpack jump with red guide poles
    fill_rect(base, 4, 2, 2, 2, 13, 5);  // Red flag left
    fill_rect(base, 4, 28, 2, 2, 13, 5); // Red flag right
    for (int y = 4; y < 15; y++) {
        u8 c = (y < 7) ? 3 : (y < 11 ? 1 : 2); // White snow crest -> icy cyan slope
        fill_rect(base, 4, 4, y, 24, 1, c);
    }
    for (int x = 6; x < 26; x += 4) {
        fill_rect(base, 4, x, 7, 1, 7, 8); // Cyan ice shimmer
    }
}

static void gen_obs_ramp_orbital(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Aerospace mag-lev booster strip
    fill_rect(base, 4, 3, 5, 26, 10, 1); // Dark chassis
    fill_rect(base, 4, 4, 6, 24, 8, 2);
    for (int ch = 0; ch < 3; ch++) {
        int cy = 6 + ch * 3;
        put_pix(base, 4, 15, cy, 13); put_pix(base, 4, 16, cy, 13); // Glowing cyan LED
        put_pix(base, 4, 14, cy + 1, 13); put_pix(base, 4, 17, cy + 1, 13);
        put_pix(base, 4, 13, cy + 2, 13); put_pix(base, 4, 18, cy + 2, 13);
    }
    fill_rect(base, 4, 2, 12, 3, 3, 11); // Safety orange pads
    fill_rect(base, 4, 27, 12, 3, 3, 11);
}

static void gen_obs_ramp_gothic(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Weathered stone crypt sarcophagus ramp
    fill_rect(base, 4, 3, 5, 26, 10, 2); // Dark crypt stone
    fill_rect(base, 4, 5, 6, 22, 8, 1);  // Ash grey slab
    fill_rect(base, 4, 15, 6, 2, 8, 3);
    fill_rect(base, 4, 11, 8, 10, 2, 3);
    put_pix(base, 4, 3, 4, 10); put_pix(base, 4, 28, 4, 10); // Iron spire studs
}

static void gen_obs_ramp_maya(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Carved stepped stone temple ramp
    fill_rect(base, 4, 2, 10, 28, 5, 2); // Base stone step
    fill_rect(base, 4, 4, 7, 24, 4, 1);  // Middle step
    fill_rect(base, 4, 7, 4, 18, 4, 13); // Sun gold top step
    put_pix(base, 4, 10, 8, 11); put_pix(base, 4, 15, 8, 11); put_pix(base, 4, 20, 8, 11);
    put_pix(base, 4, 12, 5, 11); put_pix(base, 4, 18, 5, 11);
}

// === Mechanic-Expanding Surface Slicks (32x16, 8 tiles, 4 tiles wide) ===
static void gen_obs_slick_city(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Iridescent motor oil puddle (Y: 7..15, X: 4..27)
    fill_rect(base, 4, 7, 9, 18, 5, 3); // Black oil base
    fill_rect(base, 4, 5, 10, 22, 4, 3);
    fill_rect(base, 4, 8, 10, 16, 3, 14); // Steel sheen highlight
    put_pix(base, 4, 6, 11, 6); put_pix(base, 4, 7, 11, 8); put_pix(base, 4, 23, 11, 1);
    put_pix(base, 4, 12, 9, 2); put_pix(base, 4, 18, 13, 6);
}

static void gen_obs_slick_beach(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Wet tidal surge & glistening sea foam
    fill_rect(base, 4, 5, 8, 22, 6, 7); // Deep wet sand turquoise
    fill_rect(base, 4, 7, 9, 18, 5, 6); // Ocean turquoise
    put_pix(base, 4, 6, 8, 4); put_pix(base, 4, 11, 8, 4); put_pix(base, 4, 20, 8, 4);
    put_pix(base, 4, 8, 13, 4); put_pix(base, 4, 16, 14, 4); put_pix(base, 4, 24, 12, 4);
}

static void gen_obs_slick_winter(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Crystal black ice patch
    fill_rect(base, 4, 6, 8, 20, 6, 2); // Dark asphalt ice sheen
    fill_rect(base, 4, 8, 9, 16, 4, 1); // Glacial cyan reflection
    put_pix(base, 4, 10, 9, 3); put_pix(base, 4, 11, 9, 3);
    put_pix(base, 4, 18, 11, 3); put_pix(base, 4, 19, 11, 3);
}

static void gen_obs_slick_orbital(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Glowing radioactive hyper-coolant puddle
    fill_rect(base, 4, 6, 8, 20, 6, 1);  // Dark pool basin
    fill_rect(base, 4, 8, 9, 16, 4, 13); // Glowing cyan coolant
    put_pix(base, 4, 12, 10, 5); put_pix(base, 4, 13, 10, 5);
    put_pix(base, 4, 17, 11, 5); put_pix(base, 4, 18, 11, 5);
}

static void gen_obs_slick_gothic(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Bubbling eerie ectoplasm slime
    fill_rect(base, 4, 5, 8, 22, 6, 3);  // Dark sludge outline
    fill_rect(base, 4, 7, 9, 18, 5, 11); // Green soul slime
    put_pix(base, 4, 9, 10, 1); put_pix(base, 4, 15, 10, 1); put_pix(base, 4, 21, 11, 1);
}

static void gen_obs_slick_maya(u32 *base) {
    for (int i = 0; i < 64; i++) base[i] = 0;
    // Wet jungle moss & red clay mud
    fill_rect(base, 4, 5, 8, 22, 6, 3);  // Mud boundary
    fill_rect(base, 4, 7, 9, 18, 5, 2);  // Wet clay brown
    fill_rect(base, 4, 9, 10, 14, 3, 11); // Slippery jungle moss
}

// === Track Signature Hazards ===
// Heavy Industrial Chemical Tanker (64x32, 32 tiles, 8 tiles wide - spans 2 lanes!)
static void gen_obs_sig_city(u32 *base) {
    for (int i = 0; i < 256; i++) base[i] = 0;

    // 1. Heavy Underbody Bogie Wheels (Dual tandem wheel assemblies)
    // Left tandem wheel pair
    fill_rect(base, 8, 8, 25, 8, 6, 3);   // Left-outer tire (charcoal)
    fill_rect(base, 8, 10, 27, 4, 3, 10); // Chrome rim
    fill_rect(base, 8, 20, 25, 8, 6, 3);  // Left-inner tire
    fill_rect(base, 8, 22, 27, 4, 3, 10);
    // Right tandem wheel pair
    fill_rect(base, 8, 36, 25, 8, 6, 3);  // Right-inner tire
    fill_rect(base, 8, 38, 27, 4, 3, 10);
    fill_rect(base, 8, 48, 25, 8, 6, 3);  // Right-outer tire
    fill_rect(base, 8, 50, 27, 4, 3, 10);

    // Heavy structural chassis beam & undercarriage
    fill_rect(base, 8, 6, 23, 52, 3, 14);  // Steel I-beam frame
    fill_rect(base, 8, 4, 25, 56, 2, 15);  // Black shadow underside

    // 2. Rear Underride Guard (Hazard Bumper Bar)
    fill_rect(base, 8, 6, 24, 52, 2, 1);   // High-vis hazard yellow base
    for (int x = 8; x < 56; x += 6) {
        fill_rect(base, 8, x, 24, 3, 2, 3); // Black diagonal hazard chevrons
    }
    // Bumper Taillights & Clearance Reflectors
    fill_rect(base, 8, 6, 23, 3, 2, 9);   // Left taillight red
    fill_rect(base, 8, 55, 23, 3, 2, 9);  // Right taillight red
    fill_rect(base, 8, 10, 23, 2, 1, 1);  // Amber reflector left
    fill_rect(base, 8, 52, 23, 2, 1, 1);  // Amber reflector right

    // 3. Massive Pressurized Chemical Cylinder (Main Tank Body: 52px wide x 17px tall)
    // Left and Right chrome domed pressure caps
    fill_rect(base, 8, 4, 9, 4, 11, 10);
    fill_rect(base, 8, 56, 9, 4, 11, 10);
    put_pix(base, 8, 4, 9, 0); put_pix(base, 8, 4, 19, 0);
    put_pix(base, 8, 59, 9, 0); put_pix(base, 8, 59, 19, 0);

    // Cylinder Upper Specular & Chrome Highlight
    fill_rect(base, 8, 8, 6, 48, 2, 10);  // Specular chrome curve
    fill_rect(base, 8, 10, 7, 44, 1, 4);  // Brilliant white sheen

    // Main Cylinder Tank Vessel (Clean industrial white coating)
    fill_rect(base, 8, 6, 8, 52, 6, 4);

    // Hazardous Chemical Red Identity Band
    fill_rect(base, 8, 6, 14, 52, 3, 5);  // Vivid red warning band

    // Cylinder Lower Shading & Steel Frame
    fill_rect(base, 8, 6, 17, 52, 4, 14); // Steel curve shadow
    fill_rect(base, 8, 7, 21, 50, 2, 15); // Deep lower contour shadow

    // 4. Catwalk, Inspection Hatches & Pressure Relief Valves
    fill_rect(base, 8, 14, 4, 36, 2, 14); // Top walkway grating
    fill_rect(base, 8, 18, 3, 6, 2, 10);  // Left inspection hatch
    fill_rect(base, 8, 40, 3, 6, 2, 10);  // Right inspection hatch
    fill_rect(base, 8, 30, 2, 4, 3, 10);  // Central relief valve
    put_pix(base, 8, 31, 1, 9); put_pix(base, 8, 32, 1, 9); // Red valve cap

    // 5. Hazmat Placards (Warning Diamonds)
    // Center prominent Hazmat warning diamond (X: 30..34, Y: 12..16)
    put_pix(base, 8, 32, 12, 1);
    fill_rect(base, 8, 31, 13, 3, 1, 1);
    fill_rect(base, 8, 30, 14, 5, 1, 1);
    fill_rect(base, 8, 31, 15, 3, 1, 1);
    put_pix(base, 8, 32, 16, 1);
    put_pix(base, 8, 32, 14, 3); // Black skull/hazard glyph

    // Left and Right secondary hazmat badges
    fill_rect(base, 8, 14, 13, 4, 3, 1);
    put_pix(base, 8, 15, 14, 3);
    fill_rect(base, 8, 46, 13, 4, 3, 1);
    put_pix(base, 8, 47, 14, 3);

    // 6. Upper Clearance Marker Lamps
    fill_rect(base, 8, 8, 5, 2, 1, 9);   // Left red clearance lamp
    fill_rect(base, 8, 54, 5, 2, 1, 9);  // Right red clearance lamp
    fill_rect(base, 8, 28, 4, 2, 1, 1);  // Center amber marker
    fill_rect(base, 8, 34, 4, 2, 1, 1);

    // 7. Outer Black Contour Outlines
    fill_rect(base, 8, 7, 5, 50, 1, 15);
    fill_rect(base, 8, 5, 22, 54, 1, 15);
}

static void gen_obs_sig_beach(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Tumbling Windsurf Rig
    for (int y = 4; y < 28; y++) {
        int w = (y - 3);
        if (w > 20) w = 20;
        int x0 = 6;
        u8 col = (y < 12) ? 5 : (y < 20 ? 13 : 6);
        fill_rect(base, 4, x0, y, w, 1, col);
    }
    fill_rect(base, 4, 5, 2, 2, 28, 14);  // Carbon mast
    fill_rect(base, 4, 6, 15, 18, 2, 10); // Wishbone boom
    fill_rect(base, 4, 23, 16, 2, 4, 10);
}

static void gen_obs_sig_winter(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Craggy Glacial Snowball / Ice Boulder
    int cx = 16, cy = 16, r = 12;
    for (int y = 4; y <= 28; y++) {
        for (int x = 4; x <= 28; x++) {
            int d2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
            if (d2 <= r * r) {
                u8 c = (x < cx - 2 && y < cy - 2) ? 3 :
                       (d2 < (r - 4) * (r - 4)) ? 1 :
                       (x > cx + 2) ? 2 : 1;
                put_pix(base, 4, x, y, c);
            }
        }
    }
    fill_rect(base, 4, 12, 10, 2, 6, 2);
    fill_rect(base, 4, 17, 14, 5, 2, 2);
    fill_rect(base, 4, 14, 18, 2, 5, 2);
}

static void gen_obs_sig_orbital_on(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Active Plasma Laser Barrier
    fill_rect(base, 4, 2, 6, 5, 24, 1);   // Steel pylon left
    fill_rect(base, 4, 25, 6, 5, 24, 1);  // Steel pylon right
    fill_rect(base, 4, 3, 8, 3, 20, 10);  // Chrome core
    fill_rect(base, 4, 26, 8, 3, 20, 10);
    fill_rect(base, 4, 7, 16, 18, 6, 13); // Cyan plasma glow
    fill_rect(base, 4, 7, 18, 18, 2, 5);  // Core white energy discharge
    put_pix(base, 4, 7, 15, 5); put_pix(base, 4, 24, 15, 5);
    put_pix(base, 4, 7, 22, 5); put_pix(base, 4, 24, 22, 5);
}

static void gen_obs_sig_orbital_off(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Inactive / Charging Laser Pylons (Laser beam extinguished!)
    fill_rect(base, 4, 2, 6, 5, 24, 1);
    fill_rect(base, 4, 25, 6, 5, 24, 1);
    fill_rect(base, 4, 3, 8, 3, 20, 10);
    fill_rect(base, 4, 26, 8, 3, 20, 10);
    fill_rect(base, 4, 3, 17, 3, 4, 11); // Orange charging indicators
    fill_rect(base, 4, 26, 17, 3, 4, 11);
}

static void gen_obs_sig_gothic(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Small Stone Gargoyle crouched low on asphalt:
    // Plinth / Pedestal base resting on the road (y = 26..30)
    fill_rect(base, 4, 8, 28, 16, 3, 2);      // Dark stone base
    fill_rect(base, 4, 9, 27, 14, 1, 3);      // Slate bevel
    fill_rect(base, 4, 10, 26, 12, 1, 4);     // Carved ledge highlight
    fill_rect(base, 4, 8, 30, 16, 1, 15);     // Asphalt contact shadow

    // Crouching muscular stone claws gripping the ledge (y = 23..27)
    fill_rect(base, 4, 9, 24, 4, 3, 3);       // Left claw foot
    fill_rect(base, 4, 19, 24, 4, 3, 3);      // Right claw foot
    put_pix(base, 4, 8, 26, 1); put_pix(base, 4, 10, 26, 1); put_pix(base, 4, 12, 26, 1); // Left talons
    put_pix(base, 4, 19, 26, 1); put_pix(base, 4, 21, 26, 1); put_pix(base, 4, 23, 26, 1); // Right talons
    put_pix(base, 4, 9, 24, 4); put_pix(base, 4, 20, 24, 4); // Highlights

    // Compact hunched gargoyle torso (y = 17..24, x = 11..21)
    fill_rect(base, 4, 11, 18, 10, 7, 2);     // Torso shadow core
    fill_rect(base, 4, 12, 19, 8, 5, 3);      // Midtone chest & ribcage
    fill_rect(base, 4, 13, 20, 6, 3, 4);      // Chest highlight
    put_pix(base, 4, 15, 21, 5); put_pix(base, 4, 16, 21, 5); // Sternum bone highlight

    // Low arched spiked bat wings spreading outward (y = 12..25)
    for (int y = 0; y < 9; y++) {
        int span = 2 + y * 1;
        // Left wing membrane & bone rib
        fill_rect(base, 4, 11 - span, 15 + y, span, 1, 2);
        put_pix(base, 4, 11 - span, 15 + y, 3);
        // Right wing membrane & bone rib
        fill_rect(base, 4, 21, 15 + y, span, 1, 2);
        put_pix(base, 4, 20 + span, 15 + y, 3);
    }
    // Wing tip bone claws / spikes
    put_pix(base, 4, 2, 14, 4); put_pix(base, 4, 1, 13, 5);
    put_pix(base, 4, 29, 14, 4); put_pix(base, 4, 30, 13, 5);
    put_pix(base, 4, 5, 18, 1); put_pix(base, 4, 26, 18, 1);

    // Fierce demonic horned gargoyle head (y = 11..17, x = 12..20)
    fill_rect(base, 4, 12, 12, 8, 6, 3);      // Head base
    fill_rect(base, 4, 13, 13, 6, 4, 4);      // Brow & snout
    put_pix(base, 4, 13, 11, 1); put_pix(base, 4, 18, 11, 1); // Horn bases
    put_pix(base, 4, 12, 10, 4); put_pix(base, 4, 19, 10, 4); // Horn shafts
    put_pix(base, 4, 11, 9, 5);  put_pix(base, 4, 20, 9, 5);  // Sharp curved horn tips

    // Piercing glowing blood-red eyes with violet mist aura
    put_pix(base, 4, 13, 14, 12); put_pix(base, 4, 18, 14, 12); // Violet aura
    put_pix(base, 4, 14, 14, 13); put_pix(base, 4, 17, 14, 13); // Glowing red demonic eyes
    put_pix(base, 4, 14, 15, 13); put_pix(base, 4, 17, 15, 13);

    // Fanged maw / snout
    put_pix(base, 4, 15, 16, 1); put_pix(base, 4, 16, 16, 1);   // Snarl mouth
    put_pix(base, 4, 14, 16, 14); put_pix(base, 4, 17, 16, 14); // White fangs
}

static void gen_obs_sig_maya(u32 *base) {
    for (int i = 0; i < 128; i++) base[i] = 0;
    // Maya Temple Trail: Ancient Poison Dart Trap Volley
    // Flanking carved serpent stone blowpipe totems firing horizontal stream of jade poison darts!

    // Left Totem Pillar (x = 1..7, y = 6..30)
    fill_rect(base, 4, 1, 24, 7, 7, 2);       // Base pedestal
    fill_rect(base, 4, 2, 25, 5, 5, 3);
    fill_rect(base, 4, 2, 8, 5, 16, 3);       // Pillar shaft
    fill_rect(base, 4, 3, 9, 3, 14, 4);       // Carved relief
    fill_rect(base, 4, 1, 6, 7, 6, 2);        // Serpent head capital
    fill_rect(base, 4, 2, 7, 5, 4, 3);
    put_pix(base, 4, 4, 7, 13);               // Amber snake eye
    put_pix(base, 4, 6, 9, 1);                // Open mouth / blowpipe muzzle
    put_pix(base, 4, 7, 9, 9);                // Glowing jade muzzle flash spark

    // Right Totem Pillar (x = 24..30, y = 6..30)
    fill_rect(base, 4, 24, 24, 7, 7, 2);      // Base pedestal
    fill_rect(base, 4, 25, 25, 5, 5, 3);
    fill_rect(base, 4, 25, 8, 5, 16, 3);      // Pillar shaft
    fill_rect(base, 4, 26, 9, 3, 14, 4);
    fill_rect(base, 4, 24, 6, 7, 6, 2);       // Serpent head capital
    fill_rect(base, 4, 25, 7, 5, 4, 3);
    put_pix(base, 4, 27, 7, 13);              // Amber snake eye
    put_pix(base, 4, 25, 9, 1);               // Open mouth / blowpipe muzzle
    put_pix(base, 4, 24, 9, 9);               // Glowing jade muzzle flash spark

    // Overgrown jungle vines wrapping around the stone pillars
    put_pix(base, 4, 3, 12, 7); put_pix(base, 4, 4, 13, 7); put_pix(base, 4, 3, 18, 6);
    put_pix(base, 4, 28, 14, 7); put_pix(base, 4, 27, 15, 7); put_pix(base, 4, 28, 20, 6);

    // Whistling Horizontal Jade Dart Stream (y = 8..14, x = 6..25)
    // Dart 1 (lead dart, moving right)
    fill_rect(base, 4, 19, 9, 4, 1, 1);       // Obsidian shaft
    put_pix(base, 4, 23, 9, 14);              // White needle tip glint
    put_pix(base, 4, 24, 9, 9);               // Glowing poisonous jade barb
    fill_rect(base, 4, 15, 9, 4, 1, 8);       // Toxic venom tracer line
    put_pix(base, 4, 14, 9, 7);               // Green vapor trail
    put_pix(base, 4, 18, 8, 9);               // Poison droplet spark
    put_pix(base, 4, 20, 10, 8);

    // Dart 2 (trailing dart, slightly staggered in trajectory)
    fill_rect(base, 4, 11, 11, 4, 1, 1);      // Obsidian shaft
    put_pix(base, 4, 15, 11, 14);             // White needle glint
    put_pix(base, 4, 16, 11, 9);              // Glowing poisonous jade barb
    fill_rect(base, 4, 7, 11, 4, 1, 8);       // Toxic venom tracer line
    put_pix(base, 4, 6, 11, 7);

    // Dart 3 (upper high-speed reed dart)
    fill_rect(base, 4, 14, 7, 3, 1, 1);
    put_pix(base, 4, 17, 7, 9);
    fill_rect(base, 4, 10, 7, 4, 1, 8);

    // Toxic whistling wind / speed ripples across the lane
    put_pix(base, 4, 8, 8, 14);
    put_pix(base, 4, 13, 13, 14);
    put_pix(base, 4, 21, 8, 7);
    put_pix(base, 4, 12, 10, 9);
}

// Stunt Popup: 16x8 (2 tiles: 16 pixels wide, 8 high)
static void gen_popup_stunt(u32 *base) {
    for (int i = 0; i < 16; i++) base[i] = 0;
    fill_rect(base, 2, 1, 1, 14, 6, 1); // Gold background
    fill_rect(base, 2, 2, 2, 12, 4, 3); // White text core
    for (int x = 0; x < 16; x++) { put_pix(base, 2, x, 0, 15); put_pix(base, 2, x, 7, 15); }
    for (int y = 0; y < 8; y++)  { put_pix(base, 2, 0, y, 15); put_pix(base, 2, 15, y, 15); }
}

EWRAM_DATA static u32 s_tile_buffer[1024 * 8];

// Initialize all graphics in VRAM
void gfx_init_all(void) {
    // 1. DMA Background & Sprite Palettes
    dma3_cpy(pal_bg_mem, bg_pal_data, sizeof(bg_pal_data));
    // Also copy HUD colors into Palette Bank 15 for TTE default compatibility
    dma3_cpy(&pal_bg_mem[240], bg_pal_data, 16 * sizeof(COLOR));
    dma3_cpy(pal_obj_mem, spr_pal_data, sizeof(spr_pal_data));

    // Copy track preview palettes into BG Palette Banks 3..7 for simultaneous preview rendering
    dma3_cpy(&pal_bg_mem[48],  &bg_pal_beach[32],   16 * sizeof(COLOR)); // Bank 3
    dma3_cpy(&pal_bg_mem[64],  &bg_pal_winter[32],  16 * sizeof(COLOR)); // Bank 4
    dma3_cpy(&pal_bg_mem[80],  &bg_pal_orbital[32], 16 * sizeof(COLOR)); // Bank 5
    dma3_cpy(&pal_bg_mem[96],  &bg_pal_gothic[32],  16 * sizeof(COLOR)); // Bank 6
    dma3_cpy(&pal_bg_mem[112], &bg_pal_maya[32],    16 * sizeof(COLOR)); // Bank 7

    // Copy all 6 car palettes into Sprite Palette Banks 4..9 for simultaneous swatch rendering
    for (int c = 0; c < CAR_COLOR_COUNT; c++) {
        dma3_cpy(&pal_obj_mem[(4 + c) * 16], g_car_color_pals[c], 16 * sizeof(COLOR));
    }

    // Sprite Palette 10: Palm Beach Themed Obstacles
    dma3_cpy(&pal_obj_mem[10 * 16], spr_pal_beach, 16 * sizeof(COLOR));
    // Sprite Palette 11: Winter Snow Themed Obstacles
    dma3_cpy(&pal_obj_mem[11 * 16], spr_pal_winter, 16 * sizeof(COLOR));
    // Sprite Palette 12: Cape Orbital Themed Obstacles
    dma3_cpy(&pal_obj_mem[12 * 16], spr_pal_orbital, 16 * sizeof(COLOR));
    // Sprite Palette 13: Gothic Midnight Themed Obstacles
    dma3_cpy(&pal_obj_mem[13 * 16], spr_pal_gothic, 16 * sizeof(COLOR));
    // Sprite Palette 14: Maya Temple Trail Themed Obstacles
    dma3_cpy(&pal_obj_mem[14 * 16], spr_pal_maya, 16 * sizeof(COLOR));

    // 2. Clear Sprite RAM Buffer (Full 1024 tiles)
    memset32(s_tile_buffer, 0, 1024 * 8);

    // 3. Generate Sprite Assets in RAM
    gen_player_car(s_tile_buffer + TILE_PLAYER_STRAIGHT * 8, 0);
    gen_player_car(s_tile_buffer + TILE_PLAYER_LEFT * 8, -1);
    gen_player_car(s_tile_buffer + TILE_PLAYER_RIGHT * 8, 1);
    gen_player_shrink_0(s_tile_buffer + TILE_PLAYER_SHRINK_0 * 8);
    gen_player_shrink_1(s_tile_buffer + TILE_PLAYER_SHRINK_1 * 8);
    gen_player_jump_up(s_tile_buffer + TILE_PLAYER_JUMP_UP * 8);
    gen_player_jump_down(s_tile_buffer + TILE_PLAYER_JUMP_DOWN * 8);
    gen_shadow(s_tile_buffer + TILE_SHADOW * 8);
    gen_shadow_sml(s_tile_buffer + TILE_SHADOW_SML * 8);
    gen_underglow(s_tile_buffer + TILE_UNDERGLOW * 8);

    gen_nitro_fire_0(s_tile_buffer + TILE_NITRO_FIRE_0 * 8);
    gen_nitro_fire_1(s_tile_buffer + TILE_NITRO_FIRE_1 * 8);
    gen_nitro_fire_2(s_tile_buffer + TILE_NITRO_FIRE_2 * 8);
    gen_nitro_fire_3(s_tile_buffer + TILE_NITRO_FIRE_3 * 8);
    gen_nitro_burst(s_tile_buffer + TILE_NITRO_BURST * 8);

    gen_shield_expand(s_tile_buffer + TILE_SHIELD_EXPAND * 8);
    gen_shield_spark(s_tile_buffer + TILE_SHIELD_SPARK * 8);
    gen_spark(s_tile_buffer + TILE_SPARK * 8);

    // City Highway Obstacles (Sprite Palette 1)
    gen_obstacle_barrier_lrg(s_tile_buffer + TILE_OBS_BARRIER_LRG * 8);
    gen_obstacle_barrier_sml(s_tile_buffer + TILE_OBS_BARRIER_SML * 8);
    gen_obstacle_sign_lrg(s_tile_buffer + TILE_OBS_SIGN_LRG * 8);
    gen_obstacle_sign_sml(s_tile_buffer + TILE_OBS_SIGN_SML * 8);
    gen_obstacle_truck_lrg(s_tile_buffer + TILE_OBS_TRUCK_LRG * 8);
    gen_obstacle_truck_sml(s_tile_buffer + TILE_OBS_TRUCK_SML * 8);
    gen_obstacle_civilian_lrg(s_tile_buffer + TILE_OBS_CIVILIAN_LRG * 8);
    gen_obstacle_civilian_sml(s_tile_buffer + TILE_OBS_CIVILIAN_SML * 8);
    gen_turn_signal(s_tile_buffer + TILE_TURN_SIGNAL * 8);

    // City Clearance States & Feedback Popups
    gen_obstacle_barrier_pass(s_tile_buffer + TILE_OBS_BARRIER_PASS * 8);
    gen_obstacle_sign_pass(s_tile_buffer + TILE_OBS_SIGN_PASS * 8);
    gen_star_sparkle(s_tile_buffer + TILE_STAR_SPARKLE * 8);
    gen_popup_150(s_tile_buffer + TILE_POPUP_150 * 8);
    gen_popup_100(s_tile_buffer + TILE_POPUP_100 * 8);
    gen_popup_250(s_tile_buffer + TILE_POPUP_250 * 8);
    gen_hud_coin(s_tile_buffer + TILE_HUD_COIN * 8);
    gen_hud_wheel(s_tile_buffer + TILE_HUD_WHEEL * 8);

    // Garage Swatches & Snowflakes
    gen_color_swatch(s_tile_buffer + TILE_COLOR_SWATCH * 8);
    gen_color_selector(s_tile_buffer + TILE_COLOR_SELECTOR * 8);
    gen_snowflake(s_tile_buffer + TILE_SNOWFLAKE * 8);

    // Palm Beach Themed Obstacles (Sprite Palette 10)
    gen_obs_beach_driftwood_lrg(s_tile_buffer + TILE_OBS_BEACH_DRIFTWOOD_LRG * 8);
    gen_obs_beach_driftwood_pass(s_tile_buffer + TILE_OBS_BEACH_DRIFTWOOD_PASS * 8);
    gen_obs_beach_tiki_lrg(s_tile_buffer + TILE_OBS_BEACH_TIKI_LRG * 8);
    gen_obs_beach_tiki_pass(s_tile_buffer + TILE_OBS_BEACH_TIKI_PASS * 8);
    gen_obs_beach_truck_lrg(s_tile_buffer + TILE_OBS_BEACH_TRUCK_LRG * 8);
    gen_obs_beach_civilian_lrg(s_tile_buffer + TILE_OBS_BEACH_CIVILIAN_LRG * 8);

    // Winter Snow Themed Obstacles (Sprite Palette 11)
    gen_obs_wint_ice_lrg(s_tile_buffer + TILE_OBS_WINT_ICE_LRG * 8);
    gen_obs_wint_ice_pass(s_tile_buffer + TILE_OBS_WINT_ICE_PASS * 8);
    gen_obs_wint_cable_lrg(s_tile_buffer + TILE_OBS_WINT_CABLE_LRG * 8);
    gen_obs_wint_cable_pass(s_tile_buffer + TILE_OBS_WINT_CABLE_PASS * 8);
    gen_obs_wint_truck_lrg(s_tile_buffer + TILE_OBS_WINT_TRUCK_LRG * 8);
    gen_obs_wint_civilian_lrg(s_tile_buffer + TILE_OBS_WINT_CIVILIAN_LRG * 8);

    // Cape Orbital Themed Obstacles (Sprite Palette 12)
    gen_obs_orbit_pipe_lrg(s_tile_buffer + TILE_OBS_ORBIT_PIPE_LRG * 8);
    gen_obs_orbit_pipe_pass(s_tile_buffer + TILE_OBS_ORBIT_PIPE_PASS * 8);
    gen_obs_orbit_gantry_lrg(s_tile_buffer + TILE_OBS_ORBIT_GANTRY_LRG * 8);
    gen_obs_orbit_gantry_pass(s_tile_buffer + TILE_OBS_ORBIT_GANTRY_PASS * 8);
    gen_obs_orbit_crawler_lrg(s_tile_buffer + TILE_OBS_ORBIT_CRAWLER_LRG * 8);
    gen_obs_orbit_rover_lrg(s_tile_buffer + TILE_OBS_ORBIT_ROVER_LRG * 8);

    // Gothic Midnight Themed Obstacles (Sprite Palette 13)
    gen_obs_goth_crypt_lrg(s_tile_buffer + TILE_OBS_GOTH_CRYPT_LRG * 8);
    gen_obs_goth_crypt_pass(s_tile_buffer + TILE_OBS_GOTH_CRYPT_PASS * 8);
    gen_obs_goth_gate_lrg(s_tile_buffer + TILE_OBS_GOTH_GATE_LRG * 8);
    gen_obs_goth_gate_pass(s_tile_buffer + TILE_OBS_GOTH_GATE_PASS * 8);
    gen_obs_goth_hearse_lrg(s_tile_buffer + TILE_OBS_GOTH_HEARSE_LRG * 8);
    gen_obs_goth_jalopy_lrg(s_tile_buffer + TILE_OBS_GOTH_JALOPY_LRG * 8);

    // Maya Temple Trail Themed Obstacles (Sprite Palette 14)
    gen_obs_maya_totem_lrg(s_tile_buffer + TILE_OBS_MAYA_TOTEM_LRG * 8);
    gen_obs_maya_totem_pass(s_tile_buffer + TILE_OBS_MAYA_TOTEM_PASS * 8);
    gen_obs_maya_arch_lrg(s_tile_buffer + TILE_OBS_MAYA_ARCH_LRG * 8);
    gen_obs_maya_arch_pass(s_tile_buffer + TILE_OBS_MAYA_ARCH_PASS * 8);
    gen_obs_maya_truck_lrg(s_tile_buffer + TILE_OBS_MAYA_TRUCK_LRG * 8);
    gen_obs_maya_jeep_lrg(s_tile_buffer + TILE_OBS_MAYA_JEEP_LRG * 8);

    // Collectibles & Powerups
    gen_coins(s_tile_buffer + TILE_COIN_F0 * 8);
    gen_powerups(s_tile_buffer + TILE_POW_NITRO * 8);

    // Mechanic-Expanding Stunt Ramps (32x16, 8 tiles each)
    gen_obs_ramp_city(s_tile_buffer + TILE_OBS_RAMP_CITY * 8);
    gen_obs_ramp_beach(s_tile_buffer + TILE_OBS_RAMP_BEACH * 8);
    gen_obs_ramp_winter(s_tile_buffer + TILE_OBS_RAMP_WINTER * 8);
    gen_obs_ramp_orbital(s_tile_buffer + TILE_OBS_RAMP_ORBITAL * 8);
    gen_obs_ramp_gothic(s_tile_buffer + TILE_OBS_RAMP_GOTHIC * 8);
    gen_obs_ramp_maya(s_tile_buffer + TILE_OBS_RAMP_MAYA * 8);

    // Mechanic-Expanding Surface Slicks (32x16, 8 tiles each)
    gen_obs_slick_city(s_tile_buffer + TILE_OBS_SLICK_CITY * 8);
    gen_obs_slick_beach(s_tile_buffer + TILE_OBS_SLICK_BEACH * 8);
    gen_obs_slick_winter(s_tile_buffer + TILE_OBS_SLICK_WINTER * 8);
    gen_obs_slick_orbital(s_tile_buffer + TILE_OBS_SLICK_ORBITAL * 8);
    gen_obs_slick_gothic(s_tile_buffer + TILE_OBS_SLICK_GOTHIC * 8);
    gen_obs_slick_maya(s_tile_buffer + TILE_OBS_SLICK_MAYA * 8);

    // Track Signature Hazards (32x32, 16 tiles each)
    gen_obs_sig_city(s_tile_buffer + TILE_OBS_SIG_CITY * 8);
    gen_obs_sig_beach(s_tile_buffer + TILE_OBS_SIG_BEACH * 8);
    gen_obs_sig_winter(s_tile_buffer + TILE_OBS_SIG_WINTER * 8);
    gen_obs_sig_orbital_on(s_tile_buffer + TILE_OBS_SIG_ORBITAL_ON * 8);
    gen_obs_sig_orbital_off(s_tile_buffer + TILE_OBS_SIG_ORBITAL_OFF * 8);
    gen_obs_sig_gothic(s_tile_buffer + TILE_OBS_SIG_GOTHIC * 8);
    gen_obs_sig_maya(s_tile_buffer + TILE_OBS_SIG_MAYA * 8);

    // Stunt Super-Jump Popup
    gen_popup_stunt(s_tile_buffer + TILE_POPUP_STUNT * 8);

    // 4. DMA finished tiles to OBJ VRAM (tile_mem[4])
    dma3_cpy(tile_mem[4], s_tile_buffer, sizeof(s_tile_buffer));
}

