#ifndef TEXT_TEXTURE_H
#define TEXT_TEXTURE_H
#include "texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextTexture: public Texture {
  private:
    TTF_Font* font;
  public:
    TextTexture();
    ~TextTexture();
    bool loadFont(std::string path, int size);
    bool loadFromRenderedText(std::string text, SDL_Renderer* renderer, SDL_Color color);
    void freeFont();
};

#endif // TEXT_TEXTURE_H
