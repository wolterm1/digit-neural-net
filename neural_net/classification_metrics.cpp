#include "classification_metrics.hpp"

namespace eval {
  
Vector<float> calculateRecall(const ConfusionMatrix& confMat) {
  Vector<float> rec(confMat.classCount);
  for (size_t i = 0; i < confMat.classCount; ++i) {
    float tP = static_cast<float>(confMat.truePositives[i]);
    float fP = static_cast<float>(confMat.falsePositives[i]);
    float fN = static_cast<float>(confMat.falseNegatives[i]);

    rec[i] = ((tP + fN) == 0.0F) ? 0.0F : tP / (tP + fN); // what percentage of class i were predicted correctly
  }
  return rec;
}

Vector<float> calculatePrecision(const ConfusionMatrix& confMat){
  Vector<float> prec(confMat.classCount);
  for (size_t i = 0; i < confMat.classCount; ++i) {
    float tP = static_cast<float>(confMat.truePositives[i]);
    float fP = static_cast<float>(confMat.falsePositives[i]);

    prec[i] = ((tP + fP) == 0.0F) ? 0.0F : tP / (tP + fP); // what percentage of class i predictions were correct 
  }
  return prec;
}

Vector<float> calculateF1Score(const ConfusionMatrix& confMat) {
  Vector<float> f1(confMat.classCount);
  for (size_t i = 0; i < confMat.classCount; ++i) {
    float tP = static_cast<float>(confMat.truePositives[i]);
    float fP = static_cast<float>(confMat.falsePositives[i]);
    float fN = static_cast<float>(confMat.falseNegatives[i]);

    f1[i] = ((2.0F*tP + fP + fN) == 0.0F) ? 0.0F : (2.0F*tP) / (2.0F*tP + fP + fN);
  }
  return f1;
}

float calculateAccuracy(const ConfusionMatrix& confMat) {
  size_t totalPositives = 0;

  for (auto& el : confMat.truePositives) {
    totalPositives += el;
  }

  float fTotal = static_cast<float>(totalPositives);
  float fSampleCount = static_cast<float>(confMat.sampleCount);
  return (confMat.sampleCount == 0) ? 0.0F : fTotal / fSampleCount;
}

float macroAverage(const Vector<float>& metricPerClass) {
  size_t classCount = metricPerClass.getSize();
  float total = 0;

  for (auto& val : metricPerClass) {
    total += val;
  }

  return (classCount == 0) ? 0.0F : total / static_cast<float>(classCount);
}

}
