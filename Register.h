#include<cstdint>

#pragma once

class Register {
protected:
    uint8_t value;
    bool write;
    bool read;
public:
    void setValue(uint8_t inValue) {value = inValue; write = true;}
    uint8_t getValue() {read = true; return value;}
    bool getWrite() {if (write) {write = false; return true;} else {return false;}}
    bool getRead() {if (read) {read = false; return true;} else {return false;}}
};