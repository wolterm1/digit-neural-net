#include "net.hpp"
#include "../include/linalg.hpp"
#include "helper.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <filesystem>

using lin::Vector;
using lin::Matrix;
using lin::hadamardProduct;
using eval::ConfusionMatrix;

namespace nn {

// allocates Weights, Neurons and Weights for given nn dimensions, biases start with 0
NeuralNet::NeuralNet(size_t inputNodeCount, size_t outputNodeCount, size_t hiddenLayerCount, size_t hiddenNodeCount) : inputNodeCount(inputNodeCount), outputNodeCount(outputNodeCount), hiddenLayerCount(hiddenLayerCount), hiddenNodeCount(hiddenNodeCount) {
  init_net();
}

NeuralNet::NeuralNet(const Vector<Matrix<float>>& inWeights, const Vector<Vector<float>>& inBiases) : inputNodeCount(inWeights[0].getColumns()), outputNodeCount(inWeights[inWeights.getSize()-1].getColumns()), hiddenLayerCount(inWeights.getSize()-1), hiddenNodeCount(inWeights[0].getRows()) {
  init_neurons();
  init_zvalues();
  init_gradients();
  weights = inWeights;
  biases = inBiases;
}

void NeuralNet::init_net() {
  init_neurons();
  init_zvalues();
  init_weights();
  init_biases();
  init_gradients();
}


void NeuralNet::init_neurons() {
  neurons = Vector<Vector<float>>(hiddenLayerCount+2);
  neurons[0] = Vector<float>(inputNodeCount, 0.0f);
  for (size_t currentLayer = 1; currentLayer <= hiddenLayerCount; ++currentLayer) {
    neurons[currentLayer] = Vector<float>(hiddenNodeCount, 0.0f);
  }
  neurons[hiddenLayerCount+1] = Vector<float>(outputNodeCount, 0.0f);
}

void NeuralNet::init_zvalues() {
  zvalues = Vector<Vector<float>>(hiddenLayerCount+2);
  zvalues[0] = Vector<float>(inputNodeCount, 0.0f);
  for (size_t currentLayer = 1; currentLayer <= hiddenLayerCount; ++currentLayer) {
    zvalues[currentLayer] = Vector<float>(hiddenNodeCount, 0.0f);
  }
  zvalues[hiddenLayerCount+1] = Vector<float>(outputNodeCount, 0.0f);
}

// He init
void NeuralNet::init_weights() {
  weights = Vector<Matrix<float>>(hiddenLayerCount+1);
  for (size_t i=0; i <= hiddenLayerCount; ++i) {
    size_t fan_in = neurons[i].getSize(); 
    size_t fan_out = neurons[i+1].getSize(); 
    float lower = 0.0f;
    float upper = std::sqrt(2.0f/static_cast<float>(fan_in));
    weights[i] = Matrix<float>(neurons[i+1].getSize(), neurons[i].getSize(), std::bind(normal_distribution_in, lower, upper));
  }
}

void NeuralNet::init_biases() {
  biases = Vector<Vector<float>>(hiddenLayerCount+1);
  for (size_t currentLayer = 0; currentLayer <= hiddenLayerCount; ++currentLayer) {
    biases[currentLayer] = Vector<float>(neurons[currentLayer+1].getSize(), 0.01);
  }
}

void NeuralNet::init_gradients() {
  weightGradientSum = Vector<Matrix<float>>(hiddenLayerCount+1);
  biasGradientSum = Vector<Vector<float>>(hiddenLayerCount+1);
  for (size_t i = 0; i <= hiddenLayerCount; ++i) {
    weightGradientSum[i] = Matrix<float>(neurons[i+1].getSize(), neurons[i].getSize(), 0.0f);
    biasGradientSum[i] = Vector<float>(neurons[i+1].getSize(), 0.0f);
  }
}
// initializes input Neurons with given inputVector and then computes neuron values with biases and activation Function
void NeuralNet::forward_pass(const Vector<float>& inputData) {
  neurons[0] = inputData;
  for (size_t i=1; i<neurons.getSize(); ++i) {
    assert(weights[i-1].getColumns() == neurons[i-1].getSize());
    zvalues[i] = (weights[i-1] * neurons[i-1]) + biases[i-1];
    if (i == neurons.getSize() - 1) {
      neurons[i] = softmax(zvalues[i]); //apply softmax at last layer
    } else {
      neurons[i] = apply_activation_function(zvalues[i], relu); 
    }
  }
}

void NeuralNet::backpropagation(const Vector<float>& targetLabel, size_t batchSize) {
  // Backpropagation algorithm, Computes Error Vector for each Neuron Layer, starting at the Last Layer and uses dynamic Programming to compute Error Vectors in [0 , L-2]
  size_t L = neurons.getSize();
  Vector<Vector<float>> deltas(L);

  // compute delta Error Vector at last layer
  deltas[L-1] = neurons[L-1] - targetLabel;

  for (size_t i = L-2;  i>0; --i) {

    assert(weights[i].getTranspose().getColumns() == deltas[i+1].getSize());
    deltas[i] = hadamardProduct(
        weights[i].getTranspose() * deltas[i+1],
        apply_activation_function(zvalues[i], relu_derivative)
        );
  }

  // now deltas[i] contains error Vector for every Layer in {1, ..., L-1}
  for (size_t i = 1; i < L; ++i) {
    Matrix<float> weightGradientForInstance = lin::outer_product(deltas[i], neurons[i-1]);
    const Vector<float>& biasGradientForInstance = deltas[i];

    weightGradientSum[i-1] += weightGradientForInstance / batchSize; 
    biasGradientSum[i-1] += biasGradientForInstance / batchSize;
  }
}



// takes in normalized images and one-hot encoded labels in [0,9]
// Iterates epochs often over training sample, in each epoch, calculate forward pass for batchSize trainingData and get get average gradient which is then applied in update_weights()
void NeuralNet::train(Vector<Vector<float>>& trainingData, Vector<Vector<float>>& labels, size_t epochs, size_t batchSize, Optimizer optimizer) {
  std::cout << "Started Training on " << trainingData.getSize() << " trainingData, \n" << "Model Parameters\n" << 
    "Epochs: " << epochs << '\n' <<
    "Batch Size: " << batchSize << '\n';
  for (size_t currentEpoch = 1; currentEpoch <= epochs; ++currentEpoch) {
    shuffle(trainingData, labels);
    for (size_t i = 0; i < trainingData.getSize(); ++i) {
      this->forward_pass(trainingData[i]);
      this->backpropagation(labels[i], batchSize);
      if ((i+1) % batchSize == 0) {
        optimizer.step(weights, biases, weightGradientSum, biasGradientSum);
      }
    }
  }
}

Vector<float> NeuralNet::classify(const Vector<float>& inputData) {
  this->forward_pass(inputData);
  return neurons[hiddenLayerCount+1];
}



ConfusionMatrix NeuralNet::evaluate(const Vector<Vector<float>>& testData, const Vector<Vector<float>>& labels){
  size_t sampleCount = testData.getSize();
  Vector<int> predictions(sampleCount);
  Vector<int> truth(sampleCount);

  for (size_t i = 0; i < sampleCount; ++i) {
    predictions[i] = getIndexOfMax(this->classify(testData[i]));
    truth[i] = getIndexOfMax(labels[i]);
  }

  ConfusionMatrix confMat(10);
  confMat.calculate(predictions, truth);
  return confMat;
}


void NeuralNet::save_to_file(const std::string& filename){
  auto renamedFilename = filename;
  if (std::filesystem::exists(filename)) {
    renamedFilename = genTimeStampForFilename(filename).str();
    std::cout << "File " << filename << " already exists, saving as: " << renamedFilename << '\n';
  }

  
  std::fstream file(renamedFilename, std::ios::trunc | std::ios::out | std::ios::binary);
  std::string magic = "KNNET";
  file.write(magic.data(), 5 * sizeof(char));

  uint64_t weightMatricesCount = weights.getSize(); 
  file.write(reinterpret_cast<char*>(&weightMatricesCount), sizeof(weightMatricesCount));

  for (size_t i = 0; i < weightMatricesCount; ++i) {

    uint64_t rows = weights[i].getRows();
    file.write(reinterpret_cast<char*>(&rows), sizeof(rows));
    uint64_t cols = weights[i].getColumns();
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.write(reinterpret_cast<char*>(weights[i].data()), rows * cols * sizeof(float));

    uint64_t biasSize = biases[i].getSize();
    file.write(reinterpret_cast<char*>(&biasSize), sizeof(biasSize));
    file.write(reinterpret_cast<char*>(biases[i].data()), biasSize * sizeof(float));
  }
}


NeuralNet NeuralNet::load_from_file(const std::string& filename) {
  std::fstream file(filename, std::ios::in | std::ios::binary);
  std::string magic(5, ' ');
  uint64_t weightMatricesCount;

  file.read(magic.data(), 5 * sizeof(char));
  if (magic != "KNNET") {
    throw std::invalid_argument("Given File does not encode a Neural Net");
  }

  file.read(reinterpret_cast<char*>(&weightMatricesCount), sizeof(weightMatricesCount));
  Vector<Matrix<float>> inWeights(weightMatricesCount);
  Vector<Vector<float>> inBiases(weightMatricesCount);


  for (size_t i = 0; i < weightMatricesCount; ++i) {
    uint64_t rows;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    uint64_t cols;
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    Matrix<float> mat(rows, cols);
    file.read(reinterpret_cast<char*>(mat.data()), rows * cols * sizeof(float));

    uint64_t biasSize;
    file.read(reinterpret_cast<char*>(&biasSize), sizeof(biasSize));
    Vector<float> vec(rows);
    file.read(reinterpret_cast<char*>(vec.data()), biasSize * sizeof(float));

    inWeights[i] = mat;
    inBiases[i] = vec;
  }

  return NeuralNet(inWeights, inBiases);
}



std::ostream& operator<<(std::ostream& outputstream, const NeuralNet& net){
  outputstream << "Weight Matrices: \n";
  for (size_t i = 0; i < net.weights.getSize(); ++i) {
    outputstream << std::setprecision(2) << net.weights[i] << '\n';
  }

  outputstream << "\nBiases: \n";
  for (size_t i = 0; i < net.biases.getSize(); ++i) {
    outputstream << i << " : " << std::setprecision(2) <<  net.biases[i] << '\n';
  }

  outputstream << "\nNeuron Vectors: \n";
  for (size_t i = 0; i < net.neurons.getSize(); ++i) {
    outputstream << i << " : " << std::setprecision(2) <<  net.neurons[i] << '\n';
  }

  return outputstream;
}

}
