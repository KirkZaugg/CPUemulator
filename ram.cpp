#include<fstream>
#include<iostream>
#include"ram.h"

uint8_t RAM::address(uint16_t location, bool write) {

    if (mapper == 0) {
        int index;
        if (location < 0x2000) {
            index = location & 0x7ff;
            if (write) {
                cpuRam[index] = writeValue;
                return 0;
            } else {
                return cpuRam[index];
            }
        } else if (location < 0x4000) {
            index = location & 0x7;
            if (write) {
                (PPUctrl + index)->setValue(writeValue);
                return 0;
            } else {
                return (PPUctrl + index)->getValue();
            }
        } else if (location < 0x4020){
            index = location & 0x1f;
            if (write) {
                (miscReg + index)->setValue(writeValue);
                return 0;
            } else {
                return (miscReg + index)->getValue();
            }
        } else if (location < 0x6000) {
            std::cout << "\nMemory Access error: $4020-5fff is unmapped\n";
            return 0;
        } else if (location < 0x8000) {
            //cartridge RAM???
        } else {
            index = location - 0x8000;
            return readROM(index);
        }

    }
    std::cout << "Huh\n";
    return 0;
}

uint8_t RAM::readROM(uint16_t location) {
    std::ifstream inputFileStream(file, std::ios::in | std::ios::binary);
    inputFileStream.seekg(location, std::ios::beg);
    char input;
    inputFileStream.read(&input, 1);
    inputFileStream.close();
    uint8_t ret = reinterpret_cast<uint8_t&>(input);
    return ret;
}

uint8_t RAM::getValue(uint16_t location) {

    return address(location, 0);

    
}

void RAM::setValue(uint16_t location, uint8_t value) {

    writeValue = value;
    address(location, 1);

    std::ofstream outputFileStream(file, std::ios::in | std::ios::out | std::ios::binary);
    outputFileStream.seekp(location, std::ios::beg);
    char writeval = value;
    outputFileStream.write(&writeval, 1);
    outputFileStream.close();
}

void RAM::setMapper(int imapper) {
    mapper = imapper;
}

RAM::RAM(std::string filename, Register* PPU, Register* misc) {
    file = filename;
    mapper = 0;

    PPUctrl = PPU;
    miscReg = misc;
}