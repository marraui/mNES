#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtGui/QCloseEvent>
#include <functional>
#include <string>
#include "NES/processor_window.h"
#include "NES/main_window.h"

class MainWidget : public QMainWindow {
  Q_OBJECT
  public:
    explicit MainWidget(
      QWidget* parent = nullptr,
      std::function<void(std::string, const void*)> onLoadRom = [](std::string fileName, const void* winId) {}
    );
    ~MainWidget();
    void closeEvent(QCloseEvent* event) override;
    void mainLoop(std::string fileName);
  private slots:
    void open();
  private:
    QMenu* fileMenu;
    std::function<void(std::string, const void*)> onLoadRom;
    QWidget* widget;
    bool shoudlQuit = false;
    bool loopComplete = true;
    ProcessorWindow* processorWindow = nullptr;
    MainWindow* mainWindow = nullptr;
};

#endif // MAIN_WIDGET_H