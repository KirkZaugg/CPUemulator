#include"PPUbus.h"

class PPU {
private:
    PPUbus* bus;

    uint8_t oam[64][4];
    uint8_t oam2[32];
    Register* oamdma;
    void spriteEval();

    Register* ctrl;

    uint8_t nametableA[0x400];
    uint8_t nametableB[0x400];
    enum mirror {
        VERTICAL,
        HORIZONTAL
    }; mirror mirror;
    uint8_t fetchNametable(uint16_t location);

    uint8_t screen[256][240];
    void scanLine(int line);
public:
    PPU(Register* ictrl, Register* ioamdma, PPUbus* ibus, bool imirror);
    void renderFrame();
    

};