#include <iostream>
#include "avl_tree.h"

bool print(const int &v) {
  std::printf(" %d", v);
  return true;
}

int main() {
  AVL::Node<int> *tree = nullptr;

  for (int i = 1; i <= 15; ++i) AVL::insert(&tree, i);
  tree->walk_forward(print);

  AVL::remove(&tree);
}
