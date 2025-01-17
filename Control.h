#include "Register.h"
#include "ALU.h"
#include"ProgramCounter.h"
#include"StatusRegister.h"
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
    Register* sp;
    StatusRegister* f;
    ProgramCounter* pc;

    RAM* ram;

    enum Addmode {
        A = 0b1010,
        I = 0b010,
        ZP = 0b001,
        ZPX = 0b101,
        ZPY = 0b1000,
        AB = 0b011,
        ABX = 0b111,
        ABY = 0b110,
        IND = 0b1001,
        IXIND = 0b000,
        INDIX = 0b100
    };

    char address(char mode, int offset);
    void address(char mode, char inValue, int offset);
    char addressManipFetch(char mode);
    void flags(Register* inreg);
    void pushStack(char value);
    char pullStack();
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, StatusRegister* inf, ProgramCounter* inpc, Register* insp);
    void operate();
};