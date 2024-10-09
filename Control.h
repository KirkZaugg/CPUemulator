#include "Register.h"
#include "ALU.h"
#include"ProgramCounter.h"
#include"ram.h"

class Control {
private:
    Register inputreg;
    Register extra;

    ALU* alu;
    Register* a;
    Register* x;
    Register* y;
    ProgramCounter* pc;

    RAM* ram;


    void jump();
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, ProgramCounter* inpc);
    void operate();
};