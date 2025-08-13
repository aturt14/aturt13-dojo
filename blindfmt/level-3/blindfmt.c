#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char** argv) {
    printf("Welcome to %s!\n", argv[0]);
    puts("In this challenge, you are able to change a format string which will then be used to write to /dev/null.");
    puts("Therefore, you won't be able to see the format string output.");
    puts("This challenge has all protections enabled!");
    puts("Now, I'll read the flag into memory and delete the file.");
    puts("This means that you can't (shouldn't) use bruteforce.");

    srand((uint)((ulong)argv >> 8));
    ulong rnd = (ulong)rand() << 12;
    char* flag_buffer = mmap((void*)rnd, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (flag_buffer == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }
    int fd = open("/flag", O_RDONLY);
    if (fd < 0) {
        perror("read");
        return EXIT_FAILURE;
    }
    read(fd, flag_buffer, 0x60);
    close(fd);
    remove("/flag");

    char fmtstr[0x100];
    FILE* file = fopen("/dev/null", "w");

    while (true) {
        memset(fmtstr, 0, sizeof(fmtstr));
        puts("Enter format string: ");
        int bytes_read = read(0, fmtstr, sizeof(fmtstr));
        if (bytes_read < 1 || fmtstr[0] == '\n') {
            break;
        }
        fprintf(file, fmtstr);
    }
    fclose(file);
    return 0;
}
