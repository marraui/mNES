#ifndef PIXEL_TEXTURE_H
#define PIXEL_TEXTURE_H
#include <SDL2/SDL.h>
#include <vector>
#include "texture.h"

class PixelTexture: public Texture {
  private:
    SDL_PixelFormat* pixelFormat;
  public:
    PixelTexture();
    ~PixelTexture();
    void create(SDL_Renderer* renderer, unsigned int width, unsigned int height);
    void setPixel(int x, int y, SDL_Color color);
    void setAll(std::vector<SDL_Color> colors, int rowSize);
};

#endif // PIXEL_TEXTURE_H