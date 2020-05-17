#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "cpu6502.h"
#include "ppu2C02.h"
#include "cartridge.h"
class Bus {
  private:
    Cpu6502* cpu;
    Ppu2C02* ppu;
    Cartridge* cartridge;
    uint8_t ram[2048];
    uint32_t clockCounter;
  public:
    Bus();
    ~Bus();
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    uint8_t* getRam();
    Cpu6502* getCpu();
    Ppu2C02* getPpu();
    void reset();
    void insertCatridge(Cartridge* cartridge);
    void clock();
};

#endif /* BUS_H */
