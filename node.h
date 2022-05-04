#ifndef MISOLIB_AVL_NODE_H
#define MISOLIB_AVL_NODE_H

template <typename T>
static T max(const T &a, const T &b) { return a > b ? a : b; }
template <typename T>
static void chmax(T &a, const T &b) { if (b > a) a = b; }

namespace AVL {
  template<typename T>
  struct Node {
    Node<T> *left, *right;
    T value;
    int height;

    Node(const T &val) : value(val), height(1), left(nullptr), right(nullptr) {}

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

    bool walk_forward(bool (*f)(const T &)) const {
      return this == nullptr || (
           this->left->walk_forward(f)
        && f(this->value)
        && this->right->walk_forward(f)
      );
    }
    bool walk_forward(bool (*f)(const Node<T> &)) const {
      return this == nullptr || (
           this->left->walk_forward(f)
        && f(*this)
        && this->right->walk_forward(f)
      );
    }
    bool walk_backward(bool (*f)(const T &)) const {
      return this == nullptr || (
           this->right->walk_backward(f)
        && f(this->value)
        && this->left->walk_backward(f)
      );
    }
    bool walk_backward(bool (*f)(const Node<T> &)) const {
      return this == nullptr || (
           this->right->walk_backward(f)
        && f(*this)
        && this->left->walk_backward(f)
      );
    }
  };
}

#endif //MISOLIB_AVL_NODE_H

