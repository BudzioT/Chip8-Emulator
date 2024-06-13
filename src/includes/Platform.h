//
// Created by Bartosz Budnik on 09.06.2024.
//

#ifndef CHIP8_EMULATOR_PLATFORM_H
#define CHIP8_EMULATOR_PLATFORM_H

#include <SDL.h>
#include <GL/gl.h>

class Platform
{
public:
    Platform(const char* title, int width, int height, int textureWidth, int textureHeight);
    ~Platform();

    void Update(void const* buffer, int pitch);
    static bool ProcessInput(uint8_t* keys);

private:
    SDL_Window* window { };
    SDL_Renderer* renderer { };
    SDL_Texture* texture { };
    static constexpr int realKeys[16] = {
            SDLK_x,
            SDLK_1, SDLK_2, SDLK_3,
            SDLK_q, SDLK_w, SDLK_e,
            SDLK_a, SDLK_s, SDLK_d,
            SDLK_z, SDLK_c, SDLK_4,
            SDLK_r, SDLK_f, SDLK_v
    };
};


#endif //CHIP8_EMULATOR_PLATFORM_H
