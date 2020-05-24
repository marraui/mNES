#include "NES/texture.h"
#include "NES/pixel_texture.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>

PixelTexture::PixelTexture() {
  Texture();
}

PixelTexture::~PixelTexture() {}

void PixelTexture::create(SDL_Renderer* renderer, unsigned int width, unsigned int height) {
  SDL_RendererInfo rendererInfo;
  SDL_GetRendererInfo(renderer, &rendererInfo);
  if (!rendererInfo.num_texture_formats) {
    std::cout << "Error creating PixelTexture, no texture formats found for renderer" <<std::endl;
    return;
  }
  this->pixelFormat = SDL_AllocFormat(rendererInfo.texture_formats[0]);

  this->texture = SDL_CreateTexture(
    renderer,
    this->pixelFormat->format,
    SDL_TEXTUREACCESS_STREAMING,
    width,
    height
  );
  
  if (this->texture == nullptr) {
    std::cout << "Error creating PixelTexture: " << SDL_GetError() << std::endl;
    return;
  }

  this->width = width;
  this->height = height;
}

void PixelTexture::setPixel(int x, int y, SDL_Color color) {
  uint32_t* lockedPixel = new uint32_t;
  int pitch = 4; // (r, g, b, a)
  SDL_Rect rect = { x, y, 1, 1 };
  SDL_LockTexture(this->texture, &rect, (void**) (&lockedPixel), &pitch);
  *lockedPixel = SDL_MapRGB(this->pixelFormat, color.r, color.g, color.b);
  SDL_UnlockTexture(this->texture);
}

void PixelTexture::setAll(std::vector<SDL_Color> colors, int rowSize) {
  int pitch = 4 * rowSize; // (r, g, b, a) * number of pixels in one row
  uint32_t* lockedPixels;
  SDL_LockTexture(this->texture, nullptr, (void**)(&lockedPixels), &pitch);
  std::transform(colors.begin(), colors.end(), lockedPixels, [&](SDL_Color color) -> uint32_t {
    return SDL_MapRGB(this->pixelFormat, color.r, color.g, color.b);
  });
  SDL_UnlockTexture(this->texture);
}