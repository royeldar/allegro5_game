/*
 * Rendering thread
 */

#include <allegro5/allegro.h>
#include <stdbool.h>

#include "render.h"

static bool render_setup() {
    // TODO
    return true;
}

static void render_loop(ALLEGRO_THREAD *thread) {
    while (!al_get_thread_should_stop(thread)) {
        // TODO
    }
}

static void render_cleanup() {
    // TODO
}

void *render_main(ALLEGRO_THREAD *thread, void *arg) {
    bool ret;

    // setup stuff
    ret = render_setup();

    // exit on failure
    if (!ret)
        goto cleanup;

    // render loop
    render_loop(thread);

cleanup:
    // cleanup stuff
    render_cleanup();
    return NULL;
}
