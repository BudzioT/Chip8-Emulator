//
// Created by Bartosz Budnik on 04.06.2024.
//

#include "includes/Chip8.h"
#include <fstream>
#include <memory>
#include <cstring>

/* Starting point of ROM in memory */
const unsigned int START_MEMORY = 0x200;
/* Starting point of font in memory */
const unsigned int START_FONT_ADDRESS = 0x50;
/* Size of font in bytes */
const unsigned int FONT_SIZE = 80;

/* Initialize Chip8 emulator */
Chip8::Chip8() : pc(START_MEMORY), randEng(std::chrono::system_clock::now().time_since_epoch().count()),
    rand(std::uniform_int_distribution<uint8_t>(0, 255))
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

/* Clear the screen */
void Chip8::OP_00E0() {
    /* Set all video bits to 0 */
    memset(video, 0, sizeof(video));
}

/* Return from subroutine */
void Chip8::OP_00EE() {
    pc = stack[--sp];
}

/* Jump to the given address */
void Chip8::OP_1NNN() {
    pc = opcode & 0x0FFF;
}

/* Call the subroutine at the given address */
void Chip8::OP_2NNN() {
    stack[sp++] = pc;
    pc = opcode & 0xFFF;
}

/* Skip the next instruction if register VX is equal to the given NN byte */
void Chip8::OP_3XNN() {
    if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        pc += 2;
}

/* Skip the next instruction if register VX isn't equal to the given NN byte  */
void Chip8::OP_4XNN() {
    if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        pc += 2;
}

/* Skip the next instruction if register VX is equal to the register VY */
void Chip8::OP_5XY0() {
    if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
        pc += 2;
}

/* Store NN into register VX */
void Chip8::OP_6XNN() {
    registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
}

/* Add NN to the register VX */
void Chip8::OP_7XNN() {
    registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
}

/* Store the value of register VY into register VX */
void Chip8::OP_8XY0() {
    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
}

/* Set register VX to the result of VX OR VY */
void Chip8::OP_8XY1() {
    registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
}

/* Set register VX to the result of VX AND VY */
void Chip8::OP_8XY2() {
    registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
}

/* Set register VX to the result of VX XOR VY */
void Chip8::OP_8XY3() {
    registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
}

/* Add value of register VY to VX, if there is a carry,
 * set the VF register to 1, otherwise set it to 0 */
void Chip8::OP_8XY4() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint16_t sum = registers[VX] + registers[(opcode & 0x00F0) >> 4];

    if (sum > 255)
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[VX] = sum & 0xFF;
}

/* Subtract value of register VY from VX, if there is a borrow,
 * set the VF register to 0, otherwise set it to 1 */
void Chip8::OP_8XY5() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint8_t VY = (opcode & 0x00F0) >> 4;
    if (registers[VX] < registers[VY])
        registers[0xF] = 0;
    else
        registers[0xF] = 1;

    registers[VX] -= registers[VY];
}

/* Set register VX to value of VY shifted by 1 bit to the right,
 * set register VF to the least significant bit of VX prior to the change */
void Chip8::OP_8XY6() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint8_t VY = (opcode & 0x00F0) >> 4;

    registers[0xF] = registers[VY] & 0x1;
    registers[VX] = registers[VY] >> 1;
}

/* Set register VX to the value of VY subtracted by VX. If there was a borrow,
 * set register VF to 0, otherwise set it to 1 */
void Chip8::OP_8XY7() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint8_t VY = (opcode & 0x00F0) >> 4;

    if (registers[VY] < registers[VX])
        registers[0xF] = 0;
    else
        registers[0xF] = 1;

    registers[VX] = registers[VY] - registers[VX];
}

/* Set register VX to VY shifted left by 1,
 * set the VF register to the most significant bit of VX prior to the change */
void Chip8::OP_8XYE() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint8_t VY = (opcode & 0x00F0) >> 4;

    registers[0xF] = (registers[VY] & 0x80) >> 7;
    registers[VX] = registers[VY] << 1;
}

/* If register VX isn't equal to VY, skip the next instruction */
void Chip8::OP_9XY0() {
    if (registers[(opcode & 0x0F00)] != registers[(opcode & 0x00F0)])
        pc += 2;
}

/* Store the given address into index register */
void Chip8::OP_ANNN() {
    index = opcode & 0x0FFF;
}

/* Jump to the given address added to register V0 */
void Chip8::OP_BNNN() {
    pc = (opcode & 0x0FFF) + registers[0x0];
}

/* Set register VX to random number with NN as mask */
void Chip8::OP_CXNN() {
    registers[opcode & 0x0F00] = rand(randEng) & (opcode & 0x00FF);
}

/* Draw sprite at position VX, VY, with height of N, starting from index position.
 * If any pixels are changed to unset, change register VF to 1, otherwise set it to 0 */
void Chip8::OP_DXYN() {
    uint8_t VX = (opcode & 0x0F00) >> 8;
    uint8_t VY = (opcode & 0x00F0) >> 4;

    /* Height of sprite */
    uint8_t bytes = opcode & 0x000F;

    /* Display positions */
    uint8_t xPos = registers[VX] % VIDEO_WIDTH;
    uint8_t yPos = registers[VY] % VIDEO_HEIGHT;

    /* Set VF to 0 */
    registers[0xF] = 0;

    /* Run through all the rows */
    for (unsigned int row = 0; row < bytes; ++row) {
        /* Sprite byte, stored at memory indicated by index, increases with row */
        uint8_t spriteByte = memory[index + row];

        /* Run through every column (sprite width is always 8) */
        for (unsigned int col = 0; col < 8; ++col) {
            /* Get sprite pixel bit by shifting bits based of a column */
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            /* Get display pixel from position + row and columns, multiplied by video width
             * (needed for indexing, because it is one-dimensional array) */
            uint32_t* displayPixel = &video[(xPos + col) + (yPos + row) * VIDEO_WIDTH];

            /* If sprite pixel is on, check if the display pixel is also on, if so, set VF to 1,
             * then XOR the video pixel with sprite pixel (which is on) */
            if (spritePixel) {
                if (*displayPixel == UINT32_MAX)
                    registers[0xF] = 1;

                *displayPixel ^= UINT32_MAX;
            }
        }
    }
}

void Chip8::OP_EX9E() {

}

void Chip8::OP_EXA1() {

}

void Chip8::OP_FX07() {

}

void Chip8::OP_FX0A() {

}

void Chip8::OP_FX15() {

}

void Chip8::OP_FX18() {

}

void Chip8::OP_FX1E() {

}

void Chip8::OP_FX29() {

}

void Chip8::OP_FX33() {

}

void Chip8::OP_FX55() {

}

void Chip8::OP_FX65() {

}

