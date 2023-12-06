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
    in_(path, std::ios::binary | std::ios::in) {
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
      queue_.push(ByteToInt(buffer_.data() + i * kShardSize));
    }
  }
  auto extracted = queue_.front();
  queue_.pop();
  
  return extracted;
}

ReadFileForwardIterator::ReadFileForwardIterator(ReadFileForwardIterator&& other) 
: block_size_(UpToMultipleOf(kShardSize, other.block_size_)),
  buffer_(std::move(other.buffer_)),
  queue_(std::move(other.queue_)) {
  try {
    in_ = std::move(other.in_); // no noexcept
  }
  catch (...) {
    // later
  }
}

ReadFileForwardIterator::~ReadFileForwardIterator() {
  in_.close();
}


WriteFileForwardIterator::WriteFileForwardIterator(
  const std::string& path, 
  size_t block_size
) : block_size_(UpToMultipleOf(kShardSize, block_size)),
    buffer_(block_size_),
    out_(path, std::ios::binary | std::ios::out) {
  assert(out_.is_open());
}

WriteFileForwardIterator::WriteFileForwardIterator(WriteFileForwardIterator&& other) 
: block_size_(UpToMultipleOf(kShardSize, other.block_size_)),
  buffer_(std::move(other.buffer_)),
  queue_(std::move(other.queue_)) {
  try {
    out_ = std::move(other.out_); // no noexcept
  }
  catch (...) {
    // later
  }
}

bool WriteFileForwardIterator::AssignAndNext(Shard shard) {
  if (queue_.size() == block_size_ / kShardSize) {
    for (size_t i = 0; !queue_.empty(); ++i) {
      IntToByte(
        queue_.front(), 
        buffer_.data() + i * kShardSize
      );
      
      queue_.pop();
    }
    out_.write(buffer_.data(), block_size_);
    out_.flush();
  }
  queue_.push(shard);

  return true;
}

WriteFileForwardIterator::~WriteFileForwardIterator() {
  out_.close();
}


} // namespace container::external
