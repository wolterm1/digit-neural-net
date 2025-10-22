#pragma once

#include <type_traits>
#include <concepts>

namespace lin {

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename T>
concept TensorElement  = requires(T a, T b) {
  { a + b } -> std::convertible_to<T>;
  { a == b } -> std::convertible_to<bool>;
};

}  // namespace lin
