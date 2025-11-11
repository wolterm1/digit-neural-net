#include "../neural_net/net.hpp"
#include "../neural_net/helper.hpp"
#include <iostream>
#include "../neural_net/optimizer.hpp"

using lin::Vector;
using lin::Matrix;
using nn::NeuralNet;
using nn::Optimizer;

lin::Vector<lin::Vector<float>> sliceData(const lin::Vector<lin::Vector<float>>& tData) {
  lin::Vector<lin::Vector<float>> result(100);
  for (size_t i = 0; i < result.getSize(); ++i) {
    result[i] = tData[i];
  }
  return result;
}


lin::Vector<lin::Vector<float>> sliceLabels(const lin::Vector<lin::Vector<float>>& onehotLabels) {
  lin::Vector<lin::Vector<float>> result(100);
  for (size_t i = 0; i < result.getSize(); ++i) {
    result[i] = onehotLabels[i];
  }
  return result;
}



int main() {
  auto trainingData = nn::load_mnist_images("data/train-images-idx3-ubyte");
  auto trainingNormalizedImages = nn::normalize_images(trainingData);

  lin::Vector<uint8_t> trainingLabels = nn::load_mnist_labels("data/train-labels-idx1-ubyte");
  auto trainingOneHotLabels = nn::one_hot_encode(trainingLabels);

  Optimizer adamOptimizer;
  nn::NeuralNet net(784, 10, 2, 128);

  net.train(trainingNormalizedImages, trainingOneHotLabels, 35, 128, adamOptimizer);

  net.saveToFile("test");

  auto testData = nn::normalize_images(nn::load_mnist_images("data/t10k-images-idx3-ubyte"));
  auto testLabels = nn::one_hot_encode(nn::load_mnist_labels("data/t10k-labels-idx1-ubyte"));

  std::cout << net.evaluate(testData, testLabels);

  //for(int i = 0; i < 10; ++i) {
  //  Vector<float> result = net.classify(normalizedTestImages[i]);
  //  std::cout << "Image " << i << " classified as: " << result << "\n";
  //}


}



