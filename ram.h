#pragma once
#include"Register.h"

class RAM {
private:

    Register* PPUctrl[8];
    Register* miscReg[0x20];

    uint8_t cpuRam[0x800];

    uint8_t writeValue;
    int mapper;
    std::string file;
    uint8_t address(uint16_t location, bool write);
    uint8_t readROM(uint16_t location);
public:
    uint8_t getValue(uint16_t location);
    void setValue(uint16_t location, uint8_t value);
    void setMapper(int imapper);
    RAM();
};