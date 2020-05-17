#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "NES/window_manager.h"
#include "NES/main_window.h"
#include "NES/pixel_texture.h"

MainWindow::MainWindow() {
  WindowManager();
  this->init();
  this->pixelTexture = new PixelTexture();
  this->pixelTexture->create(this->renderer, 256, 240);
}

MainWindow::~MainWindow() {}

void MainWindow::updateScreenTexture(std::vector<SDL_Color> colors, int rowSize) {
  this->pixelTexture->setAll(colors, rowSize);
  this->render(this->pixelTexture, 0, 0);
}