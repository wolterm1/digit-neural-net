#pragma once
#include <functional>
#include <iostream>
#include <cassert>

#include "concepts.hpp"

namespace lin {

template <Numeric T>
class Matrix;

template <Numeric T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &);

template <Numeric T = int>
class Matrix {
 private:
  T *matrixData;
  size_t rows;
  size_t columns;
  void allocateForMatrixData();
  void checkForEqualDimensions(const Matrix<T> &other) const;
  void checkEqualColumnToRow(const Matrix<T> &other) const;

 public:
  /*********************** Constructors **********************/

  template <typename Iter>
  Matrix(const size_t &rows, const size_t &pcolumns, Iter containerBeginIt, Iter containerEndIt);
  Matrix(const std::initializer_list<std::initializer_list<T>> &initList);
  Matrix(const size_t &rows, const size_t &columns, const T &value);
  Matrix(const size_t &rows, const size_t &columns);
  Matrix();

  // rule of five
  Matrix(const Matrix<T> &other);
  Matrix(Matrix<T> &&other) noexcept;
  Matrix<T> &operator=(const Matrix<T> &other);
  Matrix<T> &operator=(Matrix<T> &&other) noexcept;
  ~Matrix();

  static auto createIdentity(const size_t &rows, const size_t &columns) -> Matrix<T>;
  Matrix(const size_t& rows, const size_t& columns, const std::function<T()>&);
  Matrix<T> getTranspose() const;

  /*********************** Operators **********************/

  T &operator()(const size_t &p_row, const size_t &column);
  const T &operator()(const size_t &p_row, const size_t &column) const;

  bool operator==(const Matrix<T> &other) const;

  Matrix<T> operator+(const Matrix<T> &other) const;
  Matrix<T> &operator+=(const Matrix<T> &other);

  Matrix<T> operator-(const Matrix<T> &other) const;
  Matrix<T> &operator-=(const Matrix<T> &other);

  Matrix<T> operator*(const T &scalar) const;
  Matrix<T> &operator*=(const T &scalar);

  Matrix<T> operator/(const T &scalar) const;
  Matrix<T> &operator/=(const T &scalar);

  Matrix<T> operator+(const T &scalar) const;
  Matrix<T> &operator+=(const T &scalar);

  Matrix<T> operator*(const Matrix<T> &other) const;
  Matrix<T> &operator*=(const Matrix<T> &other);


  Matrix<T> &hadamardProductInplace(const Matrix<T> &other);
  Matrix<T> &hadamardDivisionInplace(const Matrix <T> &other);
  Matrix<T> &applyElementWiseFunction(std::function<T(T)> func);

  size_t getRows() const;
  size_t getColumns() const;
  T* data() const;

  /*********************** helpers **********************/

  friend std::ostream &operator<< <T>(std::ostream &outputstream, const Matrix<T> &mat);

  void swap(Matrix<T> &other) noexcept;

  /*********************** Iterator **********************/

  struct Iterator {
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    explicit Iterator(pointer p_ptr = nullptr) : ptr(p_ptr) {}

    pointer operator->() const { return ptr; }

    reference operator*() const { return *ptr; }

    Iterator &operator++() {
      ++ptr;
      return *this;
    }

    Iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    Iterator &operator--() {
      --ptr;
      return *this;
    }

    Iterator operator--(int) {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    Iterator &operator+=(difference_type n) {
      ptr += n;
      return *this;
    }
    Iterator operator+(difference_type n) const { return Iterator(ptr + n); }
    friend Iterator operator+(difference_type n, const Iterator j) { return n + j; }
    Iterator &operator-=(const difference_type n) {
      ptr -= n;
      return *this;
    }
    difference_type operator-(const Iterator j) const { return ptr - j.ptr; }
    Iterator operator-(const difference_type n) const { return Iterator(ptr - n); }
    reference operator[](difference_type n) const { return *(ptr + n); }
    auto operator<=>(const Iterator &) const = default;

   private:
    pointer ptr;
  };
  Iterator begin() { return Iterator(matrixData); }
  Iterator end() { return Iterator(matrixData + (columns * rows)); }
  Iterator begin() const { return Iterator(matrixData); }
  Iterator end() const { return Iterator(matrixData + (columns * rows)); }
};  // class declaration

}  // namespace lin

/**************************** Constructors
 * ****************************************/

