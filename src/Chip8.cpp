//
// Created by Bartosz Budnik on 04.06.2024.
//

#include "includes/Chip8.h"
#include <fstream>
#include <memory>

/* Starting point of ROM in memory */
const unsigned int START_MEMORY = 0x200;
/* Starting point of font in memory */
const unsigned int START_FONT_ADDRESS = 0x50;
/* Size of font in bytes */
const unsigned int FONT_SIZE = 80;

/* Initialize pc to ROM location */
Chip8::Chip8() : pc(START_MEMORY)
{
    /* Open the font file as binary */
    std::ifstream file("../extras/font.bin", std::ios::binary);

    /* If file is open, read it into the memory */
    if (file.is_open())
        file.read(reinterpret_cast<char*>(&memory[START_FONT_ADDRESS]), FONT_SIZE);
    /* On fail exit the process */
    else {
        printf("Error: Couldn't get the font!");
        exit(1);
    }
    
    file.close();
}

/* Load ROM from file and put it into memory */
void Chip8::LoadROM(const char *fileName) {
    /* Open the file in binary mode, go to the end */
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        /* Get size of the file, go back to the beginning */
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);
        /* Read the ROM into memory */
        file.read(reinterpret_cast<char*>(&memory[START_MEMORY]), size);
    }
}
