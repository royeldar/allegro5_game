/*
 * Rendering thread
 */

#include <allegro5/allegro.h>
#include <stdbool.h>
#include <stdio.h>

#include "render.h"
#include "synchronization.h"

#define WIDTH   640
#define HEIGHT  480

ALLEGRO_DISPLAY *g_display = NULL;

static int render_setup() {
    // create a 640x480 display
    g_display = al_create_display(WIDTH, HEIGHT);
    if (g_display == NULL) {
        printf("al_create_display() failed\n");
        return 0;
    }
    return 1;
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
    int ret;

    // wait for the main thread to signal us
    while (!check_event())
        al_rest(0.01);

    // setup stuff
    ret = render_setup();

    // signal the main thread
    acknowledge_event(ret);

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
