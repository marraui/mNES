#ifndef PROCESSOR_WINDOW_H
#define PROCESSOR_WINDOW_H
#include "window_manager.h"
#include "text_texture.h"
#include "cpu6502.h"
#include <cstdint>

class ProcessorWindow: public WindowManager {
  private:
    TextTexture textTexture;
  public:
    ProcessorWindow();
    ~ProcessorWindow();
    void renderRAM(uint8_t ram[64 * 1064], uint16_t page);
    void renderProcessor(Cpu6502* cpu);
};

#endif // PROCESSOR_WINDOW_H