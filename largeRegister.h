
class LargeRegister : public Register {
private:
    char bigValue;
public:
    void setBigValue(char inValue) {bigValue = inValue;}
    char getBigValue() {return bigValue;}
};