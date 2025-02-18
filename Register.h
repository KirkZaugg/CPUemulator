#include<cstdint>

#pragma once

class Register {
protected:
    uint8_t value;
public:
    void setValue(uint8_t inValue) {value = inValue;}
    uint8_t getValue() {return value;}
};