#include<iostream>
#include"Register.h"
#include"largeRegister.h"
#include"ProgramCounter.h"
#include"ALU.h"
#include"ram.h"
#include"Control.h"
#include"StatusRegister.h"

int main() {
    Register a;
    Register x;
    Register y;
    Register s;
    Register sp;
    sp.setValue(0xff);
    StatusRegister f;
    f.setValue(0b00110000); //set inoperable bits

    char bus;
    ALU alu(&a, &x, &y);
    RAM ram;

    ProgramCounter p;
    p.setWholeValue(0);

    Control control(&alu, &a, &x, &y, &ram, &s, &f, &p, &sp);

    while (p.getWholeValue() < 0x1f) {
        control.operate();
    }
    std::cout << a.getValue() << "\n";
    
}