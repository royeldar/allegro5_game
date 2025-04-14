/*
 * Some synchronization primitives
 */

#include "synchronization.h"

ALLEGRO_MUTEX *g_mutex = NULL;
ALLEGRO_COND *g_cond = NULL;

static atomic_bool start = false;
static bool finish = false;

/**
 * @brief Signal new thread to start setup and wait until it finishes setup
 */
void signal_new_thread_and_wait() {
    al_lock_mutex(g_mutex);
    atomic_store_explicit(&start, true, memory_order_relaxed);
    while (!finish)
        al_wait_cond(g_cond, g_mutex);
    al_unlock_mutex(g_mutex);
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
 */
void signal_main_thread() {
    al_lock_mutex(g_mutex);
    finish = true;
    al_signal_cond(g_cond);
    al_unlock_mutex(g_mutex);
}
