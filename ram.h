#pragma once

class RAM {
private:
    std::string file;
public:
    char getValue(wchar_t location);
    void setValue(wchar_t location, char value);
    RAM();
};