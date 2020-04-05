#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include "NES/bus.h"
#include "NES/processor_window.h"
#include "NES/texture.h"
#include "NES/text_texture.h"
#include "NES/utils.h"

void initializeSDL() {
  // Initialize SDL with video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL couldn't initialize, SDL error:" << std::endl << SDL_GetError() << std::endl;
    return;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cout << "Warning! Linear texture filtering not enabled" << std::endl;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cout <<"SDL_image couldn't initialize, error: " << SDL_GetError() << std::endl;
    return;
  }

  if (TTF_Init() == -1) {
    std::cout << "SDL_ttf couldn't initialize, error: " << SDL_GetError() << std::endl;
    return;
  }
}

int main() {
  initializeSDL();
  Bus bus = Bus();
  ProcessorWindow window = ProcessorWindow();
  window.init();
  TextTexture textTexture = TextTexture();

  bool quit = false;
  SDL_Event event;
  uint16_t ramPage = 0;

  // Convert hex string into bytes for RAM
  std::stringstream ss;
  ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
  uint16_t nOffset = 0x8000;
  while (!ss.eof()) {
    std::string byte;
    ss >> byte;
    bus.write(nOffset, (uint8_t)std::stoul(byte, nullptr, 16));
    nOffset += 1;
  }
  // Set up reset vector
  bus.write(0xFFFC, 0x00);
  bus.write(0xFFFD, 0x80);

  std::map<uint16_t, std::string> mapLines = bus.getCpu()->disassemble(0x0000, 0xFFFF);
  bus.getCpu()->reset();

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_LEFT:
            ramPage -= 1;
            break;
          case SDLK_RIGHT:
            ramPage += 1;
            break;
          case SDLK_SPACE:
            do {
              bus.getCpu()->clock();
            } while (!bus.getCpu()->isComplete());
            break;
          case SDLK_n:
            bus.getCpu()->nmi();
            break;
          case SDLK_i:
            bus.getCpu()->irq();
            break;
          case SDLK_r:
            bus.getCpu()->reset();
            break;
        }
      }
    }
    window.clear();
    window.renderRAM(bus.getRam(), ramPage);
    window.renderProcessor(bus.getCpu());
    window.renderCode(mapLines, bus.getCpu()->getPC(), 25);
    window.update();
  }
  textTexture.freeFont();
  textTexture.free();
  window.destroy();
  return 0;
}
