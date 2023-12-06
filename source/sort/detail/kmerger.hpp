#include <string>

namespace sort::detail {

class KMerger {
 public:
  KMerger(
    const std::string& source,
    const std::string& destination,
    const std::string& tmp_folder,
    size_t max_ram,
    size_t block_size
  );

  void Sort();

 private:
  void ChunkedSort();
  void KMergedSort();
  
 private:
  struct MergeState {
    int value;
    size_t stream;

    bool operator<(const MergeState& other) const {
      return value < other.value;
    }
  };

 private:
  const std::string source_;
  const std::string destination_;
  const std::string tmp_folder_;
  const size_t max_ram_;
  const size_t block_size_;

 private:
  size_t chunks_count_;
};

} // sort::detail