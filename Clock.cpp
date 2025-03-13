#include"Clock.h"
#include"PPU.h"

Clock::Clock(PPU* ippu) {
    ppu = ippu;
}

void Clock::cycle(int cycles) {
    pcycle(3);
}

void Clock::pcycle(int cycles) {
    for (int i = 0; i < cycles; i++) {
        ppu->draw();
    }
}