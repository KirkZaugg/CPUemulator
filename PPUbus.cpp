#include"PPUbus.h"
#include<cstdint>
#include<iostream>

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