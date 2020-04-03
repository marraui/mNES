#include <iostream>
#include "NES/cpu6502.h"
#include "NES/utils.h"
#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <map>

struct INSTRUCTION1 {
  std::string name;
  uint8_t (Cpu6502::*operate)(void);
  uint8_t (Cpu6502::*addrMode)(void);
  uint8_t cycles;
};

Cpu6502::Cpu6502(
  std::function<void(uint16_t addr, uint8_t data)> write,
  std::function<uint8_t(uint16_t addr, bool isReadonly)> read
) {
  using a = Cpu6502;
  this->lookup = {
    { "BRK", &a::BRK, "IMP", &a::IMP, 7, 1 }, { "ORA", &a::ORA, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ORA", &a::ORA, "ZP0", &a::ZP0, 3, 2 }, { "ASL", &a::ASL, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "PHP", &a::PHP, "IMP", &a::IMP, 3, 1 }, { "ORA", &a::ORA, "IMM", &a::IMM, 2, 2 }, { "ASL", &a::ASL, "ACC", &a::ACC, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ORA", &a::ORA, "ABS", &a::ABS, 4, 3 }, { "ASL", &a::ASL, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BPL", &a::BPL, "REL", &a::REL, 2, 2 }, { "ORA", &a::ORA, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ORA", &a::ORA, "ZPX", &a::ZPX, 4, 2 }, { "ASL", &a::ASL, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CLC", &a::CLC, "IMP", &a::IMP, 2, 1 }, { "ORA", &a::ORA, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ORA", &a::ORA, "ABX", &a::ABX, 4, 3 }, { "ASL", &a::ASL, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "JSR", &a::JSR, "ABS", &a::ABS, 6, 3 }, { "AND", &a::AND, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "BIT", &a::BIT, "ZP0", &a::ZP0, 3, 2 }, { "AND", &a::AND, "ZP0", &a::ZP0, 3, 2 }, { "ROL", &a::ROL, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "PLP", &a::PLP, "IMP", &a::IMP, 4, 1 }, { "AND", &a::AND, "IMM", &a::IMM, 2, 2 }, { "ROL", &a::ROL, "ACC", &a::ACC, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "BIT", &a::BIT, "ABS", &a::ABS, 4, 3 }, { "AND", &a::AND, "ABS", &a::ABS, 4, 3 }, { "ROL", &a::ROL, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BMI", &a::BMI, "REL", &a::REL, 2, 2 }, { "AND", &a::AND, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "AND", &a::AND, "ZPX", &a::ZPX, 4, 2 }, { "ROL", &a::ROL, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "SEC", &a::SEC, "IMP", &a::IMP, 2, 1 }, { "AND", &a::AND, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "AND", &a::AND, "ABX", &a::ABX, 4, 3 }, { "ROL", &a::ROL, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "RTI", &a::RTI, "IMP", &a::IMP, 6, 2 }, { "EOR", &a::EOR, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "EOR", &a::EOR, "ZP0", &a::ZP0, 3, 2 }, { "LSR", &a::LSR, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "PHA", &a::PHA, "IMP", &a::IMP, 3, 1 }, { "EOR", &a::EOR, "IMM", &a::IMM, 2, 2 }, { "LSR", &a::LSR, "ACC", &a::ACC, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "JMP", &a::JMP, "ABS", &a::ABS, 3, 3 }, { "EOR", &a::EOR, "ABS", &a::ABS, 4, 3 }, { "LSR", &a::LSR, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BVC", &a::BVC, "REL", &a::REL, 2, 2 }, { "EOR", &a::EOR, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "EOR", &a::EOR, "ZPX", &a::ZPX, 4, 2 }, { "LSR", &a::LSR, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CLI", &a::CLI, "IMP", &a::IMP, 2, 1 }, { "EOR", &a::EOR, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "EOR", &a::EOR, "ABX", &a::ABX, 4, 3 }, { "LSR", &a::LSR, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "RTS", &a::RTS, "IMP", &a::IMP, 6, 2 }, { "ADC", &a::ADC, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ADC", &a::ADC, "ZP0", &a::ZP0, 3, 2 }, { "ROR", &a::ROR, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "PLA", &a::PLA, "IMP", &a::IMP, 4, 1 }, { "ADC", &a::ADC, "IMM", &a::IMM, 2, 2 }, { "ROR", &a::ROR, "ACC", &a::ACC, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "JMP", &a::JMP, "IND", &a::IND, 5, 3 }, { "ADC", &a::ADC, "ABS", &a::ABS, 4, 3 }, { "ROR", &a::ROR, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BVS", &a::BVS, "REL", &a::REL, 2, 2 }, { "ADC", &a::ADC, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ADC", &a::ADC, "ZPX", &a::ZPX, 4, 2 }, { "ROR", &a::ROR, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "SEI", &a::SEI, "IMP", &a::IMP, 2, 1 }, { "ADC", &a::ADC, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "ADC", &a::ADC, "ABX", &a::ABX, 4, 3 }, { "ROR", &a::ROR, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "STA", &a::STA, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "STY", &a::STY, "ZP0", &a::ZP0, 3, 2 }, { "STA", &a::STA, "ZP0", &a::ZP0, 3, 2 }, { "STX", &a::STX, "ZP0", &a::ZP0, 3, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "DEY", &a::DEY, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "TXA", &a::TXA, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "STY", &a::STY, "ABS", &a::ABS, 4, 3 }, { "STA", &a::STA, "ABS", &a::ABS, 4, 3 }, { "STX", &a::STX, "ABS", &a::ABS, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BCC", &a::BCC, "REL", &a::REL, 2, 2 }, { "STA", &a::STA, "IZY", &a::IZY, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "STY", &a::STY, "ZPX", &a::ZPX, 4, 2 }, { "STA", &a::STA, "ZPX", &a::ZPX, 4, 2 }, { "STX", &a::STX, "ZPY", &a::ZPY, 4, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "TYA", &a::TYA, "IMP", &a::IMP, 2, 1 }, { "STA", &a::STA, "ABY", &a::ABY, 5, 3 }, { "TXS", &a::TXS, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "STA", &a::STA, "ABX", &a::ABX, 5, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "LDY", &a::LDY, "IMM", &a::IMM, 2, 2 }, { "LDA", &a::LDA, "IZX", &a::IZX, 6, 2 }, { "LDX", &a::LDX, "IMM", &a::IMM, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "LDY", &a::LDY, "ZP0", &a::ZP0, 3, 2 }, { "LDA", &a::LDA, "ZP0", &a::ZP0, 3, 2 }, { "LDX", &a::LDX, "ZP0", &a::ZP0, 3, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "TAY", &a::TAY, "IMP", &a::IMP, 2, 1 }, { "LDA", &a::LDA, "IMM", &a::IMM, 2, 2 }, { "TAX", &a::TAX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "LDY", &a::LDY, "ABS", &a::ABS, 4, 3 }, { "LDA", &a::LDA, "ABS", &a::ABS, 4, 3 }, { "LDX", &a::LDX, "ABS", &a::ABS, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BCS", &a::BCS, "REL", &a::REL, 2, 2 }, { "LDA", &a::LDA, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "LDY", &a::LDY, "ZPX", &a::ZPX, 4, 2 }, { "LDA", &a::LDA, "ZPX", &a::ZPX, 4, 2 }, { "LDX", &a::LDX, "ZPY", &a::ZPY, 4, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CLV", &a::CLV, "IMP", &a::IMP, 2, 1 }, { "LDA", &a::LDA, "ABY", &a::ABY, 4, 3 }, { "TSX", &a::TSX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "LDY", &a::LDY, "ABX", &a::ABX, 4, 3 }, { "LDA", &a::LDA, "ABX", &a::ABX, 4, 3 }, { "LDX", &a::LDX, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "CPY", &a::CPY, "IMM", &a::IMM, 2, 2 }, { "CMP", &a::CMP, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CPY", &a::CPY, "ZP0", &a::ZP0, 3, 2 }, { "CMP", &a::CMP, "ZP0", &a::ZP0, 3, 2 }, { "DEC", &a::DEC, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "INY", &a::INY, "IMP", &a::IMP, 2, 1 }, { "CMP", &a::CMP, "IMM", &a::IMM, 2, 2 }, { "DEX", &a::DEX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CPY", &a::CPY, "ABS", &a::ABS, 4, 3 }, { "CMP", &a::CMP, "ABS", &a::ABS, 4, 3 }, { "DEC", &a::DEC, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BNE", &a::BNE, "REL", &a::REL, 2, 2 }, { "CMP", &a::CMP, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CMP", &a::CMP, "ZPX", &a::ZPX, 4, 2 }, { "DEC", &a::DEC, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CLD", &a::CLD, "IMP", &a::IMP, 2, 1 }, { "CMP", &a::CMP, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CMP", &a::CMP, "ABX", &a::ABX, 4, 3 }, { "DEC", &a::DEC, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "CPX", &a::CPX, "IMM", &a::IMM, 2, 2 }, { "SBC", &a::SBC, "IZX", &a::IZX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CPX", &a::CPX, "ZP0", &a::ZP0, 3, 2 }, { "SBC", &a::SBC, "ZP0", &a::ZP0, 3, 2 }, { "INC", &a::INC, "ZP0", &a::ZP0, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "INX", &a::INX, "IMP", &a::IMP, 2, 1 }, { "SBC", &a::SBC, "IMM", &a::IMM, 2, 2 }, { "NOP", &a::NOP, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "CPX", &a::CPX, "ABS", &a::ABS, 4, 3 }, { "SBC", &a::SBC, "ABS", &a::ABS, 4, 3 }, { "INC", &a::INC, "ABS", &a::ABS, 6, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 },
    { "BEQ", &a::BEQ, "REL", &a::REL, 2, 2 }, { "SBC", &a::SBC, "IZY", &a::IZY, 5, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "SBC", &a::SBC, "ZPX", &a::ZPX, 4, 2 }, { "INC", &a::INC, "ZPX", &a::ZPX, 6, 2 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "SED", &a::SED, "IMP", &a::IMP, 2, 1 }, { "SBC", &a::SBC, "ABY", &a::ABY, 4, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }, { "SBC", &a::SBC, "ABX", &a::ABX, 4, 3 }, { "INC", &a::INC, "ABX", &a::ABX, 7, 3 }, { "???", &a::XXX, "IMP", &a::IMP, 2, 1 }
  };

  this->write = write;
  this->read = std::bind(read, std::placeholders::_1, false);
  this->readOnly = std::bind(read, std::placeholders::_1, true);
  std::cout << "CPU6502 callback initialization" << std::endl;
}

