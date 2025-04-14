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

    // signal the new thread and wait for it to signal us
    signal_new_thread_and_wait();

    // do main game loop
    game_loop();

cleanup:
    if (thread != NULL)
        // al_destroy_thread also calls al_join_thread
        al_destroy_thread(thread);
    if (g_mutex != NULL)
        al_destroy_mutex(g_mutex);
    if (g_cond != NULL)
        al_destroy_cond(g_cond);
    return 0;
}
