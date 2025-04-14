#include <allegro5/allegro.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // initialize allegro system
    if (!al_init()) {
        printf("al_init() failed\n");
        goto cleanup;
    }

cleanup:
    return 0;
}
