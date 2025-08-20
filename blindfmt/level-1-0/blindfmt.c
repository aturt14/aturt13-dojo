#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>

uint djb2(const char* s) {
    ulong h = 5381;
    int c;
    while ((c = (unsigned char)*s++))
        h = ((h << 5) + h) + c;
    return (uint)h;
}

int rand_from_flag() {
    char flag[0x60] = {'\0'};
    int fd = open("/flag", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }
    read(fd, flag, 0x60);
    close(fd);
    srand(djb2(flag));
    return rand();
}

int main(int argc, char** argv) {
    printf("Welcome to %s!\n", argv[0]);
    puts("In this challenge, your task is to copy a secret value from the stack.");
    puts("You'll be given the opportunity to modify a format string, but you will not see its output, as it will be written into /dev/null.");
    puts("Don't even think about bruteforcing this!");
    sleep(2);

    char fmtstr[0x100] = {'\0'};
    FILE* file = fopen("/dev/null", "w");
    ulong secret = rand_from_flag();
    ulong* definitely_not_secret = malloc(sizeof(int));
    *definitely_not_secret = secret == 42? 42 - 1 : 42;

    puts("Enter format string: ");
    read(0, fmtstr, sizeof(fmtstr));
    fprintf(file, fmtstr);

    if (secret == *definitely_not_secret) {
        int fd = open("/flag", O_RDONLY);
        sendfile(1, fd, 0, 0x60);
        close(fd);
    }

    fclose(file);
    return 0;
}
