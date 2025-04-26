/*
 * Some synchronization primitives
 */

#include "synchronization.h"

ALLEGRO_MUTEX *g_mutex = NULL;
ALLEGRO_COND *g_cond = NULL;

static atomic_int _event = 0;
static bool finish = false;
static int _status;

/**
 * @brief Send an event
 *
 * @param event any non-zero value
 */
void send_event(int event) {
    atomic_store_explicit(&_event, event, memory_order_relaxed);
}

/**
 * @brief Wait for an acknowledgement
 *
 * @return status (see below)
 */
int wait_for_acknowledgement() {
    int status;
    al_lock_mutex(g_mutex);
    while (!finish)
        al_wait_cond(g_cond, g_mutex);
    finish = false;
    status = _status;
    al_unlock_mutex(g_mutex);
    return status;
}

/**
 * @brief Receive an event (if there is any)
 *
 * @return event (a non-zero value means an event was sent)
 */
int receive_event() {
    int event = atomic_load_explicit(&_event, memory_order_relaxed);
    if (event)
        _event = 0;
    return event;
}

/**
 * @brief Acknowledge an event
 *
 * @param status some value
 */
void acknowledge_event(int status) {
    al_lock_mutex(g_mutex);
    finish = true;
    _status = status;
    al_signal_cond(g_cond);
    al_unlock_mutex(g_mutex);
}
