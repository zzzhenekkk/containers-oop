#ifndef S21_VECTOR_H
#define S21_VECTOR_H
#include <iostream>

#include "s21_array.h"

namespace s21 {
template <typename T>
class vector : public array<T> {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using size_type = size_t;
  using iterator = T *;
  using const_iterator = const T *;
  using allocator_type = std::allocator<T>;

 public:
  vector() : capacity_(0) {}

  vector(size_type n) : array<T>(n), capacity_(n) { capacity_ = this->count_; }

  vector(std::initializer_list<value_type> const &items) : array<T>(items) {
    // this->count_ = items.size();
    capacity_ = this->count_;
  }

  // just for peer readeable.
  vector(const vector &v) : array<T>(v) { this->capacity_ = v.capacity_; }
  vector(vector &&v) : array<T>(v) { this->capacity_ = v.capacity_; }
  ~vector() { capacity_ = 0; }

  vector operator=(const vector &v) {
    if (this != &v) {
      array<T>::operator=(v);
      capacity_ = v.capacity_;
    }
    return *this;
  }

  vector operator=(vector &&v) {
    if (this != &v) {
      array<T>::operator=(std::move(v));
      capacity_ = v.capacity_;
      v.capacity_ = 0;
    }
    return *this;
  }

  void reserve(size_type size) {
    if (size > capacity_) {
      pointer new_data = this->allocator_.allocate(size);
      for (size_type i = 0; i < this->count_; i++) {
        this->allocator_.construct(new_data + i, std::move(this->data_[i]));
        this->allocator_.destroy(this->data_ + i);
      }
      if (this->data_) {
        this->allocator_.deallocate(this->data_, capacity_);
      }
      this->data_ = new_data;
      capacity_ = size;
    }
  }

  size_type capacity() { return capacity_; }

  // reduces memory usage by freeing unused memory
  void shrink_to_fit() { data_realoc(this->count_); }

  void clear() {
    if (this->count_ == 0) return;
    pointer data = this->data();
    for (size_t i = 0; i < this->count_; i++) {
      data[i].~T();
    }
    this->count_ = 0;
  }

  iterator insert(iterator pos, const_reference value) {
    size_type pos_index = pos - this->begin();
    memory_realoc();
    if (this->count_ == 0) {
      iterator new_pos = this->data();
      *new_pos = value;
      this->count_++;
      return new_pos;
    } else {
      pointer data = this->data();

      for (size_type i = this->count_; i > pos_index; i--) {
        data[i] = std::move(data[i - 1]);
      }
      data[pos_index] = value;
      this->count_++;
      return this->begin() + pos_index;
    }
  }

  void erase(iterator pos) {
    // Проверка на валидность позиции
    if (pos >= this->begin() && pos < this->end()) {
      this->allocator_.destroy(pos);
      iterator next = pos + 1;
      while (next != this->end()) {
        this->allocator_.construct(pos, std::move(*next));
        this->allocator_.destroy(next);
        ++pos;
        ++next;
      }
      this->count_--;
    }
  }

  void push_back(const_reference value) {
    if (this->count_ + 1 > capacity_) {
      reserve(capacity_ > 0 ? capacity_ * 2 : 1);
    }
    this->allocator_.construct(this->data_ + this->count_, value);
    this->count_++;
  }

  void pop_back() {
    if (this->count_ > 0) {
      this->allocator_.destroy(this->data_ + this->count_ -
                               1);  // Уничтожаем последний элемент
      this->count_--;
    }
  }

  size_type max_size() override { return Container<T>::max_size(); }

  void swap(vector &other) {
    array<T>::swap(other);
    std::swap(this->capacity_, other.capacity_);
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    // after realoc pos may be invalid so we need to save a shift
    size_type pos_index = pos - this->begin();
    // we need to reverse inserting so need a temp vector
    vector<T> temp({args...});
    size_type insert_size = temp.size();
    reserve(this->capacity_ + insert_size);

    pointer new_data = temp.data();
    iterator result = nullptr;
    for (size_type i = insert_size; i > 0; i--) {
      // move insert position to left
      iterator insert_pos = this->data() + pos_index;
      result =
          insert(insert_pos, new_data[i - 1]);  // insert allready made count_++
    }
    return result;
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    (push_back(args), ...);
  }

 private:
  void data_realoc(size_type size) {
    if (size == 0) return;
    pointer old_data = this->data();
    pointer new_data = this->allocator_.allocate(size);
    // Копирование элементов в новую память
    for (size_t i = 0; i < this->count_ && i < size; ++i) {
      this->allocator_.construct(new_data + i, std::move(old_data[i]));
    }
    // Уничтожение старых элементов и освобождение старой памяти
    for (size_t i = 0; i < this->count_; ++i) {
      this->allocator_.destroy(old_data + i);
    }
    this->allocator_.deallocate(old_data, this->count_);
    // Обновление данных и емкости вектора
    this->setData(new_data);
    this->capacity_ = size;
  }

  void memory_realoc() {
    if (this->count_ + 1 > capacity_) {
      reserve(capacity_ > 0 ? this->count_ * 2 : 2);
    }
  }

  size_t capacity_;
};
}  // end namespace s21

#endif  // S21_VECTOR_H