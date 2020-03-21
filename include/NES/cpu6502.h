#ifndef CPU6502_H
#define CPU6502_H

#include <cstdint>
#include <functional>

enum STATUS_FLAGS {
  CARRY = (1 << 0),
  ZERO = (1 << 1),
  IRQ_DISABLE = (1 << 2),
  DECIMAL_MODE = (1 << 3),
  BRK_COMMAND = (1 << 4),
  UNUSED = (1 << 5),
  OVERFLOW = (1 << 6),
  NEGATIVE = (1 << 7),
};

class Cpu6502 {
  private:
    u_int16_t programCounter;
    u_int8_t accumulator;
    u_int8_t xRegister;
    u_int8_t yRegister;
    u_int8_t stackPointer;
    u_int8_t processorStatusRegister;
  public:
    std::function<void(uint16_t, uint8_t)> write;
    Cpu6502(std::function<void(uint16_t addr, uint8_t data)> write);
    ~Cpu6502(); 
};

#endif /* CPU6502_h */
