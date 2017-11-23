
// Algorithm1Assignment1
// Utility.cpp

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

#include "Utility.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// ランダムな1からnまでの自然数の順列を生成
std::vector<int> GenerateRandomPermutation(int n)
{
    assert(n > 0);
    
    // 乱数
    static std::random_device randomEngine;
    static std::mt19937 mersenneTwister { randomEngine() };

    // 1からnまでが順に並んだ順列を生成
    std::vector<int> randomPermutation;
    randomPermutation.resize(n);
    std::iota(std::begin(randomPermutation), std::end(randomPermutation), 1);

    // 生成した順列をシャッフル
    std::shuffle(std::begin(randomPermutation), std::end(randomPermutation), mersenneTwister);

    return randomPermutation;
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
