#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    window.clear();
    window.renderRAM(bus.getRam(), 0);
    window.renderProcessor(bus.getCpu());
    window.update();
  }
  textTexture.freeFont();
  textTexture.free();
  window.destroy();
  return 0;
}
