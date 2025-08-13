#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    setuid(0);
    printf("Welcome to %s!\n", argv[0]);
    puts("In this challenge, you are able to change a format string which will then be used to write to /dev/null.");
    puts("/dev/null seems to be empty:");
    puts("system(\"cat /dev/null\")");
    system("cat /dev/null");
    puts("Therefore, you won't be able to see the format string output.");
    puts("This challenge is compiled without PIE and with Partial RELRO, so it should be fairly simple!");
    FILE* file = fopen("/dev/null", "w");
    char fmtstr[0x100];

    while (true) {
        memset(fmtstr, 0, sizeof(fmtstr));
        printf("Enter format string (max length: %lu): \n", sizeof(fmtstr));
        int bytes_read = read(0, fmtstr, sizeof(fmtstr));
        if (bytes_read < 1 || fmtstr[0] == '\n') {
            break;
        }
        fprintf(file, fmtstr);
    }
    fclose(file);
    printf("Isn't this what you wanted?\n");
    puts("cat /flag");
    printf("Or..");
    puts("/bin/sh");
    puts("??");
    puts("Better luck next time!");
    return 0;
}
