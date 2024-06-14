#include <thread>
#include <iostream>
#include <zconf.h>
#include "includes/Chip8.h"
#include "includes/Platform.h"

int main(int argc, char* args[]) {
    /* Video scale factor */
    int scale = 10;

    /* Time delay */
    int delay = 1858;

    /* Check if there is correct number of arguments, if not tell the user */
    if (argc <= 1) {
        std::cout << "Path to ROM need to be specified as an argument, "
                     "see --help for usage information" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    /* Check if the user asks for help */
    if (strcmp(args[1], "--help") == 0) {
        std::cout << "Normal usage Chip8_Emulator <ROM>\n"
               "Flags:\n"
               "1: -d <value> for custom delay(default: 1500)\n"
               "2: -s <value> for custom video scale(default: 10)\n" << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    /* ROM file name */
    const char* rom = args[1];

    /* Load ROM into emulator */
    Chip8 emu;
    if (!emu.LoadROM(rom)) {
        printf("ERROR: ROM couldn't be read!\n");
        std::exit(EXIT_FAILURE);
    }

    /* Flags are specified */
    if (argc > 2) {
        /* Go through each argument not counting the ROM */
        for (int i = 2; i < argc; i++) {
            /* If -d flag is called, set the delay value */
            if (strcmp("-d", args[i]) == 0) {
                if (argc > i++) {
                    delay = atoi(args[i]);
                }
                else {
                    printf("Delay value wasn't specified!");
                    std::exit(EXIT_FAILURE);
                }
            }

            /* If -s flag is called, set the scale value */
            else if (strcmp("-s", args[i]) == 0) {
                if (argc > i++) {
                    scale = atoi(args[i]);
                }
                else {
                    printf("Scale value wasn't specified!");
                    std::exit(EXIT_FAILURE);
                }
            }
        }
    }

    /* Initialize platform (I/O processing) */
    Platform platform("CHIP8", static_cast<int>(VIDEO_WIDTH * scale),
                      static_cast<int>(VIDEO_HEIGHT * scale), VIDEO_WIDTH, VIDEO_HEIGHT);

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
        if (emu.drawFlag) {
            platform.Update(emu.video, pitch);
            emu.drawFlag = false;
        }

        /* Sleep for delay time */
        usleep(delay); /* usleep is more precise than sleep_for */
        //std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    return 0;
}