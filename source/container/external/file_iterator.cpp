#include <common/math.hpp>
#include <common/convert.hpp>
#include <container/external/file_iterator.hpp>


#include <cassert>
#include <cstddef>
#include <optional>

namespace container::external {

ReadFileForwardIterator::ReadFileForwardIterator(
  const std::string& path,
  size_t block_size
) : block_size_(UpToMultipleOf(kShardSize, block_size)),
    buffer_(block_size_),
    in_(path, std::ios::binary) {
  assert(in_.is_open());
}

std::optional<Shard> ReadFileForwardIterator::Next() {
  if (queue_.empty()) {
    in_.read(buffer_.data(), block_size_);
    
    size_t shard_count = in_.gcount() / kShardSize;
    if (shard_count == 0) {
      // we need to read at least four bytes
      return std::nullopt;
    }

    for (size_t i = 0; i < shard_count; ++i) {
      queue_.push(ToInt(buffer_.data() + i * kShardSize));
    }
  }
  auto extracted = queue_.front();
  queue_.pop();
  
  return extracted;
}


} // namespace container::external
