#include "GraphicsToolkit/gtk.h"

#include <iostream>

#include <fmt/core.h>

#include <cassert>

int main() {
  auto v = CorputSequence(10, 2);
  for (auto x : v) {
    std::cout << x << ", ";
  }
  std::cout << "\r\r\n";
}