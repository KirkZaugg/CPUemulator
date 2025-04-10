#include<iostream>
#include "Control.h"
#include "Register.h"
#include"ALU.h"
#include"ram.h"
#include"ProgramCounter.h"
#include"StatusRegister.h"
#include"Clock.h"

Control::Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, StatusRegister* inf, ProgramCounter* inpc, Register* insp, Clock* iclock) {
    alu = inALU;
    a = ina;
    x = inx;
    y = iny;
    s = ins;
    f = inf;
    ram = inRam;
    pc = inpc;
    sp = insp;
    clock = iclock;

    sp->setValue(0xff);

}

uint8_t Control::address(uint8_t mode, int offset) {
    clock->cycle(offset);
    switch (mode) {
        case AB: {
            clock->cycle(4);
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            int outtest = (extra.getValue() + (ram->getValue(pc->getWholeValue()) << 8));
            std::cout << " " << outtest << " ";
            return (extra.getValue() + (ram->getValue(pc->getWholeValue()) << 8));
        }break;
        case ABX:
            clock->cycle(4); //+page cross to implement later
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (extra.getValue() + (ram->getValue(pc->getWholeValue()) << 8) + x->getValue());
            break;
        case ABY:
            clock->cycle(4); //+page cross to implement later
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            return (extra.getValue() + (ram->getValue(pc->getWholeValue()) << 8) + y->getValue());
            break;
        case I:
            clock->cycle(2);
            pc->inc();
            return (ram->getValue(pc->getWholeValue()));
            break;
        case ZP:
            clock->cycle(3);
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue())));
            break;
        case ZPX:
            clock->cycle(4);
            pc->inc();
            return (ram->getValue(ram->getValue(pc->getWholeValue()) + x->getValue()));
            break;
        case IXIND: {
            clock->cycle(6); //+page cross to implement later
            pc->inc();
            int zpaddr = (ram->getValue(pc->getWholeValue()) + x->getValue());
            return (ram->getValue(ram->getValue(zpaddr) + (ram->getValue(zpaddr + 1) << 8)));
        }break;
        case INDIX: {
            clock->cycle(5); //+page cross to implement later
            pc->inc();
            int zpaddr = ram->getValue(pc->getWholeValue());
            return (ram->getValue(ram->getValue(zpaddr) + (ram->getValue(zpaddr + 1) << 8))) + y->getValue();
        }break;
    }

    return 0;
}

void Control::address(uint8_t mode, uint8_t inValue, int offset) {
    clock->cycle(offset);
    switch (mode) {
        case AB:
            clock->cycle(4);
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue((ram->getValue(pc->getWholeValue()) << 8) + extra.getValue(), inValue);
            break;
        case ABX:
            clock->cycle(5);
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue((ram->getValue(pc->getWholeValue()) << 8) + extra.getValue() + x->getValue(), inValue);
            break;
        case ABY:
            clock->cycle(5);
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            ram->setValue((ram->getValue(pc->getWholeValue()) << 8) + extra.getValue() + y->getValue(), inValue);
            break;
        case I:
            clock->cycle(2); //this value may be wrong
            pc->inc();
            ram->setValue(pc->getWholeValue(), inValue);
            break;
        case ZP:
            clock->cycle(2);
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()), inValue);
            break;
        case ZPX:  
            clock->cycle(4);
            pc->inc();
            ram->setValue(ram->getValue(pc->getWholeValue()) + x->getValue(), inValue);
            break;
        case IXIND: {
            clock->cycle(6);
            pc->inc();
            int zpaddr = (ram->getValue(pc->getWholeValue()) + x->getValue());
            ram->setValue(ram->getValue(zpaddr) + (ram->getValue(zpaddr + 1) << 8), inValue);
        }break;
        case INDIX: {
            clock->cycle(6);
            pc->inc();
            int zpaddr = ram->getValue(pc->getWholeValue());
            ram->setValue(ram->getValue(zpaddr) + (ram->getValue(zpaddr + 1) << 8) + y->getValue(), inValue);
        }break;
    }
}

uint8_t Control::addressManipFetch(uint8_t mode) {
    switch (mode) {
        case AB:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 1) + (ram->getValue(pc->getWholeValue() + 2) << 8)));
            break;
        case ABX:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 1) + (ram->getValue(pc->getWholeValue() + 2) << 8) + x->getValue()));
            break;
        case ABY:
            return (ram->getValue(ram->getValue(pc->getWholeValue() + 1) + (ram->getValue(pc->getWholeValue() + 2) << 8) + y->getValue()));
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
    f->setZero(inreg->getValue() == 0);
    f->setNegative(inreg->getValue() & 0x80);
}

void Control::pushStack(uint8_t value) {
    ram->setValue(sp->getValue() | 0x0100, value);
    sp->setValue(sp->getValue() - 1);
}
uint8_t Control::pullStack() {
    sp->setValue(sp->getValue() + 1);
    return ram->getValue(sp->getValue() | 0x0100);
}

