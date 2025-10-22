#include "confusion_matrix.hpp"

namespace eval {

ConfusionMatrix::ConfusionMatrix(size_t classCount) : classCount(classCount), sampleCount(0) {
  truePositives = Vector<size_t>(classCount, 0);
  trueNegatives =  Vector<size_t>(classCount, 0);
  falsePositives = Vector<size_t>(classCount, 0);
  falseNegatives = Vector<size_t>(classCount, 0);
}

void ConfusionMatrix::calculate(const Vector<int>& prediction, const Vector<int>& truth) {
  sampleCount = prediction.getSize();
  for (size_t i = 0; i < sampleCount; ++i) {
    int pred = prediction[i];
    int actual = truth[i];

    if (pred == actual) {
      truePositives[pred] += 1;
    }
    else if (pred != actual) {
      falsePositives[pred] += 1;
      falseNegatives[actual] += 1;
    }
  }
  calculateTrueNegatives();
}


void ConfusionMatrix::calculateTrueNegatives() {
  for (int i = 0; i < classCount; ++i) {
    trueNegatives[i] = sampleCount - (truePositives[i] + falsePositives[i] + falseNegatives[i]);
  }
}


std::ostream& operator<<(std::ostream& os, const ConfusionMatrix& mat) {
  os << mat.truePositives << mat.falsePositives << mat.falseNegatives;
  return os;
}

}
