/*
 * Rendering thread
 */

#include <allegro5/allegro.h>
#include <stdbool.h>

#include "render.h"
#include "synchronization.h"

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

    // wait for main thread to signal us
    wait_for_main_thread();

    // setup stuff
    ret = render_setup();

    // signal the main thread
    signal_main_thread(ret);

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