Cpu6502::~Cpu6502() { }

uint8_t Cpu6502::getStatus() {
  return this->processorStatusRegister;
}

uint8_t Cpu6502::getX() {
  return this->xRegister;
}

uint8_t Cpu6502::getY() {
  return this->yRegister;
}

uint16_t Cpu6502::getSP() {
  return this->stackPointer;
}

uint16_t Cpu6502::getPC() {
  return this->programCounter;
}

uint8_t Cpu6502::getAcc() {
  return this->accumulator;
}

std::map<uint16_t, std::string> Cpu6502::disassemble(uint16_t start, uint16_t end) {
  std::map<uint16_t, std::string> mapLines;
  uint32_t addr = start;
  uint32_t lineAddr;
  uint8_t low = 0x00;
  uint8_t high = 0x00;
  while (addr <= end) {
    lineAddr = addr;
    std::string instruction = "$" + hexToString(lineAddr, 4) + ": ";

    uint8_t opcode = this->readOnly(addr);
    addr += 1;
    instruction += this->lookup[opcode].name + " ";

    if (this->lookup[opcode].bytes == 2) {
      low = this->readOnly(addr);
      addr += 1;
      instruction += "$" + hexToString(low, 2) + " ";
    } else if (this->lookup[opcode].bytes == 3) {
      low = this->readOnly(addr);
      addr += 1;
      high = this->readOnly(addr);
      addr += 1;
      instruction += "$" + hexToString((uint16_t)(high << 8) || low, 4) + " ";
    }
    instruction += "{" + this->lookup[opcode].modeName + "}";
    mapLines[lineAddr] = instruction;
  }

  return mapLines;
}

