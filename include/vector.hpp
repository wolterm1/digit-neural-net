#pragma once

#include <cstddef>
#include <functional>
#include <iostream>

#include "concepts.hpp"

namespace lin {

template <TensorElement T>
class Vector;

template <TensorElement T>
std::ostream &operator<<(std::ostream &outputstream, const Vector<T> &vec);

template <TensorElement T>
void checkEqualDimensions(const Vector<T> &first, const Vector<T> &second);

template <TensorElement T>
class Vector {

 private:
  T *vecData;
  size_t size;
  void allocateForVecData();

 public:
  // rule of five
  Vector(const Vector<T> &other);
  Vector(Vector<T> &&vec) noexcept;
  Vector<T> &operator=(const Vector<T> &other);
  Vector<T> &operator=(Vector<T> &&other) noexcept;
  Vector();
  ~Vector();

  explicit Vector(const size_t &p_size);
  Vector(const size_t &psize, const T& fillElement);
  Vector(const std::initializer_list<T> &init);
  Vector(const size_t &psize, const std::function<T()>& func); 

  bool operator==(const Vector<T> &other) const;
  T &operator[](const size_t &pos) const;

  Vector<T> &operator+=(const Vector<T> &other);
  Vector<T> operator+(const Vector<T> &other) const ;

  Vector<T> &operator-=(const Vector<T> &other);
  Vector<T> operator-(const Vector<T> &other) const;

  Vector<T> &operator*=(const T &scalar);
  Vector<T> operator*(const T &scalar) const;

  Vector<T> &operator/=(const T &scalar);
  Vector<T> operator/(const T &scalar) const;

  Vector<T> &operator+=(const T &scalar);
  Vector<T> operator+(const T &scalar) const;

  Vector<T>& hadamardProductInplace(const Vector<T>& other);
  Vector<T>& hadamardDivisionInplace(const Vector<T>& other);
  Vector<T>& applyElementWiseFunction(std::function<T(T)> func);

  friend std::ostream &operator<< <T>(std::ostream &outputstream, const Vector<T> &vec);

  void swap(Vector<T> &) noexcept;
  size_t getSize() const;
  T* data() const;

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
  Iterator begin() { return Iterator(vecData); }
  Iterator end() { return Iterator(vecData + size); }

  Iterator begin() const { return Iterator(vecData); } 
  Iterator end() const { return Iterator(vecData + size); }
};



template <TensorElement T>
Vector<T>::Vector() : size(0) {
  allocateForVecData();
}
template <TensorElement T>
Vector<T>::Vector(const size_t &p_size) : size(p_size) {
  allocateForVecData();
}

template <TensorElement T>
Vector<T>::Vector(const size_t &psize, const T& fillElement) : size(psize) {
  allocateForVecData();
  for (size_t i=0; i<size; ++i) {
    (*this)[i] = fillElement;
  }
}

template <TensorElement T>
Vector<T>::Vector(const size_t &psize, const std::function<T()>& func) : size(psize) {
  allocateForVecData();
  for (size_t i=0; i<size; ++i) {
    (*this)[i] = func();
  }
}

template <TensorElement T>
Vector<T>::Vector(const std::initializer_list<T> &init) : size(init.size()) {
  allocateForVecData();
  std::copy(init.begin(), init.end(), vecData);
}

template <TensorElement T>
Vector<T>::Vector(const Vector<T> &other) : size(other.size) {
  allocateForVecData();
  for (int i = 0; i < size; ++i) {
    (*this)[i] = other[i];
  }
}

template <TensorElement T>
Vector<T>::Vector(Vector<T> &&movedVec) noexcept : vecData(movedVec.vecData), size(movedVec.size) {
  movedVec.vecData = nullptr;
  movedVec.size = 0;
}

template <TensorElement T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
  if (this != &other) {
    Vector<T> copy(other);
    swap(copy);
  }
  return *this;
}

template <TensorElement T>
Vector<T> &Vector<T>::operator=(Vector<T> &&other) noexcept {
  if (this == &other) {
    return *this;
  }
  swap(other);
  return *this;
}

