#include <allegro5/allegro.h>
#include <stdatomic.h>
#include <stdbool.h>

extern ALLEGRO_MUTEX *g_mutex;
extern ALLEGRO_COND *g_cond;

bool signal_new_thread_and_wait();
void wait_for_main_thread();
void signal_main_thread(bool status);
