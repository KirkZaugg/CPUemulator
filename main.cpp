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
    
    a.setValue(12);
    x.setValue(3);
    y.setValue(12);
    alu.logic(1);
    int outputest = a.getValue();
    std::cout << outputest << "\n";
}