// Copyright (c) 2025 Chardson Coelho
// Licensed under the MIT License. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SCREEN_HEIGHT 56
#define SCREEN_WIDTH 200
#define SCREEN_CHARS SCREEN_HEIGHT * SCREEN_WIDTH
#define FPS 30
#define BADAPPLE_FRAMES 6527
#define USVIDSYNC 500000

typedef long long uclock;

// screen_buffer gets printed like a string, where each lines terminates with \n\r
// the last line doesn't need that
char screen_buffer[SCREEN_CHARS + 2 * (SCREEN_HEIGHT - 1)];

typedef struct {
    char chars[SCREEN_WIDTH][SCREEN_HEIGHT];
} AImg;

typedef struct {
    AImg buffer[BADAPPLE_FRAMES]; 
    int p_buffer;
} Streamer;

Streamer *g_streamer = NULL;

Streamer* Streamer_new() {
    Streamer *new_streamer = malloc(sizeof(Streamer));
    new_streamer->p_buffer = 0;
    return new_streamer;
}

void Streamer_destroy(Streamer *streamer) {
    if (streamer) {free(streamer);}
}

int Streamer_load(Streamer *streamer, const char* path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        perror("Unable to open file\n");
        return 1;
    }

    size_t count = fread(streamer->buffer, sizeof(AImg), BADAPPLE_FRAMES, file);

    if (count != BADAPPLE_FRAMES) {
        perror("Something went wrong while fetching file");
        return 1;
    }

    fclose(file);

    return 0;
}

void Streamer_render(Streamer *streamer) {
    AImg *aimg = streamer->buffer + streamer->p_buffer;

    int i = 0;
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            screen_buffer[i++] = aimg->chars[x][y];
        }
        if (y != SCREEN_HEIGHT - 1) {
            screen_buffer[i++] = '\n';
            screen_buffer[i++] = '\r';
        }
    }

    streamer->p_buffer = (streamer->p_buffer + 1) % BADAPPLE_FRAMES;
}

uclock get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uclock)tv.tv_sec * 1000000 + tv.tv_usec;
}

void flip_screen() {
    printf("\033[2J\033[H"); // Return to top left
    printf("%s", screen_buffer);
    fflush(stdout);
}

void play_badapple_song() {
    static pid_t music_pid = 0;
    
    if (music_pid > 0) {
        kill(music_pid, SIGTERM);
        waitpid(music_pid, NULL, 0);
    }
    
    music_pid = fork();
    if (music_pid == 0) {
        execlp("mpg123", "mpg123", "-q", "./assets/badapple.mp3", NULL);
        perror("execlp");
        exit(1);
    } 
    else if (music_pid < 0) {
        perror("fork");
    }
}

void clean_up() {
    printf("\033[?25h"); // Show cursor
    Streamer_destroy(g_streamer);
    system("pkill -TERM mpg123 && clear");
    exit(0);
}

int main() {
    printf("\033[?25l"); // Hide cursor
    atexit(clean_up); // Ensure cursor is restored on exit
    
    setvbuf(stdout, NULL, _IOFBF, SCREEN_CHARS);
    signal(SIGINT, clean_up);

    printf("Loading badapple.dat ...\n");
    fflush(stdout);
    g_streamer = Streamer_new();
    if (Streamer_load(g_streamer, "./assets/badapple.dat") != 0) {
        clean_up();
        return 1;
    }
    printf("Loaded badapple.dat\n");
    fflush(stdout);

    printf("Starting stream ...");
    fflush(stdout);

    int uspf = 1000000 / FPS;
    uclock start;
    uclock end;
    uclock elapsed;
    play_badapple_song();
    usleep(USVIDSYNC); // Sync offset
    while (1) {
        start = get_time();

        if (g_streamer->p_buffer == 0) {
            play_badapple_song();
            usleep(USVIDSYNC);
        }

        Streamer_render(g_streamer);
        flip_screen();

        end = get_time();
        elapsed = end - start;
        if (elapsed < uspf) {usleep(uspf - elapsed);}
    }

    return 0;
}