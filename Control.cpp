#include<iostream>
#include "Control.h"
#include "Register.h"
#include"ALU.h"
#include"ram.h"
#include"ProgramCounter.h"
#include"StatusRegister.h"

Control::Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, StatusRegister* inf, ProgramCounter* inpc) {
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

void Control::address(char mode, char inValue) {
    switch (mode) {
        case AB:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8), inValue);
            break;
        case ABX:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8) + x->getValue(), inValue);
            break;
        case ABY:
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()) + (extra.getValue() << 8) + y->getValue(), inValue);
            break;
        case I:
            pc->inc();
            ram->setValue(pc->getWholeValue(), inValue);
            break;
        case ZP:
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()), inValue);
            break;
        case ZPX:
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()) + x->getValue(), inValue);
            break;
    }
}

void Control::operate() {

    bool run = true;

    while(run) {
    
        inputreg.setValue(ram->getValue(pc->getWholeValue()));

        switch (inputreg.getValue()) {
            case 0x00: //BRK
            case 0x20: //JSR abs
            case 0x40: //RTI
            case 0x60: //RTS
            case 0x08: //PHP
            case 0x28: //PLP
            case 0x48: //PHA
            case 0x68: //PLA
            case 0x88: //DEY
            case 0xA8: //TAY
            case 0xC8: //INY
            case 0xE8: //INX
            case 0x18: //CLC
            case 0x38: //SEC
            case 0x58: //CLI
            case 0x78: //SEI
            case 0x98: //TYA
            case 0xB8: //CLV
            case 0xD8: //CLD
            case 0xF8: //SED
            case 0x8A: //TXA
            case 0x9A: //TXS
            case 0xAA: //TAX
            case 0xBA: //TSX
            case 0xCA: //DEX
            case 0xEA: //NOP
            default:
                if ((inputreg.getValue() & 0b00011111) == 0b10000) {
                    char xstuff = inputreg.getValue() >> 6;
                    bool ystuff = (inputreg.getValue() & 0b00100000) >> 5;

                    bool status;

                    switch (xstuff) {
                        case 0b00: //negative
                            status = inputreg.getValue() >> 7;
                        case 0b01: //overflow
                            status = (inputreg.getValue() >> 6) & 1;
                        case 0b10: //carry
                            status = inputreg.getValue() & 1;
                        case 0b11: //zero
                            status = (inputreg.getValue() >> 1) & 1;
                    }

                    if (status == ystuff) {
                        //BRANCH
                    }
                }

                

                char aaa = (inputreg.getValue() & 0b11100000) >> 5;
                char bbb = (inputreg.getValue() & 0b00011100) >> 2; //addressing mode
                char cc = inputreg.getValue() & 0b00000011;

                switch (cc) {
                    case 0b01:
                        switch (aaa) {
                            case 0b000:  //ORA
                                a->setValue(address(bbb) | a->getValue());
                                if(a->getValue() < 0) {f->setNegative(1);}
                                if(a->getValue() == 0) {f->setZero(1);}
                            case 0b001:  //AND
                                a->setValue(address(bbb) & a->getValue());
                                if(a->getValue() < 0) {f->setNegative(1);}
                                if(a->getValue() == 0) {f->setZero(1);}
                            case 0b010:  //EOR
                                a->setValue(address(bbb) ^ a->getValue());
                                if(a->getValue() < 0) {f->setNegative(1);}
                                if(a->getValue() == 0) {f->setZero(1);}
                            case 0b011:  //ADC
                                int temp = address(bbb);
                                if (temp + a->getValue() > 256) {f->setCarry(1);}
                                a->setValue(temp + a->getValue());
                                if(a->getValue() < 0) {f->setNegative(1);}
                                if(a->getValue() == 0) {f->setZero(1);}
                            case 0b100:  //STA
                            case 0b101:  //LDA
                                a->setValue(address(bbb));
                                if(a->getValue() < 0) {f->setNegative(1);}
                                if(a->getValue() == 0) {f->setZero(1);}
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
                break;
        }
        pc->inc();
    }
}
