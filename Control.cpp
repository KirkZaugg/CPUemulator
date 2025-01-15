#include<iostream>
#include "Control.h"
#include "Register.h"
#include"ALU.h"
#include"ram.h"
#include"ProgramCounter.h"
#include"StatusRegister.h"
#include"Clock.h"

Control::Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, StatusRegister* inf, ProgramCounter* inpc, Register* insp) {
    alu = inALU;
    a = ina;
    x = inx;
    y = iny;
    s = ins;
    f = inf;
    ram = inRam;
    pc = inpc;
    sp = insp;
}

char Control::address(char mode, int offset) {
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

void Control::address(char mode, char inValue, int offset) {
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

char Control::addressManipFetch(char mode) {
    switch (mode) {
        case AB:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 2) + (ram->getValue(pc->getWholeValue() + 1) << 8)));
            break;
        case ABX:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 2) + (ram->getValue(pc->getWholeValue() + 1) << 8) + x->getValue()));
            break;
        case ABY:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 2) + (ram->getValue(pc->getWholeValue() + 1) << 8) + y->getValue()));
            break;
        case I:
            return (ram->getValue(pc->getWholeValue() + 1));
            break;
        case ZP:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 1)));
            break;
        case ZPX:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 1) + x->getValue()));
            break;
    }

    return 0;
}

void Control::flags(Register* inreg) {
    if(inreg->getValue() == 0) {
        f->setZero(1);
    } else if (inreg->getValue() < 0) {
        f->setNegative(1);
    }
}

