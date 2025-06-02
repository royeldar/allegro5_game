/*
 * Keyboard handling routines
 */

#include <allegro5/allegro.h>
#include <assert.h>

#include "keyboard.h"

#define KEY_DOWN        (1 << 0)
#define KEY_PRESSED     (1 << 1)
#define KEY_RELEASED    (1 << 2)

static char keys[ALLEGRO_KEY_MAX];

bool is_key_down(int keycode) {
    assert(keycode >= 0 && keycode < ALLEGRO_KEY_MAX);
    return keys[keycode] & KEY_DOWN;
}

bool is_key_pressed(int keycode) {
    assert(keycode >= 0 && keycode < ALLEGRO_KEY_MAX);
    return keys[keycode] & KEY_PRESSED;
}

bool is_key_released(int keycode) {
    assert(keycode >= 0 && keycode < ALLEGRO_KEY_MAX);
    return keys[keycode] & KEY_RELEASED;
}

void handle_keyboard_event(const ALLEGRO_KEYBOARD_EVENT *event) {
    int keycode = event->keycode;
    assert(keycode >= 0 && keycode < ALLEGRO_KEY_MAX);
    switch (event->type) {
    case ALLEGRO_EVENT_KEY_DOWN:
        keys[keycode] = KEY_DOWN | KEY_PRESSED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        keys[keycode] = KEY_RELEASED;
        break;
    }
}

void do_keyboard_tick() {
    int keycode;
    for (keycode = 0; keycode < ALLEGRO_KEY_MAX; keycode++)
        keys[keycode] &= KEY_DOWN;
}
