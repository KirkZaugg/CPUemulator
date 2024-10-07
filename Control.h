#include "Register.h"
#include "ALU.h"

class Control {
private:
    Register inputreg;

    ALU* alu;
    Register* a;
    Register* x;
    Register* y;
    Register ctrlReg;


    void jump();
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny);
    void operate();
};