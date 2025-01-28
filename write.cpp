#include<fstream>

int main() {
    std::ofstream outputFileStream("ram.bin", std::ios::in | std::ios::out | std::ios::binary);
    outputFileStream.seekp(std::ios::beg);
    char values[0xffff];
    for (int i = 0; i < 0xffff; i++) {
        values[i] = 0xea;
    }

    outputFileStream.write(values, 0xffff);
    outputFileStream.close();
}