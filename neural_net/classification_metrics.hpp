#include "confusion_matrix.hpp"

namespace eval {
using lin::Vector;

Vector<float> calculateRecall(const ConfusionMatrix& confMat);

Vector<float> calculatePrecision(const ConfusionMatrix& confMat);

Vector<float> calculateF1Score(const ConfusionMatrix& confMat);

float calculateAccuracy(const ConfusionMatrix& confMat);

float macroAverage(const Vector<float>& metricPerClass);





}


