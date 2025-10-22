#pragma once
#include <iostream>
#include <vector.hpp>

namespace eval{
using lin::Vector;

class ConfusionMatrix {
  public:

  explicit ConfusionMatrix(size_t classCount);
  void calculate(const Vector<int>& predictions, const Vector<int>& truth);

  size_t classCount;
  size_t sampleCount;

  Vector<size_t> truePositives;
  Vector<size_t> trueNegatives;
  Vector<size_t> falsePositives;
  Vector<size_t> falseNegatives;

  private:
  void calculateTrueNegatives();

  friend std::ostream& operator<<(std::ostream& os, const ConfusionMatrix& mat);
};

}
