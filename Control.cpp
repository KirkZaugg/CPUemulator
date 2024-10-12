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

        

        char aaa = (inputreg.getValue() & 0b11100000) >> 5;
        char bbb = (inputreg.getValue() & 0b00011100) >> 2; //addressing mode
        char cc = inputreg.getValue() & 0b00000011;

        switch (cc) {
            case 0b01:
                switch (aaa) {
                    case 0b000:  //ORA
                    case 0b001:  //AND
                    case 0b010:  //EOR
                    case 0b011:  //ADC
                    case 0b100:  //STA
                    case 0b101:  //LDA
                        a->setValue(address(bbb));
                        break;
                    case 0b110:  //CMP
                    case 0b111:  //SBC
                }
                break;
            case 0b10:
                switch (bbb) {
                    case 0b000:
                        bbb = I;
                        break;
                    case 0b010:
                        bbb = A;
                        break;
                }
                switch (aaa) {
                    case 0b000:  //ASL
                    case 0b001:  //ROL
                    case 0b010:  //LSR
                    case 0b011:  //ROR
                    case 0b100:  //STX
                        if(bbb == 0b101) {bbb = ZPY;}
                        break;
                    case 0b101:  //LDX
                        if(bbb == 0b101) {bbb = ZPY;}
                        if(bbb == 0b111) {bbb = ABY;}
                        break;
                    case 0b110:  //DEC
                    case 0b111:  //INC
                }
                break;
            case 0b00:
                if (bbb == 000) {bbb = I;}
                switch (aaa) {
                    case 0b001:  //BIT
                    case 0b010:  //JMP
                    case 0b011:  //JMP (abs)
                    case 0b100:  //STY
                    case 0b101:  //LDY
                    case 0b110:  //CPY
                    case 0b111:  //CPX
                }
                break;
        }

        pc->inc();
    }
}
