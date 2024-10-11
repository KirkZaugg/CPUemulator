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
    Register* s;
    Register* f;
    ProgramCounter* pc;

    RAM* ram;

    enum Addmode {
        A,
        I,
        ZP,
        ZPX,
        ZPY,
        R,
        AB,
        ABX,
        ABY,
        IND,
        IXIND,
        INDIX
    };

    char address(Addmode mode);
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, Register* inf, ProgramCounter* inpc);
    void operate();
};