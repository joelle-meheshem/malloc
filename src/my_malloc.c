#include "my_malloc.h"

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

void *heap_start = NULL;
pthread_mutex_t malloc_lock = PTHREAD_MUTEX_INITIALIZER;

/* ================= Helpers ================= */

stats *get_header() {
    return (stats *)heap_start;
}

area *get_first_block() {
    return (area *)((char *)heap_start + sizeof(stats));
}

area *find_last_block() {
    area *cur = get_first_block();
    while (cur->next != NULL) {
        cur = cur->next;
    }
    return cur;
}

/* ================= malloc ================= */

void *my_malloc(size_t size) {
    pthread_mutex_lock(&malloc_lock);

    if (heap_start == NULL) {
        heap_start = sbrk(0);
        sbrk(PAGE_SIZE);

        stats *hdr = (stats *)heap_start;
        hdr->magic = MAGIC;
        hdr->blocks = 1;
        hdr->pages = 1;

        area *first = get_first_block();
        first->marker = BLOCK_MARKER;
        first->prev = NULL;
        first->next = NULL;
        first->in_use = false;
        first->length = PAGE_SIZE - sizeof(stats) - sizeof(area);
    }

    stats *hdr = get_header();
    area *cur = get_first_block();
    area *best = NULL;

    while (cur != NULL) {
        assert(cur->marker == BLOCK_MARKER);

        if (!cur->in_use && cur->length >= size) {
            if (best == NULL || cur->length < best->length) {
                best = cur;
            }
        }
        cur = cur->next;
    }

    if (best == NULL) {
        area *last = find_last_block();

        sbrk(PAGE_SIZE);
        hdr->pages++;

        last->length += PAGE_SIZE;
        best = last;
    }

    if (best->length > size + sizeof(area)) {
        area *new_block = (area *)((char *)best + sizeof(area) + size);

        new_block->marker = BLOCK_MARKER;
        new_block->in_use = false;
        new_block->length = best->length - size - sizeof(area);
        new_block->next = best->next;
        new_block->prev = best;

        if (new_block->next)
            new_block->next->prev = new_block;

        best->next = new_block;
        best->length = size;

        hdr->blocks++;
    }

    best->in_use = true;

    pthread_mutex_unlock(&malloc_lock);

    return (char *)best + sizeof(area);
}

/* ================= free ================= */

void my_free(void *ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&malloc_lock);

    area *block = (area *)((char *)ptr - sizeof(area));

    if (block->marker != BLOCK_MARKER) {
        printf("Invalid free!\n");
        pthread_mutex_unlock(&malloc_lock);
        return;
    }

    block->in_use = false;

    if (block->next && !block->next->in_use) {
        area *next = block->next;

        block->length += sizeof(area) + next->length;
        block->next = next->next;

        if (block->next)
            block->next->prev = block;
    }

    if (block->prev && !block->prev->in_use) {
        area *prev = block->prev;

        prev->length += sizeof(area) + block->length;
        prev->next = block->next;

        if (block->next)
            block->next->prev = prev;
    }

    pthread_mutex_unlock(&malloc_lock);
}

/* ================= realloc ================= */

void *my_realloc(void *ptr, size_t size) {
    if (!ptr) return my_malloc(size);

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    area *block = (area *)((char *)ptr - sizeof(area));

    if (block->length >= size) {
        return ptr;
    }

    void *new_ptr = my_malloc(size);
    if (!new_ptr) return NULL;

    memcpy(new_ptr, ptr, block->length);
    my_free(ptr);

    return new_ptr;
}

/* ================= debug ================= */

void print_heap() {
    area *cur = get_first_block();

    printf("Heap state:\n");

    while (cur) {
        printf("[%s | size=%u] -> ",
               cur->in_use ? "USED" : "FREE",
               cur->length);
        cur = cur->next;
    }

    printf("NULL\n");
}