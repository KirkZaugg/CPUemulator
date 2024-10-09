#pragma once

#include "Register.h"
#include "largeRegister.h"

class ProgramCounter : public LargeRegister {
public:
    void inc() {setWholeValue(getWholeValue() + 1);}
};