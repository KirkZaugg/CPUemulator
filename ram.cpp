#include<fstream>
#include"ram.h"

char RAM::getValue(wchar_t location) {
    std::ifstream inputFileStream(file, std::ios::in | std::ios::binary);
    inputFileStream.seekg(location, std::ios::beg);
    char input;
    inputFileStream.read(&input, 1);
    inputFileStream.close();
    return input;
}

void RAM::setValue(wchar_t location, char value) {
    std::ofstream outputFileStream(file, std::ios::out | std::ios::binary);
    outputFileStream.seekp(location, std::ios::beg);
    outputFileStream.write(&value, 1);
    outputFileStream.close();
}

RAM::RAM() {
    file = "ram.bin";
}