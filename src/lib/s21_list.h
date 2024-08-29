#ifndef S21_LIST_H
#define S21_LIST_H

#include "s21_ordered_list.h"

namespace s21 {

template <typename T>
class list : public orderedList<T> {
 public:
  // -------------------  обьявление внутреннего класса: итератора
  // -------------------
  class ListIterator;
  class ListConstIterator;

  // -------------------- переопределения типов ------------------------
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = size_t;
  using Node = typename orderedList<value_type>::Node;

  // -------------------  конструкторы и деструкторы -------------------
  using orderedList<value_type>::orderedList;

  // -------------------  методы для работы со списком -------------------

  // clears the contents
  void clear();
  // inserts element into concrete pos and returns the iterator that points to
  // the new element
  iterator insert(iterator pos, const_reference value);
  // erases element at pos
  void erase(iterator pos);
  // adds an element to the end
  void push_back(const value_type& data);
  // removes the last element
  void pop_back();
  // adds an element to the head
  void push_front(const value_type& data);
  // removes the first element
  void pop_front();
  // merges two sorted lists
  void merge(list& other);
  // transfers elements from list other starting from pos
  void splice(ListConstIterator pos, list& other);
  // reverses the order of the elements
  void reverse();
  // removes consecutive duplicate elements
  void unique();
  // sorts the elements
  void sort();

  // -------------Getters-------------------------------------
  reference front() noexcept { return *begin(); };
  const_reference front() const noexcept { return *begin(); }
  reference back() noexcept { return *(--begin()); }
  const_reference back() const noexcept { return *(--begin()); }

  // ------------Self-Made----------------------
  void show_list();

  // ------------------- Доп. задание ---------------------------------------
  template <typename... Args>
  iterator insert_many(iterator pos, Args&&... args);
  template <typename... Args>
  void insert_many_back(Args&&... args);
  template <typename... Args>
  void insert_many_front(Args&&... args);

  // ------------------- методы для работы с итератором -------------------

  // returns an iterator to the beginning
  iterator begin() { return ListIterator(this->head_, *this); }
  // returns an iterator to the end
  iterator end() { return ListIterator(nullptr, *this); }
  // the same but const
  ListConstIterator begin() const {
    return ListConstIterator(this->head_, *this);
  }
  ListConstIterator end() const { return ListConstIterator(nullptr, *this); }

};  // list

// --------------------------------------- классы
// итераторов------------------------------------------

template <typename T>
class list<T>::ListIterator {
 public:
  ListIterator(Node* node = nullptr, list<T>& pList = nullptr)
      : current(node), pList(pList) {}
  ListIterator(const ListIterator& other)
      : current(other.current), pList(other.pList) {}

  T& operator*() { return current->data_; }

  ListIterator& operator++() {
    if (this->pList.count_ != 0) current = current->pNext_;
    return *this;
  }

  // постфиксный
  ListIterator operator++(int) {
    ListIterator temp = *this;
    ++(*this);
    return temp;
  }

  ListIterator& operator--() {
    if (current == pList.head_)
      current = pList.tail_;
    else if (current != nullptr)
      current = current->pPrev_;
    else
      current = pList.tail_;
    return *this;
  }

  // постфиксный
  ListIterator& operator--(int) {
    ListIterator temp = *this;
    --(*this);
    return temp;
  }

  ListIterator& operator=(const ListIterator& other) {
    if (this != &other) {
      this->current = other.current;
    }
    return *this;
  }

  bool operator==(const ListIterator& other) const {
    return current == other.current;
  }
  bool operator!=(const ListIterator& other) const {
    return !(current == other.current);
  }

  // текущий узел, на который указывает итератор
  Node* current;
  // доступ к самому листу для итератора
  list<T>& pList;
};

template <typename T>
class list<T>::ListConstIterator {
 public:
  ListConstIterator(const Node* node, const list<T>& pList)
      : current(node), pList(pList) {}

