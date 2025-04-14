#include <allegro5/allegro.h>
#include <stdio.h>

#include "game.h"
#include "render.h"

int main(int argc, char **argv) {
    ALLEGRO_THREAD *thread = NULL;

    // initialize allegro system
    if (!al_init()) {
        printf("al_init() failed\n");
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

    // do main game loop
    game_loop();

cleanup:
    if (thread != NULL)
        al_destroy_thread(thread);
    return 0;
}
