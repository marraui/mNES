#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <vector>
#include <cstdint>
#include <string>
#include "mapper.h"

class Cartridge {
  private:
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;
    Mapper* mapper;
    uint8_t options;
  
  public:
    Cartridge(std::string filename);
    ~Cartridge();
    uint8_t cpuRead(uint16_t address, bool readOnly = false);
    void cpuWrite(uint16_t address, uint8_t value);
    uint8_t ppuRead(uint16_t address);
    void ppuWrite(uint16_t address, uint8_t value);
};

#endif // CARTRIDGE_H