uint8_t Cpu6502::fetch() {
  if (
    this->lookup[this->opcode].addrMode != &Cpu6502::ACC &&
    this->lookup[this->opcode].addrMode != &Cpu6502::IMP
  ) {
    this->fetched = this->read(this->addrAbs);
  }
  return this->fetched;
}

void Cpu6502::setFlag(Cpu6502::STATUS_FLAGS flag, bool val) {
  if (!val) {
    this->processorStatusRegister &= ~flag;
    return;
  }
  this->processorStatusRegister |= flag;
}

uint8_t Cpu6502::getFlag(Cpu6502::STATUS_FLAGS flag) {
  return (this->processorStatusRegister & flag) > 0 ? 1 : 0;
}

void Cpu6502::clock() {
  if (this->cycles > 0) {
    this->cycles--;
    return;
  }
  // Read operation code from memory and increase program counter
  this->opcode = this->read(this->programCounter);
  this->programCounter += 1;

  // Update cycles to match based on instruction
  this->cycles = this->lookup[this->opcode].cycles;

  // These results indicate whether it's gonna take additional cycles
  uint8_t addrModeResult = (this->*lookup[this->opcode].addrMode)();
  uint8_t operateResult = (this->*lookup[this->opcode].operate)();
  
  this->cycles += (addrModeResult & operateResult);
  this->cycles--;
}

