#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SDL.h>

class WindowManager {
  private:
    SDL_Window* window;
    SDL_Surface* surface;
  public:
    WindowManager();
    ~WindowManager();
};

#endif // WINDOW_MANAGER_H
