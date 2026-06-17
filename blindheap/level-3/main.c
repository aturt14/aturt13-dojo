#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int readsize() {
    char buffer[0x10];
    fgets(buffer, sizeof(buffer), stdin);
    int sz = atoi(buffer);
    if (sz >= 0) {
        return sz;
    }
    exit(67);
}

void menu() {
    puts("new/edit/delete");
    printf(">> ");
}

void memcopy(char* dest, const char* src, size_t n) {
    for (; n >= 0; --n) {
        dest[n] = src[n];
    }
}

void do_new(void** allocations, ulong* sizes) {
    char buffer[0x1000];
    memset(buffer, 0x0, sizeof(buffer));
    puts("Size: ");
    size_t sz = readsize();
    if (sz > 0x1000 || sz == 0) exit(42);
    puts("Data: ");
    fgets(buffer, sizeof(buffer), stdin);
    puts("Index: ");
    size_t idx = readsize();
    if (idx >= 100) exit(37);
    char* alloc = malloc(sz);
    memcopy(alloc, buffer, sz);
    allocations[idx] = alloc;
    sizes[idx] = sz;
}

void do_edit(void** allocations, ulong* sizes) {
    puts("Index: ");
    size_t idx = readsize();
    if (idx >= 100) exit(37);
    puts("Data: ");
    if (allocations[idx] && sizes[idx])
        read(0, allocations[idx], sizes[idx]);
}

void do_delete(void** allocations, ulong* sizes) {
    puts("Index: ");
    size_t idx = readsize();
    if (idx >= 100) exit(37);
    if (allocations[idx] && sizes[idx]) {
        free(allocations[idx]);
        allocations[idx] = 0; // prevent double-free
        sizes[idx] = 0;
    }
}

void interactive() {
    void* allocations[100] = {0};
    ulong sizes[100] = {0};
    char cmd[0x10];
    while (1) {
        menu();
        fgets(cmd, sizeof(cmd), stdin);
        if (!strcmp("new\n", cmd)) {
            do_new(allocations, sizes);
        }
        else if (!strcmp("edit\n", cmd)) {
            do_edit(allocations, sizes);
        }
        else if (!strcmp("delete\n", cmd)) {
            do_delete(allocations, sizes);
        }
    }
}


int main(int argc, char** argv) {
    interactive();

    return 0;
}
