#include <iostream>
#include <string>
#include <functional>
#include <QtWidgets/QtWidgets>
#include <QtGui/QCloseEvent>
#include <SDL2/SDL.h>
#include "NES/main_widget.h"
#include "NES/bus.h"
#include "NES/processor_window.h"
#include "NES/main_window.h"
#ifdef __APPLE__
  #include "NES/mac/get_nswindow.h"
#endif // __APPLE__

MainWidget::MainWidget(
  QWidget* parent,
  std::function<void(std::string, const void*)> onLoadRom
) : QMainWindow(parent) {
  this->widget = new QWidget();
  this->setCentralWidget(widget);
  this->setBaseSize(640, 480);
	this->resize(640, 480);
  this->onLoadRom = onLoadRom;
  this->fileMenu = this->menuBar()->addMenu(tr("&File"));
  QAction* openAction = this->fileMenu->addAction(tr("Open ROM"));
  this->menuBar()->addAction(this->fileMenu->menuAction());
  this->setWindowTitle(tr("mNES"));
  this->connect(openAction, &QAction::triggered, this, &MainWidget::open);
}

MainWidget::~MainWidget() {
  delete this->fileMenu;
  delete this->mainWindow;
  delete this->processorWindow;
}

void MainWidget::closeEvent(QCloseEvent* event) {
  this->shoudlQuit = true;
  if (this->processorWindow->isInitialized()) this->processorWindow->destroy();
  this->mainWindow->destroy();
  event->accept();
}

void MainWidget::open() {
  std::string fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/").toStdString();
  this->mainLoop(fileName);
}

void MainWidget::mainLoop(std::string fileName) {
  this->loopComplete = false;
  const void* winId = (const void*)this->centralWidget()->effectiveWinId();
  #ifdef __APPLE__
    winId = getNSWindow(winId);
  #endif // __APPLE__


  Bus bus = Bus();
  Cartridge* cart = new Cartridge(fileName);
  bus.insertCatridge(cart);

  bool quit = false;
  SDL_Event event;
  uint8_t ramPage = 0;
  std::map<uint16_t, std::string> mapLines = bus.getCpu()->disassemble(0x0000, 0xFFFF);
  bus.reset();

  this->processorWindow = new ProcessorWindow();
  this->processorWindow->init();

  this->mainWindow = new MainWindow(winId);

  bool run = false;
  unsigned long lastFrameTime = SDL_GetTicks();

  while (!this->shoudlQuit && !quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          if (
            this->processorWindow->isInitialized() &&
            event.window.windowID == this->processorWindow->getWindowId()
          ) {
            this->processorWindow->destroy();
          } else if (
            this->mainWindow->isInitialized() &&
            event.window.windowID == this->mainWindow->getWindowId()
          ) {
            this->mainWindow->destroy();
          }
        } else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          std::cout << "resize" << std::endl;
        }
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_LEFT:
            ramPage = (ramPage + 8 - 1) % 8;
            break;
          case SDLK_RIGHT:
            ramPage = (ramPage + 1) % 8;
            break;
          case SDLK_SPACE:
            if (run) break;
            do {
              bus.clock();
            } while (!bus.getCpu()->isComplete());
            do {
              bus.clock();
            } while (bus.getCpu()->isComplete());
            break;
          case SDLK_n:
            bus.getCpu()->nmi();
            break;
          case SDLK_i:
            bus.getCpu()->irq();
            break;
          case SDLK_r:
            bus.getCpu()->reset();
            break;
          case SDLK_RETURN:
            run = !run;
            break;
        }
      }
    }
    if (run) {
      do {
        bus.clock();
      } while (!bus.getPpu()->isFrameComplete());
      do {
        bus.clock();
      } while (bus.getPpu()->isFrameComplete());
    }
    if (this->processorWindow->isInitialized()) {
      this->processorWindow->clear();
      this->processorWindow->renderRAM(bus.getRam(), ramPage);
      this->processorWindow->renderProcessor(bus.getCpu());
      this->processorWindow->renderCode(mapLines, bus.getCpu()->getPC(), 25);
      this->processorWindow->update();
    }
    if (this->mainWindow->isInitialized()) {
      this->mainWindow->clear();
      std::vector<SDL_Color> colors(
        (SDL_Color*)bus.getPpu()->getScreen(),
        (SDL_Color*)bus.getPpu()->getScreen() + 256 * 240
      );
      this->mainWindow->updateScreenTexture(colors);
      this->mainWindow->update();
    }
    unsigned long newFrameTime = SDL_GetTicks();
    std::cout << "Frame time: " << newFrameTime - lastFrameTime << " ms" << std::endl;
    lastFrameTime = newFrameTime;
  }
}