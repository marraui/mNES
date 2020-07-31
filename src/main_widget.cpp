#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <QtWidgets/QtWidgets>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
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
  delete this->widget;
  delete this->mainWindow;
  delete this->processorWindow;
  delete this->bus;
}

void MainWidget::closeEvent(QCloseEvent* event) {
  this->shouldQuit = true;
  if (
    this->processorWindow != nullptr &&
    this->processorWindow->isInitialized()
  ) {
    this->processorWindow->destroy();
  }
  if (this->processorWindow != nullptr) this->mainWindow->destroy();
  event->accept();
}

void MainWidget::open() {
  QString qFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/");
  if (qFileName.isNull()) return;
  std::string fileName = qFileName.toStdString();
  std::cout << "File name: " << fileName << std::endl;
  this->init(fileName);
}

void MainWidget::onApplicationStateChange(Qt::ApplicationState state) {
  std::cout << "Application state changed: " << (int)state << std::endl;
  this->justMove();
}

void MainWidget::init(std::string fileName) {
  this->loopComplete = false;
  const void* winId = (const void*)this->centralWidget()->effectiveWinId();
  #ifdef __APPLE__
    winId = getNSWindow(winId);
  #endif // __APPLE__


  this->bus = new Bus();
  Cartridge* cart = new Cartridge(fileName);
  this->bus->insertCatridge(cart);

  this->ramPage = 0;
  this->mapLines = this->bus->getCpu()->disassemble(0x0000, 0xFFFF);
  this->bus->reset();

  this->processorWindow = new ProcessorWindow();
  this->processorWindow->init();

  this->mainWindow = new MainWindow(winId);

  this->run = false;
  this->lastFrameTime = SDL_GetTicks();
  this->mainLoop();
  QTimer::singleShot(0, this, &MainWidget::justMove);

}

void MainWidget::justMove() {
  this->move(this->pos().x() + 1, this->pos().y());
  this->move(this->pos().x() - 1, this->pos().y());
}

void MainWidget::mainLoop() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      this->shouldQuit = true;
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
          if (this->run) break;
          do {
            this->bus->clock();
          } while (!this->bus->getCpu()->isComplete());
          do {
            this->bus->clock();
          } while (this->bus->getCpu()->isComplete());
          break;
        case SDLK_n:
          this->bus->getCpu()->nmi();
          break;
        case SDLK_i:
          this->bus->getCpu()->irq();
          break;
        case SDLK_r:
          this->bus->getCpu()->reset();
          break;
        case SDLK_RETURN:
          this->run = !this->run;
          break;
      }
    }
  }
  if (this->run) {
    do {
      this->bus->clock();
    } while (!this->bus->getPpu()->isFrameComplete());
    do {
      this->bus->clock();
    } while (this->bus->getPpu()->isFrameComplete());
  }
  if (this->processorWindow->isInitialized()) {
    this->processorWindow->clear();
    this->processorWindow->renderRAM(this->bus->getRam(), ramPage);
    this->processorWindow->renderProcessor(this->bus->getCpu());
    this->processorWindow->renderCode(this->mapLines, this->bus->getCpu()->getPC(), 25);
    this->processorWindow->update();
  }
  if (this->mainWindow->isInitialized()) {
    this->mainWindow->clear();
    std::vector<SDL_Color> colors(
      (SDL_Color*)this->bus->getPpu()->getScreen(),
      (SDL_Color*)this->bus->getPpu()->getScreen() + 256 * 240
    );
    this->mainWindow->updateScreenTexture(colors);
    this->mainWindow->update();
  }
  unsigned long newFrameTime = SDL_GetTicks();
  // std::cout << "Frame time: " << newFrameTime - this->lastFrameTime << " ms" << std::endl;
  this->lastFrameTime = newFrameTime;
  if (!this->shouldQuit) {
    QTimer::singleShot(0, this, &MainWidget::mainLoop);
  }
}