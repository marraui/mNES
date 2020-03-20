#ifndef CPU6502_H
#define CPU6502_H

#include <cstdint>

class Cpu6502 {
  private:
    u_int16_t programCounter;
    u_int8_t accumulator;
    u_int8_t xRegister;
    u_int8_t yRegister;
    u_int8_t stackPointer;
    u_int8_t processorStatusRegister;
  public:
    Cpu6502();
    ~Cpu6502(); 
};

#endif /* CPU6502_h */
