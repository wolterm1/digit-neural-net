#pragma once
#include "matrix.hpp"
#include "vector.hpp"

namespace lin {

template <Numeric T, TensorElement X>
void checkEqualMatrixColumnsToVectorSize(const Matrix<T> &mat, const Vector<X> &vec) {
  if (mat.getColumns() != vec.getSize()) {
    std::string msg =
        "Wrong dimensions for Matrix*Vector:\n MatrixColumns:" + std::to_string(mat.getColumns()) +
        "\nVector size: " + std::to_string(vec.getSize());
    throw std::invalid_argument(msg);
  }
}

template <Numeric T, TensorElement X>
void checkEqualVectorSizeToMatrixRows(const Vector<X>& vec, const Matrix<T>& mat) {
  if (mat.getRows() != vec.getSize()) {
    std::string msg =
        "Wrong dimensions for Matrix*Vector:\n MatrixRows:" + std::to_string(mat.getColumns()) +
        "\nVector size: " + std::to_string(vec.getSize());
    throw std::invalid_argument(msg);
  }
}


template <Numeric T>
void checkEqualVectorSize(const Vector<T> &first, const Vector<T> &second, const std::string& caller) {
  if (first.getSize() != 3 || second.getSize() != 3) {
    std::string msg = "Wrong dimensions for : " + caller + "  " + std::to_string(first.getSize()) +
                      "x" + std::to_string(second.getSize());
    throw std::invalid_argument(msg);
  }
}

template <TensorElement X>
Vector<X> cross(const Vector<X> &a, const Vector<X> &b) {
  return Vector<X>({(a[1] * b[2]) - (a[2] * b[1]),
                    (a[2] * b[0]) - (a[0] * b[2]),
                    (a[0] * b[1]) - (a[1] * b[0])});
}

template <Numeric T>  
Matrix<T> outer_product(const Vector<T>& first, const Vector<T>& second) {
  size_t resultRows = first.getSize(); 
  size_t resultColumns = second.getSize();
  Matrix<T> result(resultRows, resultColumns);
  for (size_t r = 0; r < resultRows; ++r) { 
    for (size_t c = 0; c < resultColumns; ++c) { 
      result(r,c) = first[r] * second[c];
    }
  }
  return result;
}


template <TensorElement T>
Vector<T> hadamardProduct(const Vector<T>& first, const Vector<T>& second)  {
  Vector<T> copy(first);
  return copy.hadamardProductInplace(second);
}

template <Numeric T>
T dot(const Vector<T> &firstVec, const Vector<T> &secondVec) {
  checkEqualDimensions(firstVec, secondVec);
  T result = 0;
  for (int i = 0; i < firstVec.getSize(); ++i) {
    result += firstVec[i] * secondVec[i];
  }
  return result;
}

//Matrix * Vector
template <Numeric T>
Vector<T> operator*(const Matrix<T> &mat, const Vector<T> &vec) {
  // Matrix.rows has to be equal to vector length, resulting vector has matrix
  // row dimensions
  checkEqualMatrixColumnsToVectorSize(mat, vec);
  Vector<T> result(mat.getRows());
  for (int i = 0; i < mat.getRows(); ++i) {
    T tmp = 0;
    for (int j = 0; j < mat.getColumns(); ++j) {
      tmp += mat(i, j) * vec[j];
    }
    result[i] = tmp;
  }
  return result;
}

//Vector * Matrix
template <Numeric T>
Vector<T> operator*(const Vector<T> &vec, const Matrix<T> &mat) {
  checkEqualVectorSizeToMatrixRows(vec, mat);
  Vector<T> result(mat.getColumns());
  for (size_t i = 0; i<mat.getColumns(); ++i) {
    T tmp = 0;
    for (size_t j = 0; j < mat.getRows(); ++j) {
      tmp += vec[j] * mat(j,i);
    }
    result[i] = tmp;
  }
  return result;
}


template <Numeric T>
Matrix<T> operator*(const T& scalar, const Matrix<T> &mat) {
  return mat*scalar;
}

template <TensorElement T>
Vector<T> operator*(const T& scalar, const Vector<T> &vec) {
  return vec*scalar; 
}

}  // namespace lin
