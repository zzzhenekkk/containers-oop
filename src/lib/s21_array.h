#ifndef S21_ARRAY_H
#define S21_ARRAY_H
#include <iterator>
#include <memory>

#include "s21_container.h"

namespace s21 {
template <typename T, size_t N = 0>
class array : public Container<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using size_type = size_t;
  using iterator = T *;
  using const_iterator = const T *;
  using allocator_type = std::allocator<T>;

  array() : data_(nullptr), allocator_(allocator_type()) {
    this->count_ = N;
    if (N > 0) {
      data_ = allocator_.allocate(N);
      for (size_t i = 0; i < this->count_; i++) {
        allocator_.construct(this->data_ + i);
      }
    }
  }

  array(size_type n) : data_(nullptr) {
    this->count_ = N > n ? N : n;
    if (this->count_ > 0) {
      data_ = allocator_.allocate(N);
      for (size_t i = 0; i < this->count_; i++) {
        allocator_.construct(this->data_ + i);
      }
    }
  }

  // In the original function, you must declare the length of the array in the N
  // parameter.
  // I think we don't need such a rudimentary thing because it's more
  // user-friendly to get the count_ of elements directly from the list. Also, I
  // don't think we need to throw an exception because we can simply correct it
  // for the user.
  array(std::initializer_list<value_type> const &items) {
    this->count_ = items.size();
    data_ = allocator_.allocate(this->count_);
    std::uninitialized_copy(items.begin(), items.end(), data_);
  }

  array(const array &a) {
    this->count_ = a.count_;
    data_ = allocator_.allocate(this->count_);
    std::uninitialized_copy(a.data_, a.data_ + a.count_, data_);
  }

  array(array &&a) : data_(a.data_), allocator_(a.allocator_) {
    this->count_ = a.count_;
    a.data_ = nullptr;
    a.count_ = 0;
  }
  ~array() {
    for (size_t i = 0; i < this->count_; i++) {
      allocator_.destroy(data_ + i);
    }
    allocator_.deallocate(data_, this->count_);
    data_ = nullptr;
    this->count_ = 0;
  }

  array &operator=(const array &a) {
    if (this != &a) {
      array temp(a);
      swap(temp);
    }
    return *this;
  }

  array &operator=(array &&a) {
    if (this != &a) {
      this->count_ = a.count_;
      this->data_ = a.data_;

      a.count_ = 0;
      a.data_ = nullptr;
    }
    return *this;
  }

  // Array Element access
  reference at(size_type pos) {
    if (pos >= this->count_) throw std::out_of_range("Out of range");
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference front() { return data_[0]; }
  const_reference back() { return data_[this->count_ - 1]; }

  // In the task, this function returns an iterator.
  // This doesn't make sense because it's the same as begin().
  // In the official documentation, this function 'returns a pointer to the
  // first element in the array object.' So, I implemented it in a logical way
  // for the original array container
  pointer data() { return data_; }

  // Array Iterators;
  iterator begin() { return iterator(data_); };
  iterator end() { return iterator(data_ + this->count_); };

  // Array Modifiers
  void swap(array &other) {
    std::swap(this->data_, other.data_);
    std::swap(this->count_, other.count_);
  }

  void fill(const_reference value) {
    for (size_t i = 0; i < this->count_; i++) data_[i] = value;
  }

  size_type max_size() override { return this->count_; }

 protected:
  void setData(pointer data) { data_ = data; }
  pointer data_;
  allocator_type allocator_;
};

}  // namespace s21
#endif