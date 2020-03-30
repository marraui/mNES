#include "NES/utils.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

std::string hexToString(int value, int size) {
  std::stringstream stream;
  stream << std::hex << value;
  std::string hex = stream.str();
  std::for_each(hex.begin(), hex.end(), [](char &c) -> void {
    c = ::toupper(c);
  });
  int zeros = std::max(size - (int)hex.length(), 0);
  return "0x" + (std::string(zeros, '0') + hex);
}