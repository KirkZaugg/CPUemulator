

class ALU {
private:
    Register* a;
    Register* x;
    Register* y;
public:
    ALU(Register* ina, Register* inx, Register* iny);
    void add();         
    void subtract();
    void shift(bool direction);    //false = left; true = right;

};