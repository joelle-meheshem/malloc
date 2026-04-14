#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
#define BLOCK_MARKER 0xDD
#define MAGIC 0x55

typedef struct area {
    uint8_t marker;
    struct area *prev;
    struct area *next;
    bool in_use;
    uint32_t length;
} area;

typedef struct stats {
    uint8_t magic;
    uint32_t blocks;
    uint32_t pages;
} stats;

/* API */
void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);

/* Debug */
void print_heap();

/* Helpers */
stats *get_header();
area *get_first_block();

#endif