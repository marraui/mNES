#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "cpu6502.h"
#include "cartridge.h"
class Bus {
  private:
    Cpu6502* cpu;
    Cartridge* cartridge;
    uint8_t ram[4 * 2048];
  public:
    Bus();
    ~Bus();
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    uint8_t* getRam();
    Cpu6502* getCpu();
    void reset();
    void insertCatridge(Cartridge* cartridge);
};

#endif /* BUS_H */