  const T& operator*() const { return current->data; }
  ListConstIterator(const ListIterator& iter)
      : current(iter.current), pList(iter.pList) {}

  ListConstIterator& operator++() {
    if (this->pList.count_ != 0) current = current->pNext_;
    return *this;
  }

  ListConstIterator operator++(int) {
    ListConstIterator temp = *this;
    ++(*this);
    return temp;
  }

  const Node* getCurrent() const { return current; }

  ListConstIterator& operator--() {
    if (current == pList.head_)
      current = pList.tail_;
    else if (current != nullptr)
      current = current->pPrev;
    else
      current = pList.tail_;
    return *this;
  }

  ListConstIterator& operator--(int) {
    ListConstIterator temp = *this;
    --(*this);
    return temp;
  }

  bool operator==(const ListConstIterator& other) const {
    return current == other.current;
  }
  bool operator!=(const ListConstIterator& other) const {
    return !(*this == other);
  }

  const Node* current;
  const list<T>& pList;
};

// --------------------------------------- методы
// листа-------------------------------------

template <typename T>
void list<T>::erase(iterator pos) {
  iterator it = this->begin();
  iterator last = this->end();
  --last;
  if (it == pos)
    pop_front();
  else if (pos == last)
    pop_back();
  else {
    Node* current = this->head_;
    Node* tmp;
    --pos;
    while (it != pos) {
      current = current->pNext_;
      ++it;
    }
    tmp = current->pNext_;
    tmp->pNext_->pPrev_ = current;
    current->pNext_ = tmp->pNext_;
    delete tmp;
    --this->count_;
  }
}

template <typename T>
typename list<T>::ListIterator list<T>::insert(ListIterator pos,
                                               const_reference value) {
  Node* current = this->head_;
  iterator it = this->begin();
  // если вначале
  if (it == pos) {
    push_front(value);
    --it;
  }  // если в конце
  else if (pos == this->end()) {
    push_back(value);
    --it;
  }  // если по середине
  else {
    Node* newNode = new Node(value);
    --pos;
    while (it != pos) {
      ++it;
      current = current->pNext_;
    }
    newNode->pNext_ = current->pNext_;
    current->pNext_ = newNode;
    newNode->pPrev_ = current;
    newNode->pNext_->pPrev_ = newNode;
    ++it;
    ++this->count_;
  }
  return it;
}

template <typename T>
void list<T>::push_back(const value_type& data) {
  Node* newNode = new Node(data);
  if (this->head_ == nullptr) {
    this->head_ = newNode;
    this->tail_ = newNode;
  } else {
    this->tail_->pNext_ = newNode;
    newNode->pPrev_ = this->tail_;
    this->tail_ = newNode;
  }
  this->count_++;
}

template <typename T>
void list<T>::show_list() {
  Node* current = this->head_;
  if (current == nullptr) std::cout << "empty list\n";
  while (current != nullptr) {
    std::cout << current->data_ << std::endl;
    current = current->pNext_;
  }
}

template <typename T>
void list<T>::pop_front() {
  if (this->head_ != nullptr) {
    Node* temp = this->head_;
    if (this->head_->pNext_ != nullptr) {
      this->head_ = this->head_->pNext_;
      this->head_->pPrev_ = nullptr;
    } else {
      this->head_ = nullptr;
      this->tail_ = nullptr;
    }
    delete temp;
  } else {
    throw std::out_of_range("List is empty");
  }
  this->count_--;
}

template <typename T>
void list<T>::pop_back() {
  if (this->tail_ != nullptr) {
    Node* temp = this->tail_;
    if (this->tail_->pPrev_ != nullptr) this->tail_ = this->tail_->pPrev_;
    this->tail_->pNext_ = nullptr;
    delete temp;
  } else {
    throw std::out_of_range("List is empty");
  }
  this->count_--;
}

template <typename T>
void list<T>::push_front(const value_type& data) {
  Node* newNode = new Node(data);
  if (this->head_ == nullptr) {
    this->head_ = newNode;
    this->tail_ = newNode;
  } else {
    newNode->pNext_ = this->head_;
    this->head_->pPrev_ = newNode;
    this->head_ = newNode;
  }
  this->count_++;
}

template <typename T>
void list<T>::clear() {
  Node* current = this->head_;
  while (current != nullptr) {
    Node* next = current->pNext_;
    delete current;
    current = next;
  }
  this->head_ = nullptr;
  this->tail_ = nullptr;
  this->count_ = 0;
}

// вставляет все элементы второго списка в указанную позицию первого листа,
// после этого второй лист зачищается
template <typename T>
void list<T>::splice(ListConstIterator pos, list& other) {
  if (this != &other && other.count_ != 0) {
    Node* posNode = const_cast<Node*>(pos.getCurrent());
    if (posNode == nullptr) {
      if (this->count_ == 0) {
        this->tail_ = other.tail_;
        this->head_ = other.head_;
      } else {
        this->tail_->pNext_ = other.head_;
        this->tail_ = other.tail_;
      }

    } else if (posNode == this->head_) {
      other.tail_->pNext_ = this->head_;
      this->head_->pPrev_ = other.tail_;
      this->head_ = other.head_;

    } else {
      Node* before = posNode->pPrev_;
      before->pNext_ = other.head_;
      other.head_->pPrev_ = before;

      Node* after = posNode;
      after->pPrev_ = other.tail_;
      other.tail_->pNext_ = after;
    }

    this->count_ += other.count_;

    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.count_ = 0;
  }
}

// меняет голову с хвостом, а также next и prev у кажого узла
template <typename T>
void list<T>::reverse() {
  Node* current = this->head_;
  Node* prev = nullptr;
  Node* next = nullptr;

  while (current != nullptr) {
    next = current->pNext_;

    current->pNext_ = prev;
    current->pPrev_ = next;

    prev = current;
    current = next;
  }

  this->tail_ = this->head_;
  this->head_ = prev;
}

// удаляет последовательно идущие совпадающие элементы
template <typename T>
void list<T>::unique() {
  iterator begin_iterator = begin();
  iterator end_iterator = end();
  iterator prev_iterator = begin_iterator;

  ++begin_iterator;
  while (begin_iterator != end_iterator) {
    if (*begin_iterator == *prev_iterator) {
      erase(begin_iterator);
      begin_iterator = prev_iterator;
    } else {
      ++prev_iterator;
    }
    ++begin_iterator;
  }
}

template <typename T>
void list<T>::sort() {
  if (this->count_ <= 1) return;

  int swapped = 1;
  while (swapped) {
    swapped = 0;
    Node* current = this->head_;
    while (current != nullptr && current->pNext_ != nullptr) {
      if (current->data_ > current->pNext_->data_) {
        // обмен элементов местами
        T temp = current->data_;
        current->data_ = current->pNext_->data_;
        current->pNext_->data_ = temp;
        swapped = 1;
      }
      current = current->pNext_;
    }
  }
}

template <typename T>
void list<T>::merge(list& other) {
  iterator it = this->begin();
  while (!other.empty()) {
    value_type value = std::move(other.front());
    while (it != this->end() && *it < value) {
      ++it;
    }
    this->insert(it, std::move(value));
    other.pop_front();
  }
}

template <typename T>
template <typename... Args>
typename list<T>::ListIterator list<T>::insert_many(iterator pos,
                                                    Args&&... args) {
  int shift = 0;
  for (auto& arg : {args...}) {
    insert(pos, arg);
    shift++;
  }
  iterator buf = pos;
  for (int i = 0; i < shift; i++) --buf;
  return buf;
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_back(Args&&... args) {
  iterator it = end();
  (insert(it, std::forward<Args>(args)), ...);
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_front(Args&&... args) {
  iterator it = begin();
  (insert(it, std::forward<Args>(args)), ...);
}

}  // namespace s21

#endif