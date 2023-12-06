#include <sort/detail/kmerger.hpp>

namespace sort {

inline void ExternalSort(
  const std::string& source,
  const std::string& destination,
  const std::string& tmp_folder = "./tmp/",
  size_t max_ram = 4096,
  size_t block_size = 32
) {
  detail::KMerger sorter(
    source, destination, tmp_folder, max_ram, block_size
  );
  
  sorter.Sort();
}

} // sort