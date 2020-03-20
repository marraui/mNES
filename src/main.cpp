#include <iostream>
#include "NES/bus.h"
#include "NES/cpu6502.h"

int main() {
  Bus bus = Bus();
  Cpu6502 cpu6502 = Cpu6502();
  std::cout << "Hello World!" << std::endl;
  return 0;
}
