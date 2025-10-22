#include "optimizer.hpp"
#include <cmath>
#include "matrix.hpp"
#include "vector.hpp"
#include "linalg.hpp"

using lin::Vector;
using lin::Matrix;

namespace nn {

constexpr auto square_root = [](float x) { return std::sqrt(x); };


Optimizer::Optimizer(float learnRate, float beta1, float beta2) : learnRate(learnRate), beta1(beta1), beta2(beta2), stepTime(0) {
  initialized = false;
}

void Optimizer::lazyInitMeanAndVariance(const Vector<Matrix<float>>& weights, const Vector<Vector<float>>& biases) {
  if(!initialized) {
    initialized = true;

    size_t layerCount = weights.getSize();
    allocateMomentVectors(layerCount);
    for (size_t i = 0; i < layerCount; ++i) {
      weightMean[i] = Matrix<float>(weights[i].getRows(), weights[i].getColumns(), 0.0F);
      weightVariance[i] = Matrix<float>(weights[i].getRows(), weights[i].getColumns(), 0.0F);
      biasMean[i] = Vector<float>(biases[i].getSize(), 0.0F);
      biasVariance[i] = Vector<float>(biases[i].getSize(), 0.0F);
    }
  }
}

void Optimizer::allocateMomentVectors(size_t layers) {
  weightMean = Vector<Matrix<float>>(layers);
  weightVariance = Vector<Matrix<float>>(layers);
  biasMean = Vector<Vector<float>>(layers);
  biasVariance = Vector<Vector<float>>(layers);
}

void Optimizer::resetGradientSum(Vector<Matrix<float>>& wGradSum, Vector<Vector<float>>& bGradSum) {
  for (size_t i = 0; i < wGradSum.getSize(); ++i) {
    wGradSum[i] = Matrix(wGradSum[i].getRows(), wGradSum[i].getColumns(), 0.0F);
    bGradSum[i] = Vector(bGradSum[i].getSize(), 0.0F);
  }
}

void Optimizer::step(Vector<Matrix<float>>& weights, Vector<Vector<float>>& biases,
                      Vector<Matrix<float>>& wGradientSum, Vector<Vector<float>>& bGradientSum) {

  computeAdam(weights, biases, wGradientSum, bGradientSum);
  resetGradientSum(wGradientSum, bGradientSum);
}


void Optimizer::computeAdam(Vector<Matrix<float>>& weights, Vector<Vector<float>>& biases,
                      Vector<Matrix<float>>& wGradientSum, Vector<Vector<float>>& bGradientSum) {

  lazyInitMeanAndVariance(weights, biases);

  stepTime++;
  for (size_t i = 0; i < weights.getSize(); ++i) {
    // 1.0F - std::pow(beta, stepTime) should be cached for efficiency

    weightMean[i] = beta1 * weightMean[i] + (1-beta1) * wGradientSum[i];
    weightVariance[i] = beta2 * weightVariance[i] + (1.0F - beta2) * (wGradientSum[i].hadamardProductInplace(wGradientSum[i]));
    auto weightMeanCorrectedEst = weightMean[i] / (1.0F - std::pow(beta1, stepTime));
    auto weightVarianceCorrectedEst = weightVariance[i] / (1.0F - std::pow(beta2, stepTime));
    weights[i] -= learnRate * weightMeanCorrectedEst.hadamardDivisionInplace(weightVarianceCorrectedEst.applyElementWiseFunction(square_root) + eps);


    biasMean[i] = beta1 * biasMean[i] + (1-beta1) * bGradientSum[i];
    biasVariance[i] = beta2 * biasVariance[i] + (1.0F - beta2) * (bGradientSum[i].hadamardProductInplace(bGradientSum[i]));
    auto biasMeanCorrectedEst = biasMean[i] / (1.0F - std::pow(beta1, stepTime));
    auto biasVarianceCorrectedEst = biasVariance[i] / (1.0F - std::pow(beta2, stepTime));
    biases[i] -= learnRate * biasMeanCorrectedEst.hadamardDivisionInplace(biasVarianceCorrectedEst.applyElementWiseFunction(square_root) + eps);
  }
}

}
