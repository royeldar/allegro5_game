/*
 * Main game loop
 */

#include <stdbool.h>

#include "game.h"
#include "shared_state.h"

static const struct shared_state initial_shared_state = {
    // TODO
};

bool game_setup() {
    initialize_shared_state(&initial_shared_state);
    return true;
}

void update_frame(struct shared_state *shared_state) {
    // TODO
}

void game_loop() {
    while (true) {
        struct shared_state *shared_state = get_shared_state_for_writing();
        update_frame(shared_state);
        write_shared_state();
    }
}

void game_cleanup() {
    // TODO
}
