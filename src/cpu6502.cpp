#include <iostream>
#include "NES/cpu6502.h"
#include <functional>

Cpu6502::Cpu6502(
  std::function<void(uint16_t addr, uint8_t data)> write,
  std::function<uint8_t(uint16_t addr)> read
) {
  this->write = write;
  this->read = read;
  std::cout << "CPU6502 callback initialization" << std::endl;
}

Cpu6502::~Cpu6502() { }

void Cpu6502::clock() {
}