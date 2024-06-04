//
// Created by Bartosz Budnik on 04.06.2024.
//

#include "Chip8.h"
#include <fstream>
#include <memory>

/* Starting point of ROM */
const unsigned int START_MEMORY = 0x200;

/* Load ROM from file and put it into memory */
void Chip8::LoadROM(const char *fileName) {
    /* Open the file in binary mode, go to the end */
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        /* Get size of the file, go back to the beginning */
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);

        file.read(reinterpret_cast<char*>(&memory[START_MEMORY]), size);
    }
}
