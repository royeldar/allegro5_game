/*
 * Hashmap implementation where the keys are null-terminated strings and the values are pointers (inspired by git.git)
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

struct hashmap_entry {
    struct hashmap_entry *next;
    unsigned int hash;
    char *str;
    void *ptr;
};

struct hashmap {
    struct hashmap_entry **table;
    unsigned int size;
    unsigned int capacity;
    unsigned int grow_at;
    unsigned int shrink_at;
};

struct hashmap_iter {
    const struct hashmap *map;
    struct hashmap_entry *next;
    unsigned int pos;
};

#define FNV32_BASE 0x811c9dc5
#define FNV32_PRIME 0x01000193

static unsigned int strhash(const char *str) {
    unsigned int c, hash = FNV32_BASE;
    while ((c = (unsigned char) *str++))
        hash = (hash * FNV32_PRIME) ^ c;
    return hash;
}

// default initial size
#define HASHMAP_INITIAL_SIZE    64
// grow / shrink by 2^2
#define HASHMAP_RESIZE_BITS     2
// load factor in percent
#define HASHMAP_LOAD_FACTOR     80

static bool alloc_table(struct hashmap *map, unsigned int size) {
    struct hashmap_entry **table = calloc(size, sizeof(struct hashmap_entry *));
    if (table == NULL)
        return false;
    map->table = table;
    map->capacity = size;
    map->grow_at = (unsigned int)((unsigned long)size * HASHMAP_LOAD_FACTOR / 100);
    if (size <= HASHMAP_INITIAL_SIZE)
        map->shrink_at = 0;
    else
        map->shrink_at = map->grow_at / ((1 << HASHMAP_RESIZE_BITS) + 1);
    return true;
}

static bool rehash(struct hashmap *map, unsigned int newsize) {
    unsigned int oldsize = map->capacity;
    struct hashmap_entry **oldtable = map->table;
    unsigned int i;
    if (!alloc_table(map, newsize))
        return false;
    for (i = 0; i < oldsize; i++) {
        struct hashmap_entry *e = oldtable[i];
        while (e != NULL) {
            struct hashmap_entry *n = e->next;
            e->next = map->table[e->hash % map->capacity];
            map->table[e->hash % map->capacity] = e;
            e = n;
        }
    }
    free(oldtable);
    return true;
}

static struct hashmap_entry **find_entry_ptr(const struct hashmap *map, const char *str) {
    unsigned int hash = strhash(str);
    struct hashmap_entry **e = &map->table[hash % map->capacity];
    for (; *e != NULL; e = &(*e)->next) {
        if ((*e)->hash == hash && !strcmp((*e)->str, str))
            break;
    }
    return e;
}

/**
 * @brief Get the name of a given hashmap entry
 *
 * @param entry hashmap entry
 * @return key
 */
const char *hashmap_entry_get_str(const struct hashmap_entry *entry) {
    return entry->str;
}

/**
 * @brief Get a pointer to the value of a given hashmap entry
 *
 * @param entry hashmap entry
 * @return pointer to the value of the given entry
 */
void **hashmap_entry_get_value_ptr(struct hashmap_entry *entry) {
    return &entry->ptr;
}

/**
 * @brief Create an empty hashmap
 *
 * @param initial_size initial capacity
 * @return newly created hashmap (NULL if allocation has failed)
 */
struct hashmap *hashmap_create(size_t initial_size) {
    struct hashmap *map = NULL;
    unsigned int size = HASHMAP_INITIAL_SIZE;
    map = malloc(sizeof(*map));
    if (map == NULL)
        return NULL;
    map->size = 0;
    initial_size = (unsigned int)((unsigned long)initial_size * 100 / HASHMAP_LOAD_FACTOR);
    while (initial_size > size)
        size <<= HASHMAP_RESIZE_BITS;
    if (!alloc_table(map, size)) {
        free(map);
        return NULL;
    }
    return map;
}

/**
 * @brief Destroy a hashmap (including its entries)
 *
 * @param map pointer to hashmap
 * @param callback optional callback which is called for every hashmap entry
 * @param extra extra parameter that is given to the callback
 */
