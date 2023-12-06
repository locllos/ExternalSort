#include <common/type.hpp>
#include <container/core.hpp>

#include <queue>
#include <fstream>
#include <ios>
#include <memory>
#include <optional>

namespace container::external {

const uint64_t kShardSize = 4;
using Shard = int;

class ReadFileForwardIterator {
 public:
  
  ReadFileForwardIterator(
    const std::string& path, 
    size_t block_size
  );
  ReadFileForwardIterator(ReadFileForwardIterator&& other);

  std::optional<Shard> Next();
  
  ~ReadFileForwardIterator();

 private:
  const size_t block_size_;
 private:

  std::vector<type::Byte> buffer_;
  std::queue<Shard> queue_; // can be optimized by using the only buffer
  std::ifstream in_;
};

class WriteFileForwardIterator {
 public:
  
  WriteFileForwardIterator(
    const std::string& path, 
    size_t block_size
  );
  WriteFileForwardIterator(WriteFileForwardIterator&& other);

  bool AssignAndNext(Shard shard);

  ~WriteFileForwardIterator();

 private:
  const size_t block_size_;
 private:

  std::vector<type::Byte> buffer_;
  std::queue<Shard> queue_; // can be optimized by using the only buffer
  std::ofstream out_;
};

} // container::external