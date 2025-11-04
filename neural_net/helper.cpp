#include "helper.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <chrono>

namespace nn {

using lin::Vector;

void shuffle(lin::Vector<lin::Vector<float>>& trainingData, lin::Vector<lin::Vector<float>>& labels) {

  // create index list
  Vector<size_t> indexList(trainingData.getSize());
  for (size_t i = 0; i < indexList.getSize(); ++i) {
    indexList[i] = i;
  }

  // shuffle index list
  std::random_device rand;
  std::mt19937 generator(rand());
  std::shuffle(indexList.begin(), indexList.end(), generator);

  // project each element in tData and labels onto their new index
  // this algorithm swaps the element at i (if not done) to its correct index according to the shuffled indexList
  // then it keeps swapping the previously swapped element at i with its corresponding index in indexList until it detects a cycle
  // meaning the element at i is at its correct index according to indexList
  Vector<bool> done(trainingData.getSize(), false);
  for (size_t i = 0; i < trainingData.getSize(); ++i) {
    size_t j=i; 
    while(!done[i]) { 
      std::swap(trainingData[i], trainingData[indexList[j]]);
      std::swap(labels[i], labels[indexList[j]]);
      done[indexList[j]] = true;
      j = indexList[j];
    }
  }
}

int getIndexOfMax(const Vector<float>& vec) {
  size_t maxSoFarIndex = 0;
  for (size_t i = 0; i < vec.getSize(); ++i) {
    if (vec[i] > vec[maxSoFarIndex]) {
      maxSoFarIndex = i;
    }
  }
  return maxSoFarIndex;
}

Vector<float> apply_activation_function(Vector<float>& vec, const std::function<float(float)>& func) {
  for(size_t i = 0; i<vec.getSize(); ++i) {
    vec[i] = func(vec[i]);
  }
  return vec;
}

float sigmoid(float x) {
  return 1.0/(1.0+std::exp(-x));
}

float sigmoid_derivative(float x) {
  return sigmoid(x) * (1-sigmoid(x)); 
}

float relu(float x) {
  return std::max(0.0f, x);
}

float relu_derivative(float x) {
  return x > 0 ? 1.0f : 0.0f;
}


lin::Vector<float> softmax(const lin::Vector<float>& inputVector) {
  // softmax function, determine largest first and subtract from each to make exp() numerically stable
  Vector<float> result(inputVector.getSize());
  float total = 0.0f;
  float max = inputVector[0];

  for (size_t i = 1; i < inputVector.getSize(); ++i) {
    max = std::max(max, inputVector[i]);
  }

  for (size_t i = 0; i < inputVector.getSize(); ++i) {
    result[i] = std::exp(inputVector[i] - max);
    total += result[i];
  }

  for (size_t i = 0; i < inputVector.getSize(); ++i) {
    result[i] /= total;
  }

  return result;
}

float uniform_distribution_in(float lower, float upper) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(lower, upper);
    return static_cast<float>(dist(gen));
}

float normal_distribution_in(float mean, float stddev) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stddev);
    return static_cast<float>(dist(gen));
}

float calculate_mse(const lin::Vector<float>& actual, const lin::Vector<float>& target) {
  float error = 0.0f;
  for (int i = 0; i < actual.getSize(); ++i) {
    error += (target[i] - actual[i]) * (target[i] - actual[i]) ;
  }
  return error /= static_cast<float>(actual.getSize());
}

Vector<Vector<uint8_t>> load_mnist_images(const std::string& filename){


    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Datei nicht gefunden: " + filename);
    int32_t magic;
    int32_t num_images;
    int32_t rows;
    int32_t cols;
    file.read((char*)&magic, 4);
    file.read((char*)&num_images, 4);
    file.read((char*)&rows, 4);
    file.read((char*)&cols, 4);
    magic = __builtin_bswap32(magic);
    num_images = __builtin_bswap32(num_images);
    rows = __builtin_bswap32(rows);
    cols = __builtin_bswap32(cols);
    lin::Vector<lin::Vector<uint8_t>> images(num_images, Vector<uint8_t>(rows * cols));
    for (int i = 0; i < num_images; ++i)
        file.read((char*)images[i].data(), rows * cols);
    return images;
}


Vector<uint8_t> load_mnist_labels(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Datei nicht gefunden: " + filename);
    }

    // Header: Magic Number + Anzahl der Labels (jeweils 4 Bytes, Big Endian)
    uint32_t magic = 0;
    uint32_t num_items = 0;

    file.read(reinterpret_cast<char*>(&magic), 4);
    file.read(reinterpret_cast<char*>(&num_items), 4);

    // Big Endian → Little Endian umwandeln (für Intel/AMD)
    magic     = __builtin_bswap32(magic);
    num_items = __builtin_bswap32(num_items);

    if (magic != 2049) {
        throw std::runtime_error("Falsche Magic Number in Label-Datei: " + std::to_string(magic));
    }

    // Labels einlesen
    Vector<uint8_t> labels(num_items);
    file.read(reinterpret_cast<char*>(labels.data()), num_items);

    if (!file) {
        throw std::runtime_error("Fehler beim Lesen der Labels aus: " + filename);
    }
    return labels;
}

// normalize 8 bit integer into [-1,1]
Vector<Vector<float>> normalize_images(const Vector<Vector<uint8_t>>& images) {
  Vector<Vector<float>> results(images.getSize());
  for(int i=0; i<images.getSize(); ++i) {
    Vector<float> normalized(images[i].getSize());
    for(int j=0; j<images[i].getSize(); ++j) {
      normalized[j] = ((static_cast<float>(images[i][j])/255.0F - 0.5F)) * 2.0F;
    }
    results[i] = (std::move(normalized));
  }
  return results;
}




Vector<float> normalizeImage(const Vector<uint8_t>& image) {
  Vector<float> result(image.getSize());
  for (int i = 0; i < image.getSize(); ++i) {
    result[i] = static_cast<float>(((image[i] / 255.0F) - 0.5F) * 2.0);
  }
  return result;
}

lin::Vector<lin::Vector<float>> one_hot_encode(const Vector<uint8_t>& labels) {
  lin::Vector<lin::Vector<float>> result(labels.getSize());
  for(int i=0; i<labels.getSize(); ++i) {
    lin::Vector<float> one_hot(10, 0.0f);
    one_hot[labels[i]] = 1.0;
    result[i] = std::move(one_hot);
  }
  return result;
}



void visualize_mnist_images(const Vector<Vector<uint8_t>>& vec) {
  for (size_t t=0; t < vec.getSize(); ++t) {
    std::cout << "Image " << t << ":\n";
    for (size_t k=0; k < vec[t].getSize(); ++k) {
      std::cout << (vec[t][k] > 140 ? '#' : ' ');
      if ((k+1) % 28 == 0) {
        std::cout << '\n';
      }
    }
    std::cout << '\n';
  }
}


void print_labels(const Vector<uint8_t>& labels) { 
  for (int i = 0; i < labels.getSize(); ++i)  {
    std::cout << static_cast<int>(labels[i]) << '\n';
  }
}

std::stringstream genTimeStampForFilename(const std::string& filename) {
  auto t = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(t);
  std::stringstream ss;
  ss << filename << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
  return ss;
}



}
