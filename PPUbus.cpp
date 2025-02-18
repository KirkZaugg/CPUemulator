#include"PPUbus.h"
#include<cstdint>
#include<iostream>
#include<fstream>
#include<algorithm>

uint8_t* PPUbus::addr(uint16_t location) {
    uint8_t* target;
    if (location < 0x3f00) {
        target = &cart[location];
    } else if (location < 0x4000) {
        target = &palette[(location & 0xff) % 0x20];
    } else {
        std::cout << "PPU error: invalid address\n";
    }
    return target;
}

uint8_t PPUbus::read(uint16_t location) {
    uint8_t* target = addr(location);
    return *target;
}

void PPUbus::write(uint16_t location, uint8_t value) {
    uint8_t* target = addr(location);
    *target = value;
    return;
}

PPUbus::PPUbus(std::string filename) {
    std::ifstream inputFileStream(filename, std::ios::in | std::ios::binary);
    inputFileStream.seekg(0x8000, std::ios::beg);
    char charcart[0x3f00];
    inputFileStream.read(charcart, 0x3f00);

    for (int i = 0; i < 0x3f00; i++) {
        cart[i] = reinterpret_cast<uint8_t&>(charcart[i]);
    }

    char pal[0x20];
    inputFileStream.read(pal, 0x20);
    inputFileStream.close();

    for (int i = 0; i < 0x20; i++) {
        palette[i] = reinterpret_cast<uint8_t&>(pal[i]);
    }
}