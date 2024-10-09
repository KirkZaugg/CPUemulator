#include<iostream>
#include "Control.h"
#include "Register.h"
#include"ALU.h"
#include"ram.h"
#include"ProgramCounter.h"

Control::Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, ProgramCounter* inpc) {
    alu = inALU;
    a = ina;
    x = inx;
    y = iny;
    ram = inRam;
    pc = inpc;
}

void Control::operate() {
    
    inputreg.setValue(ram->getValue(pc->getWholeValue()));

    switch (inputreg.getValue()) {
        case 0x11:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            pc->setBigValue(ram->getValue(pc->getWholeValue()));
            pc->setValue(extra.getValue());
            break;
        case 0x01:
            std::cout << "w\n";
            break;
        
    }

}




void Control::jump() {
    
}