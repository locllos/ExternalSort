#include <fmt/core.h>
#include <iostream>

int main() {
  int var = 0;

  for (int i = 0; i < 3; ++i) {
    std::cin >> var;                                                                                                  
    std::cout << 2 * var;

    fmt::print("Hello, world!\n");
  }


  return 0;
}