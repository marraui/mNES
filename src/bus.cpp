#include <iostream>
#include <cstdint>
#include "NES/bus.h"
#include "NES/cpu6502.h"
#include <functional>
Bus::Bus() {
  for (int i = 0; i < 64 * 1024; i++) this->ram[i] = 0x00;
  Cpu6502* cpu = new Cpu6502(
    [&](uint16_t addr, uint8_t data) -> void {
      this->write(addr, data);
    },
    [&](uint16_t addr, bool readOnly) -> uint8_t {
      return this->read(addr, readOnly);
    }
  );
  this->cpu = cpu;
  std::cout << "Bus object initialized" << std::endl;
}

Bus::~Bus() { }

void Bus::write(uint16_t addr, uint8_t data, bool writeOnly) {
  if (addr < 0x0000 || addr > 0xFFFF) return;
  this->ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool writeOnly) {
  if (addr < 0x000 || addr > 0xFFFF) return 0x00;
  return this->ram[addr];
}

uint8_t* Bus::getRam() {
  return this->ram;
}

Cpu6502* Bus::getCpu() {
  return this->cpu;
}
