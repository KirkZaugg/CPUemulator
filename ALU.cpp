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

void ALU::shift(bool direction) {
    char newValue = direction ? (a->getValue() << 1) : (a->getValue() >> 1);
    a->setValue(newValue);
}

void ALU::logic(int logicType) {

}