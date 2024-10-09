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
    char bus;
    ALU alu(&a, &x, &y);
    RAM ram;

    ProgramCounter f;
    f.setWholeValue(0);

    Control control(&alu, &a, &x, &y, &ram, &f);

    control.operate();
    control.operate();
    
}