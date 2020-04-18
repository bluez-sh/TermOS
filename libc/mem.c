#include "mem.h"

void mem_cpy(void *src, const void *dst, unsigned nbytes)
{
    unsigned i;
    for (i = 0; i < nbytes; i++)
        *((uint8_t *)dst + i) = *((uint8_t *)src + i);
}

void mem_set(void *dst, int val, unsigned nbytes)
{
    unsigned i;
    for (i = 0; i < nbytes; i++)
        *((uint8_t *)dst + i) = (uint8_t)val;
}

#define PAGE_SIZE 512
#define MEM_START (0x10000)
static uint32_t free_mem_ptr = MEM_START;

static uint32_t alloc_page()
{
    uint32_t ret = free_mem_ptr;
    free_mem_ptr += PAGE_SIZE;
    return ret;
}

static void free_page()
{
    if (free_mem_ptr - PAGE_SIZE >= MEM_START)
        free_mem_ptr -= PAGE_SIZE;
}

uint32_t kmalloc(size_t size)
{
    uint32_t ret = alloc_page();
    if (size > PAGE_SIZE) {
        uint32_t n;
        size -= PAGE_SIZE;
        n = size/PAGE_SIZE + 1;
        while (n--) alloc_page();
    }
    return ret;
}

uint32_t kmallocz(size_t size)
{
    char *ptr = (char*) kmalloc(size);
    mem_set(ptr, 0, size);
    return (uint32_t) ptr;
}

void free(size_t size)
{
    uint32_t n = size/PAGE_SIZE + 1;
    while (n--) free_page();
}
