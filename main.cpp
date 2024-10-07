#include<iostream>
#include"Register.h"
#include"largeRegister.h"
#include"ProgramCounter.h"
#include"ALU.h"

int main() {
    Register a;
    Register x;
    Register y;
    char bus;
    ALU alu(&a, &x, &y);

    ProgramCounter f;

    
    
    f.setWholeValue(0x41ff);
    std::cout << 0x41 << "\n";
    int r = f.getBigValue();
    std::cout << r << "\n";
    std::cout << ((f.getWholeValue() & 0xff00) >> 8) << "\n";
    std::cout << 0xff << "\n";
    r = f.getValue();
    std::cout << r << "\n";
    std::cout << (f.getWholeValue() & 0xff) << "\n";
}