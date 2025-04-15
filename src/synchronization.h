#include <allegro5/allegro.h>
#include <stdatomic.h>
#include <stdbool.h>

extern ALLEGRO_MUTEX *g_mutex;
extern ALLEGRO_COND *g_cond;

int send_event_and_wait(int event);
int check_event();
void acknowledge_event(int status);
