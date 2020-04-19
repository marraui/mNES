#ifndef MAPPER_H
#define MAPPER_H
#include <cstdint>
class Mapper {
  protected:
    uint8_t prgBanks;
    uint8_t chrBanks;
  public:
    virtual uint16_t cpuMapRead(uint16_t addr);
    virtual uint16_t cpuMapWrite(uint16_t addr, uint8_t data);
    virtual uint16_t ppuMapRead(uint16_t addr);
    virtual uint16_t ppuMapWrite(uint16_t addr, uint8_t data); 
};

#endif // MAPPER_H
