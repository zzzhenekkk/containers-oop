#ifndef S21_STACK_H
#define S21_STACK_H

#include "s21_ordered_list.h"

namespace s21 {
template <typename T>
class stack : public orderedList<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  // для наследования всех конструкторв
  // using orderedList<value_type>::orderedList;
  stack() : orderedList<value_type>(){};
  stack(std::initializer_list<value_type> const &items)
      : orderedList<value_type>(items){};
  // copy constructor
  stack(const stack &s) : orderedList<value_type>(s){};
  // move constructor
  stack(stack &&s) : orderedList<value_type>(std::move(s)){};
  // assignment operator overload for moving object
  stack &operator=(stack &&s) {
    return static_cast<stack &>(
        orderedList<value_type>::operator=(std::move(s)));
  }
  ~stack();
  // accesses the top element
  const_reference top() { return this->back(); }
  // inserts element at the top
  void push(const_reference value) { this->push_back(value); }
  // removes the top element
  void pop();

  // ------------------- Доп. задание ---------------------------------------
  // Appends new elements to the top of the container.
  template <typename... Args>
  void insert_many_front(Args &&...args);
};
}  // namespace s21

template <typename T>
s21::stack<T>::~stack() {
  while (this->tail_ != nullptr) pop();
  this->head_ = nullptr;
}

template <typename T>
void s21::stack<T>::pop() {
  if (this->tail_ != nullptr) {
    auto *temp = this->tail_;
    if (this->tail_->pPrev_ != nullptr) {
      this->tail_ = this->tail_->pPrev_;
      this->tail_->pNext_ = nullptr;
    } else
      this->tail_ = nullptr;
    delete temp;
    this->count_--;
  }
}

template <typename T>
template <typename... Args>
void s21::stack<T>::insert_many_front(Args &&...args) {
  for (auto &arg : {args...}) {
    push(arg);
  }
}

#endif