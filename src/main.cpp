#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include "NES/bus.h"
#include "NES/processor_window.h"
#include "NES/main_window.h"
#include "NES/texture.h"
#include "NES/text_texture.h"
#include "NES/utils.h"
#include "NES/cartridge.h"
#include "NES/ppu2C02.h"

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

void quitSDL() {
  //Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char ** argv) {
  initializeSDL();

  Bus bus = Bus();
  Cartridge* cart = new Cartridge("nestest.ines");
  bus.insertCatridge(cart);

  bool quit = false;
  SDL_Event event;
  uint8_t ramPage = 0;
  std::map<uint16_t, std::string> mapLines = bus.getCpu()->disassemble(0x0000, 0xFFFF);
  bus.reset();

  ProcessorWindow processorWindow = ProcessorWindow();
  processorWindow.init();

  MainWindow mainWindow = MainWindow();

  bool run = false;
  unsigned long lastFrameTime = SDL_GetTicks();

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (
        event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE
      ) {
        if (
          processorWindow.isInitialized() &&
          event.window.windowID == processorWindow.getWindowId()
        ) {
          processorWindow.destroy();
        } else if (
          mainWindow.isInitialized() &&
          event.window.windowID == mainWindow.getWindowId()
        ) {
          mainWindow.destroy();
        }
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_LEFT:
            ramPage = (ramPage + 8 - 1) % 8;
            break;
          case SDLK_RIGHT:
            ramPage = (ramPage + 1) % 8;
            break;
          case SDLK_SPACE:
            if (run) break;
            do {
              bus.clock();
            } while (!bus.getCpu()->isComplete());
            do {
              bus.clock();
            } while (bus.getCpu()->isComplete());
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
          case SDLK_RETURN:
            run = true;
            break;
        }
      }
    }
    if (run) {
      do {
        bus.clock();
      } while (!bus.getPpu()->isFrameComplete());
      do {
        bus.clock();
      } while (bus.getPpu()->isFrameComplete());
    }
    if (processorWindow.isInitialized()) {
      processorWindow.clear();
      processorWindow.renderRAM(bus.getRam(), ramPage);
      processorWindow.renderProcessor(bus.getCpu());
      processorWindow.renderCode(mapLines, bus.getCpu()->getPC(), 25);
      processorWindow.update();
    }
    if (mainWindow.isInitialized()) {
      mainWindow.clear();
      std::vector<SDL_Color> colors(
        (SDL_Color*)bus.getPpu()->getScreen(),
        (SDL_Color*)bus.getPpu()->getScreen() + 256 * 240
      );
      mainWindow.updateScreenTexture(colors);
      mainWindow.update();
    }
    unsigned long newFrameTime = SDL_GetTicks();
    std::cout << "Frame time: " << newFrameTime - lastFrameTime << " ms" << std::endl;
    lastFrameTime = newFrameTime;
  }
  quitSDL();
  return 0;
}
