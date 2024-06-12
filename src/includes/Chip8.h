//
// Created by Bartosz Budnik on 04.06.2024.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <cstdint>
#include <chrono>
#include <random>

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

class Chip8
{
public:
    Chip8();

    bool LoadROM(const char* fileName);
    void Cycle();

private:
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
    void OP_NULL();

    void Table0();
    void Table8();
    void TableE();
    void TableF();

public:
    uint32_t video[64 * 32] { };
    uint8_t keys[16] { };

private:
    std::default_random_engine randEng;
    std::uniform_int_distribution<uint8_t> rand;

    uint8_t memory[4096] { };
    uint8_t registers[16] { };
    uint16_t stack[16] { };
    uint16_t index = 0;
    uint16_t pc;
    uint8_t sp = 0;
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;
    uint16_t opcode = 0;

    typedef void (Chip8::*Chip8Func)();

    Chip8Func table[0xF + 1] { };
    Chip8Func table0[0xE + 1] { };
    Chip8Func table8[0xE + 1] { };
    Chip8Func tableE[0xE + 1] { };
    Chip8Func tableF[0x65 + 1] { };
};


#endif //CHIP8_EMULATOR_CHIP8_H
