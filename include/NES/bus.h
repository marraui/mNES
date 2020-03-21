#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "cpu6502.h"
class Bus {
  private:
    Cpu6502* cpu;
    uint8_t ram[64 * 1064];
  public:
    Bus();
    ~Bus();
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);
};

#endif /* BUS_H */
