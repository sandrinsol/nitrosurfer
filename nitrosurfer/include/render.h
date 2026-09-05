#ifndef RENDER_H
#define RENDER_H

#include "common.h"

void render_init(void);
void render_frame(void);
void safe_oam_copy(void);

// Screen-space transitions
void render_flash_screen(int frames);
void render_shake_screen(int intensity);

// UI Rendering
void render_hud(int *spr_idx_io);
void render_start_screen(void);
void render_title_screen(void);
void render_map_select_screen(void);
void render_map_boxes(int selected_map);
void render_gameover_screen(void);
void render_pause_screen(void);
void render_clear_pause_text(void);
void render_clear_title_boxes(void);

#endif // RENDER_H
