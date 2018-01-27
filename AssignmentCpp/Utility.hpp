
// Algorithm1Assignment1
// Utility.hpp

#ifndef INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_UTILITY_HPP
#define INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_UTILITY_HPP

#include <vector>

#include "BinarySearchTree.hpp"
#include "BinarySearchTreeEx.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// ランダムな1からnまでの自然数の順列を生成
std::vector<int> GenerateRandomPermutation(int n);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算
void TryAllPossibleBinarySearchTree(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 2分探索木を作るためのデータ列を生成
std::vector<std::vector<int>> GenerateDataSequences(int dataCount);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (高速)
void CalculateBinarySearchTreeHeight(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (高速) (マルチスレッド)
void CalculateBinarySearchTreeHeightMultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (更に高速)
void CalculateBinarySearchTreeHeight2(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (更に高速) (マルチスレッド)
void CalculateBinarySearchTreeHeight2MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (バージョン3) (マルチスレッド)
void CalculateBinarySearchTreeHeight3MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (バージョン4) (マルチスレッド)
void CalculateBinarySearchTreeHeight4MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

} // namespace Cpp

} // namespace Algorithm1Assignment1

#endif // INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_UTILITY_HPP
