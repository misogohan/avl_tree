#ifndef MISOLIB_AVL_TREE_H
#define MISOLIB_AVL_TREE_H

#include "node.h"

namespace AVL {
  template<typename T>
  void left_rotate(Node<T> **target) {
    Node<T> *left = *target;
    Node<T> *right = left->right;

    *target = right;
    left->right = right->left;
    right->left = left;

    left->calc_height();
    right->calc_height();
  }

  template<typename T>
  void right_rotate(Node<T> **target) {
    Node<T> *right = *target;
    Node<T> *left = right->left;

    *target = left;
    right->left = left->right;
    left->right = right;

    right->calc_height();
    left->calc_height();
  }

  template<typename T>
  void rotate(Node<T> **target) {
    Node<T> *p = *target;
    int bf = p->balance_factor();

    if (bf < -1) {
      if (p->right->balance_factor() > 0) right_rotate(&p->right);
      left_rotate(target);
      rotate(&(*target)->left);
    } else if (bf > 1) {
      if (p->left->balance_factor() < 0) left_rotate(&p->left);
      right_rotate(target);
      rotate(&(*target)->right);
    }
  }

  template<typename T>
  Node<T> *find(Node<T> **from, const T &x) {
    Node<T> *p = *from;
    if (p == nullptr) return nullptr;

    if (p->value == x) return p;

    Node<T> **next_target = &(Node<T>::less(p->value, x) ? p->left : p->right);
    return find(next_target, x);
  }

  template<typename T>
  void insert(Node<T> **target, const T &value) {
    Node<T> *p = *target;

    if (p == nullptr) {
      *target = new Node<T>(value);
      return;
    }

    Node<T> **next_target = &(value < p->value ? p->left : p->right);
    insert(next_target, value);
    (*target)->calc_height();
    rotate(target);
  }

  template<typename T>
  Node<T> *remove_first(Node<T> **target) {
    Node<T> *p = *target;
    // if (p == nullptr) return nullptr;
    if (p->left == nullptr) {
      *target = p->right;
      return p;
    } else {
      Node<T> *res = remove_first(&p->left);
      (*target)->calc_height();
      rotate(target);
      return res;
    }
  }

  template<typename T>
  void remove(Node<T> **target, const T &value) {
    Node<T> *p = *target;
    if (p == nullptr) return;

    if (value == p->value) {
      remove(&p->left, value);
      remove(&p->right, value);

      if (p->left == nullptr && p->right == nullptr) {
        *target = nullptr;
        delete p;
      } else if (p->left != nullptr && p->right != nullptr) {
        Node<T> *sub = remove_first(&p->right);
        p->value = sub->value;
        delete sub;
        rotate(target);
      } else {
        Node<T> *next = p->left != nullptr ? p->left : p->right;
        *target = next;
        delete p;
      }
      return;
    } else {
      Node<T> **next_target = &(value < p->value ? p->left : p->right);
      remove(next_target, value);
      (*target)->calc_height();
      rotate(target);
    }
  }
  template<typename T>
  void remove(Node<T> **target) {
    Node<T> *p = *target;
    if (p == nullptr) return;
    remove(&p->left);
    remove(&p->right);
    *target = nullptr;
    delete p;
  }

  template <typename T>
  void consume_lt(Node<T> **target, const T &base, void (*consumer)(const T &)) {
    Node<T> *p = *target;
    if (p == nullptr) return;

    if (p->value < base) {
      consume_lt(&p->left, base, consumer);
      consumer(p->value);
      consume_lt(&p->right, base, consumer);
      *target = p->right;
      delete p;
    } else {
      consume_lt(&p->left, base, consumer);
      rotate(target);
    }
  }
  template <typename T>
  void consume_gt(Node<T> **target, const T &base, void (*consumer)(const T &)) {
    Node<T> *p = *target;
    if (p == nullptr) return;

    if (p->value > base) {
      consume_gt(&p->right, base, consumer);
      consumer(p->value);
      consume_gt(&p->left, base, consumer);
      *target = p->left;
      delete p;
    } else {
      consume_gt(&p->right, base, consumer);
      rotate(target);
    }
  }
}

#endif //MISOLIB_AVL_TREE_H

