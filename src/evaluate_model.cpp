#include <iomanip>
#include <string>
#include "../neural_net/net.hpp"
#include "../neural_net/helper.hpp"
#include "../neural_net/classification_metrics.hpp" 

using namespace eval;

int main() {

  std::cout << " Enter Model To Evaluate: \n";
  std::string str;
  std::cin >> str;
  auto net = nn::NeuralNet::loadFromFile(str);

  auto testData = nn::normalize_images(nn::load_mnist_images("data/t10k-images-idx3-ubyte"));
  auto testLabels = nn::one_hot_encode(nn::load_mnist_labels("data/t10k-labels-idx1-ubyte"));


  auto confMat = net.evaluate(testData, testLabels);
  std::cout << std::fixed << std::setprecision(4) << "Evaluation of Net " << str << ": \n" <<
    "Accuracy: " << calculateAccuracy(confMat) << '\n' <<
    "Precision: " <<  calculatePrecision(confMat) << '\n' <<
    "Recall: " << calculateRecall(confMat) << '\n' <<
    "F1-Score: " << calculateF1Score(confMat) << '\n';
}

