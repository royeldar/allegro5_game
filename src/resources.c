/*
 * Resource loading routines
 */

#include <allegro5/allegro.h>
#include <stdbool.h>
#include <string.h>

#include "flatpak.h"
#include "hashmap.h"
#include "resources.h"

struct load_resource_callback_param {
    ALLEGRO_FS_ENTRY *dir;
    struct hashmap *map;
    void *(*load_resource)(ALLEGRO_FILE *file, const char *ext);
    void (*destroy_resource)(void *resource);
};

static const char *remove_prefix(const char *str, const char *initial) {
    size_t len = strlen(initial);
    if (strncmp(str, initial, len))
        return NULL;
    return str + len;
}

static int load_resource_callback(ALLEGRO_FS_ENTRY *entry, void *extra) {
    const struct load_resource_callback_param *param = extra;
    ALLEGRO_FS_ENTRY *dir = param->dir;
    struct hashmap *map = param->map;
    void *(*load_resource)(ALLEGRO_FILE *file, const char *ext) = param->load_resource;
    void (*destroy_resource)(void *resource) = param->destroy_resource;
    int ret = ALLEGRO_FOR_EACH_FS_ENTRY_ERROR;
    ALLEGRO_PATH *path = NULL;
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
        const char *dirname = al_get_fs_entry_name(dir);
        const char *filename = al_get_fs_entry_name(entry);
        const char *name;
        const char *ext;
        ALLEGRO_FILE *file;
        void *resource;
        name = remove_prefix(filename, dirname);
        if (name == NULL)
            goto cleanup;
        path = al_create_path(name);
        if (path == NULL)
            goto cleanup;
        if (!al_make_path_canonical(path))
            goto cleanup;
        if (al_get_path_num_components(path) > 0 && al_get_path_component(path, 0)[0] == '\0')
            al_remove_path_component(path, 0);
        name = al_path_cstr(path, '/');
        ext = al_get_path_extension(path);
        file = al_fopen(filename, "rb");
        if (file == NULL)
            goto cleanup;
        resource = load_resource(file, ext);
        al_fclose(file);
        if (resource == NULL)
            goto cleanup;
        if (!hashmap_add(map, name, resource)) {
            destroy_resource(resource);
            goto cleanup;
        }
        al_destroy_path(path);
        path = NULL;
    }

    ret = ALLEGRO_FOR_EACH_FS_ENTRY_OK;

cleanup:
    if (path != NULL)
        al_destroy_path(path);
    return ret;
}

/**
 * @brief Load resources from a given directory into a hashmap
 *
 * @param subpath relative path to the directory
 * @param load_resource callback which loads a resource from a given file
 * @param destroy_resource callback which destroys a resource
 * @return hashmap containing all resources (or NULL on failure)
 */
struct hashmap *load_resources(const char *subpath, void *(*load_resource)(ALLEGRO_FILE *file, const char *ext), void (*destroy_resource)(void *resource)) {
    ALLEGRO_PATH *path = NULL;
    ALLEGRO_PATH *path2 = NULL;
    ALLEGRO_FS_ENTRY *dir = NULL;
    struct hashmap *map = NULL;
    struct hashmap *ret = NULL;
    int res;
    struct load_resource_callback_param param;

    if (g_flatpak_id != NULL) {
        path = al_create_path("/app/share/");
        if (path == NULL)
            goto cleanup;
        al_append_path_component(path, g_flatpak_id);
    } else {
        path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        if (path == NULL)
            goto cleanup;
    }
    path2 = al_create_path_for_directory(subpath);
    if (path2 == NULL)
        goto cleanup;
    if (!al_join_paths(path, path2))
        goto cleanup;

    dir = al_create_fs_entry(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    if (dir == NULL)
        goto cleanup;

    map = hashmap_create(0);
    if (map == NULL)
        goto cleanup;

    param.dir = dir;
    param.map = map;
    param.load_resource = load_resource;
    param.destroy_resource = destroy_resource;
    res = al_for_each_fs_entry(dir, load_resource_callback, &param);
    if (res != ALLEGRO_FOR_EACH_FS_ENTRY_OK)
        goto cleanup;

    ret = map;
    map = NULL;

cleanup:
    if (map != NULL)
        destroy_resources(map, destroy_resource);
    if (dir != NULL)
        al_destroy_fs_entry(dir);
    if (path2 != NULL)
        al_destroy_path(path2);
    if (path != NULL)
        al_destroy_path(path);
    return ret;
}

static void destroy_resource_callback(struct hashmap_entry *entry, void *extra) {
    void (*destroy_resource)(void *resource) = extra;
    void **ptr = hashmap_entry_get_value_ptr(entry);
    destroy_resource(*ptr);
}

/**
 * @brief Destroy a hashmap (including its resources)
 *
 * @param map hashmap containing resources
 * @param destroy_resource callback which destroys a resource
 */
void destroy_resources(struct hashmap *map, void (*destroy_resource)(void *resource)) {
    hashmap_destroy(map, destroy_resource_callback, destroy_resource);
}

/**
 * @brief Get the resource corresponding to the given filename
 *
 * @param map hashmap containing resources
 * @param path filename (relative to the directory the resources were loaded from)
 * @return resource corresponding to the given filename (NULL if none)
 */
void *get_resource(struct hashmap *map, const char *path) {
    struct hashmap_entry *entry = hashmap_get(map, path);
    return entry ? *hashmap_entry_get_value_ptr(entry) : NULL;
}
