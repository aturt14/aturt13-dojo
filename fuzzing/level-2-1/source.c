#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#define OPPONENT_NAME "cubic"
#define IDX_ERROR -1

typedef struct {
    char name[0x20];
    int money;
} Player;

void new_player(Player* players, char* name, int idx) {
    Player p;
    memcpy(p.name, name, sizeof(p.name));
    // TODO: implement a more random algorithm
    p.money = 0x42 + p.name[0];
    players[idx] = p;
}

int read_player_index() {
    char idx_buf[8];
    puts("Enter player index (max 6):");
    fgets(idx_buf, sizeof(idx_buf), stdin);
    int idx = atoi(idx_buf);
    if (idx < 7) {
        return idx;
    }
    return IDX_ERROR;
}

void remove_newline(char* s, int size) {
    for (int i = 0; i < size; i++) {
        if (s[i] == '\n') {
            s[i] = '\0';
            return;
        }
    }
}

void new_player_interactive(Player* players) {
    char name[0x20];

    puts("Enter player name:");
    fgets(name, sizeof(name), stdin);
    remove_newline(name, sizeof(name));

    int idx = read_player_index();
    if (idx != IDX_ERROR) {
        new_player(players, name, idx);
    }
    else {
        puts("Error creating this player!");
    }
}

void fight(Player* players) {
    new_player(players, OPPONENT_NAME, 7);
    int idx = read_player_index();
    if (idx != IDX_ERROR) {
        if (players[idx].money == 0) {
            puts("Invalid player!");
            return;
        }
        printf("%s fights against %s!\n", players[idx].name, players[7].name);
        if (players[idx].money > players[7].money) {
            printf("%s won!\n", players[idx].name);
        }
        else {
            puts("You lost forever!");
            exit(0);
        }
    }
}

void challenge() {
    char cmd[0x10];
    Player players[8] = {0};

    puts("new_player/fight/exit");
    while (fgets(cmd, sizeof(cmd), stdin)) {
        remove_newline(cmd, sizeof(cmd));
        if (strcmp("new_player", cmd) == 0) {
            new_player_interactive(players);
        }
        else if (strcmp("fight", cmd) == 0) {
            fight(players);
        }
        else if (strcmp("exit", cmd) == 0) {
            return;
        }
        puts("new_player/fight/exit");
    }
}

int main(int argc, char** argv) {
    puts("In this module, you'll learn about fuzzing with AFL++.");
    puts("You can check out the AFL++ project on GitHub:");
    puts("https://github.com/AFLplusplus/AFLplusplus");
    puts("You can also do `man afl-fuzz` to read the manpages.\n");
    puts("Fuzzing is an effective way of finding vulnerabilities in complex codebases.");
    puts("afl-fuzz and afl-clang-fast are already installed in this dojo as they will be the primary tools for solving these challenges.");
    puts("For this challenge, you'll need to create a dictionary. A dictionary is typically a file in the following format:");
    puts("\"long_string\"");
    puts("\"another_long_string\"");
    puts("You can tell afl-fuzz where the dictionary is using the -x argument.");
    puts("In this challenge, you will have the source code available. This means that you can use afl-clang-fast to compile it for a better fuzzing experience.");
    puts("This challenge reads input from stdin!");

    challenge();
    return 0;
}
