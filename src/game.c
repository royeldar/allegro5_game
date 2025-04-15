/*
 * Main game loop
 */

#include <allegro5/allegro.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "render.h"
#include "shared_state.h"
#include "synchronization.h"

static const struct shared_state initial_shared_state = {
    // TODO
};

ALLEGRO_EVENT_QUEUE *g_event_queue = NULL;

bool game_setup() {
    initialize_shared_state(&initial_shared_state);

    // create an empty event queue
    g_event_queue = al_create_event_queue();
    if (g_event_queue == NULL) {
        printf("al_create_event_queue() failed\n");
        return false;
    }

    return true;
}

void update_frame(struct shared_state *shared_state) {
    // TODO
}

void game_loop() {
    bool exit = false;
    while (!exit) {
        bool update = false;
        ALLEGRO_EVENT event;
        al_wait_for_event(g_event_queue, &event);
        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            exit = true;
            break;
        case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
            send_event_and_wait(HALT_DRAWING);
            al_acknowledge_drawing_halt(g_display);
            break;
        case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
            al_acknowledge_drawing_resume(g_display);
            send_event_and_wait(RESUME_DRAWING);
            break;
        }
        if (update) {
            struct shared_state *shared_state = get_shared_state_for_writing();
            update_frame(shared_state);
            write_shared_state();
        }
    }
}

void game_cleanup() {
    if (g_event_queue != NULL)
        al_destroy_event_queue(g_event_queue);
}
