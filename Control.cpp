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

char Control::address(Addmode mode) {
    switch (mode) {
        case AB:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8)));
            break;
    }

    return 0;
}

void Control::operate() {

    bool run = true;

    while(run) {
    
        inputreg.setValue(ram->getValue(pc->getWholeValue()));

        const char JMP = 0x11;
        const char NOOP = 0x01;
        const char LDAi = 0xa9;
        const char LDAz = 0xa5;
        const char LDAzx = 0xb5;
        const char LDAa = 0xad;
        const char STP = 0x00;

        switch (inputreg.getValue()) {
            case JMP:
                pc->inc();
                extra.setValue(ram->getValue(pc->getWholeValue()));
                pc->inc();
                pc->setBigValue(ram->getValue(pc->getWholeValue()));
                pc->setValue(extra.getValue());
                break;
            case NOOP:
                std::cout << "w\n";
                break;
            case LDAi:
                pc->inc();
                a->setValue(ram->getValue(pc->getWholeValue()));
                break;
            case LDAz:
                pc->inc();
                a->setValue(ram->getValue(ram->getValue(pc->getWholeValue())));
                break;
            case LDAzx:
                pc->inc();
                a->setValue(ram->getValue(ram->getValue(pc->getWholeValue()) + x->getValue()));
                break;
            case LDAa:
                a->setValue(address(AB));
                break;
            case STP:
                std::cout << "Stopping operations...";
                run = false;
                break;
        
        }

        pc->inc();
    }
}
