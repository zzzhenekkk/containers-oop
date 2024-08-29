#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include <iostream>
#include <utility>  // Для std::pair

#include "s21_set.h"
#include "s21_tree.h"
#include "s21_vector.h"

namespace s21 {

template <typename Key>
class multiset : public set<Key> {
 public:
  using key_type = Key;
  using value_type = key_type;
  using NodeType = typename multiset<Key>::Node;
  using reference = value_type&;
  using const_reference = const value_type&;
  using tree_type = set<key_type>;
  using iterator = typename tree_type::Iterator;
  using const_iterator = typename tree_type::ConstIterator;

  using set<Key>::set;

  iterator insert(const value_type& value) {
    return this->insert_tree(value, Tree<Key>::INSERT_DUPLICATES).first;
  }

  size_t count(const Key& key) const {
    size_t count = 0;
    for (auto it = this->find_tree(key); it != this->end() && *it == key;
         ++it) {
      if (*it == key) {
        ++count;
      }
    }
    return count;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    auto it_end = this->find_tree(key);
    auto it_begin(it_end);
    while (it_end != this->end() && *it_end == key) {
      ++it_end;
    }
    return std::make_pair(it_begin, it_end);
  }

  iterator upper_bound(const Key& key) {
    NodeType* current = this->root_;
    NodeType* result = nullptr;
    while (current != nullptr) {
      if (key >= current->data) {
        current = current->right;
      } else {
        result = current;
        current = current->left;
      }
    }
    iterator it(result, *this);
    return (result != nullptr) ? it : this->end();
  }

  iterator lower_bound(const Key& key) {
    NodeType* current = this->root_;
    NodeType* result = nullptr;
    while (current != nullptr) {
      if (key > current->data) {
        current = current->right;
      } else {
        result = current;
        current = current->left;
      }
    }
    iterator it(result, *this);
    return (result != nullptr) ? it : this->end();
  }

  void merge(Tree<Key>& other) override {
    if (this != &other) {
      iterator it = other.begin();
      while (it != other.end()) {
        this->insert_tree(*it, Tree<Key>::INSERT_DUPLICATES);
        iterator delPos = it;
        ++it;
        other.erase(delPos);
      }
    }
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> results;
    (void)(results.push_back(
               std::make_pair((this->insert(std::forward<Args>(args))), true)),
           ...);
    return results;
  }

 protected:
  using set<Key>::insert;
};

}  // namespace s21

#endif  // S21_MULTISET_H