void Control::operate() {

    bool run = true;

    while(run) {
    
        inputreg.setValue(ram->getValue(pc->getWholeValue()));

        switch (inputreg.getValue()) {
            case 0x00: //BRK
            case 0x20: //JSR (abs)

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
                Clock::cycle(2);
                break;
            default:
                if ((inputreg.getValue() & 0b00011111) == 0b10000) {
                    char xstuff = inputreg.getValue() >> 6;
                    bool ystuff = (inputreg.getValue() & 0b00100000) >> 5;

                    bool branch;

                    switch (xstuff) {
                        case 0b00: //negative
                            branch = !(f->getNegative() ^ ystuff);
                            break;
                        case 0b01: //overflow
                            branch = !(f->getOverflow() ^ ystuff);
                            break;
                        case 0b10: //carry
                            branch = !(f->getCarry() ^ ystuff);
                            break;
                        case 0b11: //zero
                            branch = !(f->getZero() ^ ystuff);
                            break;
                    }

                    if (branch) {
                        //BRANCH
                        pc->inc();
                        extra.setValue(ram->getValue(pc->getWholeValue()));
                        for (int i = 0; i < extra.getValue(); i++) {
                            pc->inc();
                            if (0) { //Page cross REMEMBER TO IMPLEMENT PAGE CROSSING PLEASE
                                Clock::cycle(2);
                            }
                        }
                        Clock::cycle(3);
                        break;
                    } else {
                        Clock::cycle(2);
                        break;
                    }
                }

                

                char aaa = (inputreg.getValue() & 0b11100000) >> 5;
                char bbb = (inputreg.getValue() & 0b00011100) >> 2; //addressing mode
                char cc = inputreg.getValue() & 0b00000011;

                switch (cc) {
                    case 0b01:
                        switch (aaa) {
                            case 0b000:  //ORA
                                a->setValue(address(bbb, 0) | a->getValue());
                                flags(a);
                                break;
                            case 0b001:  //AND
                                a->setValue(address(bbb, 0) & a->getValue());
                                flags(a);
                                break;
                            case 0b010:  //EOR
                                a->setValue(address(bbb, 0) ^ a->getValue());
                                flags(a);
                                break;
                            case 0b011:  //ADC
                                int temp = address(bbb, 0);
                                if (temp + a->getValue() > 256) {f->setCarry(1);}
                                a->setValue(temp + a->getValue());
                                flags(a);
                                break;
                            case 0b100:  //STA
                                address(bbb, a->getValue(), 0);
                                break;
                            case 0b101:  //LDA
                                a->setValue(address(bbb, 0));
                                flags(a);
                                break;
                            case 0b110:  //CMP
                                char extra = a->getValue() - address(bbb, 0);
                                if (extra < 0) {
                                    f->setNegative(1);
                                } else if (extra == 0) {
                                    f->setZero(1);
                                } else if (extra > 0) {
                                    f->setCarry(1);
                                }
                                break;
                            case 0b111:  //SBC
                                int temp = address(bbb, 0);
                                a->setValue(temp + a->getValue());
                                flags(a);
                                break;
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
                                if (bbb == A) {
                                    f->setCarry(a->getValue() >> 7);
                                    a->setValue(a->getValue() << 1);
                                    Clock::cycle(2);
                                } else {
                                    extra.setValue(addressManipFetch(bbb));
                                    f->setCarry(extra.getValue() >> 7);
                                    extra.setValue(extra.getValue() << 1);
                                    address(bbb, extra.getValue(), 2);
                                }
                                break;
                            case 0b001:  //ROL
                                bool car = f->getCarry();
                                if(bbb == A) {
                                    f->setCarry(a->getValue() & 0x80);
                                    a->setValue((a->getValue() << 1) + car);
                                    Clock::cycle(2);
                                } else {
                                    extra.setValue(addressManipFetch(bbb));
                                    f->setCarry(extra.getValue() & 0x80);
                                    extra.setValue((extra.getValue() << 1) + car);
                                    address(bbb, extra.getValue(), 2);
                                }
                                break;
                            case 0b010:  //LSR
                                if (bbb == A) {
                                    f->setCarry(a->getValue() & 1);
                                    a->setValue(a->getValue() >> 1);
                                    Clock::cycle(2);
                                } else {
                                    extra.setValue(addressManipFetch(bbb));
                                    f->setCarry(extra.getValue() & 1);
                                    extra.setValue(extra.getValue() >> 1);
                                    address(bbb, extra.getValue(), 2);
                                }
                                break;
                            case 0b011:  //ROR
                                bool car = f->getCarry();
                                if(bbb == A) {
                                    f->setCarry(a->getValue() & 1);
                                    a->setValue((a->getValue() >> 1) + (0x80 * car));
                                    Clock::cycle(2);
                                } else {
                                    extra.setValue(addressManipFetch(bbb));
                                    f->setCarry(extra.getValue() & 1);
                                    extra.setValue((extra.getValue() >> 1) + (0x80 * car));
                                    address(bbb, extra.getValue(), 2);
                                }
                                break;
                            case 0b100:  //STX
                                if(bbb == 0b101) {bbb = ZPY;}
                                address(bbb, x->getValue(), 0);
                                break;
                            case 0b101:  //LDX
                                if(bbb == 0b101) {bbb = ZPY;}
                                if(bbb == 0b111) {bbb = ABY;}
                                x->setValue(address(bbb, 0));
                                flags(x);
                                break;
                            case 0b110:  //DEC
                                address(bbb, (addressManipFetch(bbb) + 1), 2);
                                break;
                            case 0b111:  //INC
                                address(bbb, (addressManipFetch(bbb) + 1), 2);
                                break;
                        }
                        break;
                    case 0b00:
                        if (bbb == 000) {bbb = I;}
                        switch (aaa) {
                            case 0b001:  //BIT
                                char mvalue = address(bbb, 0);
                                f->setOverflow((mvalue >> 6) & 1);
                                f->setNegative((mvalue >> 7) & 1);
                                f->setZero((mvalue & a->getValue()) == 0);
                                break;
                            case 0b010:  //JMP (ind)
                                pc->inc();
                                extra.setValue(ram->getValue(pc->getWholeValue()));
                                pc->inc();
                                wchar_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                                pc->setWholeValue(target);   //first jump

                                extra.setValue(ram->getValue(pc->getWholeValue()));
                                pc->inc();
                                wchar_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                                pc->setWholeValue(target);   //second jump
                                Clock::cycle(5);
                                break;
                            case 0b011:  //JMP (abs)
                                pc->inc();
                                extra.setValue(ram->getValue(pc->getWholeValue()));
                                pc->inc();
                                wchar_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                                pc->setWholeValue(target);
                                Clock::cycle(3);
                                break;
                            case 0b100:  //STY
                                address(bbb, y->getValue(), 0);
                                break;
                            case 0b101:  //LDY
                                y->setValue(address(bbb, 0));
                                flags(x);
                                break;
                            case 0b110:  //CPY
                                char mvalue = address(bbb, 0);
                                char yvalue = y->getValue();
                                f->setCarry(yvalue >= mvalue);
                                f->setZero(yvalue == mvalue);
                                f->setNegative(yvalue < mvalue);
                                break;
                            case 0b111:  //CPX
                                char mvalue = address(bbb, 0);
                                char xvalue = x->getValue();
                                f->setCarry(xvalue >= mvalue);
                                f->setZero(xvalue == mvalue);
                                f->setNegative(xvalue < mvalue);
                                break;
                        }
                        break;
                }
                break;
        }
        pc->inc();
    }
}