void Control::NMI() {
    interrupt(0xfa);
}

void Control::IRQ() {
    interrupt(0xfe);
}

void Control::reset() {
    clock->cycle(6);
    uint16_t resvec = (ram->getValue(0xfffc) + (ram->getValue(0xfffd) << 8)) & 0xffff;
    pc->setWholeValue(resvec);
    std::cout << "reset: " << std::hex << resvec << "\n";
}

void Control::interrupt(uint8_t vector) {
    pushStack(pc->getBigValue());
    pushStack(pc->getValue());
    pushStack(f->getValue());
    f->setID(1);
    pc->setWholeValue(0xff00 | vector);
}

void Control::operate() {

    uint8_t opcode = ram->getValue(pc->getWholeValue());
    int op = opcode;
    std::cout << "op:" << std::hex << op << "   ";

    switch (opcode) {
        case 0x00: //BRK
            interrupt(0xfe);
            break;
        case 0x20: {//JSR (abs)
            pushStack(pc->getBigValue());
            pushStack(pc->getValue() + 2);
            pc->inc();
            extra.setValue(ram->getValue(pc->getWholeValue()));
            pc->inc();
            uint16_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
            pc->setWholeValue(target - 1);
            clock->cycle(6);
        }break;
        case 0x40: {//RTI
            uint8_t small = pullStack();
            uint8_t big = pullStack();
            uint16_t target = (big << 8) + small;
            pc->setWholeValue(target);
            clock->cycle(6);
        }
        case 0x60: {//RTS
            uint8_t small = pullStack();
            uint8_t big = pullStack();
            uint16_t target = (big << 8) + small;
            pc->setWholeValue(target);
            clock->cycle(6);
        }break;
        case 0x08: //PHP
            pushStack(f->getValue());
            clock->cycle(3);
            break;
        case 0x28: //PLP
            f->setValue(pullStack());
            clock->cycle(4);
            break;
        case 0x48: //PHA
            pushStack(a->getValue());
            clock->cycle(3);
            break;
        case 0x68: //PLA
            a->setValue(pullStack());
            clock->cycle(4);
            break;
        case 0x88: //DEY
            y->setValue(y->getValue() - 1);
            flags(y);
            clock->cycle(2);
            break;
        case 0xA8: //TAY
            y->setValue(a->getValue());
            flags(y);
            clock->cycle(2);
            break;
        case 0xC8: //INY
            y->setValue(y->getValue() + 1);
            flags(y);
            clock->cycle(2);
            break;
        case 0xE8: //INX
            x->setValue(x->getValue() + 1);
            flags(x);
            clock->cycle(2);
            break;
        case 0x18: //CLC
            f->setCarry(0);
            clock->cycle(2);
            break;
        case 0x38: //SEC
            f->setCarry(1);
            clock->cycle(2);
            break;
        case 0x58: //CLI
            f->setID(0);
            clock->cycle(2);
            break;
        case 0x78: //SEI
            f->setID(1);
            clock->cycle(2);
            break;
        case 0x98: //TYA
            a->setValue(y->getValue());
            flags(a);
            clock->cycle(2);
            break;
        case 0xB8: //CLV
            f->setOverflow(0);
            clock->cycle(2);
            break;
        case 0xD8: //CLD
            f->setDecimal(0);
            clock->cycle(2);
            break;
        case 0xF8: //SED
            f->setDecimal(1);
            clock->cycle(2);
            break;
        case 0x8A: //TXA
            a->setValue(x->getValue());
            flags(a);
            clock->cycle(2);
            break;
        case 0x9A: //TXS
            pushStack(x->getValue());
            clock->cycle(2);
            break;
        case 0xAA: //TAX
            x->setValue(a->getValue());
            flags(x);
            clock->cycle(2);
            break;
        case 0xBA: //TSX
            x->setValue(pullStack());
            flags(x);
            clock->cycle(2);
            break;
        case 0xCA: //DEX
            x->setValue(x->getValue() - 1);
            flags(x);
            clock->cycle(2);
            break;
        case 0xEA: //NOP
        clock->cycle(2);
            break;
        default: {
            if ((opcode & 0b00011111) == 0b10000) {
                uint8_t xstuff = opcode >> 6;
                bool ystuff = (opcode & 0b00100000) >> 5;

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
                    uint8_t offset = ram->getValue(pc->getWholeValue());
                    pc->setWholeValue(pc->getWholeValue() + reinterpret_cast<char&>(offset)); //remember to implement page crossing time
                    clock->cycle(3);
                    int out = pc->getWholeValue();
                } else {
                    pc->inc();
                    clock->cycle(2);
                }
                break;
            }

            
            
            uint8_t aaa = (opcode & 0b11100000) >> 5;
            uint8_t bbb = (opcode & 0b00011100) >> 2; //addressing mode
            uint8_t cc = opcode & 0b00000011;

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
                        case 0b011: {//ADC
                            int temp = address(bbb, 0);
                            if (temp + a->getValue() > 256) {f->setCarry(1);}
                            a->setValue(temp + a->getValue());
                            flags(a);
                        }break;
                        case 0b100:  //STA
                            address(bbb, a->getValue(), 0);
                            break;
                        case 0b101:  //LDA
                            a->setValue(address(bbb, 0));
                            flags(a);
                            break;
                        case 0b110: { //CMP
                            uint8_t extra = a->getValue() - address(bbb, 0);
                            if (extra < 0) {
                                f->setNegative(1);
                            } else if (extra == 0) {
                                f->setZero(1);
                            } else if (extra > 0) {
                                f->setCarry(1);
                            }
                        }break;
                        case 0b111:  {//SBC
                            int temp = address(bbb, 0);
                            a->setValue(temp + a->getValue());
                            flags(a);
                        }break;
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
                                clock->cycle(2);
                            } else {
                                extra.setValue(addressManipFetch(bbb));
                                f->setCarry(extra.getValue() >> 7);
                                extra.setValue(extra.getValue() << 1);
                                address(bbb, extra.getValue(), 2);
                            }
                            break;
                        case 0b001:  {//ROL
                            bool car = f->getCarry();
                            if(bbb == A) {
                                f->setCarry(a->getValue() & 0x80);
                                a->setValue((a->getValue() << 1) + car);
                                clock->cycle(2);
                            } else {
                                extra.setValue(addressManipFetch(bbb));
                                f->setCarry(extra.getValue() & 0x80);
                                extra.setValue((extra.getValue() << 1) + car);
                                address(bbb, extra.getValue(), 2);
                            }
                        }break;
                        case 0b010:  //LSR
                            if (bbb == A) {
                                f->setCarry(a->getValue() & 1);
                                a->setValue(a->getValue() >> 1);
                                clock->cycle(2);
                            } else {
                                extra.setValue(addressManipFetch(bbb));
                                f->setCarry(extra.getValue() & 1);
                                extra.setValue(extra.getValue() >> 1);
                                address(bbb, extra.getValue(), 2);
                            }
                            break;
                        case 0b011:  {//ROR
                            bool car = f->getCarry();
                            if(bbb == A) {
                                f->setCarry(a->getValue() & 1);
                                a->setValue((a->getValue() >> 1) + (0x80 * car));
                                clock->cycle(2);
                            } else {
                                extra.setValue(addressManipFetch(bbb));
                                f->setCarry(extra.getValue() & 1);
                                extra.setValue((extra.getValue() >> 1) + (0x80 * car));
                                address(bbb, extra.getValue(), 2);
                            }
                        }break;
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
                        case 0b001:  {//BIT
                            uint8_t mvalue = address(bbb, 0);
                            f->setOverflow((mvalue >> 6) & 1);
                            f->setNegative((mvalue >> 7) & 1);
                            f->setZero((mvalue & a->getValue()) == 0);
                        }break;
                        case 0b011:  {//JMP (ind) 
                            pc->inc();
                            extra.setValue(ram->getValue(pc->getWholeValue()));
                            pc->inc();
                            uint16_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                            pc->setWholeValue(target);   //first jump

                            extra.setValue(ram->getValue(pc->getWholeValue()));
                            pc->inc();
                            target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                            pc->setWholeValue(target - 1);   //second jump
                            clock->cycle(5);
                        }break;
                        case 0b010:  {//JMP (abs)
                            pc->inc();
                            extra.setValue(ram->getValue(pc->getWholeValue()));
                            pc->inc();
                            uint16_t target = ((ram->getValue(pc->getWholeValue()) << 8) + (extra.getValue()));
                            pc->setWholeValue(target - 1);
                            clock->cycle(3);
                        }break;
                        case 0b100:  //STY
                            address(bbb, y->getValue(), 0);
                            break;
                        case 0b101:  //LDY
                            y->setValue(address(bbb, 0));
                            flags(x);
                            break;
                        case 0b110:  {//CPY
                            uint8_t mvalue = address(bbb, 0);
                            uint8_t yvalue = y->getValue();
                            f->setCarry(yvalue >= mvalue);
                            f->setZero(yvalue == mvalue);
                            f->setNegative(yvalue < mvalue);
                        }break;
                        case 0b111:  {//CPX
                            uint8_t mvalue = address(bbb, 0);
                            uint8_t xvalue = x->getValue();
                            f->setCarry(xvalue >= mvalue);
                            f->setZero(xvalue == mvalue);
                            f->setNegative(xvalue < mvalue);
                        }break;
                    }
                    break;
            }
        }break;
    }
    pc->inc();
}
