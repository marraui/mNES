#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <QtWidgets/QApplication>
#include "NES/main_widget.h"

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
  QApplication application(argc, argv);
  MainWidget widget;
  widget.show();

  int res = application.exec();

  quitSDL();

  return res;
}
