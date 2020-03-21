#include <iostream>
#include "NES/cpu6502.h"
#include <functional>

Cpu6502::Cpu6502(std::function<void(uint16_t addr, uint8_t data)> write) {
  this->write = write;
  std::cout << "CPU6502 callback initialization" << std::endl;
}

Cpu6502::~Cpu6502() { }