//Interrupts

// Reset
void Cpu6502::reset() {
  this->stackPointer = 0xFD;
  this->accumulator = 0x00;
  this->xRegister = 0x00;
  this->yRegister = 0x00;
  this->processorStatusRegister = 0x00 | this->UNUSED_SF;

  this->addrAbs = 0x0000;
  this->addrRel = 0x0000;
  this->fetched = 0x0000;

  this->programCounter = ((uint16_t)this->read(0xFFFD) << 8) | (uint16_t)this->read(0xFFFC);

  this->cycles = 8;
}

// Non maskable interrupt
void Cpu6502::nmi() {
  this->write(0x100 + this->stackPointer, (this->programCounter >> 8) & 0x00FF);
  this->stackPointer -= 1;
  
  this->write(0x100 + this->stackPointer, this->programCounter & 0x00FF);
  this->stackPointer -= 1;

  this->setFlag(this->BRK_COMMAND_SF, 0);
  this->setFlag(this->UNUSED_SF, 1);
  this->setFlag(this->IRQ_DISABLE_SF, 1);
  this->write(0x100 + this->stackPointer, this->processorStatusRegister);
  this->stackPointer -= 1;

  this->cycles = 8;

  this->programCounter = ((uint16_t)this->read(0xFFFB) << 8) | (uint16_t)this->read(0xFFFA);
}

// Interrupt request
void Cpu6502::irq() {
  if (this->getFlag(this->IRQ_DISABLE_SF)) return;

  this->write(0x100 + this->stackPointer, (this->programCounter >> 8) & 0x00FF);
  this->stackPointer -= 1;

  this->write(0x100 + this->stackPointer, this->programCounter & 0x00FF);
  this->stackPointer -= 1;

  this->setFlag(this->BRK_COMMAND_SF, 0);
  this->setFlag(this->UNUSED_SF, 1);
  this->setFlag(this->IRQ_DISABLE_SF, 1);
  this->write(0x100 + this->stackPointer, this->processorStatusRegister);
  this->stackPointer -= 1;

  this->cycles = 7;

  this->programCounter = ((uint16_t)this->read(0xFFFF) << 8) | (uint16_t) this->read(0xFFFE);
}



// Accumulator Addressing
uint8_t Cpu6502::ACC() {
  this->fetched = this->accumulator;
  return 0;
}

// Immediate Addressing
uint8_t Cpu6502::IMM() {
  this->addrAbs = this->programCounter;
  this->programCounter += 1;
  return 0;
}

// Zero Page Addressing
uint8_t Cpu6502::ZP0() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;
  this->addrAbs = 0x00FF & low;
  return 0; 
}

// Indexed Zero Page Addressing X
uint8_t Cpu6502::ZPX() {
  uint8_t low = this->read(this->programCounter) + this->xRegister;
  this->programCounter += 1;
  this->addrAbs = 0x00FF & low;
  return 0;
}

// Indexed Zero Page Addressing Y
uint8_t Cpu6502::ZPY() {
  uint8_t low = this->read(this->programCounter) + this->yRegister;
  this->programCounter += 1;
  this->addrAbs = 0x00FF & low;
  return 0;
}

// Absolute Addressing
uint8_t Cpu6502::ABS() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;
  uint8_t high = this->read(this->programCounter);
  this->programCounter += 1;
  this->addrAbs = (uint16_t)(high << 8) | (uint16_t)low;
  return 0; 
}

