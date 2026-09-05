#include <tonc.h>
#include "common.h"
#include "game.h"
#include "render.h"
#include "audio.h"

int main(void) {
    // 1. Initialize GBA Interrupt Handler (Required for VBlankIntrWait)
    irq_init(NULL);
    irq_enable(II_VBLANK);

    // 2. Initialize Subsystems
    audio_init();
    render_init();
    game_init();

    // 3. Main 60 FPS Game Loop
    while (1) {
        VBlankIntrWait();
        safe_oam_copy(); // DMA copy to hardware OAM immediately at start of VBlank
        key_poll();
        game_update();
        audio_update();
        render_frame();
    }

    return 0;
}
