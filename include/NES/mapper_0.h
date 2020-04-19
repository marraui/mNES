#ifndef MAPPER_0_H
#define MAPPER_0_H
#include "mapper.h"

class Mapper0: public Mapper {
  public:
    Mapper0(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper0();
    uint16_t cpuMapRead(uint16_t addr) override;
    uint16_t cpuMapWrite(uint16_t addr, uint8_t data) override;
    uint16_t ppuMapRead(uint16_t addr) override;
    uint16_t ppuMapWrite(uint16_t addr, uint8_t data) override;
};

#endif // MAPPER_0_H