namespace lin {

template <Numeric T>
Matrix<T>::Matrix(const size_t &prows, const size_t &pcolumns, const T &value)
    : rows(prows), columns(pcolumns), matrixData(nullptr) {
  allocateForMatrixData();
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      (*this)(i, j) = value;
    }
  }
}

template <Numeric T>
Matrix<T>::Matrix() : rows(0), columns(0), matrixData(nullptr) {
}

template <Numeric T>
Matrix<T>::Matrix(const size_t &prows, const size_t &pcolumns) : rows(prows), columns(pcolumns), matrixData(nullptr) {
  allocateForMatrixData();
}

template <Numeric T>
template <typename Iter>
Matrix<T>::Matrix(const size_t &prows,
                  const size_t &pcolumns,
                  Iter containerBeginIt,
                  Iter containerEndIt)
    : rows(prows), columns(pcolumns), matrixData(nullptr) {
  allocateForMatrixData();
  auto startOfFillIt = std::copy(containerBeginIt, containerEndIt, matrixData);
  std::fill(startOfFillIt, (matrixData + (rows * columns)), 0);
}

template <Numeric T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& initList)
    : rows(initList.size()), columns(initList.begin()->size()), matrixData(nullptr) {
  allocateForMatrixData();
  size_t i = 0;
  for (auto rowIt = initList.begin(); rowIt != initList.end(); ++rowIt, ++i){
    size_t j = 0;
    for (auto columnIt = rowIt->begin(); columnIt != rowIt->end(); ++columnIt, ++j) {
      (*this)(i,j) =  *columnIt;
    }
  }

}

template <Numeric T>
Matrix<T> Matrix<T>::createIdentity(const size_t &rows, const size_t &columns) {
  Matrix<T> res(rows, columns);
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.columns; j++) {
      if (j == i) {
        res(i, j) = 1;
      } else {
        res(i, j) = 0;
      }
    }
  }
  return res;
}

template <Numeric T>
Matrix<T>::Matrix(const size_t& rows, const size_t& columns, const std::function<T()>& func) : rows(rows), columns(columns), matrixData(nullptr) {
  allocateForMatrixData();
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
     (*this)(i,j) = func(); 
    }
  }
}

template <Numeric T>
Matrix<T> Matrix<T>::getTranspose() const {
  Matrix<T> result(columns, rows);  // use columns for rows and rows for columns
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      result(j, i) = (*this)(i,j);  // use columns of original to access rows of result and vice versa
    }
  }
  return result;
}

template <Numeric T>
Matrix<T>::Matrix(const Matrix<T> &other) : rows(other.rows), columns(other.columns), matrixData(nullptr) {
  allocateForMatrixData();
  for (int i = 0; i < other.rows; i++) {
    for (int j = 0; j < other.columns; j++) {
      (*this)(i, j) = other(i, j);
    }
  }
}

template <Numeric T>
Matrix<T>::Matrix(Matrix<T> &&other) noexcept
    : matrixData(other.matrixData), rows(other.rows), columns(other.columns) {
  other.matrixData = nullptr;
  other.rows = 0;
  other.columns = 0;
}

template <Numeric T>
Matrix<T>::~Matrix() {
  delete[] matrixData;
}

template <Numeric T>
void Matrix<T>::allocateForMatrixData() {
  if (rows <= 0 || columns <= 0) {
    matrixData = nullptr;
  } else {
  matrixData = new T[rows * columns];
  }
}

}  // namespace lin

/**********************************Operators************************************************/

namespace lin {

template <Numeric T>
const T &Matrix<T>::operator()(const size_t &p_row, const size_t &p_column) const {
  return *(matrixData + p_column + (p_row * columns));
}

template <Numeric T>
T &Matrix<T>::operator()(const size_t &p_row, const size_t &p_column) {
  return *(matrixData + p_column + (p_row * columns));
}

template <Numeric T>
size_t Matrix<T>::getRows() const {
  return rows;
}

template <Numeric T>
size_t Matrix<T>::getColumns() const {
  return columns;
}

template <Numeric T> 
T* Matrix<T>::data() const {
  return matrixData;
}
template <Numeric T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
  if (rows != other.rows || columns != other.columns) {
    return false;
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if ((*this)(i, j) != other(i, j)) {
        return false;
      }
    }
  }
  return true;
}

