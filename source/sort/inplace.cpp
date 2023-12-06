#include <sort/inplace.hpp>

#include <algorithm>

namespace sort {

void FastSort(std::vector<int>& array) {
  std::sort(array.begin(), array.end());
}

} // sort