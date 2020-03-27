#include <iostream>
#include "NES/bus.h"
#include "NES/window_manager.h"

int main() {
  Bus bus = Bus();
  WindowManager windowManager = WindowManager(); 
  std::cout << "Hello World!" << std::endl;
  return 0;
}
