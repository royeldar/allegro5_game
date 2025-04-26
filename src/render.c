/*
 * Rendering thread
 */

#include <allegro5/allegro.h>
#include <stdbool.h>
#include <stdio.h>

#include "render.h"
#include "shared_state.h"
#include "synchronization.h"

#define WIDTH   640
#define HEIGHT  480

ALLEGRO_DISPLAY *g_display = NULL;

static int render_setup() {
    // create a 640x480 display
    g_display = al_create_display(WIDTH, HEIGHT);
    if (g_display == NULL) {
        printf("al_create_display() failed\n");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}

void render_frame(const struct shared_state *shared_state) {
    // TODO
}

static void render_loop(ALLEGRO_THREAD *thread) {
    bool dont_draw = false;
    while (!al_get_thread_should_stop(thread)) {
        int event = receive_event();
        if (event == 0 && !dont_draw) {
            const struct shared_state *shared_state;
            read_shared_state();
            shared_state = get_shared_state_for_reading();
            render_frame(shared_state);
        } else {
            switch (event) {
            case HALT_DRAWING:
                dont_draw = true;
                break;
            case RESUME_DRAWING:
                dont_draw = false;
                break;
            }
            acknowledge_event(STATUS_SUCCESS);
        }
    }
}

static void render_cleanup() {
    // TODO
}

void *render_main(ALLEGRO_THREAD *thread, void *arg) {
    int ret;

    // setup stuff
    ret = render_setup();

    // tell the main thread that setup has finished
    acknowledge_event(ret);

    // exit on failure
    if (ret != STATUS_SUCCESS)
        goto cleanup;

    // render loop
    render_loop(thread);

cleanup:
    // cleanup stuff
    render_cleanup();
    return NULL;
}
