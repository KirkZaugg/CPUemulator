#include<iostream>
#include"Register.h"
#include"largeRegister.h"
#include"ProgramCounter.h"
#include"ALU.h"
#include"ram.h"
#include"Control.h"

int main() {
    Register a;
    Register x;
    Register y;
    Register s;
    Register f;
    char bus;
    ALU alu(&a, &x, &y);
    RAM ram;

    ProgramCounter p;
    p.setWholeValue(0);

    Control control(&alu, &a, &x, &y, &ram, &s, &f, &p);

    control.operate();
    control.operate();
    
}