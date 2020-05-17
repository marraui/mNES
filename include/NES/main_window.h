#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <vector>
#include <SDL2/SDL.h>
#include "window_manager.h"
#include "pixel_texture.h"

class MainWindow: public WindowManager {
  private:
    PixelTexture* pixelTexture;
  public:
    MainWindow();
    ~MainWindow();
    void updateScreenTexture(std::vector<SDL_Color> colors, int rowSize = 256);
};

#endif // MAIN_WINDOW_H