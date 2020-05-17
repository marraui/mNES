#include "NES/processor_window.h"
#include "NES/text_texture.h"
#include "NES/utils.h"
#include "NES/cpu6502.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <SDL2/SDL.h>

ProcessorWindow::ProcessorWindow() {
  WindowManager();
  this->textTexture.loadFont("assets/PressStart2P.ttf", 8);
}

ProcessorWindow::~ProcessorWindow() {}

void ProcessorWindow::renderRAM(uint8_t ram[2048], uint16_t page) {
  uint32_t offset = page * 256;
  for (int i = 0; i < 16; i++) {
    std::string row = "$" + hexToString(page, 2) + hexToString(i, 1) + "0: ";
    for (int j = 0; j < 16; j++) {
      row += " " + hexToString(ram[offset + i * 16 + j], 2);
    }
    this->textTexture.loadFromRenderedText(row, this->renderer, { 0xFF, 0xFF, 0xFF });
    SDL_Rect renderQuad = { 0, i * 10, this->textTexture.getWidth(), this->textTexture.getHeight() };
    this->render(&(this->textTexture), &renderQuad);
  }
}

void ProcessorWindow::renderProcessor(Cpu6502* cpu) {
  std::string statusText = "STATUS: N V - B D I Z C";
  std::string greenText = statusText;
  std::string redText = statusText;
  std::for_each(greenText.begin(), greenText.end(), [cpu](char &c) -> void {
    Cpu6502::STATUS_FLAGS flag;
    
    switch (c) {
      case 'N':
        flag = cpu->NEGATIVE_SF;
        break;
      case '-':
        flag = cpu->UNUSED_SF;
        break;
      case 'B':
        flag = cpu->BRK_COMMAND_SF;
        break;
      case 'D':
        flag = cpu->DECIMAL_MODE_SF;
        break;
      case 'I':
        flag = cpu->IRQ_DISABLE_SF;
        break;
      case 'Z':
        flag = cpu->ZERO_SF;
        break;
      case 'C':
        flag = cpu->CARRY_SF;
        break;
      default:
        c = ' ';
        return;
    }
    c = cpu->getFlag(flag) ? c : ' ';
  });
  std::for_each(redText.begin(), redText.end(), [cpu](char &c) -> void {
    Cpu6502::STATUS_FLAGS flag;
    
    switch (c) {
      case 'N':
        flag = cpu->NEGATIVE_SF;
        break;
      case 'V':
        flag = cpu->OVERFLOW_SF;
        break;
      case '-':
        flag = cpu->UNUSED_SF;
        break;
      case 'B':
        flag = cpu->BRK_COMMAND_SF;
        break;
      case 'D':
        flag = cpu->DECIMAL_MODE_SF;
        break;
      case 'I':
        flag = cpu->IRQ_DISABLE_SF;
        break;
      case 'Z':
        flag = cpu->ZERO_SF;
        break;
      case 'C':
        flag = cpu->CARRY_SF;
        break;
      default:
        c = ' ';
        return;
    }
    c = cpu->getFlag(flag) ? ' ' : c;
  });
  this->textTexture.loadFromRenderedText(statusText, this->renderer, { 0xFF, 0xFF, 0xFF });
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, 16 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
  this->textTexture.loadFromRenderedText(greenText, this->renderer, { 0x00, 0xFF, 0x00 });
  this->render(
    &this->textTexture, 0,
    new SDL_Rect{ 0, 16 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
  this->textTexture.loadFromRenderedText(redText, this->renderer, { 0xFF, 0x00, 0x00 });
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, 16 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );

  std::string str = "PC: $" + hexToString(cpu->getPC(), 4) + "  [" + std::to_string(cpu->getPC()) + "]";
  this->textTexture.loadFromRenderedText(str, this->renderer, { 0xFF, 0xFF, 0xFF });
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, 17 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
  str = "A: $" + hexToString(cpu->getAcc(), 2) + " [" + std::to_string(cpu->getAcc()) + "]";
  this->textTexture.loadFromRenderedText(str, this->renderer, { 0xFF, 0xFF, 0xFF });
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, 18 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
  str = "X: $" + hexToString(cpu->getX(), 2) + " [" + std::to_string(cpu->getX()) + "]";
  this->textTexture.loadFromRenderedText(str, this->renderer, { 0xFF, 0xFF, 0xFF });
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, 19 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
  str = "Y: $" + hexToString(cpu->getY(), 2) + " [" + std::to_string(cpu->getY()) + "]";
  this->textTexture.loadFromRenderedText(str, this->renderer, { 0xFF, 0xFF, 0xFF });
  this->render(
    &this->textTexture, 0,
    new SDL_Rect{ 0, 20 * 10, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );
}

void ProcessorWindow::renderCode(std::map<uint16_t, std::string> mapLines, uint16_t currentLine, int linesToRender) {
  int lineCount = linesToRender;
  int yOffset = 21 * 10 + ((linesToRender >> 1) * 10);
  std::map<uint16_t, std::string>::iterator mapIterator = mapLines.find(currentLine);
  if (mapIterator != mapLines.end()) {
    this->textTexture.loadFromRenderedText((*mapIterator).second, this->renderer, { 0x00, 0xFF, 0xFF });
    this->render(
      &this->textTexture,
      new SDL_Rect{ 0, yOffset, this->textTexture.getWidth(), this->textTexture.getHeight() }
    );
    yOffset += 10;
    lineCount -= 1;
    mapIterator++;
    while (mapIterator != mapLines.end() && lineCount > (linesToRender >> 1)) {
      this->textTexture.loadFromRenderedText((*mapIterator).second, this->renderer, { 0xFF, 0xFF, 0xFF });
      this->render(
        &this->textTexture,
        new SDL_Rect{ 0, yOffset, this->textTexture.getWidth(), this->textTexture.getHeight() }
      );
      yOffset += 10;
      mapIterator++;
      lineCount -= 1;
    }
  }
  this->textTexture.loadFromRenderedText(
    "<-: PREV PAGE  ->: NEXT PAGE  SPACE: NEXT INSTRUCTION  I: IRQ  N: NMI  R: RESET",
    this->renderer,
    { 0xFF, 0xFF, 0xFF }
  );
  this->render(
    &this->textTexture,
    new SDL_Rect{ 0, yOffset, this->textTexture.getWidth(), this->textTexture.getHeight() }
  );

  mapIterator = mapLines.find(currentLine);
  yOffset = 21 * 10 + ((linesToRender >> 1) * 10);
  if (mapIterator != mapLines.end()) {
    mapIterator--;

    while (mapIterator != mapLines.end() && lineCount > 0) {
      yOffset -= 10;
      this->textTexture.loadFromRenderedText((*mapIterator).second, this->renderer, { 0xFF, 0xFF, 0xFF });
      this->render(
        &this->textTexture,
        new SDL_Rect{ 0, yOffset, this->textTexture.getWidth(), this->textTexture.getHeight() }
      );
      mapIterator--;
      lineCount -= 1;
    }
  }


}