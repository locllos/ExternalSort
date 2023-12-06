#include <sort/inplace.hpp>
#include <sort/detail/kmerger.hpp>
#include <container/external/file_iterator.hpp>

#include <fmt/core.h>

#include <queue>
#include <cassert>
#include <fstream>
#include <filesystem>

namespace sort::detail {

using container::external::Shard;
using container::external::kShardSize;
using container::external::ReadFileForwardIterator;
using container::external::WriteFileForwardIterator;

void KMerger::Sort() {
  if (!std::filesystem::exists(fmt::format("./{}", tmp_folder_))) {
    std::filesystem::create_directory(fmt::format("./{}", tmp_folder_));
  }

  ChunkedSort();
  KMergedSort();

  // do not delete while debugging
  // for (size_t chunk = 0; chunk < chunks_count_; ++chunk) {
  //   std::filesystem::remove(fmt::format("./{}/{}", tmp_folder_, chunk).c_str());
  // }
}

KMerger::KMerger(
  const std::string& source,
  const std::string& destination,
  const std::string& tmp_folder,
  size_t max_ram,
  size_t block_size
) : source_(source),
    destination_(destination),
    tmp_folder_(tmp_folder),
    max_ram_(max_ram),
    block_size_(block_size),
    chunks_count_(0) {
}

void KMerger::ChunkedSort() {
  std::ifstream in(source_, std::ios::binary | std::ios::in);
  assert(in.is_open());

  size_t read_size = 0;
  size_t desired_size = max_ram_ / 2;
  std::vector<Shard> buffer(max_ram_ / (2 * kShardSize), 0);
  do {
    in.read(
      reinterpret_cast<char*>(buffer.data()), 
      desired_size
    );
    read_size = in.gcount();
    
    std::memset(
      reinterpret_cast<char*>(buffer.data()) + read_size, 
      INT_MAX,
      desired_size - read_size
    );
    FastSort(buffer);

    std::ofstream out(
      fmt::format("{}/{}", tmp_folder_, chunks_count_),
      std::ios::binary | std::ios::out
    );
    out.write(
      reinterpret_cast<char*>(buffer.data()),
      read_size
    );
    out.close();

    ++chunks_count_;
  } while (read_size == desired_size);

  in.close();
}

void KMerger::KMergedSort() {
  std::priority_queue<MergeState> heap;
  std::vector<ReadFileForwardIterator> streams;
  for (size_t chunk = 0; chunk < chunks_count_; ++chunk) {
    streams.emplace_back(
      fmt::format("{}/{}", tmp_folder_, chunk),
      block_size_
    );
    // imply that at least one number is in file
    heap.push({*streams.back().Next(), chunk});
  }
  WriteFileForwardIterator out(destination_, block_size_);

  while (!heap.empty()) {
    auto top = heap.top();
    heap.pop();

    out.AssignAndNext(top.value);

    auto extracted = streams[top.stream].Next();
    if (extracted) {
      heap.push({*extracted, top.stream});
    }
  }
}

} // sort::detail