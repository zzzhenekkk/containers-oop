#ifndef S21_SET_H
#define S21_SET_H

#include <iostream>
#include <utility>  // Для std::pair
#include <vector>

#include "s21_tree.h"
#include "s21_vector.h"

namespace s21 {

template <typename Key>
class set : public Tree<Key> {
 public:
  using key_type = Key;
  using value_type = key_type;
  using NodeType = typename set<Key>::Node;
  using reference = value_type&;
  using const_reference = const value_type&;
  using tree_type = Tree<key_type>;
  using iterator = typename tree_type::Iterator;
  using const_iterator = typename tree_type::ConstIterator;

  using tree_type::Tree;

  std::pair<iterator, bool> insert(const value_type& value) {
    return this->insert_tree(value, Tree<Key>::INSERT_NO_DUPLICATE);
  }

  iterator find(const Key& key) { return this->find_tree(key); }

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
};

}  // namespace s21

#endif  // S21_SET_H
