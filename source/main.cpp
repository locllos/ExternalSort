#include <filesystem>
#include <random>
#include <sort/kmerge.hpp>
#include <container/external/file_iterator.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>

void GenerateTest(int num) {
  std::ofstream out("test_in");
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<int32_t> sample(0, 10'000);

  for (int i = 0; i < num; ++i) {
    int32_t value = sample(generator);

    out.write(reinterpret_cast<char *>(&value), 4);
  }
}

void Test() {
  container::external::ReadFileForwardIterator in("test_out", 4);
  
  size_t i = 0;
  auto prev = *in.Next();
  while (auto next = in.Next()) {
    if (prev > *next) {
      fmt::print("prev={} | next={} | index of next={}. Wrong order\n", prev, *next, i);
      return;
    } 
    else {
      prev = *next;
    }
    ++i;
  }
  fmt::print("Ok! :)\n");
}

int main(int argC, const char* argV[]) {
  if (argC == 2 && std::string(argV[1]) == "test") {
    GenerateTest(256 * 1024 / 4 + 7);
    
    size_t max_ram = 4 * 2 * 123;
    size_t block_size = 49;
    std::string source = "test_in";
    std::string destination = "test_out";
    std::string tmp_folder = "test_tmp";

    sort::ExternalSort(
      source,
      destination,
      tmp_folder,
      max_ram,
      block_size
    );

    Test();
    return 0;
  }

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