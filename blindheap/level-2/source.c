#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define ALLOCATIONS_SIZE 128
#define MIN_ALLOC 0x1
#define MAX_ALLOC 0x100000

// #define CZECK

#ifdef CZECK
#define TCACHE_MAX_BINS 64
typedef struct tcache_perthread_struct {
    u_int16_t num_slots[TCACHE_MAX_BINS];
    void* entries[TCACHE_MAX_BINS];
} tcache_perthread_struct;
tcache_perthread_struct* tps;
#endif

void fail() {
    puts("You failed!");
    exit(42);
}

size_t read_ulong() {
    char int_buf[0x10];
    char* endptr;
    fgets(int_buf, sizeof(int_buf), stdin);
    return strtoull(int_buf, &endptr, 10);
}

size_t read_size() {
    puts("Enter chunk size:");
    size_t size = read_ulong();
    if (size >= MIN_ALLOC && size <= MAX_ALLOC) {
        return size;
    }
    fail();
}

size_t read_slot() {
    puts("Enter allocation position:");
    size_t pos = read_ulong();
    if (pos < ALLOCATIONS_SIZE) {
        return pos;
    }
    fail();
}

size_t read_offset() {
    puts("Enter offset:");
    return read_ulong();
}

void u_malloc(char** allocations, size_t* sizes) {
    size_t pos = read_slot();
    size_t size = read_size();
    char* a = malloc(size);
    allocations[pos] = a;
    sizes[pos] = size;
}

void u_free(char** allocations, size_t* sizes) {
    size_t pos = read_slot();
    free(allocations[pos]);
}

void u_read(char** allocations, size_t* sizes) {
    size_t pos = read_slot();
    size_t offset = read_offset();
    read(0, allocations[pos] + offset, sizes[pos] - offset);
}

void remove_newline(char* str) {
    char* newline = strchr(str, '\n');
    if (newline) *newline = '\0';
}

#ifdef CZECK
void u_leak(char** allocations) {
    u_int8_t heap_leak = ((size_t)allocations[0] >> 12) & 0xf;
    puts("Least significant ASLR nibble of heap:");
    printf("Heap: 0x%hhx\n", heap_leak);
}

void u_check(char** allocations) {
    if (tps->entries[0] == (void*)0x4242424242424242) {
        char flag_buf[128] = {};
        int fd = open("/flag", O_RDONLY);
        size_t bts = read(fd, flag_buf, 64);
        write(1, flag_buf, bts);
    }
    else fail();
}
#endif

bool challenge_loop(char** allocations, size_t* sizes) {
#ifdef CZECK
    puts("malloc/free/read/check/leak/exit");
#else
    puts("malloc/free/read/exit");
#endif
    printf(">> ");

    char cmd_buf[0x10];
    fgets(cmd_buf, sizeof(cmd_buf), stdin);
    remove_newline(cmd_buf);

    if (strcmp("malloc", cmd_buf) == 0) u_malloc(allocations, sizes);
    else if (strcmp("free", cmd_buf) == 0) u_free(allocations, sizes);
    else if (strcmp("read", cmd_buf) == 0) u_read(allocations, sizes);
#ifdef CZECK
    else if (strcmp("check", cmd_buf) == 0) u_check(allocations);
    else if (strcmp("leak", cmd_buf) == 0) u_leak(allocations);
#endif
    else if (strcmp("exit", cmd_buf) == 0) return false;
    else fail();

    return true;
}

int main(int argc, char** argv) {
    char* allocations[ALLOCATIONS_SIZE] = {};
    size_t sizes[ALLOCATIONS_SIZE] = {};

    puts("In this challenge, you will perform a heap exploit without having access to a write function.");
#ifdef CZECK
    tps = sbrk(0) - 0x21000 + 0x10;
    puts("This challenge is in czeck mode enabled!");
    puts("That means you only need to achieve control of the tcache_perthread_struct (at the start of the heap).");
    puts("Afterwards, you can call check to get your flag!");
    puts("Additionally, you get a nibble worth of leaks!");
#endif
    while (challenge_loop(allocations, sizes));
    return 0;
}
