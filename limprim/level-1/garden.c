#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define NAME_LEN 32

typedef struct {
    char name[NAME_LEN];
    unsigned int health;
} Plant;

typedef struct {
    Plant **plots;
    size_t capacity;
    size_t count;
} Garden;

// Forward declarations
void init_garden(Garden *g);
void cleanup_garden(Garden *g);
void expand_garden(Garden *g);
void cmd_plant(Garden *g);
void cmd_harvest(Garden *g);
void cmd_water(Garden *g);
void cmd_show(const Garden *g);

int main(void) {
    setvbuf(stdout, 0, _IOLBF, 0);
    Garden garden;
    init_garden(&garden);

    char line[16];
    printf("Welcome to the Gardening Simulator!\n");
    printf("Commands: plant, harvest, water, show, exit\n");

    while (1) {
        printf("> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin))
            break;

        if (strncmp(line, "plant", 5) == 0) {
            cmd_plant(&garden);
        } else if (strncmp(line, "harvest", 7) == 0) {
            cmd_harvest(&garden);
        } else if (strncmp(line, "water", 5) == 0) {
            cmd_water(&garden);
        } else if (strncmp(line, "show", 4) == 0) {
            cmd_show(&garden);
        } else if (strncmp(line, "exit", 4) == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    cleanup_garden(&garden);
    printf("Goodbye!\n");
    return 0;
}

void init_garden(Garden *g) {
    g->capacity = INITIAL_CAPACITY;
    g->count = 0;
    g->plots = malloc(sizeof(Plant*) * g->capacity);
    if (!g->plots) {
        perror("malloc");
        exit(1);
    }
    for (size_t i = 0; i < g->capacity; i++) {
        g->plots[i] = NULL;
    }
}

void cleanup_garden(Garden *g) {
    for (size_t i = 0; i < g->capacity; i++) {
        free(g->plots[i]);
    }
    free(g->plots);
}

void expand_garden(Garden *g) {
    size_t new_cap = g->capacity * 2;
    Plant **new_plots = realloc(g->plots, sizeof(Plant*) * new_cap);
    if (!new_plots) {
        perror("realloc");
        cleanup_garden(g);
        exit(1);
    }
    // Initialize new slots
    for (size_t i = g->capacity; i < new_cap; i++) {
        new_plots[i] = NULL;
    }
    g->plots = new_plots;
    g->capacity = new_cap;
}

void cmd_plant(Garden *g) {
    if (g->count >= g->capacity) {
        expand_garden(g);
        printf("Garden expanded to %zu plots.\n", g->capacity);
    }

    // Find first empty slot
    size_t idx = 0;
    while (idx < g->capacity && g->plots[idx] != NULL) {
        idx++;
    }

    Plant *p = malloc(sizeof(Plant));
    if (!p) {
        perror("malloc");
        return;
    }

    printf("Enter plant name:\n");
    if (!fgets(p->name, NAME_LEN, stdin)) {
        free(p);
        return;
    }
    // Strip newline
    p->name[strcspn(p->name, "\n")] = '\0';
    p->health = 10;

    g->plots[idx] = p;
    g->count++;
    printf("Planted \"%s\" in plot %zu (health=%d).\n", p->name, idx, p->health);
}

void cmd_harvest(Garden *g) {
    int idx;
    printf("Which plot to harvest?\n");
    if (scanf("%d%*c", &idx) != 1) {
        printf("Invalid input.\n");
        return;
    }
    if (idx < 0 || (size_t)idx >= g->capacity || g->plots[idx] == NULL) {
        printf("Nothing to harvest there.\n");
        return;
    }

    free(g->plots[idx]);
    g->plots[idx] = NULL;
    g->count--;
    printf("Harvested plot %d.\n", idx);
}

void cmd_water(Garden *g) {
    int idx;
    unsigned int quality;
    printf("Which plot to water?\n");
    if (scanf("%d%*c", &idx) != 1) {
        printf("Invalid input.\n");
        return;
    }
    if (idx < 0 || (size_t)idx >= g->capacity || g->plots[idx] == NULL) {
        printf("There's nothing planted there.\n");
        return;
    }
    printf("Enter water quality (0-100):\n");
    if (scanf("%u%*c", &quality) != 1) {
        printf("Invalid input.\n");
        return;
    }

    Plant *p = g->plots[idx];
    p = (Plant*)((char*)p - quality * sizeof(Plant));  
    p->health += quality;

    printf("Watered plot %d (quality=%u). \"%s\" health is now %d.\n",
           idx, quality, g->plots[idx]->name, g->plots[idx]->health);
}

void cmd_show(const Garden *g) {
    printf("Garden status (capacity=%zu, occupied=%zu):\n", g->capacity, g->count);
    for (size_t i = 0; i < g->capacity; i++) {
        if (g->plots[i]) {
            printf(" [%2zu] %s (health=%d)\n", i,
                   g->plots[i]->name, g->plots[i]->health);
        } else {
            printf(" [%2zu] (empty)\n", i);
        }
    }
}

