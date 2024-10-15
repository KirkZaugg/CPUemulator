#pragma once
#include "Register.h"

class StatusRegister : public Register {
public:
    bool getCarry() {return value & 1;}
    void setCarry(bool inValue) {value = (value - (value & 1)) + inValue;}
    bool getZero() {return (value >> 1) & 1;}
    void setZero(bool inValue) {value = (value - (value & 0b10)) + (inValue << 1);}
    bool getID() {return (value >> 2) & 1;}
    void setID(bool inValue) {value = (value - (value & 0b100)) + (inValue << 2);}
    bool getDecimal() {return (value >> 3) & 1;}
    void setDecimal(bool inValue) {value = (value - (value & 0b1000)) + (inValue << 3);}
    bool getOverflow() {return (value >> 6) & 1;}
    void setOverflow(bool inValue) {value = (value - (value & 0b100000)) + (inValue << 6);}
    bool getNegative() {return (value >> 7) & 1;}
    void setNegative(bool inValue) {value = (value - (value & 0b1000000)) + (inValue << 7);}
};