#include "Register.h"
#include "ALU.h"
#include"ProgramCounter.h"
#include"StatusRegister.h"
#include"ram.h"
#include "Clock.h"

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

    Clock* clock;

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

    uint8_t address(uint8_t mode, int offset);
    void address(uint8_t mode, uint8_t inValue, int offset);
    uint8_t addressManipFetch(uint8_t mode);
    void flags(Register* inreg);
    void pushStack(uint8_t value);
    uint8_t pullStack();

    void interrupt(uint8_t vector);
public:
    Control(ALU* inALU, Register* ina, Register* inx, Register* iny, RAM* inRam, Register* ins, StatusRegister* inf, ProgramCounter* inpc, Register* insp, Clock* iclock);
    void operate();
    void reset();
    void NMI();
    void IRQ();
};