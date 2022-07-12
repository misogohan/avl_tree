#include <cstdio>
#include <iostream>
#include "avl_tree.h"

using namespace misolib;
using namespace std;

bool print(const int &v) {
  printf(" %d", v);
  return true;
}

int main() {
  AVL::Node<int> *tree = nullptr;

  for (int i = 1; i <= 15; ++i) AVL::insert(&tree, i);
  tree->scan_forward(print);
  puts("");
  // 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

  AVL::insert(&tree, 8);
  tree->scan_forward(print);
  puts("");
  // 1 2 3 4 5 6 7 8 8 9 10 11 12 13 14 15

  AVL::consume_lt<int>(&tree, 9, [](const int &v) { printf(" %d", v); });
  puts("");
  // 1 2 3 4 5 6 7 8 8

  tree->scan_forward(print);
  puts("");
  // 9 10 11 12 13 14 15 

  AVL::remove(&tree);
}
