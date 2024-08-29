#ifndef S21_ORDERED_orederedList_H
#define S21_ORDERED_orederedList_H

#include <iostream>

#include "s21_container.h"

namespace s21 {
template <typename T>
class orderedList : public Container<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  // -------------------  конструкторы и деструкторы -------------------
  // default constructor, creates empty orderedList
  orderedList() : Container<value_type>(), head_(nullptr), tail_(nullptr) {}
  explicit orderedList(size_type n)
      : Container<value_type>(), head_(nullptr), tail_(nullptr) {
    for (size_type i = 0; i < n; i++) push_back(const_reference{});
  }
  // initializer list constructor, creates orderedList initizialized using
  // std::initializer_list
  orderedList(std::initializer_list<value_type> const &items)
      : Container<value_type>(), head_(nullptr), tail_(nullptr) {
    for (const_reference item : items) push_back(item);
  }
  // copy constructor
  orderedList(const orderedList &l);
  //  move constructor
  orderedList(orderedList &&l);
  orderedList &operator=(orderedList &&l);
  // destructor
  ~orderedList();
  // получение элемента по индексу
  reference operator[](size_type ind);
  // распечатка для себя
  void print() const {
    Node *current = head_;
    for (size_type i = 0; i < this->count_; i++) {
      std::cout << current->data_ << std::endl;
      current = current->pNext_;
    }
  }
  // респечатка наоборот
  void print_reverse() const {
    Node *current = tail_;
    for (size_type i = 0; i < this->count_; i++) {
      std::cout << current->data_ << std::endl;
      current = current->pPrev_;
    }
  }
  // swaps the contents
  void swap(orderedList &other);

  // Для наследуемых классов определены классы, методы и поля
  // класс узел
 protected:
  class Node {
   public:
    value_type data_;
    Node *pNext_;
    Node *pPrev_;

    explicit Node(const value_type &data_ = value_type(),
                  Node *pNext_ = nullptr, Node *pPrev_ = nullptr)
        : data_(data_), pNext_(pNext_), pPrev_(pPrev_) {}
  };
  // указатель на голову
  Node *head_{};
  // указатель на конец
  Node *tail_{};

  // access the first element
  const_reference front() {
    if (head_ == nullptr) throw ::std::range_error("\nempty orderedlist\n");
    return (const_reference)head_->data_;
  }
  // access the last element
  const_reference back() {
    if (tail_ == nullptr) throw ::std::range_error("\nempty orderedlist\n");
    return (const_reference)tail_->data_;
  }

  void push_back(const_reference value);
};

}  // namespace s21

template <typename T>
s21::orderedList<T>::orderedList(const orderedList &l) {
  Node *current = l.head_;
  while (current != nullptr) {
    push_back(current->data_);
    current = current->pNext_;
  }
}

template <typename T>
s21::orderedList<T>::orderedList(orderedList &&l) {
  this->count_ = l.count_;
  head_ = l.head_;
  tail_ = l.tail_;
  l.count_ = 0;
  l.head_ = nullptr;
  l.tail_ = nullptr;
}

template <typename T>
s21::orderedList<T> &s21::orderedList<T>::operator=(orderedList &&l) {
  if (this != &l) {
    this->~orderedList();
    this->count_ = l.count_;
    head_ = l.head_;
    tail_ = l.tail_;
    l.count_ = 0;
    l.head_ = nullptr;
    l.tail_ = nullptr;
  }
  return *this;
}

template <typename T>
s21::orderedList<T>::~orderedList() {
  Node *current = head_;
  while (current != nullptr) {
    Node *temp = current;
    current = current->pNext_;
    delete temp;
  }
}

template <typename T>
T &s21::orderedList<T>::operator[](size_type ind) {
  // проверка на границы области на совести разработчика
  Node *current = head_;
  for (size_type i = 0; i < ind && current != nullptr; i++) {
    current = current->pNext_;
  }
  return current->data_;
}

template <typename T>
void s21::orderedList<T>::swap(orderedList &other) {
  if (this != &other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(this->count_, other.count_);
  }
}

template <typename T>
void s21::orderedList<T>::push_back(const_reference value) {
  Node *current = new Node(value);
  if (tail_ == nullptr) {
    head_ = current;
    tail_ = current;
  } else {
    tail_->pNext_ = current;
    current->pPrev_ = tail_;
    tail_ = current;
  }
  this->count_++;
}
#endif  // S21_ORDERED_ORDEREDLIST_H