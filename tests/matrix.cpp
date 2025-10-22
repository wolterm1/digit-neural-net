#include "../include/matrix.hpp"

#include <cmath>
#include <iostream>
#include <unordered_set>
using lin::Matrix;
using std::cout;
using std::unordered_multiset;




int main() {
  Matrix<int> einheitsMatrix = Matrix<int>::createIdentity(4, 4);
  cout << "einheitsMatrix: \n" << einheitsMatrix << '\n';

  Matrix<int> fillMatrix(4, 4, 3);
  cout << "fillMatrix: \n" << fillMatrix << '\n';

  unordered_multiset<long long> multiset({213, 23, 4, 345});
  Matrix<int> itMat(2, 4, multiset.begin(), multiset.end());
  cout << "Constructed Matrix from begin and end iterators from multiset "
          "{213,23,4,345} \n"
       << itMat << '\n';

  Matrix<int> vecMat({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  cout << "vecMatrix: \n" << vecMat << '\n';

  Matrix<int> Identity = Matrix<int>::createIdentity(4, 4);
  cout << "Identity: \n" << Identity << '\n';

  cout << "swapping vecMatrix and Identity" << '\n';
  Identity.swap(vecMat);
  cout << "vecMat:\n " << vecMat << "\n Identity: \n" << Identity << '\n';

  cout << "testing iterator, range based for-loop: " << '\n';
  for (auto matrixEntry : Identity) {
    cout << matrixEntry;
  }
  cout << "\n" << '\n';

  cout << "testing move assignment operator" << '\n';
  Matrix<int> tmpMat = Matrix<int>::createIdentity(4, 4);
  Matrix<int> targetMat = std::move(tmpMat);
  cout << "Moving tmpMat to target mat: \n tmpMat:" << tmpMat
       << "\n targetMat\n"
       << targetMat << '\n';

  cout << "transposing: " << '\n';
  Matrix<int> transposedUnit = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  cout << "transposedUnitMat: \n" << transposedUnit << '\n';

  Matrix<float> base(4, 5, 36.0F);
  auto lsqrt = [](float x) { return std::sqrt(x);};
  std::cout << base << "applying function std::sqrt(): " << base.applyElementWiseFunction(lsqrt) << '\n';

  std::cout << base.hadamardProductInplace(base);

}
