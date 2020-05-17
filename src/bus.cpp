#include <iostream>
#include <cstdint>
#include "NES/bus.h"
#include "NES/cpu6502.h"
#include "NES/ppu2C02.h"
#include <functional>
Bus::Bus() {
  this->clockCounter = 0;

  for (int i = 0; i < 2048; i++) this->ram[i] = 0x00;

  Cpu6502* cpu = new Cpu6502(
    [&](uint16_t addr, uint8_t data) -> void {
      this->cpuWrite(addr, data);
    },
    [&](uint16_t addr, bool readOnly) -> uint8_t {
      return this->cpuRead(addr, readOnly);
    }
  );
  this->cpu = cpu;

  Ppu2C02* ppu = new Ppu2C02(
    [&](uint16_t addr, uint8_t data) -> void {
      this->ppuWrite(addr, data);
    },
    [&](uint16_t addr, bool readOnly) -> uint8_t {
      return this->ppuRead(addr, readOnly);
    }
  );
  this->ppu = ppu;

  std::cout << "Bus object initialized" << std::endl;
}

Bus::~Bus() { }

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
  if (addr < 0x0000) return;
  else if (addr < 0x2000) this->ram[addr & 0x07FF] = data;
  else if (addr < 0x4000) return;
  else if (addr >= 0x4020 && addr <= 0xFFFF) this->cartridge->cpuWrite(addr, data);
  return;
}

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly) {
  if (addr < 0x0000) return 0x00;
  else if (addr < 0x2000) return this->ram[addr & 0x07FF];
  else if (addr < 0x4000) return 0x00;
  else if (addr >= 0x4020 && addr <= 0xFFFF) return this->cartridge->cpuRead(addr, readOnly);
  return 0x00;
}

void Bus::ppuWrite(uint16_t addr, uint8_t data) {
  this->cartridge->ppuWrite(addr, data);
}

uint8_t Bus::ppuRead(uint16_t addr, bool readOnly) {
  return this->cartridge->ppuRead(addr);
}

uint8_t* Bus::getRam() {
  return this->ram;
}

Cpu6502* Bus::getCpu() {
  return this->cpu;
}

void Bus::reset() {
  this->cpu->reset();
  this->clockCounter = 0;
}

void Bus::insertCatridge(Cartridge* cartridge) {
  this->cartridge = cartridge;
}

void Bus::clock() {
  this->ppu->clock();

  if (this->clockCounter % 3 == 0) {
    this->cpu->clock();
  }
}

Ppu2C02* Bus::getPpu() {
  return this->ppu;
}