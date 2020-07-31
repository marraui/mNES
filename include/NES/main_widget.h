#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtGui/QCloseEvent>
#include <functional>
#include <string>
#include <stdint.h>
#include <map>
#include "NES/processor_window.h"
#include "NES/main_window.h"
#include "NES/bus.h"

class MainWidget : public QMainWindow {
  Q_OBJECT
  public:
    explicit MainWidget(
      QWidget* parent = nullptr,
      std::function<void(std::string, const void*)> onLoadRom = [](std::string fileName, const void* winId) {}
    );
    ~MainWidget();
    void closeEvent(QCloseEvent* event) override;
    void mainLoop();
    void init(std::string fileName);
    void justMove();
  private slots:
    void open();
  public slots:
    void onApplicationStateChange(Qt::ApplicationState state);
  private:
    QMenu* fileMenu;
    std::function<void(std::string, const void*)> onLoadRom;
    QWidget* widget;
    bool shouldQuit = false;
    bool loopComplete = true;
    ProcessorWindow* processorWindow = nullptr;
    MainWindow* mainWindow = nullptr;
    Bus* bus = nullptr;
    uint8_t ramPage = 0;
    std::map<uint16_t, std::string> mapLines;
    bool run;
    unsigned long lastFrameTime;
};

#endif // MAIN_WIDGET_H