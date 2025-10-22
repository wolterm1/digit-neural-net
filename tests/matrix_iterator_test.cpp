#include <concepts>
#include <iterator>
#include <type_traits>
#include "../include/matrix.hpp"

template <typename It>
void test_iterator_concepts() {
    static_assert(std::input_iterator<It>,               "Not an input iterator");
    static_assert(std::forward_iterator<It>,             "Not a forward iterator");
    static_assert(std::bidirectional_iterator<It>,       "Not a bidirectional iterator");
    static_assert(std::random_access_iterator<It>,       "Not a random access iterator");
    static_assert(std::contiguous_iterator<It>,       "Not a contiguous iterator");
}




int main()  {
    test_iterator_concepts<lin::Matrix<int>::Iterator>();
}
