#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    printf("Welcome to %s!\n", argv[0]);
    puts("In this challenge, you are able to change a format string which will then be used to write to /dev/null.");
    puts("Therefore, you won't be able to see the format string output.");
    puts("This challenge has all protections enabled!");
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
