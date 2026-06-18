#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ssize_t readsize() {
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

void memcopy(char* dest, const char* src, ssize_t n) {
    for (; n >= 0; --n) {
        dest[n] = src[n];
    }
}

void do_new(void** allocations, ssize_t* sizes) {
    char buffer[0x1010];
    memset(buffer, 0x0, sizeof(buffer));
    puts("Size: ");
    ssize_t sz = readsize();
    if (sz > 0x1000 || sz == 0) exit(42);
    puts("Data: ");
    ssize_t bread = read(0x0, buffer, sz);
    if (bread < 0) exit(123);
    puts("Index: ");
    ssize_t idx = readsize();
    if (idx >= 100) exit(37);
    char* alloc = malloc(sz);
    if (!alloc) exit(-64);
    memcopy(alloc, buffer, bread);
    allocations[idx] = alloc;
    sizes[idx] = sz;
}

void do_edit(void** allocations, ssize_t* sizes) {
    puts("Index: ");
    size_t idx = readsize();
    if (idx >= 100) exit(37);
    puts("Data: ");
    if (allocations[idx] && sizes[idx])
        read(0, allocations[idx], sizes[idx]);
}

void do_delete(void** allocations, ssize_t* sizes) {
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
    ssize_t sizes[100] = {0};
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
