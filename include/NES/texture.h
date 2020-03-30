#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL.h>
#include <string>
#include <cstdint>

class Texture {
  protected:
    SDL_Texture* texture;
    int height;
    int width;
  public:
    Texture();
    ~Texture();
    void free();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setBlendMode(SDL_BlendMode blendMode);
    void setAlpha(uint8_t alpha);
    int getWidth();
    int getHeight();
    SDL_Texture* getTexture();

};

#endif // TEXTURE_H