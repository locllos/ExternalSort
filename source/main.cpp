#include <sort/kmerge.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>

int main(int argC, const char* argV[]) {
  if (argC < 5) {
    fmt::print("Not enough arguments!\n");
    return 1;
  }

  size_t max_ram = std::atoi(argV[1]);
  size_t block_size = std::atoi(argV[2]);
  std::string source = argV[3];
  std::string destination = argV[4];
  std::string tmp_folder = argV[5];

  sort::ExternalSort(
    source,
    destination,
    tmp_folder,
    max_ram,
    block_size
  );

  return 0;
}