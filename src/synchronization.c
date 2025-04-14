/*
 * Some synchronization primitives
 */

#include "synchronization.h"

ALLEGRO_MUTEX *g_mutex = NULL;
ALLEGRO_COND *g_cond = NULL;

static atomic_bool start = false;
static bool finish = false;
static bool _status;

/**
 * @brief Signal new thread to start setup and wait until it finishes setup
 *
 * @return setup status
 */
bool signal_new_thread_and_wait() {
    bool status;
    al_lock_mutex(g_mutex);
    atomic_store_explicit(&start, true, memory_order_relaxed);
    while (!finish)
        al_wait_cond(g_cond, g_mutex);
    status = _status;
    al_unlock_mutex(g_mutex);
    return status;
}

/**
 * @brief Wait for main thread (should be called before setup)
 */
void wait_for_main_thread() {
    while (!atomic_load_explicit(&start, memory_order_relaxed))
        al_rest(0.01);
}

/**
 * @brief Signal main thread (should be called after setup)
 *
 * @param status setup status
 */
void signal_main_thread(bool status) {
    al_lock_mutex(g_mutex);
    finish = true;
    _status = status;
    al_signal_cond(g_cond);
    al_unlock_mutex(g_mutex);
}
