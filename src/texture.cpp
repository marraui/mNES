#include "NES/texture.h"
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Texture::Texture() {
  this->texture = nullptr;
  this->width = 0;
  this->height = 0;
}

Texture::~Texture() {}

void Texture::free() {
  if (this->texture == nullptr) return;
  SDL_DestroyTexture(this->texture);
  this->texture = nullptr;
  this->height = 0;
  this->width = 0;
}

void Texture::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  SDL_SetTextureColorMod(this->texture, red, green, blue);
}

void Texture::setAlpha(uint8_t alpha) {
  SDL_SetTextureAlphaMod(this->texture, alpha);
}

void Texture::setBlendMode(SDL_BlendMode blendMode) {
  SDL_SetTextureBlendMode(this->texture, blendMode);
}

int Texture::getWidth() {
  return this->width;
}
int Texture::getHeight() {
  return this->height;
}

SDL_Texture* Texture::getTexture() {
  return this->texture;
}