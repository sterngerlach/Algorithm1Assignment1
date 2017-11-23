
// Algorithm1Assignment1
// Utility.cpp

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

#include "Utility.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// �����_����1����n�܂ł̎��R���̏���𐶐�
std::vector<int> GenerateRandomPermutation(int n)
{
    assert(n > 0);
    
    // ����
    static std::random_device randomEngine;
    static std::mt19937 mersenneTwister { randomEngine() };

    // 1����n�܂ł����ɕ��񂾏���𐶐�
    std::vector<int> randomPermutation;
    randomPermutation.resize(n);
    std::iota(std::begin(randomPermutation), std::end(randomPermutation), 1);

    // ��������������V���b�t��
    std::shuffle(std::begin(randomPermutation), std::end(randomPermutation), mersenneTwister);

    return randomPermutation;
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
