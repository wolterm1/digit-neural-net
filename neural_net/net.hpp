#pragma once

#include <matrix.hpp>
#include <vector.hpp>
#include <iostream>
#include "confusion_matrix.hpp"
#include "optimizer.hpp"

namespace nn {


class NeuralNet {
  public: 
  NeuralNet(size_t inputNodeCount, size_t outputNodeCount, size_t hiddenLayerCount, size_t hiddenNodeCount);
  NeuralNet(const lin::Vector<lin::Matrix<float>>& inWeights, const lin::Vector<lin::Vector<float>>& inBiases); 
  void train(lin::Vector<lin::Vector<float>>& trainingData, lin::Vector<lin::Vector<float>>& labels, size_t epochs, size_t batchSize, Optimizer optimizer);
  void forward_pass(const lin::Vector<float>& inputData);
  void backpropagation(const lin::Vector<float>& targetLabel, size_t batchSize);
  void update_weights(float LearnRate, size_t batchSize);
  lin::Vector<float> classify(const lin::Vector<float>& inputData);
  eval::ConfusionMatrix evaluate(const lin::Vector<lin::Vector<float>>& testData, const lin::Vector<lin::Vector<float>>& labels);

  void saveToFile(const std::string& filename);
  static NeuralNet loadFromFile(const std::string& filename); // factory method to construct a Neural Net from File

  friend std::ostream& operator<<(std::ostream& outputstream, const NeuralNet& net);

  size_t inputNodeCount;
  size_t outputNodeCount;
  size_t batchSize;
  size_t hiddenLayerCount;
  size_t hiddenNodeCount;

  lin::Vector<lin::Vector<float>> zvalues; // pre activation function neuron values
  lin::Vector<lin::Vector<float>> neurons; 
  lin::Vector<lin::Matrix<float>> weights;
  lin::Vector<lin::Vector<float>> biases;
  lin::Vector<lin::Matrix<float>> weightGradientSum;
  lin::Vector<lin::Vector<float>> biasGradientSum;

  private:


  void init_net();
  void init_neurons();
  void init_zvalues();
  void init_weights();
  void init_biases();
  void init_gradients();

};

}
