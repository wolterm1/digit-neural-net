#include <cassert>

#include "../include/linalg.hpp"
#include "../include/matrix.hpp"
#include "../include/vector.hpp"

using lin::Matrix;
using lin::Vector;
using lin::outer_product;

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

void testMatrixVectorMult() {
  Matrix<int> mat({
      {1, 2, 3},
      {4, 5, 6}
  });
  Vector<int> vec({1, 0, -1});

  Vector<int> result = mat * vec;
  Vector<int> expected({-2, -2});

  assert(result == expected);
}

void testMatrix_getTranspose(){
  Matrix<float> mat({{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}});
  auto I = mat.getTranspose();
}

void testCompMult() {
  Vector<int> vec({1, 2, -3});
  Vector<int> vec2({3,2,2});
  Vector<int> result = (vec, vec2);
  Vector<int> expected({3,4,-6});

  assert(result == expected);
}

void testVectorMatrixMult() {
  Vector<int> vec({2, 1, 1});
  Matrix<int> mat({
      {2,3},
      {1,4},
      {0,1}
  });
  Vector<int> result = vec * mat;
  Vector<int> expected({5,11});

  assert(result == expected);
}

void test_outer_product(){
  Vector<int> vec1({4,3,5});
  Vector<int> vec2({2,9});

  auto result = outer_product(vec1, vec2);

  Matrix<int> expected({{8,36}, {6,27}, {10,45}});


  assert(result == expected);
}
}  // namespace

int main() {
  testDot();
  testCross();
  testMatrixVectorMult();
  testVectorMatrixMult();
  testMatrix_getTranspose();
  test_outer_product();
}
