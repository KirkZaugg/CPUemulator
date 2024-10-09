#pragma once

class RAM {
private:
    char info[10] = {0x11, 0x09, 0x00, 0, 0, 0, 0, 0, 0, 0x01};
public:
    char getValue(wchar_t location) {return info[location];}
    void setValue(wchar_t location, char value) {info[location] = value;}
};