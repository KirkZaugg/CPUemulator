#include"PPU.h"
#include"Register.h"
#include<iostream>

PPU::PPU(Register* ictrl, Register* ioamdma, PPUbus* ibus, Interface* iout, bool* inmi) {
    ppuctrl = ictrl;
    ppumask = ictrl + 1;
    ppustatus = ictrl + 2;
    oamaddr = ictrl + 3;
    oamdata = ictrl + 4;
    ppuscroll = ictrl + 5;
    ppuaddr = ictrl + 6;
    ppudata = ictrl + 7;
    
    oamdma = ioamdma;
    bus = ibus;

    x = 0;
    line = 0;
    dot = 0;

    out = iout;

    nmi = inmi;
}



void PPU::draw() {
    bool render = true;
    if(render) {
        
        if (line < 240) { //visible
            
            if ((dot < 257) || ((dot > 320) && (dot < 337))) {
                if (dot == 256) {               //y inc
                    if ((v & 0x7000) == 0x7000) {
                        v &= ~0x7000;
                        uint8_t cy = (v & 0x03E0) >> 5;
                        if (cy == 29) {
                            cy = 0;
                            v ^= 0x0800;
                        } else if (cy == 31) {
                            cy == 0;
                        } else {
                            cy++;
                        }
                        v = (v & ~0x03E0) | (cy << 5);
                    } else {
                        v += 0x1000;
                    }
                }
                int j = dot % 8;
                if (j == 0) {
                    currTile = midTile;
                    midTile = nextTile;
                    if (dot==0) {} else {   //x inc
                        if ((v & 0x001f) == 31) {
                            v &= ~0x001f;
                            v ^= 0x400;
                        } else {
                            v++;
                        }
                    }
                } else if (j == 1) {
                    nextTile.name = fetchNametable(v & 0xfff);
                } else if (j == 3) {
                    nextTile.attribute = fetchNametable(0x3C0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07));
                } else if (j == 5) {
                    bool side = (ppuctrl->getValue() >> 4) & 1;
                    uint8_t finey = (v & 0xf00) >> 12;
                    uint16_t location = ((side << 12) + (nextTile.name << 4) + finey) & 0xfff7;
                    uint8_t lsb = bus->read(location);
                    uint8_t msb = bus->read(location | 0b1000);
                    nextTile.patt_lsb = lsb;
                    nextTile.patt_msb = msb;
                }
            } else if (dot == 257){ //hori(v) = hori(t)
                v &= ~0x001F;
                v |= (t & 0x001f);
            }

            if (dot < 256) {
                uint8_t* curpix = &screen[dot][line];
                int pixVal = (((currTile.patt_lsb >> x) & 1) << 1) + ((currTile.patt_msb >> x) & 1);
                bool xtile = (v & 0x1f) % 2;
                bool ytile = ((v & 0x03e0) >> 5) % 2;
                int palNum = (currTile.attribute >> (4 * ytile)) >> (2 * xtile);
                bool select = 0; //just render the background for now
                uint16_t palAddr = ((select << 4) + (palNum << 2) + pixVal) + 0x3f00;
                *curpix = bus->read(palAddr);
            }

        } else if (line == 240) { //postrender
            
        } else if (line < 261) { //vblank
            if (line == 241) {
                if (dot == 1) {ppustatus->setValue(ppustatus->getValue() | 0x80); *nmi = true; } //set Vblank and NMI
                

                frameOut();

            }
        } else {

        }

        if (dot == 340) {
            dot = 0;
            if (line == 261) {
                line = 0;
            } else {
                line++;
            }
        } else {
            dot++;
            if (x == 7) {
                x = 0;
            } else {
                x++;
            }
        }
    }
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

void PPU::frameOut() {
    uint8_t color;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 240; j++) {
            color = screen[i][j];
            out->drawPix(i, j, color);
        }
    }
}