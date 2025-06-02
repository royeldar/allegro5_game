#include <allegro5/allegro.h>

#define HALT_DRAWING    1
#define RESUME_DRAWING  2

#define STATUS_SUCCESS  0
#define STATUS_FAILURE  1

extern ALLEGRO_DISPLAY *g_display;

void *render_main(ALLEGRO_THREAD *thread, void *arg);
