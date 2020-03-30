#include <iostream>
#include <SDL2/SDL.h>
#include "NES/bus.h"
#include "NES/window_manager.h"
#include "NES/texture.h"
#include "NES/text_texture.h"

int main() {
  Bus bus = Bus();
  WindowManager window = WindowManager();
  window.init();
  TextTexture textTexture = TextTexture();

  textTexture.loadFont("assets/lazy.ttf", 22);
  textTexture.loadFromRenderedText("Goodmorning Gamers", window.getRenderer(), { 0xFF, 0xFF, 0xFF });
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    window.clear();
    window.render(&textTexture, 0, 0);
    window.update();
  }
  textTexture.freeFont();
  textTexture.free();
  window.destroy();
  return 0;
}
