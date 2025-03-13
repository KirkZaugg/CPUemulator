#pragma once
#include"PPU.h"

class Clock {
private:
    PPU* ppu;
public:
    Clock(PPU* ippu);
    void cycle(int cycles);
    void pcycle(int cycles);
};