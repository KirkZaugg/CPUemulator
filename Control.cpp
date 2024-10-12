#include<iostream>
#include "Control.h"
#include "Register.h"
#include"ALU.h"
#include"ram.h"
#include"ProgramCounter.h"

Control::Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, Register* inf, ProgramCounter* inpc) {
    alu = inALU;
    a = ina;
    x = inx;
    y = iny;
    s = ins;
    f = inf;
    ram = inRam;
    pc = inpc;
}

char Control::address(char mode) {
    switch (mode) {
        case AB:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8)));
            break;
        case ABX:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8) + x->getValue()));
            break;
        case ABY:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8) + y->getValue()));
            break;
        case I:
            pc->inc();
            return (ram->getValue(pc->getWholeValue()));
            break;
        case ZP:
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue())));
            break;
        case ZPX:
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + x->getValue()));
            break;
    }

    return 0;
}

void Control::operate() {

    bool run = true;

    while(run) {
    
        inputreg.setValue(ram->getValue(pc->getWholeValue()));

        const char LDA = 0xa1;

        char opcode = inputreg.getValue() & 0b11100011;
        char aMode = (inputreg.getValue() & 0b00011100) >> 2;
        switch(opcode) {
            case LDA:
                a->setValue(address(aMode));
                break;
        }

        pc->inc();
    }
}
