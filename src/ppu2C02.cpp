#include "NES/ppu2C02.h"
#include <functional>
#include <cstdint>
#include <cstdint>
#include <stdlib.h>
#include <iostream>

Ppu2C02::Ppu2C02(
  std::function<void(uint16_t addr, uint8_t data)> write,
  std::function<uint8_t(uint16_t addr, bool isReadOnly)> read
) {
  this->writeCart = write;
  this->readCart = read;
  this->cycle = 0;
  this->scanline = 0;
  
  for (unsigned int i = 0; i < 256 * 240; i++) {
    this->screen[i] = { 0, 0, 0 };
  }

  this->paletteColors[0x00] = { 84, 84, 84 };
  this->paletteColors[0x01] = { 0, 30, 116 };
  this->paletteColors[0x02] = { 8, 16, 144 };
  this->paletteColors[0x03] = { 48, 0, 136 };
  this->paletteColors[0x04] = { 68, 0, 100 };
  this->paletteColors[0x05] = { 92, 0, 48 };
  this->paletteColors[0x06] = { 84, 4, 0 };
  this->paletteColors[0x07] = { 60, 24, 0 };
  this->paletteColors[0x08] = { 32, 42, 0 };
  this->paletteColors[0x09] = { 8, 58, 0 };
  this->paletteColors[0x0A] = { 0, 64, 0 };
  this->paletteColors[0x0B] = { 0, 60, 0 };
  this->paletteColors[0x0C] = { 0, 50, 60 };
  this->paletteColors[0x0D] = { 0, 0, 0 };
  this->paletteColors[0x0E] = { 0, 0, 0 };
  this->paletteColors[0x0F] = { 0, 0, 0 };

  this->paletteColors[0x10] = { 152, 150, 152 };
  this->paletteColors[0x11] = { 8, 76, 196 };
  this->paletteColors[0x12] = { 48, 50, 236 };
  this->paletteColors[0x13] = { 92, 30, 228 };
  this->paletteColors[0x14] = { 136, 20, 176 };
  this->paletteColors[0x15] = { 160, 20, 100 };
  this->paletteColors[0x16] = { 152, 34, 32 };
  this->paletteColors[0x17] = { 120, 60, 0 };
  this->paletteColors[0x18] = { 84, 90, 0 };
  this->paletteColors[0x19] = { 40, 114, 0 };
  this->paletteColors[0x1A] = { 8, 124, 0 };
  this->paletteColors[0x1B] = { 0, 118, 40 };
  this->paletteColors[0x1C] = { 0, 102, 120 };
  this->paletteColors[0x1D] = { 0, 0, 0 };
  this->paletteColors[0x1E] = { 0, 0, 0 };
  this->paletteColors[0x1F] = { 0, 0, 0 };

  this->paletteColors[0x20] = { 236, 238, 236 };
  this->paletteColors[0x21] = { 76, 154, 236 };
  this->paletteColors[0x22] = { 120, 124, 236 };
  this->paletteColors[0x23] = { 176, 98, 236 };
  this->paletteColors[0x24] = { 228, 84, 236 };
  this->paletteColors[0x25] = { 236, 88, 180 };
  this->paletteColors[0x26] = { 236, 106, 100 };
  this->paletteColors[0x27] = { 212, 136, 32 };
  this->paletteColors[0x28] = { 160, 170, 0 };
  this->paletteColors[0x29] = { 116, 196, 0 };
  this->paletteColors[0x2A] = { 76, 208, 32 };
  this->paletteColors[0x2B] = { 56, 204, 108 };
  this->paletteColors[0x2C] = { 56, 180, 204 };
  this->paletteColors[0x2D] = { 60, 60, 60 };
  this->paletteColors[0x2E] = { 0, 0, 0 };
  this->paletteColors[0x2F] = { 0, 0, 0 };

  this->paletteColors[0x30] = { 236, 238, 236 };
  this->paletteColors[0x31] = { 168, 204, 236 };
  this->paletteColors[0x32] = { 188, 188, 236 };
  this->paletteColors[0x33] = { 212, 178, 236 };
  this->paletteColors[0x34] = { 236, 174, 236 };
  this->paletteColors[0x35] = { 236, 174, 212 };
  this->paletteColors[0x36] = { 236, 180, 176 };
  this->paletteColors[0x37] = { 228, 196, 144 };
  this->paletteColors[0x38] = { 204, 210, 120 };
  this->paletteColors[0x39] = { 180, 222, 120 };
  this->paletteColors[0x3A] = { 168, 226, 144 };
  this->paletteColors[0x3B] = { 152, 226, 180 };
  this->paletteColors[0x3C] = { 160, 214, 228 };
  this->paletteColors[0x3D] = { 160, 162, 160 };
  this->paletteColors[0x3F] = { 0, 0, 0 };
  this->paletteColors[0x3F] = { 0, 0, 0 };
  
}

Ppu2C02::~Ppu2C02() {}

void Ppu2C02::clock() {
  if (
    this->cycle >= 0 && this->cycle < 256 &&
    this->scanline >= 0 && this->scanline < 240
  ) {
    this->screen[this->cycle + this->scanline * 256] = this->paletteColors[rand() % (16 * 4)];
  }
  this->cycle += 1;
  if (this->cycle >= 341) {
    this->cycle = 0;
    this->scanline += 1;
    if (this->scanline >= 261) {
      this->scanline = -1;
    }
  }
}

uint8_t Ppu2C02::cpuRead(uint16_t addr, bool readOnly) {
  return 0x00;
}

void Ppu2C02::cpuWrite(uint16_t addr, uint8_t data) {}

uint8_t Ppu2C02::ppuRead(uint16_t addr, bool readOnly) {
  if (addr < 0x2000) return this->readCart(addr, readOnly);
  if (addr < 0x3F00) return this->nameTable[addr & 0x0FFF];
  if (addr < 0x4000) return this->palette[addr & 0x001F];
  return 0x00;
}

Ppu2C02::COLOR* Ppu2C02::getScreen() {
  return this->screen;
}

bool Ppu2C02::isFrameComplete() {
  return this->scanline > 240;
}