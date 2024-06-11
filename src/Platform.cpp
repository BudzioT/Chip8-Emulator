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
            switch (event.key.keysym.sym) {
                case SDLK_x:
                    keys[0] = 1;
                    break;
                case SDLK_1:
                    keys[0x1] = 1;
                    break;
                case SDLK_2:
                    keys[0x2] = 1;
                    break;
                case SDLK_3:
                    keys[0x3] = 1;
                    break;
                case SDLK_q:
                    keys[0x4] = 1;
                    break;
                case SDLK_w:
                    keys[0x5] = 1;
                    break;
                case SDLK_e:
                    keys[0x6] = 1;
                    break;
                case SDLK_a:
                    keys[0x7] = 1;
                    break;
                case SDLK_s:
                    keys[0x8] = 1;
                    break;
                case SDLK_d:
                    keys[0x9] = 1;
                    break;
                case SDLK_z:
                    keys[0xA] = 1;
                    break;
                case SDLK_c:
                    keys[0xB] = 1;
                    break;
                case SDLK_4:
                    keys[0xC] = 1;
                    break;
                case SDLK_r:
                    keys[0xD] = 1;
                    break;
                case SDLK_f:
                    keys[0xE] = 1;
                    break;
                case SDLK_v:
                    keys[0xF] = 1;
                    break;

                case SDLK_ESCAPE:
                    quit = true;
                    break;
            }
        }

        /* If certain key isn't pressed anymore, set the correct one to up */
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_x:
                    keys[0] = 0;
                    break;
                case SDLK_1:
                    keys[0x1] = 0;
                    break;
                case SDLK_2:
                    keys[0x2] = 0;
                    break;
                case SDLK_3:
                    keys[0x3] = 0;
                    break;
                case SDLK_q:
                    keys[0x4] = 0;
                    break;
                case SDLK_w:
                    keys[0x5] = 0;
                    break;
                case SDLK_e:
                    keys[0x6] = 0;
                    break;
                case SDLK_a:
                    keys[0x7] = 0;
                    break;
                case SDLK_s:
                    keys[0x8] = 0;
                    break;
                case SDLK_d:
                    keys[0x9] = 0;
                    break;
                case SDLK_z:
                    keys[0xA] = 0;
                    break;
                case SDLK_c:
                    keys[0xB] = 0;
                    break;
                case SDLK_4:
                    keys[0xC] = 0;
                    break;
                case SDLK_r:
                    keys[0xD] = 0;
                    break;
                case SDLK_f:
                    keys[0xE] = 0;
                    break;
                case SDLK_v:
                    keys[0xF] = 0;
                    break;
            }
        }

        /* If user requested quiting, set the quit flag to true */
        else if (event.type == SDL_QUIT)
            quit = true;
    }

    /* Return quit flag */
    return quit;
}
