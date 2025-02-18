#include<iostream>
#include"Register.h"
#include"largeRegister.h"
#include"ProgramCounter.h"
#include"ALU.h"
#include"ram.h"
#include"Control.h"
#include"StatusRegister.h"
#include"PPUbus.h"
#include"PPU.h"

int main() {
    Register a;
    Register x;
    Register y;
    Register s;
    Register sp;
    sp.setValue(0xff);
    StatusRegister f;
    f.setValue(0b00110000); //set inoperable bits

    Register PPUctrl[8];

    uint8_t bus;
    ALU alu(&a, &x, &y);

    const std::string filename = "Super_mario_brothers.nes";
    RAM ram(filename);
    PPUbus pbus(filename);

    ProgramCounter p;
    p.setWholeValue(0);

    Control control(&alu, &a, &x, &y, &ram, &s, &f, &p, &sp);

    control.reset();
    for (int i = 0; i < 20; i++) {
        int addr = p.getWholeValue();
        int val = ram.getValue(p.getWholeValue());
        std::cout << "\naddr:" << std::hex << addr << "    val:" << std::hex << val << "    ";
        control.operate();
    }
    
}