#include<iostream>
#include"Register.h"
#include"largeRegister.h"
#include"ALU.h"

int main() {
    Register a;
    Register x;
    Register y;
    char bus;
    ALU alu(&a, &x, &y);
    

    x.setValue('A');
    y.setValue('B');
    alu.add();
    std::cout << a.getValue() << "\n";
}