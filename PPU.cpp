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
    checkRegisters();
    bool render = (renderBackground | renderSprites);


    if ((line < 240) && render) { //visible
        
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
            v &= ~0x041F;
            v |= (t & 0x041f);
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
            if (dot == 1) {
                if (nmienable) { //set Vblank and NMI
                    ppustatus->setValue(ppustatus->getValue() | 0x80); *nmi = true; 
                }
            }
            

            frameOut();

        }
    } else { //prerender
        if ((dot > 279) && (dot < 305) && render) {
            v &= 0x041F;
            v |= (t & ~0x041f);
        }
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
        return bus->read(location | 0x2000);
        /*if (location < 0x400) {
            return nametableA[location];
        } else {
            return nametableB[location - 0x400];
        }*/
    } else {
        location&=0xd00; //000 == 400; 800 == c00;
        return bus->read(location | 0x2000);
        /*if (location < 0x400) {
            return nametableA[location];
        } else {
            return nametableB[location - 0x800];
        }*/
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
    out->PresentFrame();
}

void PPU::checkRegisters() {
    bool c;
    c = ppuctrl->getWrite();
    if(c) {
        uint8_t val = ppuctrl->getValue();
        t = (t & 0x73ff) | ((val & 0b11) << 10);
        nmienable          = (val & 0b10000000);
        masterSlave        = (val & 0b01000000);
        spriteSize         = (val & 0b00100000);
        backgroundSelect   = (val & 0b00010000);
        spriteSelect       = (val & 0b00001000);
        incctrl            = (val & 0b00000100);
    }
    c = ppumask->getWrite();
    if(c) {
        uint8_t val = ppumask->getValue();
        renderBackground = (val & 0b00001000);
        renderSprites    = (val & 0b00010000);
    }
    c = ppustatus->getRead();
    if (c) {
        w = 0;
    }
    c = ppuscroll->getWrite();
    if (c) {
        if (!w) {
            t = (t & 0x7fe0) | (ppuscroll->getValue() >> 3);
            x = ppuscroll->getValue() & 0x7;
        } else {
            uint16_t ad = ppuscroll->getValue();
            ad = ((ad & 0x7) << 12) | ((ad & 0xf8) << 2);
            t = (t & 0x0c1f) | ad;
        }
        w^=1;
    }
    c = ppuaddr->getWrite();
    if(c) {
        uint8_t ad = ppuaddr->getValue();
        int adr = ad;
        std::cout << " " << w << " " << adr << " ";
        if (!w) {
            ad&=0b00111111;
            t = (ad << 8) | (t & 0xff); 
        } else {
            t = (t & 0xff00) | ad;
            v = t;
        }
        w^=1;
        ppuaddr->getWrite();
    }
    c = ppudata->getRead();
    bool d = ppudata->getWrite();
    if(c) {
        ppudata->setValue(bus->read(v));
        if (!d) {
            ppudata->getWrite();
        }
    }
    if(d) {
        bus->write(v, ppudata->getValue());
        if (!c) {
            ppudata->getRead();
        }
    }
    if (c || d) {
        std::cout << incctrl;
        if (incctrl) {
            v+=32;
            t+=32;
        } else {
            v++;
            t++;
        }
    }
}