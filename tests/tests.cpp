#include <cassert>

#include "../include/linalg.hpp"
#include "../include/vector.hpp"
using lin::Vector;

namespace {
void testDot() {
  Vector<int> firstVec({1, 2, 3});
  Vector<int> secondVec({2, 4, 8});

  assert(dot(firstVec, secondVec) == 34);
}

void testCross() {
  Vector<int> firstVec({1, 2, 3});
  Vector<int> secondVec({2, 4, 8});

  Vector<int> crossVector = cross(firstVec, secondVec);
  Vector<int> expected({4, -2, 0});

  assert(crossVector == expected);
}
}  // namespace

int main() {
  testDot();
  testCross();
}
