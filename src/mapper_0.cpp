#include "NES/mapper_0.h"

Mapper0::Mapper0(uint8_t prgBanks, uint8_t chrBanks) {
  this->prgBanks = prgBanks;
  this->chrBanks = chrBanks;
}

Mapper0::~Mapper0() {}

uint16_t Mapper0::cpuMapRead(uint16_t addr) {
  return addr & (this->prgBanks > 1 ? 0x7FFF : 0x3FFF);
}

uint16_t Mapper0::cpuMapWrite(uint16_t addr, uint8_t data) {
  return addr & (this->prgBanks > 1 ? 0x7FFF : 0x3FFF);
}

uint16_t Mapper0::ppuMapRead(uint16_t addr) {
  return addr;
}

uint16_t Mapper0::ppuMapWrite(uint16_t addr, uint8_t data) {
  return addr;
}