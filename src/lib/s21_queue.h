#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include "s21_ordered_list.h"

namespace s21 {
template <typename T>
class queue : public orderedList<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  // default constructor, creates empty queue
  queue() : orderedList<value_type>(){};
  // initializer list constructor, creates queue initizialized using
  // std::initializer_list
  queue(std::initializer_list<value_type> const &items)
      : orderedList<value_type>(items){};
  // copy constructor
  queue(const queue &q) : orderedList<value_type>(q){};
  // move constructor
  queue(queue &&q) : orderedList<value_type>(std::move(q)){};
  // assignment operator overload for moving object
  queue &operator=(queue &&q) {
    return static_cast<queue &>(
        orderedList<value_type>::operator=(std::move(q)));
  }
  // destructor
  ~queue();
  // access the first element
  const_reference front() {
    return static_cast<const_reference>(orderedList<value_type>::front());
  }
  // access the last element
  const_reference back() {
    return static_cast<const_reference>(orderedList<value_type>::back());
  }

  // inserts element at the end
  void push(const_reference value) { this->push_back(value); }
  // removes the first element
  void pop();

  // ------------------- Доп. задание ---------------------------------------
  // Appends new elements to the end of the container.
  template <typename... Args>
  void insert_many_back(Args &&...args);
};
}  // namespace s21

template <typename T>
s21::queue<T>::~queue() {
  while (this->head_ != nullptr) pop();
  this->tail_ = nullptr;
}

template <typename T>
void s21::queue<T>::pop() {
  if (this->head_ != nullptr) {
    auto *temp = this->head_;
    // проверка на очередь из 1 элемента
    if (this->head_->pNext_ != nullptr) {
      this->head_ = this->head_->pNext_;
      this->head_->pPrev_ = nullptr;
    } else {
      this->head_ = nullptr;
      this->tail_ = nullptr;
    }
    delete temp;
    this->count_--;
  }
}

template <typename T>
template <typename... Args>
void s21::queue<T>::insert_many_back(Args &&...args) {
  for (auto &arg : {args...}) {
    push(arg);
  }
}

#endif