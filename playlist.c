// real_playlist.c
// Music Playlist Manager - plays actual songs from folder
// Works on Windows, Linux, macOS (uses default player)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAX_PATH_LEN 512

typedef struct Song {
    char filename[MAX_PATH_LEN];
    struct Song *prev;
    struct Song *next;
} Song;

typedef struct {
    Song *head;
    Song *tail;
    Song *current;
    size_t size;
    int shuffle;
    int repeat; // 0=none, 1=one, 2=all
    char folder[MAX_PATH_LEN];
} Playlist;

void init_playlist(Playlist *pl, const char *folder) {
    pl->head = pl->tail = pl->current = NULL;
    pl->size = 0;
    pl->shuffle = 0;
    pl->repeat = 0;
    strncpy(pl->folder, folder, MAX_PATH_LEN - 1);
    srand((unsigned)time(NULL));
}

Song *create_song(const char *name) {
    Song *s = malloc(sizeof(Song));
    if (!s) { perror("malloc"); exit(1); }
    strncpy(s->filename, name, MAX_PATH_LEN - 1);
    s->prev = s->next = NULL;
    return s;
}

void add_song(Playlist *pl, const char *name) {
    Song *s = create_song(name);
    if (!pl->head) pl->head = pl->tail = pl->current = s;
    else {
        pl->tail->next = s;
        s->prev = pl->tail;
        pl->tail = s;
    }
    pl->size++;
}

void load_songs_from_folder(Playlist *pl) {
    DIR *d;
    struct dirent *dir;
    d = opendir(pl->folder);
    if (!d) {
        printf("Cannot open folder: %s\n", pl->folder);
        return;
    }

    printf("\nScanning folder: %s\n", pl->folder);
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".mp3") || strstr(dir->d_name, ".wav")) {
            add_song(pl, dir->d_name);
        }
    }
    closedir(d);
    printf("Loaded %zu songs.\n\n", pl->size);
}

void display_playlist(Playlist *pl) {
    printf("\nPlaylist (%zu songs)\n", pl->size);
    if (!pl->head) {
        printf("  [Empty playlist]\n");
        return;
    }
    Song *iter = pl->head;
    int i = 1;
    while (iter) {
        printf(" %2d. %s%s\n", i, iter->filename, iter == pl->current ? "  <-- current" : "");
        iter = iter->next;
        i++;
    }
    printf("\n");
}

void play_song(Playlist *pl) {
    if (!pl->current) {
        printf("No song selected.\n");
        return;
    }
    char path[MAX_PATH_LEN * 2];
    snprintf(path, sizeof(path), "%s/%s", pl->folder, pl->current->filename);
#ifdef _WIN32
    char cmd[MAX_PATH_LEN * 2];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", path);
#elif __APPLE__
    char cmd[MAX_PATH_LEN * 2];
    snprintf(cmd, sizeof(cmd), "open \"%s\"", path);
#else
    char cmd[MAX_PATH_LEN * 2];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", path);
#endif
    printf(" Playing: %s\n", pl->current->filename);
    system(cmd);
}

void next_song(Playlist *pl) {
    if (!pl->current) return;
    if (pl->repeat == 1) {
        play_song(pl);
        return;
    }
    if (pl->current->next)
        pl->current = pl->current->next;
    else if (pl->repeat == 2)
        pl->current = pl->head;
    else {
        printf("End of playlist.\n");
        return;
    }
    play_song(pl);
}

void prev_song(Playlist *pl) {
    if (!pl->current) return;
    if (pl->repeat == 1) {
        play_song(pl);
        return;
    }
    if (pl->current->prev)
        pl->current = pl->current->prev;
    else if (pl->repeat == 2)
        pl->current = pl->tail;
    else {
        printf("Start of playlist.\n");
        return;
    }
    play_song(pl);
}

void shuffle_playlist(Playlist *pl) {
    if (pl->size < 2) return;
    Song **arr = malloc(sizeof(Song*) * pl->size);
    Song *it = pl->head;
    size_t i = 0;
    while (it) { arr[i++] = it; it = it->next; }

    for (size_t j = pl->size - 1; j > 0; j--) {
        size_t k = rand() % (j + 1);
        Song *temp = arr[j];
        arr[j] = arr[k];
        arr[k] = temp;
    }

    pl->head = arr[0];
    pl->head->prev = NULL;
    for (size_t j = 0; j < pl->size - 1; j++) {
        arr[j]->next = arr[j + 1];
        arr[j + 1]->prev = arr[j];
    }
    pl->tail = arr[pl->size - 1];
    pl->tail->next = NULL;
    pl->current = pl->head;
    free(arr);
    printf("Playlist shuffled!\n");
}

void cycle_repeat(Playlist *pl) {
    pl->repeat = (pl->repeat + 1) % 3;
    printf("Repeat mode: %s\n",
           pl->repeat == 0 ? "NONE" :
           pl->repeat == 1 ? "ONE" : "ALL");
}

void menu() {
    puts("\n:) MUSIC PLAYLIST MANAGER :) ");
    puts("1. View playlist ");
    puts("2. Play current song ");
    puts("3. Next song ->");
    puts("4. Previous song <-");
    puts("5. Shuffle playlist");
    puts("6. Cycle repeat mode ");
    puts("7. Exit ");
    printf("Choose: ");
}

int main() {
    Playlist pl;
    char folder[MAX_PATH_LEN];

    printf("Enter music folder path : ");
    fgets(folder, sizeof(folder), stdin);
    folder[strcspn(folder, "\n")] = 0;

    init_playlist(&pl, folder);
    load_songs_from_folder(&pl);

    if (!pl.head) {
        printf("No songs found. Exiting.\n");
        return 0;
    }

    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: display_playlist(&pl); break;
            case 2: play_song(&pl); break;
            case 3: next_song(&pl); break;
            case 4: prev_song(&pl); break;
            case 5: shuffle_playlist(&pl); break;
            case 6: cycle_repeat(&pl); break;
            case 7: printf("Chill mode end! -_- \n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
