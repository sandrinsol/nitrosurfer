#ifndef AUDIO_H
#define AUDIO_H

#include <tonc.h>

void audio_init(void);
void audio_update(void);

void bgm_init(void);
void bgm_update(void);
void bgm_set_overdrive(bool active);
void sfx_scrape(void);
void sfx_streak_levelup(int level);

void sfx_engine_hum(int speed_level);
void sfx_jump(void);
void sfx_shrink(void);
void sfx_coin(void);
void sfx_powerup(void);
void sfx_hurdle(void);
void sfx_crash(void);
void sfx_nitro(void);
void sfx_high_score(void);
void sfx_touchdown(void);
void sfx_pause(void);
void sfx_close_call(void);
void sfx_stunt_ramp(void);
void sfx_skid(void);
void sfx_shield_activate(void);
void sfx_shield_deflect(void);
void audio_pause(void);
void audio_resume(void);

#endif // AUDIO_H
