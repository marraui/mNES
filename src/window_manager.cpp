#include "NES/window_manager.h"
#include "NES/texture.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

WindowManager::WindowManager() {
  this->window = nullptr;
  this->renderer = nullptr;
}

WindowManager::~WindowManager() {}

void WindowManager::init() {
  // Create window
  this->window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (this->window == nullptr) {
    std::cout << "SDL couldn't create window, SDL error:" << std::endl << SDL_GetError() << std::endl;
    return;
  }
  
  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
  if (this->renderer == nullptr) {
    std::cout << "SDL couldn't create renderer, error: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_RendererInfo rendererInfo;
  SDL_GetRendererInfo(this->renderer, &rendererInfo);

  std::cout << "Renderer: " << rendererInfo.name << std::endl;

  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void WindowManager::init(const void* winId) {
  // Create window
  this->window = SDL_CreateWindowFrom(winId);
  if (this->window == nullptr) {
    std::cout << "SDL couldn't create window, SDL error:" << std::endl << SDL_GetError() << std::endl;
    return;
  }
  
  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
  if (this->renderer == nullptr) {
    std::cout << "SDL couldn't create renderer, error: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void WindowManager::destroy() {
  if (this->renderer != nullptr) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }
  if (this->window != nullptr) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }
}

void WindowManager::render(
  Texture* texture,
  SDL_Rect* renderQuad,
  SDL_Rect* clip,
  double angle,
  SDL_Point* center,
  SDL_RendererFlip flip
) {
  if (renderQuad == nullptr) {
    renderQuad = new SDL_Rect{ 0, 0, texture->getWidth(), texture->getHeight() };
    if (clip != nullptr) {
      renderQuad->w = clip->w;
      renderQuad->h = clip->h;
    }
  }

  int errorCode = SDL_RenderCopyEx(this->renderer, texture->getTexture(), clip, renderQuad, angle, center, flip);
  if (errorCode < 0) {
    std::cout << "Error rendering texture: " << SDL_GetError() << std::endl;
  }
}

void WindowManager::clear() {
  SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(this->renderer);
}

void WindowManager::update() {
  SDL_RenderPresent(this->renderer);
}

SDL_Renderer* WindowManager::getRenderer() {
  return this->renderer;
}

int WindowManager::getWidth() {
  int w;
  SDL_GetWindowSize(this->window, &w, nullptr);
  return w;
}

int WindowManager::getHeight() {
  int h;
  SDL_GetWindowSize(this->window, nullptr, &h);
  return h;
}

uint32_t WindowManager::getWindowId() {
  return SDL_GetWindowID(this->window);
}

bool WindowManager::isInitialized() {
  return this->window != nullptr && this->renderer != nullptr;
}