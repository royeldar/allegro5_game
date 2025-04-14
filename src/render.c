/*
 * Rendering thread
 */

#include <allegro5/allegro.h>

#include "render.h"

void *render_main(ALLEGRO_THREAD *thread, void *arg) {
    // render loop
    while (!al_get_thread_should_stop(thread)) {
        // TODO
    }

    return NULL;
}
