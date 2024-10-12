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
        I = 0b010,
        ZP = 0b001,
        ZPX = 0b101,
        AB = 0b011,
        ABX = 0b111,
        ABY = 0b110,
        IXIND = 0b000,
        INDIX = 0b100
    };

    char address(char mode);
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, Register* inf, ProgramCounter* inpc);
    void operate();
};