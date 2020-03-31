#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SDL.h>
#include "texture.h"

class WindowManager {
  protected:
    SDL_Renderer* renderer;
    SDL_Window* window;
  public:
    WindowManager();
    ~WindowManager();
    void init();
    void destroy();
    void clear();
    void update();
    void render(
      Texture* texture,
      int x,
      int y,
      SDL_Rect* clip = nullptr,
      double angle = 0,
      SDL_Point* center = nullptr,
      SDL_RendererFlip flip = SDL_FLIP_NONE
    );
    SDL_Renderer* getRenderer();
    int getWidth();
    int getHeight();
};

#endif // WINDOW_MANAGER_H
