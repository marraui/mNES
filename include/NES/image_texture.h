#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "texture.h"
#include <SDL2/SDL.h>
#include <string>

class ImageTexture: public Texture {
  private:
    std::string path;
  public:
    ImageTexture();
    ~ImageTexture();
    bool loadFromFile(std::string path, SDL_Renderer* renderer);
};

#endif // IMAGE_TEXTURE_H
