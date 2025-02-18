#include<cstdint>
#pragma once

class LargeRegister : public Register {
protected:
    uint8_t bigValue;
public:
    void setBigValue(uint8_t inValue) {bigValue = inValue;}
    uint8_t getBigValue() {return bigValue;}

    void setWholeValue(uint16_t inValue) {setValue(inValue & 0xff); setBigValue(inValue >> 8);}
    uint16_t getWholeValue() {return ((getBigValue() << 8) | getValue()) & 0xffff;}
};