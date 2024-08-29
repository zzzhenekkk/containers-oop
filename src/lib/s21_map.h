#ifndef S21_MAP_H
#define S21_MAP_H

#include <iostream>
#include <utility>  // Для std::pair
#include <vector>

#include "s21_tree.h"
#include "s21_vector.h"

namespace s21 {

template <typename Key, typename Value>
class map : public Tree<std::pair<Key, Value>> {
 public:
  using key_type = Key;
  using mapped_type = Value;
  using NodeType = typename Tree<std::pair<Key, Value>>::Node;
  using value_type = std::pair<const Key, Value>;
  using iterator = typename Tree<std::pair<Key, Value>>::Iterator;
  using reference = value_type&;
  using const_reference = const value_type&;

  using Tree<std::pair<Key, Value>>::Tree;

  std::pair<iterator, bool> insert(const std::pair<const Key, Value>& value) {
    return this->insert_tree(value,
                             Tree<std::pair<Key, Value>>::INSERT_NO_DUPLICATE);
  }

  std::pair<iterator, bool> insert(const Key& key, const Value& obj) {
    return this->insert_tree(std::make_pair(key, obj),
                             Tree<std::pair<Key, Value>>::INSERT_NO_DUPLICATE);
  }

  std::pair<iterator, bool> insert_or_assign(const Key& key, const Value& obj) {
    return this->insert_tree(std::make_pair(key, obj),
                             Tree<std::pair<Key, Value>>::INSERT_WITH_UPDATE);
  }

  iterator find(const Key& key) {
    NodeType* current = this->root_;
    iterator result = this->end();
    while (current != nullptr) {
      if (key > current->data.first) {
        current = current->right;
      } else if (current->data.first > key) {
        current = current->left;
      } else {
        result = iterator(current, *this);
        break;
      }
    }
    return result;
  }

  // Доступ к элементу по ключу
  Value& at(const Key& key) {
    iterator it = find(key);
    if (it == this->end()) {
      throw std::out_of_range("Key not found");
    }
    return (*it).second;
  }

  // Доступ или вставка элемента по заданному ключу
  Value& operator[](const Key& key) {
    iterator it = find(key);
    if (it == this->end()) {
      auto result =
          this->insert_tree(std::make_pair(key, Value{}),
                            Tree<std::pair<Key, Value>>::INSERT_WITH_UPDATE);
      if (result.second) {  // Если элемент был вставлен
        it = result.first;
      }
    }
    return (*it).second;
  }

  // Содержит ли map элемент с ключом Key
  bool contains(const Key& key) {
    iterator it = find(key);
    return (it != this->end());
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> results;
    (void)(results.push_back(this->insert(std::forward<Args>(args))), ...);
    return results;
  }

 protected:
  virtual bool const_comparator(
      const std::pair<Key, Value>& data1,
      const std::pair<Key, Value>& data2) const override {
    return data1.first > data2.first;
  }
};

}  // namespace s21

#endif  // S21_MAP_H
