#include <common/type.hpp>
#include <container/core.hpp>

#include <queue>
#include <fstream>
#include <ios>
#include <memory>
#include <optional>

namespace container::external {

const uint64_t kShardSize = 4;
using Shard = int; // std::array<type::Byte, kShardSize>;

class ReadFileForwardIterator : public IForwardIterator<Shard> {
 public:
  
  ReadFileForwardIterator(
    const std::string& path, 
    size_t block_size
  );
  
  virtual std::optional<Shard> Next() override;
  
  ~ReadFileForwardIterator();

 private:
  const std::streamsize block_size_;
 private:

  std::vector<type::Byte> buffer_;
  std::queue<Shard> queue_;
  std::ifstream in_;
};

class WriteFileForwardIterator : public IForwardIterator<Shard> {
 public:
  
  WriteFileForwardIterator(
    const std::string& path, 
    size_t block_size
  );
  
  virtual bool AssignAndNext() override;
  
  ~WriteFileForwardIterator();

 private:
  const std::streamsize block_size_;
 private:

  std::vector<type::Byte> buffer_;
  std::queue<Shard> queue_;
  std::ifstream in_;
};

} // container::external