#include "NES/image_texture.h"
#include <SDL2/SDL_image.h>
#include <iostream>

ImageTexture::ImageTexture() {
  Texture();
  this->path = "";
}

ImageTexture::~ImageTexture() {}

bool ImageTexture::loadFromFile(std::string path, SDL_Renderer* renderer) {
  this->free();

  SDL_Texture* texture;
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (surface == nullptr) {
    std::cout << "Unable to load image, SDL error: " << path.c_str() << IMG_GetError() << std::endl;
    return false;
  }

  SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == nullptr) {
    std::cout << "IUnable to create texture from " << path.c_str() << SDL_GetError() << std::endl;
    return false;
  }

  this->width = surface->w;
  this->height = surface->h;
  this->texture = texture;
  return true;
}