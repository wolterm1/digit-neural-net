#pragma once
#include "matrix.hpp"
#include "vector.hpp"

namespace nn {

using lin::Matrix;
using lin::Vector;


class Optimizer {

  private: 
  bool initialized;
  Vector<Matrix<float>> weightMean;
  Vector<Matrix<float>> weightVariance;
  Vector<Vector<float>> biasMean;
  Vector<Vector<float>> biasVariance;

  size_t stepTime;
  const float learnRate;

  const float beta1;
  const float beta2; 

  void lazyInitMeanAndVariance(const Vector<Matrix<float>>& weights, const Vector<Vector<float>>& biases);
  void allocateMomentVectors(size_t Layers);
  void resetGradientSum(Vector<Matrix<float>>& wGradSum, Vector<Vector<float>>& bGradSum);

  public:
  static constexpr float eps = 1e-8F;

  explicit Optimizer(float learnRate = 1e-3, float beta1 = 0.9, float beta2 = 0.999);

  void step(Vector<Matrix<float>>& weights, Vector<Vector<float>>& biases,
                      Vector<Matrix<float>>& wGradientSum, Vector<Vector<float>>& bGradientSum);

  void computeAdam(Vector<Matrix<float>>& weights, Vector<Vector<float>>& biases,
                      Vector<Matrix<float>>& wGradientSum, Vector<Vector<float>>& bGradientSum);
};

}
