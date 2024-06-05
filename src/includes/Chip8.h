//
// Created by Bartosz Budnik on 04.06.2024.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <cstdint>

class Chip8
{
public:
    Chip8();

    void LoadROM(const char* fileName);

public:
    uint8_t memory[4096] { };
    uint8_t registers[16] { };
    uint16_t stack[16] { };
    uint16_t index = 0;
    uint16_t pc;
    uint8_t sp = 0;
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;
    uint32_t video[64 * 32] { };
    uint8_t keys[16] { };
    uint16_t opcode = 0;
};


#endif //CHIP8_EMULATOR_CHIP8_H
