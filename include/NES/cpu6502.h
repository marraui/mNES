#ifndef CPU6502_H
#define CPU6502_H

#include <cstdint>
#include <functional>

class Cpu6502 {
  private:
    // CPU core registers
    u_int16_t programCounter = 0x0000;
    u_int8_t accumulator = 0x00;
    u_int8_t xRegister = 0x00;
    u_int8_t yRegister = 0x00;
    u_int8_t stackPointer = 0x00;
    u_int8_t processorStatusRegister = 0x00;

    // Aux variables
    uint8_t cycles = 0; // Cycles remaining from instruction
    uint32_t clockCount = 0; // Global accumulation of clock cycles
    uint8_t fetched = 0x00; // Data fetched from ram
    uint8_t opcode = 0x00; // Instruction byte

    // Addresing modes
    uint8_t ACC();  uint8_t ABY();
    uint8_t IMM();  uint8_t IMP();
    uint8_t ABS();  uint8_t REL();
    uint8_t ZP0();  uint8_t IND();
    uint8_t ZPX();  uint8_t IZX();
    uint8_t ZPY();  uint8_t IZY();
    uint8_t ABX();
    
    // OP Codes
    uint8_t ADC();
    uint8_t AND();
    uint8_t ASL();

    uint8_t BCC();
    uint8_t BCS();
    uint8_t BEQ();
    uint8_t BIT();
    uint8_t BMI();
    uint8_t BNE();
    uint8_t BPM();
    uint8_t BRK();
    uint8_t BVC();
    uint8_t BVS();

    uint8_t CLC();
    uint8_t CLD();
    uint8_t CLI();
    uint8_t CLV();
    uint8_t CMP();
    uint8_t CPX();
    uint8_t CPY();

    uint8_t DEC();
    uint8_t DEX();
    uint8_t DEY();
    
    uint8_t EOR();
    
    uint8_t INC();
    uint8_t INX();
    uint8_t INY();


    uint8_t JMP();
    uint8_t JSR();
    
    uint8_t LDA();
    uint8_t LDX();
    uint8_t LDY();
    uint8_t LSR();

    uint8_t NOP();

    uint8_t ORA();

    uint8_t PHA();
    uint8_t PHP();
    uint8_t PLA();
    uint8_t PLP();

    uint8_t ROL();
    uint8_t ROR();
    uint8_t RTI();
    uint8_t RTS();

    uint8_t SBC();
    uint8_t SEC();
    uint8_t SED();
    uint8_t SEI();
    uint8_t STA();
    uint8_t STX();
    uint8_t STY();

    uint8_t TAX();
    uint8_t TAY();
    uint8_t TSX();
    uint8_t TXA();
    uint8_t TXS();
    uint8_t TYA();

  public:
    // Initialize with read and write callbacks
    Cpu6502(
      std::function<void(uint16_t addr, uint8_t data)> write,
      std::function<uint8_t(uint16_t addr)> read
    );
    ~Cpu6502(); 
    std::function<void(uint16_t, uint8_t)> write;
    std::function<uint8_t(uint16_t)> read;

    // Execute one clock cycle
    void clock();

    // Read from the ram and store it into fetched
    void fetch();

    enum STATUS_FLAGS {
      CARRY = (1 << 0),
      ZERO = (1 << 1),
      IRQ_DISABLE = (1 << 2),
      DECIMAL_MODE = (1 << 3),
      BRK_COMMAND = (1 << 4),
      UNUSED = (1 << 5),
      OVERFLOW = (1 << 6),
      NEGATIVE = (1 << 7),
    };
};

#endif /* CPU6502_h */
