#pragma once
#include"PPUbus.h"
#include"Register.h"
#include "Interface.h"

class PPU {
private:
    PPUbus* bus;

    uint16_t v;
    uint16_t t;
    uint8_t x;
    bool w;

    Register* ppuctrl;
    Register* ppumask;
    Register* ppustatus;
    Register* oamaddr;
    Register* oamdata;
    Register* ppuscroll;
    Register* ppuaddr;
    Register* ppudata;

    uint8_t oam[64][4];
    uint8_t oam2[32];
    Register* oamdma;
    void spriteEval();

    int dot;
    int line;

    uint8_t nametableA[0x400];
    uint8_t nametableB[0x400];
    enum mirror {
        VERTICAL,
        HORIZONTAL
    }; mirror mirror;
    uint8_t fetchNametable(uint16_t location);

    struct Tile {
        uint8_t name;
        uint8_t attribute;
        uint8_t patt_msb;
        uint8_t patt_lsb;
    };
    Tile currTile;
    Tile midTile;
    Tile nextTile;

    uint8_t screen[256][240];

    Interface* out;

    void frameOut();

public:
    PPU(Register* ictrl, Register* ioamdma, PPUbus* ibus, Interface* iout);
    void draw();
    

};