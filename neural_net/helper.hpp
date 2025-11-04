#pragma once
#include "../include/vector.hpp"
#include <cstdint>
#include <functional>

namespace nn {

using lin::Vector;

Vector<float> apply_activation_function(Vector<float>& vec, const std::function<float(float)>& func);

void shuffle(Vector<Vector<float>>& trainingData, Vector<Vector<float>>& labels);

int getIndexOfMax(const Vector<float>& vec);

float sigmoid(float x);

float sigmoid_derivative(float x);

float relu(float x);

float relu_derivative(float x);

Vector<float> softmax(const Vector<float>& inputVector);

float uniform_distribution_in(float lower, float upper);

float normal_distribution_in(float mean, float stddev);

float calculate_mse(const Vector<float>& actual, const Vector<float>& target);

Vector<Vector<uint8_t>> load_mnist_images(const std::string& filename);

Vector<uint8_t> load_mnist_labels(const std::string& filename);

Vector<Vector<float>> normalize_images(const Vector<Vector<uint8_t>>& images);

Vector<float> normalizeImage(const Vector<uint8_t>& image);

Vector<Vector<float>> one_hot_encode(const Vector<uint8_t>& labels);

void visualize_mnist_images(const Vector<Vector<uint8_t>>& vec);

void print_labels(const Vector<uint8_t>& vec);

std::stringstream genTimeStampForFilename(const std::string& filename); 


}
