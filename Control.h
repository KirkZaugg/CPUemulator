#include "Register.h"
#include "ALU.h"

class Control {
private:
    Register inputreg;

    ALU* alu;
    Register* a;
    Register* x;
    Register* y;

public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny);
    void operate();
};