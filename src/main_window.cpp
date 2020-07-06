#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include "NES/window_manager.h"
#include "NES/main_window.h"
#include "NES/pixel_texture.h"

MainWindow::MainWindow() {
  WindowManager();
  this->init();
  this->pixelTexture = new PixelTexture();
  this->pixelTexture->create(this->renderer, 256, 240);
}

MainWindow::MainWindow(const void* winId) {
  WindowManager();
  this->init(winId);
  this->pixelTexture = new PixelTexture();
  this->pixelTexture->create(this->renderer, 256, 240);
}

MainWindow::~MainWindow() {}

void MainWindow::updateScreenTexture(std::vector<SDL_Color> colors, int rowSize) {
  this->pixelTexture->setAll(colors, rowSize);
  float widthRatio = this->getWidth() / 256;
  float heightRatio = this->getHeight() / 240;
  float ratio = std::min(widthRatio, heightRatio);
  SDL_Rect renderQuad = { 0, 0, (int) std::floor(256 * ratio), (int) std::floor(240 * ratio) };
  this->render(this->pixelTexture, &renderQuad);
}