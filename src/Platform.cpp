//
// Created by Bartosz Budnik on 09.06.2024.
//

#include <glad/glad.h>
#include <cstdio>
#include "includes/Platform.h"

/* Initialize platform */
Platform::Platform(const char *title, int width, int height,
                   int textureWidth, int textureHeight) {
    /* Initialize video */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("ERROR: SDL couldn't be initialized! SDL_Error: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    /* Create a window from given arguments which works on OpenGL and is resizable */
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("ERROR: Window couldn't be initialized! SDL_Error: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    /* Create renderer, initialize it with certain logical size */
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, width, height);

    /* Initialize texture */
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

/* Cleanup */
Platform::~Platform() {
    /* Destroy all SDL variables */
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    /* Quit SDL */
    SDL_Quit();
}

/* Update texture, render it */
void Platform::Update(const void *buffer, int pitch) {
    /* Update texture with the given buffer and pitch */
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    /* Clear the renderer, prepare it for the next draw instruction */
    SDL_RenderClear(renderer);
    /* Copy the texture to the renderer */
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    /* Render present texture */
    SDL_RenderPresent(renderer);
}

/* Process given keys */
bool Platform::ProcessInput(uint8_t *keys) {
    /* Quit flag */
    bool quit = false;

    /* Poll event */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /* If certain key is pressed, set the correct one to pressed */
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
            else {
                for (int i = 0; i < 0xF; ++i) {
                    if (event.key.keysym.sym == realKeys[i])
                        keys[i] = 1;
                }
            }
        }

        /* If certain key isn't pressed anymore, set the correct one to up */
        else if (event.type == SDL_KEYUP) {
            for (int i = 0; i < 0xF; ++i) {
                if (event.key.keysym.sym == realKeys[i])
                    keys[i] = 0;
            }
        }

        /* If user requested quiting, set the quit flag to true */
        else if (event.type == SDL_QUIT)
            quit = true;
    }

    /* Return quit flag */
    return quit;
}
