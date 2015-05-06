#include <SDL2/SDL.h>
#include <stdio.h>

#include "dungen.h"
#define RECT_SIZE 8
#define D_WIDTH 80
#define D_HEIGHT 80

int running = 1;
SDL_Event e;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void render_cell(int x, int y, enum dg_cell_kind k)
{
    SDL_Rect rect = {
        .x = x * RECT_SIZE,
        .y = y * RECT_SIZE,
        .w = RECT_SIZE,
        .h = RECT_SIZE 
    };

    switch (k) {
        case c_stone:
            SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
            break;
        case c_wall:
            SDL_SetRenderDrawColor(renderer, 32, 32, 128, 255);
            break;
        case c_floor:
            SDL_SetRenderDrawColor(renderer, 32, 128, 32, 255);
            break;
    }

    SDL_RenderDrawRect(renderer, &rect);
}

void render_step(dg_dungeon d, int step)
{
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = 0;
        }
    }

    if (!running) {
        SDL_Quit();
        exit(0);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    dg_each(d, render_cell);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

int main(int argc, char **argv) {
    int d_width = D_WIDTH;
    int d_height = D_HEIGHT;

    if (argc == 3) {
        d_width  = atoi(argv[1]);
        d_height = atoi(argv[2]);
    }

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "dungen",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (d_width * RECT_SIZE),
        (d_height * RECT_SIZE),
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    (void)dg_generate(d_width, d_height, render_step);

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(100);
    }

    SDL_Quit();
}