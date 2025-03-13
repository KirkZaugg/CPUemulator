#include"PPU.h"
#include"Register.h"

PPU::PPU(Register* ictrl, Register* ioamdma, PPUbus* ibus) {
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
}



void PPU::draw() {
    bool render = true;
    if(render) {
        
        if (line < 240) { //visible
            for (int i = 0; i < 341; i++) {
                if ((i < 257) || ((i > 320) && (i < 337))) {
                    if (i == 256) {               //y inc
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
                    int j = i % 8;
                    if (j == 0) {
                        currTile = midTile;
                        midTile = nextTile;
                        if (i==0) {} else {   //x inc
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
                } else if (i == 257){ //hori(v) = hori(t)
                    v &= ~0x001F;
                    v |= (t & 0x001f);
                }

            }

        } else if (line == 240) { //postrender
            
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
