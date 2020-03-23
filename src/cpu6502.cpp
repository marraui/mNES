#include <iostream>
#include "NES/cpu6502.h"
#include <functional>
#include <vector>

struct INSTRUCTION1 {
  std::string name;
  uint8_t (Cpu6502::* operate)(void);
  uint8_t (Cpu6502::* addrMode)(void);
  uint8_t cycles;
};

Cpu6502::Cpu6502(
  std::function<void(uint16_t addr, uint8_t data)> write,
  std::function<uint8_t(uint16_t addr)> read
) {
  using a = Cpu6502;
  this->lookup = {
    { "BRK", &a::BRK, &a::IMP, 7 }, { "ORA", &a::ORA, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ZP0, 3 }, { "ASL", &a::ASL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "PHP", &a::PHP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::IMM, 2 }, { "ASL", &a::ASL, &a::ACC, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ABS, 4 }, { "ASL", &a::ASL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BPL", &a::BPL, &a::REL, 2 }, { "ORA", &a::ORA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ZPX, 4 }, { "ASL", &a::ASL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "CLC", &a::CLC, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ABX, 4 }, { "ASL", &a::ASL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 },
    { "JSR", &a::JSR, &a::ABS, 6 }, { "AND", &a::AND, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "BIT", &a::BIT, &a::ZP0, 3 }, { "AND", &a::AND, &a::ZP0, 3 }, { "ROL", &a::ROL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "PLP", &a::PLP, &a::IMP, 4 }, { "AND", &a::AND, &a::IMM, 2 }, { "ROL", &a::ROL, &a::ACC, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "BIT", &a::BIT, &a::ABS, 4 }, { "AND", &a::AND, &a::ABS, 4 }, { "ROL", &a::ROL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BMI", &a::BMI, &a::REL, 2 }, { "AND", &a::AND, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "AND", &a::AND, &a::ZPX, 4 }, { "ROL", &a::ROL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "SEC", &a::SEC, &a::IMP, 2 }, { "AND", &a::AND, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "AND", &a::AND, &a::ABX, 4 }, { "ROL", &a::ROL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 },
    { "RTI", &a::RTI, &a::IMP, 6 }, { "EOR", &a::EOR, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ZP0, 3 }, { "LSR", &a::LSR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "PHA", &a::PHA, &a::IMP, 3 }, { "EOR", &a::EOR, &a::IMM, 2 }, { "LSR", &a::LSR, &a::ACC, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::ABS, 3 }, { "EOR", &a::EOR, &a::ABS, 4 }, { "LSR", &a::LSR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BVC", &a::BVC, &a::REL, 2 }, { "EOR", &a::EOR, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ZPX, 4 }, { "LSR", &a::LSR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "CLI", &a::CLI, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ABX, 4 }, { "LSR", &a::LSR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 },
    { "RTS", &a::RTS, &a::IMP, 6 }, { "ADC", &a::ADC, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ZP0, 3 }, { "ROR", &a::ROR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "PLA", &a::PLA, &a::IMP, 4 }, { "ADC", &a::ADC, &a::IMM, 2 }, { "ROR", &a::ROR, &a::ACC, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::IND, 5 }, { "ADC", &a::ADC, &a::ABS, 4 }, { "ROR", &a::ROR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BVS", &a::BVS, &a::REL, 2 }, { "ADC", &a::ADC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ZPX, 4 }, { "ROR", &a::ROR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "SEI", &a::SEI, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ABX, 4 }, { "ROR", &a::ROR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 },
    { "???", &a::XXX, &a::IMP, 2 }, { "STA", &a::STA, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STY", &a::STY, &a::ZP0, 3 }, { "STA", &a::STA, &a::ZP0, 3 }, { "STX", &a::STX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 2 }, { "DEY", &a::DEY, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "TXA", &a::TXA, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STY", &a::STY, &a::ABS, 4 }, { "STA", &a::STA, &a::ABS, 4 }, { "STX", &a::STX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BCC", &a::BCC, &a::REL, 2 }, { "STA", &a::STA, &a::IZY, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STY", &a::STY, &a::ZPX, 4 }, { "STA", &a::STA, &a::ZPX, 4 }, { "STX", &a::STX, &a::ZPX, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "TYA", &a::TYA, &a::IMP, 2 }, { "STA", &a::STA, &a::ABY, 5 }, { "TXS", &a::TXS, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STA", &a::STA, &a::ABX, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 },
    { "LDY", &a::LDY, &a::IMM, 2 }, { "LDA", &a::LDA, &a::IZX, 6 }, { "LDX", &a::LDX, &a::IMM, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ZP0, 3 }, { "LDA", &a::LDA, &a::ZP0, 3 }, { "LDX", &a::LDX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 2 }, { "TAY", &a::TAY, &a::IMP, 2 }, { "LDA", &a::LDA, &a::IMM, 2 }, { "TAX", &a::TAX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ABS, 4 }, { "LDA", &a::LDA, &a::ABS, 4 }, { "LDX", &a::LDX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BNE", &a::BNE, &a::REL, 2 }, { "CMP", &a::CMP, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ZPX, 4 }, { "LDA", &a::LDA, &a::ZPX, 4 }, { "LDX", &a::LDX, &a::ZPX, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "CLV", &a::CLV, &a::IMP, 2 }, { "LDA", &a::LDA, &a::ABY, 4 }, { "TSX", &a::TSX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ABX, 4 }, { "LDA", &a::LDA, &a::ABX, 4 }, { "LDX", &a::LDX, &a::ABX, 4 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BCS", &a::BCS, &a::REL, 2 }, { "LDA", &a::LDA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPY", &a::CPY, &a::ZP0, 3 }, { "CMP", &a::STA, &a::ZP0, 3 }, { "DEC", &a::DEC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "INY", &a::INY, &a::IMP, 2 }, { "CMP", &a::CMP, &a::IMM, 2 }, { "DEX", &a::DEX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPY", &a::CPY, &a::ABS, 4 }, { "CMP", &a::CMP, &a::ABS, 4 }, { "DEC", &a::DEC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "CPY", &a::CPY, &a::IMM, 2 }, { "CMP", &a::CMP, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CMP", &a::STA, &a::ZPX, 4 }, { "DEC", &a::DEC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "CLD", &a::CLD, &a::IMP, 2 }, { "CMP", &a::CMP, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CMP", &a::CMP, &a::ABX, 4 }, { "DEC", &a::DEC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 },
    { "CPX", &a::CPX, &a::IMM, 2 }, { "SBC", &a::SBC, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPX", &a::CPX, &a::ZP0, 3 }, { "SBC", &a::LDA, &a::ZP0, 3 }, { "ROR", &a::INC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "INX", &a::INX, &a::IMP, 2 }, { "SBC", &a::SBC, &a::IMM, 2 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPX", &a::CPX, &a::ABS, 4 }, { "SBC", &a::SBC, &a::ABS, 4 }, { "INC", &a::INC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 2 },
    { "BEQ", &a::BEQ, &a::REL, 2 }, { "SBC", &a::SBC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "SBC", &a::LDA, &a::ZPX, 4 }, { "ROR", &a::INC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "SED", &a::SED, &a::IMP, 2 }, { "SBC", &a::SBC, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "SBC", &a::SBC, &a::ABX, 4 }, { "INC", &a::INC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 2 }
  };

  this->write = write;
  this->read = read;
  std::cout << "CPU6502 callback initialization" << std::endl;
}

Cpu6502::~Cpu6502() { }

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
  this->addrAbs = (high << 8) | low;
  return 0; 
}

// Indexed Absolute Addressing X
uint8_t Cpu6502::ABX() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t high = this->read(this->programCounter);
  this->programCounter += 1;

  this->addrAbs = ((high << 8) | low) + this->xRegister;
  return (this->addrAbs & 0xFF00) != (high << 8);
}

// Indexed Absolute Addresing Y
uint8_t Cpu6502::ABY() {
  uint8_t low = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t high = this->read(this->programCounter);
  this->programCounter +=1 ;

  this->addrAbs = ((high << 8) | low) + this->xRegister;
  return (this->addrAbs && 0xFF00) != (high << 8);
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

uint8_t Cpu6502::IND() {
  uint8_t addr = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t low = this->read(addr);
  uint8_t high = this->read(addr + 1);

  this->addrAbs = (high << 8) | low;
  return 0;
}

// Indexed Indirect Addressing (IND, X)
uint8_t Cpu6502::IZX() {
  uint8_t addr = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t low = this->read(addr + this->xRegister);
  uint8_t high = this->read(addr + 1 + this->xRegister);

  this->addrAbs = (high << 8) | low;

  return 0;
}

// Indirect Indexed Addressing (IND, Y)
uint8_t Cpu6502::IZY() {
  uint8_t addr = this->read(this->programCounter);
  this->programCounter += 1;

  uint8_t low = this->read(addr);
  uint8_t high = this->read(addr + 1);

  this->addrAbs = ((high << 8) | low) + this->yRegister;

  return (this->addrAbs & 0xFF00) != (high << 8);
}
