#ifndef S21_CONTAINER_H
#define S21_CONTAINER_H
#include <stddef.h>  // for size_t

#include <limits>     // for max_size
#include <stdexcept>  // for exceptions

namespace s21 {
template <typename T>
class Container {
 public:
  Container() : count_(0) {}

  using size_type = size_t;

  bool empty() { return count_ <= 0 ? true : false; }

  size_type size() { return count_; }

  virtual size_type max_size() {
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
  }

 protected:
  size_type count_;
};
}  // namespace s21
#endif
