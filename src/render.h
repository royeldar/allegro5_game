#include <allegro5/allegro.h>

#define SETUP_RENDER    1
#define HALT_DRAWING    2
#define RESUME_DRAWING  3

#define STATUS_SUCCESS  0
#define STATUS_FAILURE  1

extern ALLEGRO_DISPLAY *g_display;

void *render_main(ALLEGRO_THREAD *thread, void *arg);
