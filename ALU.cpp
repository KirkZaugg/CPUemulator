#include"ALU.h"
#include"Register.h"

ALU::ALU(Register* ina, Register* inx, Register* iny) {
    a = ina;
    x = inx;
    y = iny;
}

void ALU::add() {
    a->setValue(x->getValue() + y->getValue());
}

void ALU::subtract() {
    a->setValue(x->getValue() - y->getValue());
}

void ALU::logic(int logicType) {   //0 = AND; 1 = OR; 2 = XOR; 3 = SHIFT L; 4 = SHIFT R; 
    switch (logicType) {
    case 0:
        a->setValue(x->getValue() & y->getValue());
        break;
    case 1:
        a->setValue(x->getValue() | y->getValue());
        break;
    case 2:
        a->setValue(x->getValue() ^ y->getValue());
        break;
    case 3:
        a->setValue(x->getValue() << y->getValue());
        break;
    case 4:
        a->setValue(x->getValue() >> y->getValue());
        break;
    default:
        break;
    }
}