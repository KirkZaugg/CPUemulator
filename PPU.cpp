#include"PPU.h"
#include"Register.h"
#include"Clock.h"

PPU::PPU(Register* ictrl, Register* ioamdma, PPUbus* ibus, bool imirror) {
    ctrl = ictrl;
    oamdma = ioamdma;
    bus = ibus;
    mirror = imirror? VERTICAL : HORIZONTAL;
}

void PPU::renderFrame() {
    for (int i = 0; i < 262; i++) {
        scanLine(i);
    }
}

void PPU::scanLine(int line) {
    bool render = true;
    if(render) {
        if (line < 240) { //visible

            for (int i = 0; i < 283; i++) {
                uint8_t name = fetchNametable();
            }

        } else if (line == 240) { //postrender
            Clock::pcycle(1);
        } else if (line < 261) { //vblank
            if (line == 241) {
                //set NMI
            }
        } else {

        }
    }
    spriteEval();
}

void PPU::spriteEval() {
    for(int i = 0; i < 32; i++) { oam2[i] = 0xff; } //clear secondary OAM
    int o = 0; //oam2 index
    int n = 0;
    while(n < 64) {
        if (oam[n][0] == 0/*idk*/) {
            oam2[o] = oam[n][0];
            o++;
            oam2[o] = oam[n][1];
            o++;
            oam2[o] = oam[n][2];
            o++;
            oam2[o] = oam[n][3];
        }
        n++;
        if (n == 64) {
            
        }
    }

}

uint8_t PPU::fetchNametable(uint16_t location) {
    if (mirror == VERTICAL) {
        location&=0x700; //000 == 800; 400 == c00;
        if (location < 0x400) {
            return nametableA[location];
        } else {
            return nametableB[location - 0x400];
        }
    } else {
        location&=0xd00; //000 == 400; 800 == c00;
        if (location < 0x400) {
            return nametableA[location];
        } else {
            return nametableB[location - 0x800];
        }
    }
}
