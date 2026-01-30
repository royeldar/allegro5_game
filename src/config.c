/*
 * Configuration-related routines
 */

#include <allegro5/allegro.h>
#include <assert.h>
#include <stdbool.h>

#include "config.h"

ALLEGRO_CONFIG *g_config = NULL;

/**
 * @brief Load a configuration from a file, or create an empty one if the file doesn't exist
 *
 * @param subpath relative path to the file
 * @return true if the configuration was loaded (or created) successfully
 * @return false otherwise
 */
bool load_config(const char *subpath) {
    ALLEGRO_PATH *path = NULL;
    ALLEGRO_PATH *path2 = NULL;
    ALLEGRO_FILE *file = NULL;
    assert(g_config == NULL);

    path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
    if (path == NULL)
        goto cleanup;
    path2 = al_create_path(subpath);
    if (path2 == NULL)
        goto cleanup;
    if (!al_join_paths(path, path2))
        goto cleanup;

    if (al_filename_exists(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
        file = al_fopen(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), "r");
        if (file == NULL)
            goto cleanup;
        g_config = al_load_config_file_f(file);
    } else {
        g_config = al_create_config();
    }

cleanup:
    if (file != NULL)
        al_fclose(file);
    if (path2 != NULL)
        al_destroy_path(path2);
    if (path != NULL)
        al_destroy_path(path);
    return (g_config != NULL);
}

/**
 * @brief Save the configuration to a file (which may already exist)
 *
 * @param subpath relative path to the file
 * @return true if the configuration was saved successfully
 * @return false otherwise
 */
bool save_config(const char *subpath) {
    ALLEGRO_PATH *path = NULL;
    ALLEGRO_PATH *path2 = NULL;
    ALLEGRO_PATH *dir_path = NULL;
    bool ret = false;
    assert(g_config != NULL);

    path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
    if (path == NULL)
        goto cleanup;
    path2 = al_create_path(subpath);
    if (path2 == NULL)
        goto cleanup;
    if (!al_join_paths(path, path2))
        goto cleanup;

    if (!al_filename_exists(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
        dir_path = al_clone_path(path);
        if (dir_path == NULL)
            goto cleanup;
        al_set_path_filename(dir_path, NULL);
        if (!al_make_directory(al_path_cstr(dir_path, ALLEGRO_NATIVE_PATH_SEP)))
            goto cleanup;
    }

    ret = al_save_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), g_config);

cleanup:
    if (dir_path != NULL)
        al_destroy_path(dir_path);
    if (path2 != NULL)
        al_destroy_path(path2);
    if (path != NULL)
        al_destroy_path(path);
    return ret;
}

/**
 * @brief Destroy the configuration
 */
void destroy_config() {
    if (g_config != NULL) {
        al_destroy_config(g_config);
        g_config = NULL;
    }
}
