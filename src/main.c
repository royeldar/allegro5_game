#include <allegro5/allegro.h>
#include <stdio.h>

#include "game.h"
#include "render.h"
#include "synchronization.h"

int main(int argc, char **argv) {
    ALLEGRO_THREAD *thread = NULL;

    // initialize allegro system
    if (!al_init()) {
        printf("al_init() failed\n");
        goto cleanup;
    }

    // install keyboard driver
    if (!al_install_keyboard()) {
        printf("al_install_keyboard() failed\n");
        goto cleanup;
    }

    // setup stuff
    if (!game_setup()) {
        printf("game_setup() failed\n");
        goto cleanup;
    }

    // create a mutex object
    g_mutex = al_create_mutex();
    if (g_mutex == NULL) {
        printf("al_create_mutex() failed\n");
        goto cleanup;
    }

    // create a condition variable
    g_cond = al_create_cond();
    if (g_cond == NULL) {
        printf("al_create_cond() failed\n");
        goto cleanup;
    }

    // spawn a new thread
    thread = al_create_thread(render_main, NULL);
    if (thread == NULL) {
        printf("al_create_thread() failed\n");
        goto cleanup;
    }

    // start the new thread
    al_start_thread(thread);

    // wait for setup to finish
    if (wait_for_acknowledgement() != STATUS_SUCCESS) {
        printf("render_setup() failed\n");
        goto cleanup;
    }

    // register display event source
    al_register_event_source(g_event_queue, al_get_display_event_source(g_display));

    // register timer event source
    al_register_event_source(g_event_queue, al_get_timer_event_source(g_timer));

    // register keyboard event source
    al_register_event_source(g_event_queue, al_get_keyboard_event_source());

    // start timer
    al_start_timer(g_timer);

    // do main game loop
    game_loop();

    // stop timer
    al_stop_timer(g_timer);

    // unregister display event source
    al_unregister_event_source(g_event_queue, al_get_display_event_source(g_display));

    // unregister timer event source
    al_unregister_event_source(g_event_queue, al_get_timer_event_source(g_timer));

    // unregister keyboard event source
    al_unregister_event_source(g_event_queue, al_get_keyboard_event_source());

    // flush event queue
    al_flush_event_queue(g_event_queue);

cleanup:
    if (thread != NULL)
        // al_destroy_thread also calls al_join_thread
        al_destroy_thread(thread);
    if (g_display != NULL)
        // al_create_display is called in the rendering thread
        al_destroy_display(g_display);
    if (g_mutex != NULL)
        al_destroy_mutex(g_mutex);
    if (g_cond != NULL)
        al_destroy_cond(g_cond);
    game_cleanup();
    return 0;
}