void hashmap_destroy(struct hashmap *map, void (*callback)(struct hashmap_entry *entry, void *extra), void *extra) {
    unsigned int i;
    for (i = 0; i < map->capacity; i++) {
        struct hashmap_entry *e = map->table[i];
        while (e != NULL) {
            struct hashmap_entry *n = e->next;
            if (callback != NULL)
                callback(e, extra);
            free(e->str);
            free(e);
            e = n;
        }
    }
    free(map->table);
    free(map);
}

/**
 * @brief Get the first hashmap entry corresponding to the given key
 *
 * @param map pointer to hashmap
 * @param str key
 * @return entry corresponding to the given key (NULL if none)
 */
struct hashmap_entry *hashmap_get(const struct hashmap *map, const char *str) {
    return *find_entry_ptr(map, str);
}

/**
 * @brief Get the next hashmap entry having the same key as a given entry
 *
 * @param map pointer to hashmap
 * @param entry hashmap entry
 * @return entry having the same key as the given entry (NULL if none)
 */
struct hashmap_entry *hashmap_get_next(const struct hashmap *map, const struct hashmap_entry *entry) {
    unsigned int hash = entry->hash;
    const char *str = entry->str;
    struct hashmap_entry *e = entry->next;
    for (; e != NULL; e = e->next) {
        if (e->hash == hash && !strcmp(e->str, str))
            break;
    }
    return e;
}

/**
 * @brief Add an entry to the hashmap
 *
 * @param map pointer to hashmap
 * @param str key
 * @param ptr value
 * @return true if entry was added successfully
 * @return false otherwise
 */
bool hashmap_add(struct hashmap *map, const char *str, void *ptr) {
    unsigned int hash = strhash(str);
    struct hashmap_entry *entry = malloc(sizeof(struct hashmap_entry));
    if (entry == NULL)
        return false;
    entry->str = malloc(strlen(str) + 1);
    if (entry->str == NULL) {
        free(entry);
        return false;
    }
    entry->next = map->table[hash % map->capacity];
    entry->hash = hash;
    strcpy(entry->str, str);
    entry->ptr = ptr;
    map->size++;
    map->table[hash % map->capacity] = entry;
    if (map->size > map->grow_at) {
        if (!rehash(map, map->capacity << HASHMAP_RESIZE_BITS)) {
            map->size--;
            map->table[hash % map->capacity] = entry->next;
            free(entry->str);
            free(entry);
            return false;
        }
    }
    return true;
}

/**
 * @brief Remove the first hashmap entry corresponding to the given key
 *
 * @param map pointer to hashmap
 * @param str key
 * @return true if entry was removed successfully
 * @return false otherwise
 */
bool hashmap_remove(struct hashmap *map, const char *str) {
    struct hashmap_entry **e = find_entry_ptr(map, str);
    struct hashmap_entry *o;
    if (*e == NULL)
        return false;
    o = *e;
    *e = o->next;
    map->size--;
    if (map->size < map->shrink_at) {
        if (!rehash(map, map->capacity >> HASHMAP_RESIZE_BITS)) {
            map->size++;
            *e = o;
            return false;
        }
    }
    free(o->str);
    free(o);
    return true;
}

/**
 * @brief Create a hashmap iterator
 *
 * @param map pointer to hashmap
 * @return newly created hashmap iterator (NULL if allocation has failed)
 */
struct hashmap_iter *hashmap_iter_create(const struct hashmap *map) {
    struct hashmap_iter *iter = malloc(sizeof(*iter));
    if (iter == NULL)
        return NULL;
    iter->map = map;
    iter->next = NULL;
    iter->pos = 0;
    return iter;
}

/**
 * @brief Reset a hashmap iterator
 *
 * @param iter pointer to iterator
 */
void hashmap_iter_reset(struct hashmap_iter *iter) {
    iter->next = NULL;
    iter->pos = 0;
}

/**
 * @brief Advance a hashmap iterator
 *
 * @param iter pointer to iterator
 * @return current hashmap entry (NULL if iteration has finished)
 */
struct hashmap_entry *hashmap_iter_next(struct hashmap_iter *iter) {
    struct hashmap_entry *e = iter->next;
    for (;;) {
        if (e != NULL) {
            iter->next = e->next;
            break;
        }
        if (iter->pos >= iter->map->capacity)
            break;
        e = iter->map->table[iter->pos++];
    }
    return e;
}

/**
 * @brief Destroy a hashmap iterator
 *
 * @param iter pointer to iterator
 */
void hashmap_iter_destroy(struct hashmap_iter *iter) {
    free(iter);
}