// Indexed Absolute Addressing X
uint8_t Cpu6502::ABX() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t high = this->read(this->programCounter);
  this->programCounter += 1;

  this->addrAbs = ((uint16_t)(high << 8) | (uint16_t)low) + this->xRegister;
  return (this->addrAbs & 0xFF00) != (high << 8);
}

// Indexed Absolute Addresing Y
uint8_t Cpu6502::ABY() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t high = this->read(this->programCounter);
  this->programCounter +=1 ;

  this->addrAbs = ((uint16_t)(high << 8) | (uint16_t)low) + this->xRegister;
  return (this->addrAbs & 0xFF00) != (high << 8);
}

uint8_t Cpu6502::IMP() {
  return 0;
}

// Read for the realtive address
// It can only be between -128 to 127 (7 bits)
// Since it's signed the last bit of the byte is used to indicate whether it's negative
uint8_t Cpu6502::REL() {
  this->addrRel = this->read(this->programCounter);
  this->programCounter += 1;

  if (this->addrRel & 0x80) { // Check if last bit of low byte is set to check if it's negative
    // If it is then set all bits from the higher byte
    // To avoid overflowing when substracting from it
    this->addrRel |= 0xFF00;
  }
  return 0;
}

// Indirect (poniter)
uint8_t Cpu6502::IND() {
  uint8_t addrLow = this->read(this->programCounter);
  this->programCounter += 1;
  uint8_t addrHigh = this->read(this->programCounter);
  this->programCounter += 1;
  uint16_t addr = (addrHigh << 8) | addrLow;

  uint8_t low = this->read(addr);
  uint8_t high;

  // Simulate 6502 bug
  if ((addr & 0x00FF) == 0xFF) high = this->read(addr & 0xFF00);
  else high = this->read(addr + 1);

  this->addrAbs = (uint16_t)(high << 8) | (uint16_t)low;
  return 0;
}

// Indexed Indirect Addressing (IND, X)
uint8_t Cpu6502::IZX() {
  uint8_t addr = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t low = this->read(addr + this->xRegister);
  uint8_t high = this->read(addr + 1 + this->xRegister);
  this->addrAbs = (uint16_t)(high << 8) | (uint16_t)low;

  return 0;
}

// Indirect Indexed Addressing (IND, Y)
uint8_t Cpu6502::IZY() {
  uint8_t addr = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t low = this->read(addr);
  uint8_t high = this->read(addr + 1);

  this->addrAbs = ((uint16_t)(high << 8) | (uint16_t)low) + this->yRegister;

  return (this->addrAbs & 0xFF00) != (high << 8);
}

// Add with carry
uint8_t Cpu6502::ADC() {
  this->fetch();
  
  uint16_t result =
    (uint16_t)this->accumulator +
    (uint16_t)this->fetched +
    (uint16_t)this->getFlag(this->CARRY_SF);
  
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, (0x00FF & result) == 0);
  this->setFlag(this->CARRY_SF, result > 0xFF);
  this->setFlag(
    this->OVERFLOW_SF, 
    (
      ~((uint16_t)this->accumulator ^ (uint16_t)this->fetched) &
      ((uint16_t)this->accumulator ^ (uint16_t)result) &
      0x0080
    )
  );

  this->accumulator = result & 0x00FF;
  return 1;
}

// AND operation
uint8_t Cpu6502::AND() {
  this->fetch();
  this->accumulator &= this->fetched;
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  return 1;
}

// Shift left one bit
uint8_t Cpu6502::ASL() {
  this->fetch();
  uint16_t result = (uint16_t)this->fetched << 1;
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, (result & 0x00FF) == 0);
  this->setFlag(this->CARRY_SF, (result & 0xFF00) > 0);

  if (this->lookup[this->opcode].addrMode == &Cpu6502::ACC) {
    this->accumulator = result;
  } else {
    this->write(this->addrAbs, result & 0x00FF);
  }
  return 0;
}

// Branch on carry clear
uint8_t Cpu6502::BCC() {
  if (this->getFlag(this->CARRY_SF)) return 0;

  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;

  this->programCounter = result;
  return 0;
}

