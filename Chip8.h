//
// Created by Bartosz Budnik on 04.06.2024.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <cstdint>

class Chip8
{
public:
    void LoadROM(const char* fileName);

public:
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t stack[16];
    uint16_t index;
    uint16_t pc;
    uint8_t sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint32_t video[64 * 32];
    uint8_t keys[16];
    uint16_t opcode;
};


#endif //CHIP8_EMULATOR_CHIP8_H
