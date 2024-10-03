#pragma once
#include "Register.h"

class ALU {
private:
    Register* a;
    Register* x;
    Register* y;
public:
    ALU(Register* ina, Register* inx, Register* iny);
    void add();         
    void subtract();
    void logic(int logicType);     //0 = AND; 1 = OR; 2 = XOR; 3 = SHIFT L; 4 = SHIFT R; 
};