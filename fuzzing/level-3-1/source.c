#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdlib.h>

const char kitten[] = 
        "     /\\    /\\\n"
        "    {**`---'**}\n"
        "    {**O***O**}\n"
        "    ~~>**V**<~~\n"
        "     \\*\\|/*/\n"
        "      `-----'____\n"
        "      /******\\****\\_\n"
        "     {********}\\**)_\\\n"
        "     |**\\_/**)*/*/\n"
        "      \\__/**/(_/\n"
        "        (___/\n";

typedef struct {
    char food[0x10];
    char name[0x20];
    char fur_char;
    char kitten[sizeof(kitten)];
} Kitten;

void remove_newline(char* buf, int size) {
    for (int i = 0; i < size; i++) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            return;
        }
    }
}

unsigned int get_index() {
    char idx_buf[0x10];
    puts("Enter kitten index:");
    fgets(idx_buf, sizeof(idx_buf), stdin);
    unsigned int idx = strtoul(idx_buf, NULL, 10);
    if (idx < 0x10) {
        return idx;
    }
    puts("Invalid index!");
    puts("I'm tired of you.");
    puts("zzz");
    sleep(2);
    return -1;
}

void add_kitten_interactive(Kitten** kittens) {
    unsigned int idx = 0;
    while (idx < 0x10 && kittens[idx] != NULL) {
        idx++;
    }

    if (idx == 0x10) {
        puts("Kittengarten is full!");
        return;
    }

    kittens[idx] = malloc(sizeof(Kitten));
    strcpy(kittens[idx]->kitten, kitten);
    kittens[idx]->fur_char = '*';
    
    puts("Enter kitten's name:");
    fgets(kittens[idx]->name, sizeof(kittens[idx]->name), stdin);
    remove_newline(kittens[idx]->name, sizeof(kittens[idx]->name));

    printf("Kitten '%s' has been added at index %d!\n", kittens[idx]->name, idx);
}

void remove_kitten_interactive(Kitten** kittens) {
    unsigned int idx = get_index();
    if (idx == -1 || kittens[idx] == NULL) {
        puts("No kitten at that index.");
        return;
    }
    printf("Kitten '%s' has been set free.\n", kittens[idx]->name);
    free(kittens[idx]);
}

void view_kitten_interactive(Kitten** kittens) {
    unsigned int idx = get_index();
    if (idx == -1 || kittens[idx] == NULL) {
        puts("No kitten at that index.");
        return;
    }
    Kitten* k = kittens[idx];
    printf("Name: %s\n", k->name);
    printf("Favorite food: %s\n", k->food);
    printf("Fur character: %c\n", k->fur_char);
    printf("%s", k->kitten);
}

void feed_kitten_interactive(Kitten** kittens) {
    unsigned int idx = get_index();
    if (idx == -1 || kittens[idx] == NULL) {
        puts("No kitten at that index.");
        return;
    }
    puts("Enter food:");
    fgets(kittens[idx]->food, sizeof(kittens[idx]->food), stdin);
    remove_newline(kittens[idx]->food, sizeof(kittens[idx]->food)); 
    puts("Eating...");
    sleep(2);
    puts("Yum!");
}

void change_fur_interactive(Kitten** kittens) {
    unsigned int idx = get_index();
    if (idx == -1 || kittens[idx] == NULL) {
        puts("No kitten at that index.");
        return;
    }
    puts("New fur:");
    char buf[4];
    fgets(buf, sizeof(buf), stdin);
    char new_fur = buf[0];

    int is_alnum = (new_fur >= 'a' && new_fur <= 'z') || (new_fur >= 'A' && new_fur <= 'Z') || (new_fur >= '0' && new_fur <= '9');
    if (!isalnum(new_fur)) {
        puts("Nope!");
        return;
    }

    char old_fur = kittens[idx]->fur_char;
    for (int i = 0; i < strlen(kittens[idx]->kitten); i++) {
        if (kittens[idx]->kitten[i] == old_fur) {
            kittens[idx]->kitten[i] = new_fur;
        }
    }
    kittens[idx]->fur_char = new_fur;
}

void challenge() {
    puts("Welcome to Kittengarten!");
    puts("Kittengarten is eternal!");
    Kitten** kittens = calloc(0x100, sizeof(Kitten*));
    char cmd[0x20];
    short exit = 0;
    while (!exit) {
        puts("\nadd_kitten / remove_kitten / view_kitten / feed_kitten / change_fur / exit");
        fgets(cmd, sizeof(cmd), stdin);
        remove_newline(cmd, sizeof(cmd));
        if (strcmp("add_kitten", cmd) == 0) {
            add_kitten_interactive(kittens);
        }
        else if (strcmp("remove_kitten", cmd) == 0) {
            remove_kitten_interactive(kittens);
        }
        else if (strcmp("view_kitten", cmd) == 0) {
            view_kitten_interactive(kittens);
        }
        else if (strcmp("feed_kitten", cmd) == 0) {
            feed_kitten_interactive(kittens);
        }
        else if (strcmp("change_fur", cmd) == 0) {
            change_fur_interactive(kittens);
        }
        else if (strcmp("exit", cmd) == 0) {
            exit = 1;
        }

        if (exit = 0) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    puts("In this module, you'll learn about fuzzing with AFL++.");
    puts("You can check out the AFL++ project on GitHub:");
    puts("https://github.com/AFLplusplus/AFLplusplus");
    puts("You can also do `man afl-fuzz` to read the manpages.\n");
    puts("Fuzzing is an effective way of finding vulnerabilities in complex codebases.");
    puts("afl-fuzz and afl-clang-fast are already installed in this dojo as they will be the primary tools for solving these challenges.");
    puts("In this challenge, you will have the source code available. This means that you can use afl-clang-fast to compile it for a better fuzzing experience.");
    puts("This challenge is not very fuzzing friendly.");
    puts("That means that you will have to modify it slightly in order to fuzz effectively.");
    puts("To do this, you can write a fuzzing harness which will handle to io communication with the program.");
    puts("This usually involves changing the main function.");
    puts("This challenge reads input from stdin!");
    puts("");

    challenge();
    return 0;
}
