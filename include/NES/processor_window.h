#ifndef PROCESSOR_WINDOW_H
#define PROCESSOR_WINDOW_H
#include "window_manager.h"
#include "text_texture.h"
#include "cpu6502.h"
#include <cstdint>
#include <map>
#include <string>

class ProcessorWindow: public WindowManager {
  private:
    TextTexture textTexture;
  public:
    ProcessorWindow();
    ~ProcessorWindow();
    void renderRAM(uint8_t ram[64 * 1064], uint16_t page);
    void renderProcessor(Cpu6502* cpu);
    void renderCode(std::map<uint16_t, std::string> mapLines, uint16_t currentLine, int linesToRender);
};

#endif // PROCESSOR_WINDOW_H