#ifndef S21_TREE_H
#define S21_TREE_H

#include <iostream>
#include <utility>  // Для std::pair

#include "s21_container.h"
#include "s21_list.h"

namespace s21 {

template <typename DataType>
class Tree : public Container<DataType> {
 public:
  using size_type = std::size_t;
  struct Node;
  Tree() : root_(nullptr) {}
  Tree(std::initializer_list<DataType> const& items);
  Tree(const Tree& t);
  Tree(Tree&& t);
  ~Tree() { clear(); }

  class Iterator;
  class ConstIterator;

  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const { return ConstIterator(nullptr, this); }

  void erase(Iterator pos);
  void clear();
  void swap(Tree& other);
  virtual void merge(Tree& other);

  Tree& operator=(Tree&& other);

 protected:
  enum InsertMode {
    INSERT_NO_DUPLICATE = 1,  // Insert without duplicates and without updating
    INSERT_WITH_UPDATE = 2,   // Inserting and updating an existing element
    INSERT_DUPLICATES = 3     // Inserting duplicates
  };

  std::pair<typename Tree<DataType>::Iterator, bool> insert_tree(
      const DataType& data, int mode);
  Iterator find_tree(const DataType& data);
  ConstIterator find_tree(const DataType& data) const;
  void copy_tree(const Tree<DataType>& t);
  Node* find_min(Node* MinNode);
  Node* find_max(Node* node);
  const Node* find_min(const Node* node) const;
  const Node* find_max(const Node* node) const;

  virtual bool const_comparator(const DataType& data1,
                                const DataType& data2) const {
    return data1 > data2;
  }

  Node* root_ = nullptr;
};

// -------------------- constructors and destructors ------------------------
template <typename DataType>
Tree<DataType>::Tree(std::initializer_list<DataType> const& items) {
  for (const auto& item : items) {
    insert_tree(item, INSERT_DUPLICATES);
  }
}

template <typename DataType>
Tree<DataType>::Tree(const Tree<DataType>& t) : Tree() {
  copy_tree(t);
}

template <typename DataType>
Tree<DataType>::Tree(Tree<DataType>&& t) : Tree() {
  std::swap(root_, t.root_);
  std::swap(this->count_, t.count_);
}

template <typename DataType>
void Tree<DataType>::copy_tree(const Tree<DataType>& t) {
  clear();
  for (auto it = t.begin(); it != t.end(); ++it) {
    insert_tree(*it, INSERT_DUPLICATES);
  }
  this->count_ = t.count_;
}

template <typename DataType>
Tree<DataType>& Tree<DataType>::operator=(Tree<DataType>&& other) {
  clear();
  std::swap(this->root_, other.root_);
  std::swap(this->count_, other.count_);
  return *this;
}

// ---------------------------------- Node ---------------------------------
template <typename DataType>
struct Tree<DataType>::Node {
  DataType data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  Node(DataType data, Node* p = nullptr) : data(data), parent(p) {}
};

// ---------------------------------- Iterator ---------------------------------
template <typename DataType>
class Tree<DataType>::Iterator {
 protected:
  Node* current_;
  Tree<DataType>* tree_;

 public:
  Iterator(Node* node, Tree<DataType>& tree) : current_(node), tree_(&tree) {}
  Iterator(const Iterator& other) = default;
  DataType& operator*() const { return current_->data; }
  DataType* operator->() const { return &(current_->data); }
  Iterator& operator++();
  Iterator operator++(int);
  Iterator& operator--();
  Iterator operator--(int);
  Iterator& operator=(Iterator& other) {
    if (this != &other) {
      current_ = other.current_;
      tree_ = other.tree_;
    }
    return *this;
  }
  Iterator& operator=(Iterator&& other) {
    if (this != &other) {
      current_ = other.current_;
      tree_ = other.tree_;
    }
    return *this;
  }
  Node* getNode() { return current_; }

  bool operator==(const Iterator& other) const {
    return current_ == other.current_;
  }
  bool operator!=(const Iterator& other) const {
    return current_ != other.current_;
  }
};

template <typename DataType>
class Tree<DataType>::ConstIterator {
 protected:
  const Node* current_;
  const Tree<DataType>* tree_;

