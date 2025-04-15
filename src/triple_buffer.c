/*
 * Triple buffering implementation which is thread-safe in single-producer/single-consumer scenarios
 */

#include <stdatomic.h>
#include <stdbool.h>

#include "triple_buffer.h"

/**
 * @brief Get buffer address
 *
 * @param triple_buffer shared triple buffer
 * @param index buffer index
 * @return void* buffer address
 */
void *get_buffer(const struct triple_buffer *triple_buffer, int index) {
    return triple_buffer->buf[index];
}

/**
 * @brief Swap the write buffer with the temp buffer (may overwrite it)
 *
 * @param triple_buffer shared triple buffer
 * @param index write buffer index
 * @return true if the temp buffer was swapped with the read buffer
 * @return false otherwise (the temp buffer is overwritten)
 */
bool swap_write_buffer(struct triple_buffer *triple_buffer, int *index) {
    int new_state = *index | 4;
    int old_state = atomic_exchange_explicit(&triple_buffer->state, new_state, memory_order_acq_rel);
    *index = old_state & 3;
    return !(old_state & 4);
}

/**
 * @brief Swap the read buffer with the temp buffer (only if available)
 *
 * @param triple_buffer shared triple buffer
 * @param index read buffer index
 * @return true if the temp buffer was swapped with the write buffer
 * @return false otherwise (the temp buffer not ready yet)
 */
bool swap_read_buffer(struct triple_buffer *triple_buffer, int *index) {
    int ready = atomic_load_explicit(&triple_buffer->state, memory_order_relaxed) & 4;
    if (ready) {
        int new_state = *index;
        int old_state = atomic_exchange_explicit(&triple_buffer->state, new_state, memory_order_acq_rel);
        *index = old_state & 3;
    }
    return ready;
}
