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
    SDL_GLContext glContext { };
    SDL_Renderer* renderer { };
    SDL_Texture* texture { };
    GLuint framebufferTexture;
};


#endif //CHIP8_EMULATOR_PLATFORM_H
