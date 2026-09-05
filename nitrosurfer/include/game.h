#ifndef GAME_H
#define GAME_H

#include "common.h"

void game_init(void);
void game_start(void);
void game_update(void);
void game_trigger_shake(int amp, int frames);

void player_init(void);
void player_update(void);
void player_jump(void);
void player_shrink(void);
void player_shift_lane(int dir);

void obstacles_init(void);
void obstacles_update(void);
void spawn_obstacle(void);

void coins_init(void);
void coins_update(void);
void spawn_coin_group(int lane, int pattern);

void powerups_init(void);
void powerups_update(void);
void spawn_powerup(void);

void check_collisions(void);

// Perspective projection: takes a lane (0,1,2) and distance Z (8.8 FP),
// returns screen X, screen Y, and scale factor (0..3).
void project_to_screen(int lane, int z_fp, int y_elevation, int *out_x, int *out_y, int *out_scale);

#endif // GAME_H
