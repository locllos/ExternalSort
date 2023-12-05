#include <optional>

namespace container {

template <typename T>
struct IForwardIterator {  
  virtual std::optional<T> Next() = 0;
  virtual bool Assign(T value) = 0;
};

} // namespace container