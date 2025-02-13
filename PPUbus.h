#include<cstdint>

class PPUbus {
private:
    uint8_t cart[0x3f00];
    uint8_t palette[0x20];

    uint8_t* addr(uint16_t location);
public:
    uint8_t read(uint16_t location);
    void write(uint16_t location, uint8_t value);
};