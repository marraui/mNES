#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "NES/cartridge.h"
#include "NES/utils.h"
#include "NES/mapper_0.h"

Cartridge::Cartridge(std::string filename) {
  std::ifstream romFile;
  romFile.open(filename, std::ios::in | std::ios::binary);
  if (!romFile.is_open()) {
    std::cout << "Error opening rom" << std::endl;
    return;
  }

  char* buffer = new char[10];

  // Header
  romFile.read(buffer, 11);

  std::cout << "NES Header ";
  for (int i = 0; i < 8; i++) {
    std::cout << hexToString((uint8_t)buffer[i], 2) << " ";
  }
  std::cout << std::endl;

  if (
    buffer[0] != 0x4E && // N
    buffer[1] != 0x45 && // E
    buffer[2] != 0x53 && // S
    buffer[3] != 0x1A // Character break
  ) {
    std::cout << "File selected is not an ines rom" << std::endl;
    return;
  }

  uint8_t prgBanks = buffer[4];
  uint8_t chrBanks = buffer[5];
  unsigned long prgSize = prgBanks * 16384;
  unsigned long chrSize = chrBanks * 8192;

  // Select mapper
  unsigned int mapperId = (buffer[7] & 0xF0) | (buffer[0] & 0xF0);

  switch(mapperId) {
    case 0:
      this->mapper = new Mapper0(prgBanks, chrBanks);
      break;
    default:
      this->mapper = new Mapper0(prgBanks, chrBanks);
      break;
  }

  this->options = buffer[6] & 0x0F;

  // Not used yet
  unsigned long prgRamSize = buffer[8] * 8192;
  bool tvSystem = buffer[9];

  // Read prg
  this->prg.resize(prgSize);
  romFile.seekg(16 + ((this->options & 0x04) ? 512 : 0));
  romFile.read((char*)this->prg.data(), prgSize);
  std::cout << "PRG ";
  for (int i = 0; i < 15; i++) {
    std::cout << hexToString(this->prg[i], 2) << " ";
  }
  std::cout << std::endl;

  this->chr.resize(chrSize);
  romFile.seekg(16 + ((this->options & 0x04) ? 512 : 0) + prgSize);
  romFile.read((char*)(this->chr.data()), chrSize);
  std::cout << "CHR ";
  for (int i = 0; i < 15; i++) {
    std::cout << hexToString(this->chr[i], 2) << " ";
  }
  std::cout << std::endl;
}

Cartridge::~Cartridge() {}

uint8_t Cartridge::cpuRead(uint16_t address, bool readOnly) {
  uint16_t mappedAddr = this->mapper->cpuMapRead(address);
  return this->prg[mappedAddr];
}

void Cartridge::cpuWrite(uint16_t address, uint8_t value) {
  uint16_t mappedAddr = this->mapper->cpuMapWrite(address, value);
  this->prg[mappedAddr] = value;
}

uint8_t Cartridge::ppuRead(uint16_t address) {
  uint16_t mappedAddr = this->mapper->ppuMapRead(address);
  return this->chr[mappedAddr];
}

void Cartridge::ppuWrite(uint16_t address, uint8_t value) {
  uint16_t mappedAddr = this->mapper->ppuMapWrite(address, value);
  this->chr[mappedAddr] = value;
}
