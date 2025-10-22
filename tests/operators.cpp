#include <cassert>

#include "../include/matrix.hpp"

int main() {
    using lin::Matrix;

    std::cout << "testing operators" << '\n';
    Matrix<float> base(3, 3, 1.5);
    Matrix<float> compare_equal(3, 3, 1.5);
    Matrix<float> compare_unequal(3, 3, 2.0);
    std::cout << "base: \n" << base << '\n';

    assert(base == compare_equal);
    assert(!(base == compare_unequal));

    assert(base != compare_unequal);
    assert(!(base != compare_equal));

    std::cout << "\nadding Identity to floatMatrix" << '\n';

    base += Matrix<float>::createIdentity(3, 3);
    std::cout << base << '\n';

    std::cout << Matrix<int>::createIdentity(3, 3) - Matrix<int>::createIdentity(3, 3) << '\n';

    Matrix<int> first_operand({{2, 3, 4}, {2, 0, 4}, {1, 5, 2}});
    Matrix<int> second_operand({{2, 1, 1}, {2, 3, 1}, {2, 3, 1}});
    std::cout << "Multiplying: \n" << first_operand << "with\n" << second_operand << '\n';
    first_operand *= second_operand;
    std::cout << first_operand << '\n';

    std::cout << "scalar multiplication: Identity*100000\n "
              << Matrix<long long>::createIdentity(4, 4) * 100000 << '\n';

    std::cout << "scalar division: Identity/3\n "
              << Matrix<double>::createIdentity(4, 4) / 3 << '\n';
}
