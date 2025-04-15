/*
 * Some synchronization primitives
 */

#include "synchronization.h"

ALLEGRO_MUTEX *g_mutex = NULL;
ALLEGRO_COND *g_cond = NULL;

static atomic_int _event = 0;
static bool finish;
static int _status;

/**
 * @brief Send an event to a thread and wait for acknowledgement
 *
 * @param event any non-zero value
 * @return status (see below)
 */
int send_event_and_wait(int event) {
    int status;
    al_lock_mutex(g_mutex);
    finish = false;
    atomic_store_explicit(&_event, event, memory_order_relaxed);
    while (!finish)
        al_wait_cond(g_cond, g_mutex);
    status = _status;
    al_unlock_mutex(g_mutex);
    return status;
}

/**
 * @brief Check whether an event was sent
 *
 * @return event (a non-zero value means an event was sent)
 */
int check_event() {
    return atomic_load_explicit(&_event, memory_order_relaxed);
}

/**
 * @brief Acknowledge an event
 *
 * @param status some value
 */
void acknowledge_event(int status) {
    _event = 0;
    al_lock_mutex(g_mutex);
    finish = true;
    _status = status;
    al_signal_cond(g_cond);
    al_unlock_mutex(g_mutex);
}