template <Numeric T>
void Matrix<T>::swap(Matrix<T> &other) noexcept {
  std::swap(this->rows, other.rows);
  std::swap(this->columns, other.columns);
  std::swap(this->matrixData, other.matrixData);
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
  // copy and swap idiom
  if (this != &other) {
    Matrix<T> copy(other);
    swap(copy);
  }
  return *this;
}

template <Numeric T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
  Matrix<T> copy(*this);
  copy += other;
  return copy;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &other) {
  checkForEqualDimensions(other);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) += other(i, j);
    }
  }
  return *this;
}

template <Numeric T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const {
  Matrix<T> copy(*this);
  copy -= other;
  return copy;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &other) {
  checkForEqualDimensions(other);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) -= other(i, j);
    }
  }
  return *this;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) noexcept {
  if (this == &other) {
    return *this;
  }
  swap(other);
  return *this;
}

template <Numeric T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const {
  Matrix<T> copy(*this);
  copy *= other;
  return copy;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &other) {
  checkEqualColumnToRow(other);
  Matrix<T> tempResults(rows, other.columns, 0);
  for (int i = 0; i < tempResults.rows; ++i) {
    for (int j = 0; j < tempResults.columns; ++j) {
      for (int k = 0; k < columns; ++k) {
        tempResults(i, j) += (*this)(i, k) * other(k, j);
      }
    }
  }
  *this = std::move(tempResults);
  return *this;
}


template <Numeric T>
Matrix<T> Matrix<T>::operator*(const T &scalar) const {
  Matrix<T> copy(*this);
  copy *= scalar;
  return copy;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator*=(const T &scalar) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) *= scalar;
    }
  }
  return *this;
}

template <Numeric T>
Matrix<T> Matrix<T>::operator+(const T &scalar) const {
  Matrix<T> copy(*this);
  copy += scalar;
  return copy;
}

template <Numeric T>
Matrix<T>& Matrix<T>::operator+=(const T &scalar) {
  for (auto &el : *this) {
    el += scalar;
  }
  return *this;
}

template<Numeric T>
Matrix<T>& Matrix<T>::hadamardProductInplace(const Matrix<T>& other) {
  assert(rows == other.getRows() && columns == other.getColumns());
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) *= other(i, j);
    }
  }
  return *this;
}

template<Numeric T>
Matrix<T>& Matrix<T>::hadamardDivisionInplace(const Matrix<T>& other) {
  assert(rows == other.getRows() && columns == other.getColumns());
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) /= other(i, j);
    }
  }
  return *this;
}


template<Numeric T>
Matrix<T>& Matrix<T>::applyElementWiseFunction(std::function<T(T)> func) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) = func((*this)(i, j));
    }
  }
  return *this;
}

template <Numeric T>
Matrix<T> &Matrix<T>::operator/=(const T &scalar) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      (*this)(i, j) /= scalar;
    }
  }
  return *this;
}

template <Numeric T>
Matrix<T> Matrix<T>::operator/(const T &scalar) const {
  Matrix<T> copy(*this);
  copy /= scalar;
  return copy;
}

template <Numeric T>
void Matrix<T>::checkEqualColumnToRow(const Matrix<T> &other) const {
  if (this->columns != other.rows) {
    std::string msg = std::to_string(this->rows) + "x" + std::to_string(this->columns) + " and " +
                      std::to_string(other.rows) + "x" + std::to_string(other.columns);
    throw std::runtime_error(
        "using incompatible matrix dimension for "
        "Matrix::operator*(): dimensions: " +
        msg);
  }
}

template <Numeric T>
void Matrix<T>::checkForEqualDimensions(const Matrix<T> &other) const {
  if (this->rows != other.rows || this->columns != other.columns) {
    std::string msg = std::to_string(this->rows) + "x" + std::to_string(this->columns) + " and " +
                      std::to_string(other.rows) + "x" + std::to_string(other.columns);
    throw std::runtime_error(
        "using incompatible matrix dimension for "
        "Matrix::operator+(): dimensions: " +
        msg);
  }
}

template <Numeric T>
std::ostream &operator<<(std::ostream &outputstream, const Matrix<T> &mat) {
  for (int i = 0; i < mat.rows; ++i) {
    for (int j = 0; j < mat.columns; ++j) {
      outputstream << mat(i, j) << ", ";
    }
    outputstream << "\n";
  }
  return outputstream;
}

}  // namespace lin
