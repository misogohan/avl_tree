#ifndef MISOLIB_AVL_TREE_H
#define MISOLIB_AVL_TREE_H

#include <functional>

template <typename T>
inline static T max(const T &a, const T &b) { return a > b ? a : b; }
template <typename T>
static void chmax(T &a, const T &b) { if (b > a) a = b; }

namespace misolib {
  namespace AVL {
    template<typename T>
    struct Node {
      Node<T> *left, *right;
      T value;
      int height;

      Node(const T &val) : left(nullptr), right(nullptr), value(val), height(1) {}

      int left_height() const {
        if (left == nullptr) return 0;
        return left->height;
      }

      int right_height() const {
        if (right == nullptr) return 0;
        return right->height;
      }

      int calc_height() {
        height = max(left_height(), right_height()) + 1;
        return height;
      }

      int deep_calc_height() {
        height = 0;
        if (left != nullptr) chmax(height, left->deep_calc_height());
        if (right != nullptr) chmax(height, right->deep_calc_height());
        return ++height;
      }

      int balance_factor() const {
        return left_height() - right_height();
      }

      bool walk_forward(std::function<bool(const Node<T>&)> f) const {
        return (
             (this->left == nullptr || this->left->walk_forward(f))
          && f(*this)
          && (this->right == nullptr || this->right->walk_forward(f))
        );
      }
      bool scan_forward(std::function<bool(const T&)> f) const {
        return (
             (this->left == nullptr || this->left->scan_forward(f))
          && f(this->value)
          && (this->right == nullptr || this->right->scan_forward(f))
        );
      }
      bool walk_backward(std::function<bool(const Node<T>&)> f) const {
        return (
             (this->right == nullptr || this->right->walk_backward(f))
          && f(*this)
          && (this->left == nullptr || this->left->walk_backward(f))
        );
      }
      bool scan_backward(std::function<bool(const T&)> f) const {
        return (
             (this->right == nullptr || this->right->scan_backward(f))
          && f(this->value)
          && (this->left == nullptr || this->left->scan_backward(f))
        );
      }
    };



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
    bool find(Node<T> **from, const T &x) {
      Node<T> *p = *from;
      if (p == nullptr) return false;

      if (p->value == x) return true;

      Node<T> **next_target = &(x < p->value ? p->left : p->right);
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
    Node<T> *pop_front(Node<T> **target) {
      Node<T> *p = *target;
      if (p == nullptr) return nullptr;

      if (p->left == nullptr) {
        *target = p->right;
        p->right = nullptr;
        
        return p;
      } else {
        Node<T> *res = pop_front(&p->left);

        p->calc_height();
        rotate(target);

        return res;
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
    void consume_lt(Node<T> **target, const T &base, std::function<void(const T &)> consumer) {
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
    void consume_gt(Node<T> **target, const T &base, std::function<void(const T &)> consumer) {
      Node<T> *p = *target;
      if (p == nullptr) return;

      if (base < p->value) {
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
}

#endif //MISOLIB_AVL_TREE_H

