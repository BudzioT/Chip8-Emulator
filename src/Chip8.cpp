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

    /* Load opcodes into function table */
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1NNN;
    table[0x2] = &Chip8::OP_2NNN;
    table[0x3] = &Chip8::OP_3XNN;
    table[0x4] = &Chip8::OP_4XNN;
    table[0x5] = &Chip8::OP_5XY0;
    table[0x6] = &Chip8::OP_6XNN;
    table[0x7] = &Chip8::OP_7XNN;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9XY0;
    table[0xA] = &Chip8::OP_ANNN;
    table[0xB] = &Chip8::OP_BNNN;
    table[0xC] = &Chip8::OP_CXNN;
    table[0xD] = &Chip8::OP_DXYN;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    /* Load null opcodes into tables that depend on one digit */
    for (int i = 0; i <= 0xE; ++i) {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }

    /* Load implemented opcodes into table 0 */
    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    /* Load implemented opcodes into table 8 */
    table8[0x0] = &Chip8::OP_8XY0;
    table8[0x1] = &Chip8::OP_8XY1;
    table8[0x2] = &Chip8::OP_8XY2;
    table8[0x3] = &Chip8::OP_8XY3;
    table8[0x4] = &Chip8::OP_8XY4;
    table8[0x5] = &Chip8::OP_8XY5;
    table8[0x6] = &Chip8::OP_8XY6;
    table8[0x7] = &Chip8::OP_8XY7;
    table8[0xE] = &Chip8::OP_8XYE;

    /* Load implemented opcodes into table E */
    tableE[0x1] = &Chip8::OP_EXA1;
    tableE[0xE] = &Chip8::OP_EX9E;

    /* Load null opcodes into table F that depends on two digits */
    for (int i = 0; i <= 0x65; ++i) {
        tableF[i] = &Chip8::OP_NULL;
    }

    tableF[0x07] = &Chip8::OP_FX07;
    tableF[0x0A] = &Chip8::OP_FX0A;
    tableF[0x15] = &Chip8::OP_FX15;
    tableF[0x18] = &Chip8::OP_FX18;
    tableF[0x1E] = &Chip8::OP_FX1E;
    tableF[0x29] = &Chip8::OP_FX29;
    tableF[0x33] = &Chip8::OP_FX33;
    tableF[0x55] = &Chip8::OP_FX55;
    tableF[0x65] = &Chip8::OP_FX65;
}

/* Load ROM from file and put it into memory */
bool Chip8::LoadROM(const char *fileName) {
    /* Open the file in binary mode, go to the end */
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        /* Get size of the file, go back to the beginning */
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);
        /* Read the ROM into memory */
        file.read(reinterpret_cast<char*>(&memory[START_MEMORY]), size);
        file.close();
        return true;
    }
    file.close();
    return false;
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
    pc = opcode & 0x0FFF;
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

    registers[0xF] = registers[VX] & 0x1;
    registers[VX] >>= 1;
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

    registers[0xF] = (registers[VX] & 0x80) >> 7;
    registers[VX] <<= 1;
}

/* If register VX isn't equal to VY, skip the next instruction */
void Chip8::OP_9XY0() {
    if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
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
    registers[(opcode & 0x0F00) >> 8] = rand(randEng) & (opcode & 0x00FF);
}

/* Draw sprite at position VX, VY, with height of N, starting from index position.
 * If any pixels are changed to unset, change register VF to 1, otherwise set it to 0 */
void Chip8::OP_DXYN() {
    drawFlag = true;
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
                if (*displayPixel == 0xFFFFFFFF)
                    registers[0xF] = 1;

                *displayPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

/* If the key corresponding to value at register's VX is pressed, skip the next instruction */
void Chip8::OP_EX9E() {
    if (keys[registers[(opcode & 0x0F00) >> 8]])
        pc += 2;
}

/* If the key corresponding to value at register's VX isn't pressed, skip the next instruction */
void Chip8::OP_EXA1() {
    if (!keys[registers[(opcode & 0x0F00) >> 8]])
        pc += 2;
}

/* Set the register VX to the value of delay timer */
void Chip8::OP_FX07() {
    registers[(opcode & 0x0F00) >> 8] = delayTimer;
}

/* Wait for a keypress and store the key in register VX */
void Chip8::OP_FX0A() {
    /* Go through every key, if it is on, save it to register VX, else repeat this instruction */
    for (int i = 0; i < 16; i++) {
        if (keys[i]) {
            registers[(opcode & 0x0F00) >> 8] = i;
            return;
        }
    }

    /* Go back to this instruction */
    pc -= 2;
}

/* Set the delay timer to the value of register VX */
void Chip8::OP_FX15() {
    delayTimer = registers[(opcode & 0x0F00) >> 8];
}

/* Set the sound timer to the value of register VX */
void Chip8::OP_FX18() {
    soundTimer = registers[(opcode & 0x0F00) >> 8];
}

/* Add the value of register VX to the register I */
void Chip8::OP_FX1E() {
    index += registers[(opcode & 0x0F00) >> 8];
}

/* Set register I to the address of sprite corresponding to the hex value stored in register VX */
void Chip8::OP_FX29() {
    index = START_FONT_ADDRESS + (registers[(opcode & 0x0F00) >> 8]) * 5;
}

/* Store binary-coded decimal value of register VX at addresses I, I + 1 and I + 2 */
void Chip8::OP_FX33() {
    uint8_t val = registers[(opcode & 0x0F00) >> 8];

    /* Go through ones-place, tens-place and hundreds-place. Place them at the right location */
    for (int i = 2; i >= 0; i--, val /= 10)
        memory[index + i] = val % 10;
}

/* Store the value of registers V0 to VX inclusive in memory starting at address I.
 * Set I to I + X + 1 */
void Chip8::OP_FX55() {
    int x = ((opcode & 0x0F00) >> 8);

    /* Go through each register from V0 to VX inclusive, insert values from them into memory starting at I */
    for (uint8_t i = 0; i <= x; i++)
        memory[index + i] = registers[i];

    /* Set the register I */
    index = index + x + 1;
}

/* Store the value of memory starting at address I in registers ranging from V0 to VX inclusive.
 * Set I to I + X + 1 */
void Chip8::OP_FX65() {
    int x = ((opcode & 0x0F00) >> 8);

    /* Go through each register from V0 to VX inclusive,
     * set their values to memory addresses values starting at I */
    for (uint8_t i = 0; i <= x; i++)
        registers[i] = memory[index + i];
}

/* Do nothing, that Opcode isn't available */
void Chip8::OP_NULL()
{ }

/* Use right opcode method from table 0, based off of last digit */
void Chip8::Table0() {
    ((*this).*(table0[opcode & 0x000F]))();
}

/* Use right opcode method from table 8, based off of last digit */
void Chip8::Table8() {
    ((*this).*(table8[opcode & 0x000F]))();
}

/* Use right opcode method from table E, based off of last digit */
void Chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000F]))();
}

/* Use right opcode method from table F, based off of two last digits */
void Chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FF]))();
}

/* Fetch, decode and execute the instruction, move pc to the next one */
void Chip8::Cycle() {
    /* Fetch the opcode */
    opcode = (memory[pc] << 8) | memory[pc + 1];

    /* Move to the next instruction */
    pc += 2;

    /* Decode, execute the instruction based of the first digit */
    ((*this).*(table[(opcode & 0xF000) >> 12]))();

    /* If delay timer is on, decrement it */
    if (delayTimer > 0)
        --delayTimer;

    /* If sound timer is on, decrement it */
    if (soundTimer > 0)
        --soundTimer;
}