 public:
  ConstIterator(const Node* node, const Tree<DataType>* tree)
      : current_(node), tree_(tree) {}
  ConstIterator(const ConstIterator& other) = default;
  const DataType& operator*() const { return current_->data; }
  const DataType* operator->() const { return &(current_->data); }
  ConstIterator& operator++();
  ConstIterator operator++(int);
  ConstIterator& operator--();
  ConstIterator operator--(int);

  ConstIterator& operator=(ConstIterator& other) {
    if (this != &other) {
      current_ = other.current_;
      tree_ = other.tree_;
    }
    return *this;
  }
  ConstIterator& operator=(ConstIterator&& other) {
    if (this != &other) {
      current_ = other.current_;
      tree_ = other.tree_;
    }
    return *this;
  }

  bool operator==(const ConstIterator& other) const {
    return current_ == other.current_;
  }

  bool operator!=(const ConstIterator& other) const {
    return current_ != other.current_;
  }
};

template <typename DataType>
typename Tree<DataType>::Iterator Tree<DataType>::begin() {
  Node* current = find_min(root_);
  return Iterator(current, *this);
}

template <typename DataType>
typename Tree<DataType>::Iterator Tree<DataType>::end() {
  return Iterator(nullptr, *this);
}

template <typename DataType>
typename Tree<DataType>::Iterator& Tree<DataType>::Iterator::operator++() {
  if (current_ == nullptr && tree_->count_) {
    current_ = tree_->find_min(tree_->root_);
  } else if (current_->right != nullptr) {
    current_ = tree_->find_min(current_->right);
  } else {
    Node* parent = current_->parent;
    while (parent != nullptr && current_ == parent->right) {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template <typename DataType>
typename Tree<DataType>::Iterator Tree<DataType>::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename DataType>
typename Tree<DataType>::Iterator& Tree<DataType>::Iterator::operator--() {
  if (current_ == nullptr && tree_->count_) {
    current_ = tree_->find_max(tree_->root_);
  } else if (current_->left != nullptr) {
    current_ = current_->left;

  } else {
    Node* parent = current_->parent;
    while (parent != nullptr && current_ == parent->left) {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template <typename DataType>
typename Tree<DataType>::Iterator Tree<DataType>::Iterator::operator--(int) {
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

////////////////////////////////////////////////////////////////////

template <typename DataType>
typename Tree<DataType>::ConstIterator&
Tree<DataType>::ConstIterator::operator++() {
  if (current_ == nullptr) {
    current_ = tree_->find_min(tree_->root_);
  } else if (current_->right != nullptr) {
    current_ = tree_->find_min(current_->right);
  } else {
    const Node* parent = current_->parent;
    while (parent != nullptr && current_ == parent->right) {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template <typename DataType>
typename Tree<DataType>::ConstIterator
Tree<DataType>::ConstIterator::operator++(int) {
  ConstIterator tmp(*this);
  ++(*this);
  return tmp;
}

template <typename DataType>
typename Tree<DataType>::ConstIterator&
Tree<DataType>::ConstIterator::operator--() {
  if (current_ == nullptr) {
    current_ = tree_->find_max(tree_->root_);
  } else if (current_->left != nullptr) {
    current_ = tree_->find_max(current_->left);
  } else {
    const Node* parent = current_->parent;
    while (parent != nullptr && current_ == parent->left) {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template <typename DataType>
typename Tree<DataType>::ConstIterator
Tree<DataType>::ConstIterator::operator--(int) {
  ConstIterator tmp(*this);
  --(*this);
  return tmp;
}

template <typename DataType>
typename Tree<DataType>::ConstIterator Tree<DataType>::begin() const {
  return ConstIterator(find_min(root_), this);
}

// ----------------------------  methods  ------------------------------

template <typename DataType>
typename Tree<DataType>::Node* Tree<DataType>::find_min(Node* MinNode) {
  while (MinNode && MinNode->left) {
    MinNode = MinNode->left;
  }
  return MinNode;
}

template <typename DataType>
const typename Tree<DataType>::Node* Tree<DataType>::find_min(
    const Node* node) const {
  if (!node) return nullptr;
  while (node->left) node = node->left;
  return node;
}

template <typename DataType>
const typename Tree<DataType>::Node* Tree<DataType>::find_max(
    const Node* node) const {
  if (!node) return nullptr;
  while (node->right) node = node->right;
  return node;
}

template <typename DataType>
typename Tree<DataType>::Node* Tree<DataType>::find_max(Node* node) {
  if (!node) return nullptr;
  while (node->right) node = node->right;
  return node;
}

template <typename DataType>
void Tree<DataType>::clear() {
  while (root_ != nullptr) {
    erase(Iterator(root_, *this));
  }
  this->count_ = 0;
}

template <typename DataType>
std::pair<typename Tree<DataType>::Iterator, bool> Tree<DataType>::insert_tree(
    const DataType& data, int mode) {
  Node* current = root_;
  Node* parent = nullptr;
  bool isLeft = false;

  while (current != nullptr) {
    parent = current;  // Обновляем родителя на каждом шаге
    if (!const_comparator(data, current->data) &&
        !const_comparator(current->data, data)) {  // Элементы равны
      if (mode == INSERT_WITH_UPDATE) {
        current->data = data;
        return {Iterator(current, *this), false};
      } else if (mode == INSERT_NO_DUPLICATE) {
        return {Iterator(current, *this), false};
      }
      // Продолжаем движение вправо для режима с дубликатами
      isLeft = false;
      current = current->right;
    } else if (const_comparator(data, current->data)) {
      isLeft = false;
      current = current->right;
    } else {
      isLeft = true;
      current = current->left;
    }
  }

  Node* newNode = new Node(data, parent);
  if (parent == nullptr) {
    root_ = newNode;
  } else if (isLeft) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }

  this->count_++;
  return {Iterator(newNode, *this), true};
}

template <typename DataType>
typename Tree<DataType>::Iterator Tree<DataType>::find_tree(
    const DataType& data) {
  Node* current = root_;
  Iterator result = end();

  while (current != nullptr) {
    if (const_comparator(data, current->data)) {
      current = current->right;
    } else if (const_comparator(current->data, data)) {
      current = current->left;
    } else {
      result = Iterator(current, *this);
      break;
    }
  }

  return result;
}

template <typename DataType>
typename Tree<DataType>::ConstIterator Tree<DataType>::find_tree(
    const DataType& data) const {
  Node* current = root_;
  ConstIterator result = end();
  while (current != nullptr) {
    if (const_comparator(data, current->data)) {
      current = current->right;
    } else if (const_comparator(current->data, data)) {
      current = current->left;
    } else {
      result = ConstIterator(current, this);
      break;
    }
  }
  return result;
}

template <typename DataType>
void Tree<DataType>::erase(Iterator pos) {
  if (pos.getNode() == nullptr) return;

  Node* toDelete = pos.getNode();
  Node* child = nullptr;
  Node* parent = nullptr;

  // Node haven't child
  if (toDelete->left == nullptr && toDelete->right == nullptr) {
    if (toDelete == root_) {
      root_ = nullptr;
    } else {
      parent = toDelete->parent;
      if (parent->left == toDelete)
        parent->left = nullptr;
      else
        parent->right = nullptr;
    }
  }
  // Node have 1 child
  else if (toDelete->left == nullptr || toDelete->right == nullptr) {
    child = (toDelete->left != nullptr) ? toDelete->left : toDelete->right;
    if (toDelete == root_) {
      root_ = child;
      child->parent = nullptr;
    } else {
      parent = toDelete->parent;
      if (parent->left == toDelete)
        parent->left = child;
      else
        parent->right = child;
      child->parent = parent;
    }
  }
  // Node have 2 child
  else {
    Node* successor = find_min(toDelete->right);
    toDelete->data = successor->data;
    child = successor->right;
    parent = successor->parent;
    if (parent->left == successor)
      parent->left = child;
    else
      parent->right = child;
    if (child != nullptr) child->parent = parent;
    toDelete = successor;
  }

  delete toDelete;
  --this->count_;
}

template <typename DataType>
void Tree<DataType>::swap(Tree<DataType>& other) {
  std::swap(other.root_, root_);
  std::swap(other.count_, this->count_);
}

template <typename DataType>
void Tree<DataType>::merge(Tree<DataType>& other) {
  if (this != &other) {
    Iterator it = other.begin();
    while (it != other.end()) {
      if (find_tree(*it) == end()) {
        insert_tree(*it, INSERT_NO_DUPLICATE);
        Iterator delPos = it;
        ++it;
        other.erase(delPos);
      } else
        ++it;
    }
  }
}

}  // namespace s21
#endif  // S21_TREE_H
