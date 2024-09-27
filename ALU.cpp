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
    
}

void ALU::shift(bool direction) {

}