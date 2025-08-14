#include <stdio.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>

int challenge() {
    char buf[0x60];
    return read(0, buf, 0x100);
}

int main(int argc, char** argv) {
    puts("In this module, you'll learn about fuzzing with AFL++.");
    puts("You can check out the AFL++ project on GitHub:");
    puts("https://github.com/AFLplusplus/AFLplusplus");
    puts("You can also do `man afl-fuzz` to read the manpages.\n");
    puts("Fuzzing is an effective way of finding vulnerabilities in complex codebases.");
    puts("afl-fuzz and afl-clang-fast are already installed in this dojo as they will be the primary tools for solving these challenges.");
    puts("In this challenge, you will have the source code available. This means that you can use afl-clang-fast to compile it for a better fuzzing experience.");
    puts("This challenge reads input from stdin!");

    challenge();
    return 0;
}
