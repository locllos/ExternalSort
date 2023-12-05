#include <cstddef>

inline size_t UpToMultipleOf(size_t factor, size_t target) {
  return target % factor ?
         target + (factor - target % factor) :
         target;
}
