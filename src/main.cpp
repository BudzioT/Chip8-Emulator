#include <iostream>
#include <chrono>
#include <thread>
#include "includes/Chip8.h"
#include "includes/Platform.h"

int main(int argc, char* args[]) {
    /* Check if there is correct number of arguments */
    if (argc != 3) {
        std::cerr << "Proper usage: " << args[0] << "<Scale> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    /* Video scale factor */
    int scale = std::stoi(args[1]);

    /* Time delay */
    int delay = 2000;

    /* ROM file name */
    const char* rom = args[2];

    /* Initialize platform (I/O processing) */
    Platform platform("CHIP8", static_cast<int>(VIDEO_WIDTH * scale),
                      static_cast<int>(VIDEO_HEIGHT * scale), VIDEO_WIDTH, VIDEO_HEIGHT);

    /* Load ROM into emulator */
    Chip8 emu;
    emu.LoadROM(rom);

    /* Set pitch of output */
    int pitch = sizeof(emu.video[0]) * VIDEO_WIDTH;

    /* Prepare last cycle time */
    auto lastTime = std::chrono::high_resolution_clock ::now();

    /* Quit flag */
    bool quit = false;
    /* Emulate until quit is requested */
    while (!quit) {
        /* Process keys, check if the user wants to quit */
        quit = Platform::ProcessInput(emu.keys);

        /* Emulate current opcode */
        emu.Cycle();

        /* Update output based on set pixels in emulator */
        platform.Update(emu.video, pitch);

        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    return 0;
}