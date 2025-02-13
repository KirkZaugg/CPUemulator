#pragma once

class Register {
protected:
    unsigned char value;
public:
    void setValue(unsigned char inValue) {value = inValue;}
    unsigned char getValue() {return value;}
};