// Branch on carry set
uint8_t Cpu6502::BCS() {
  if (!this->getFlag(this->CARRY_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Branch on result zero
uint8_t Cpu6502::BEQ() {
  if (!this->getFlag(this->ZERO_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Test bits in memory with accumulator
// bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V)
// the zeroflag is set to the result of operand AND accumulator.
uint8_t Cpu6502::BIT() {
  this->fetch();
  uint8_t result = this->accumulator & this->fetched;
  this->setFlag(this->NEGATIVE_SF, (this->fetched & (1 << 7)) > 0);
  this->setFlag(this->OVERFLOW_SF, (this->fetched & (1 << 6)) > 0);
  this->setFlag(this->ZERO_SF, result == 0);
  return 0;
}

// Branch on result minus
uint8_t Cpu6502::BMI() {
  if (!this->getFlag(this->NEGATIVE_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Branch on result not zero
uint8_t Cpu6502::BNE() {
  if (this->getFlag(this->ZERO_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Branch on result plus
uint8_t Cpu6502::BPL() {
  if (this->getFlag(this->NEGATIVE_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Force Break
uint8_t Cpu6502::BRK() {
  this->programCounter += 1;
  
  this->write(0x0100 + this->stackPointer, (this->programCounter >> 8) & 0x00FF);
  this->stackPointer -= 1;

  this->write(0x100 + this->stackPointer, (this->programCounter & 0x00FF));
  this->stackPointer -= 1;

  this->setFlag(this->IRQ_DISABLE_SF, 1);
  this->setFlag(this->BRK_COMMAND_SF, 1);
  this->write(0x100 + this->stackPointer, this->processorStatusRegister);
  this->stackPointer -= 1;
  this->setFlag(this->BRK_COMMAND_SF, 0);

  this->programCounter = ((uint16_t)this->read(0xFFFF) << 8) | (uint16_t)this->read(0xFFFE);
  return 0;
}

// Branch if overflow clear
uint8_t Cpu6502::BVC() {
  if (this->getFlag(this->OVERFLOW_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Branch if overflow set
uint8_t Cpu6502::BVS() {
  if (!this->getFlag(this->NEGATIVE_SF)) return 0;
  
  this->cycles += 1;
  uint16_t result = this->programCounter + this->addrRel;

  if ((result & 0xFF00) != (this->programCounter & 0xFF00))
    this->cycles += 1;
  
  this->programCounter = result;
  return 0;
}

// Clear carry
uint8_t Cpu6502::CLC() {
  this->setFlag(this->CARRY_SF, 0);
  return 0;
}

// Clear decimal
uint8_t Cpu6502::CLD() {
  this->setFlag(this->DECIMAL_MODE_SF, 0);
  return 0;
}

// Clear IRQ_DISABlE
uint8_t Cpu6502::CLI() {
  this->setFlag(this->IRQ_DISABLE_SF, 0);
  return 0;
}

// Clear overflow
uint8_t Cpu6502::CLV() {
  this->setFlag(this->OVERFLOW_SF, 0);
  return 0;
}

// Compare accumulator to memory
uint8_t Cpu6502::CMP() {
  this->fetch();
  uint8_t result = this->accumulator - this->fetched;
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->CARRY_SF, this->accumulator >= this->fetched);
  return 0;
}

// Compare x to memory
uint8_t Cpu6502::CPX() {
  this->fetch();
  uint8_t result = this->xRegister - this->fetched;
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->CARRY_SF, this->xRegister >= this->fetched);
  return 0;
}

// Compare y to memory
uint8_t Cpu6502::CPY() {
  this->fetch();
  uint8_t result = this->yRegister - this->fetched;
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->CARRY_SF, this->yRegister >= this->fetched);
  return 0;
}

// Decrement memory
uint8_t Cpu6502::DEC() {
  this->fetch();
  uint8_t result = this->fetched - 1;
  this->write(this->addrAbs, result);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  return 0;
}

// Decrement x
uint8_t Cpu6502::DEX() {
  this->xRegister -= 1;
  this->setFlag(this->ZERO_SF, this->xRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->xRegister & 0x80);
  return 0;
}

// Decrement y
uint8_t Cpu6502::DEY() {
  this->yRegister -= 1;
  this->setFlag(this->ZERO_SF, this->yRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->yRegister & 0x80);
  return 0;
}

// Exclusive OR
uint8_t Cpu6502::EOR() {
  this->fetch();
  this->accumulator ^= this->fetched;
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 0;
}

// Increment memory
uint8_t Cpu6502::INC() {
  this->fetch();
  uint8_t result = this->fetched + 1;
  this->write(this->addrAbs, result);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  return 0;
}

// Increment x
uint8_t Cpu6502::INX() {
  this->xRegister += 1;
  this->setFlag(this->ZERO_SF, this->xRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->xRegister & 0x80);
  return 0;
}

// Increment y
uint8_t Cpu6502::INY() {
  this->yRegister += 1;
  this->setFlag(this->ZERO_SF, this->yRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->yRegister & 0x80);
  return 0;
}

// Jump
uint8_t Cpu6502::JMP() {
  this->programCounter = this->addrAbs;
  return 0;
}

// Jump to subroutine
uint8_t Cpu6502::JSR() {
  this->programCounter -= 1;

  this->write(0x0100 + this->stackPointer, this->programCounter >> 8);
  this->stackPointer -= 1;
  this->write(0x0100 + this->stackPointer, this->programCounter & 0x00FF);
  this->stackPointer -= 1;

  this->programCounter = this->addrAbs;
  return 0;
}

// Load accumulator
uint8_t Cpu6502::LDA() {
  this->fetch();
  this->accumulator = this->fetched;
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 1;
}

// Load x
uint8_t Cpu6502::LDX() {
  this->fetch();
  this->xRegister = this->fetched;
  this->setFlag(this->ZERO_SF, this->xRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 1;
}

// Load y
uint8_t Cpu6502::LDY() {
  this->fetch();
  this->yRegister = this->fetched;
  this->setFlag(this->ZERO_SF, this->yRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 1;
}

// Logical shift right
uint8_t Cpu6502::LSR() {
  this->fetch();
  uint8_t result = this->fetched >> 1;
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->CARRY_SF, this->fetched & 0x01);
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  if (this->lookup[this->opcode].addrMode == &Cpu6502::ACC) {
    this->accumulator = result;
  } else {
    this->write(this->addrAbs, result);
  }
  return 0;
}

// No operation
uint8_t Cpu6502::NOP() {
  // Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
  return 0;
}

// Logic inclusive or on accumulator
uint8_t Cpu6502::ORA() {
  this->fetch();
  this->accumulator |= this->fetched;
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 1;
}

// Push accumulator onto stack
uint8_t Cpu6502::PHA() {
  this->write(0x0100 + this->stackPointer, this->accumulator >> 8);
  this->stackPointer -= 1;
  this->write(0x0100 + this->stackPointer, this->accumulator & 0x00FF);
  this->stackPointer -= 1;
  return 0;
}

// Push processor status onto stack
uint8_t Cpu6502::PHP() {
  this->write(0x0100 + this->stackPointer, this->processorStatusRegister | this->UNUSED_SF | this->BRK_COMMAND_SF);
  this->stackPointer -= 1;
  return 0;
}

// Pull accumulator
uint8_t Cpu6502::PLA() {
  this->stackPointer += 1;
  this->accumulator = this->read(this->stackPointer);
  return 0;
}

// Pull processor status
uint8_t Cpu6502::PLP() {
  this->stackPointer += 1;
  this->processorStatusRegister = this->read(this->stackPointer);
  return 0;
}

// Rotate left (like shift left but carry gets put into bit 0)
uint8_t Cpu6502::ROL() {
  this->fetch();
  bool newCarry = this->fetched & 0x80;
  uint8_t result = (this->fetched << 1) | this->getFlag(this->CARRY_SF);
  this->setFlag(this->CARRY_SF, newCarry);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  if (this->lookup[this->opcode].addrMode == &Cpu6502::ACC) {
    this->accumulator = result;
  } else {
    this->write(this->addrAbs, result);
  }
  return 0;
}

// Rotate right
uint8_t Cpu6502::ROR() {
  this->fetch();
  bool newCarry = this->fetched & 0x01;
  uint8_t result = (this->fetched >> 1) | (this->getFlag(this->CARRY_SF) << 8);
  this->setFlag(this->CARRY_SF, newCarry);
  this->setFlag(this->ZERO_SF, result == 0);
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  if (this->lookup[this->opcode].addrMode == &Cpu6502::ACC) {
    this->accumulator = result;
  } else {
    this->write(this->addrAbs, result);
  }
  return 0;
}

// Return from interrupt
uint8_t Cpu6502::RTI() {
  this->stackPointer += 1;
  this->processorStatusRegister = this->read(0x100 + this->stackPointer);
  
  this->stackPointer += 1;
  uint8_t low = this->read(0x0100 + this->stackPointer);
  this->stackPointer += 1;
  uint8_t high = this->read(0x0100 + this->stackPointer);

  this->programCounter = (uint16_t)(high << 8) | (uint16_t)low;
  return 0;
}

// Return from subroutine
uint8_t Cpu6502::RTS() {
  this->stackPointer += 1;
  uint8_t low = this->read(0x0100 + this->stackPointer);
  this->stackPointer += 1;
  uint8_t high = this->read(0x0100 + this->stackPointer);

  this->programCounter = (uint16_t)(high << 8) | (uint16_t)low;
  this->programCounter += 1;

  return 0;
}

// Subtraction with carry
// A - M - (1 - C) -> A - M - 1 + C -> A + (~M + 1) - 1 + C -> A + ~M + C
uint8_t Cpu6502::SBC() {
  this->fetch();

  uint16_t value = ((uint16_t)this->fetched) ^ 0x00FF;
  
  uint16_t result =
    (uint16_t)this->accumulator +
    value +
    (uint16_t)this->getFlag(this->CARRY_SF);
  
  this->setFlag(this->NEGATIVE_SF, result & 0x80);
  this->setFlag(this->ZERO_SF, (0x00FF & result) == 0);
  this->setFlag(this->CARRY_SF, result > 0xFF);
  this->setFlag(
    this->OVERFLOW_SF, 
    (
      ~((uint16_t)this->accumulator ^ (uint16_t)this->fetched) &
      ((uint16_t)this->accumulator ^ (uint16_t)result) &
      0x0080
    )
  );

  this->accumulator = result & 0x00FF;
  return 1;
}

// Set carri flag
uint8_t Cpu6502::SEC() {
  this->setFlag(this->CARRY_SF, true);
  return 0;
}

// Set Decimal flag 
uint8_t Cpu6502::SED() {
  this->setFlag(this->DECIMAL_MODE_SF, true);
  return 0;
}

// Set interrupt disable
uint8_t Cpu6502::SEI() {
  this->setFlag(this->IRQ_DISABLE_SF, true);
  return 0;
}

// Store accumulutator
uint8_t Cpu6502::STA() {
  this->write(this->addrAbs, this->accumulator);
  return 0;
}

// Store x
uint8_t Cpu6502::STX() {
  this->write(this->addrAbs, this->xRegister);
  return 0;
}

// Store y
uint8_t Cpu6502::STY() {
  this->write(this->addrAbs, this->yRegister);
  return 0;
}

// Transfer accumulator to x
uint8_t Cpu6502::TAX() {
  this->xRegister = this->accumulator;
  this->setFlag(this->ZERO_SF, this->xRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->xRegister & 0x80);
  return 0;
}

// Transfer accumulator to y
uint8_t Cpu6502::TAY() {
  this->yRegister = this->accumulator;
  this->setFlag(this->ZERO_SF, this->yRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->yRegister & 0x80);
  return 0;
}

// Transfer stack pointer to x
uint8_t Cpu6502::TSX() {
  this->xRegister = this->stackPointer;
  this->setFlag(this->ZERO_SF, this->xRegister == 0);
  this->setFlag(this->NEGATIVE_SF, this->yRegister & 0x80);
  return 0;
}

// Transfer x to accumulator
uint8_t Cpu6502::TXA() {
  this->accumulator = this->xRegister;
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 0;
}

// Transfer x to stack pointer
uint8_t Cpu6502::TXS() {
  this->stackPointer = this->xRegister;
  return 0;
}

// Transfer y to accumulator 
uint8_t Cpu6502::TYA() {
  this->accumulator = this->yRegister;
  this->setFlag(this->ZERO_SF, this->accumulator == 0);
  this->setFlag(this->NEGATIVE_SF, this->accumulator & 0x80);
  return 0;
}

uint8_t Cpu6502::XXX() {
  return 0;
}