template <TensorElement T>
bool Vector<T>::operator==(const Vector<T> &other) const {
  if (size != other.size) {
    return false;
  }
  for (int i = 0; i < size; ++i) {
    if ((*this)[i] != other[i]) {
      return false;
    }
  }
  return true;
}

template <TensorElement T>
T &Vector<T>::operator[](const size_t &pos) const {
  return vecData[pos];
}

template <TensorElement T>
Vector<T> &Vector<T>::operator+=(const Vector<T> &other) {
  checkEqualDimensions(*this, other);
  for (int i = 0; i < size; ++i) {
    (*this)[i] += other[i];
  }
  return *this;
}

template <TensorElement T>
Vector<T> Vector<T>::operator+(const Vector<T> &other) const {
  Vector<T> copy(*this);
  copy += other;
  return copy;
}

template <TensorElement T>
Vector<T> &Vector<T>::operator-=(const Vector<T> &other) {
  checkEqualDimensions(*this, other);
  for (int i = 0; i < size; ++i) {
    (*this)[i] -= other[i];
  }
  return *this;
}

template <TensorElement T>
Vector<T> Vector<T>::operator-(const Vector<T> &other) const {
  Vector<T> copy(*this);
  copy -= other;
  return copy;
}

template <TensorElement T>
Vector<T> &Vector<T>::operator*=(const T &scalar) {
  for (int i = 0; i < size; ++i) {
    (*this)[i] *= scalar;
  }
  return *this;
}

template <TensorElement T>
Vector<T> Vector<T>::operator*(const T &scalar) const{
  Vector<T> copy(*this);
  copy *= scalar;
  return copy;
}

template <TensorElement T>
Vector<T> &Vector<T>::operator/=(const T &scalar) {
  for (int i = 0; i < size; ++i) {
    (*this)[i] /= scalar;
  }
  return *this;
}

template <TensorElement T>
Vector<T> Vector<T>::operator/(const T &scalar) const{
  Vector<T> copy(*this);
  copy /= scalar;
  return copy;
}

template <TensorElement T>
Vector<T>& Vector<T>::operator+=(const T &scalar) {
  for (auto &el : *this) {
    el += scalar;
  }
  return *this;
}

template <TensorElement T> 
Vector<T> Vector<T>::operator+(const T &scalar) const {
  Vector<T> copy(*this);
  copy += scalar;
  return copy;
}

template <TensorElement T>
Vector<T>& Vector<T>::hadamardProductInplace(const Vector<T>& other) {
  for (size_t i = 0; i<size; ++i){
    (*this)[i] *= other[i];
  }
  return *this;
}

template <TensorElement T>
Vector<T>& Vector<T>::hadamardDivisionInplace(const Vector<T>& other) {
  for (size_t i = 0; i<size; ++i) {
    (*this)[i] /= other[i];
  }
  return *this;
}

template <TensorElement T>
Vector<T>& Vector<T>::applyElementWiseFunction(std::function<T(T)> func) {
  for (auto& el : *this) {
    el = func(el);
  }
  return *this;
}



template <TensorElement T>
Vector<T>::~Vector() {
  delete[] vecData;
}

template <TensorElement T>
void Vector<T>::allocateForVecData() {
  vecData = new T[size];
}

template <TensorElement T>
void Vector<T>::swap(Vector<T> &other) noexcept {
  std::swap(vecData, other.vecData);
  std::swap(size, other.size);
}

template <TensorElement T>
size_t Vector<T>::getSize() const {
  return size;
}


template <TensorElement T>
T* Vector<T>::data() const {
  return vecData;
}
/***** friend functons *****/

template <TensorElement T>
void checkEqualDimensions(const Vector<T> &first, const Vector<T> &second) {
  assert(first.getSize() == second.getSize());
}

template <TensorElement T>
std::ostream &operator<<(std::ostream &outputstream, const Vector<T> &vec) {
  outputstream << "[";
  for (int i = 0; i < vec.size-1; i++) {
    outputstream << vec[i] << ", ";
  }
  outputstream << vec[vec.size-1] << "]";
  return outputstream;
}

}  // namespace linAlg
