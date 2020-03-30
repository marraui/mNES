#include "NES/text_texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

TextTexture::TextTexture() {
  Texture();
  this->font = nullptr;
}

TextTexture::~TextTexture() {}

bool TextTexture::loadFont(std::string path, int size) {
  this->font = TTF_OpenFont(path.c_str(), size);
  if (this->font == nullptr) {
    std::cout << "Unable to load font at " << path.c_str() << ", error: " << TTF_GetError() << std::endl;
    return false;
  }
  return true;
}

bool TextTexture::loadFromRenderedText(std::string text, SDL_Renderer* renderer, SDL_Color color) {
  this->free();

  SDL_Surface* surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
  if (surface == nullptr) {
    std::cout << "Unable to render text surface, error: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == nullptr) {
    std::cout << "Unable to create texture from rendered text, error: " << SDL_GetError() << std::endl;
    return false; 
  }

  this->texture = texture;
  this->height = surface->h;
  this->width = surface->w;
  return true;
}

void TextTexture::freeFont() {
  TTF_CloseFont(this->font);
  this->font = nullptr;
}