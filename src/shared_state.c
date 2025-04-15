/*
 * Shared state produced by the main thread and consumed by the rendering thread (using triple buffering)
 */

#include <string.h>

#include "shared_state.h"
#include "triple_buffer.h"

static struct shared_state buffers[3];

static struct triple_buffer triple_buffer = {
    .buf = {
        &buffers[0],
        &buffers[1],
        &buffers[2],
    },
    .state = 2,
};

static int write_index = 0;
static int read_index = 1;

/**
 * @brief Initialize the shared state
 *
 * @param shared_state initial shared state
 */
void initialize_shared_state(const struct shared_state *shared_state) {
    memcpy(get_buffer(&triple_buffer, write_index), shared_state, sizeof(*shared_state));
    memcpy(get_buffer(&triple_buffer, read_index), shared_state, sizeof(*shared_state));
}

/**
 * @brief Get the write buffer of the shared state
 *
 * @return the write buffer of the shared state
 */
struct shared_state *get_shared_state_for_writing() {
    return get_buffer(&triple_buffer, write_index);
}

/**
 * @brief Get the read buffer of the shared state
 *
 * @return the read buffer of the shared state
 */
const struct shared_state *get_shared_state_for_reading() {
    return get_buffer(&triple_buffer, read_index);
}

/**
 * @brief Swap the write buffer of the shared state (should be called after writing)
 */
void write_shared_state() {
    swap_write_buffer(&triple_buffer, &write_index);
}

/**
 * @brief Swap the read buffer of the shared state (should be called before reading)
 */
void read_shared_state() {
    swap_read_buffer(&triple_buffer, &read_index);
}
