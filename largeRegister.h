#pragma once

class LargeRegister : public Register {
protected:
    unsigned char bigValue;
public:
    void setBigValue(unsigned char inValue) {bigValue = inValue;}
    char getBigValue() {return bigValue;}

    void setWholeValue(wchar_t inValue) {setValue(inValue & 0xff); setBigValue(inValue >> 8);}
    wchar_t getWholeValue() {return (getBigValue() << 8) + getValue();}
};