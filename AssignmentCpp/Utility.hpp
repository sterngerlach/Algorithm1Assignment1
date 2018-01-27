
// Algorithm1Assignment1
// Utility.hpp

#ifndef INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_UTILITY_HPP
#define INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_UTILITY_HPP

#include <vector>

#include "BinarySearchTree.hpp"
#include "BinarySearchTreeEx.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// �����_����1����n�܂ł̎��R���̏���𐶐�
std::vector<int> GenerateRandomPermutation(int n);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z
void TryAllPossibleBinarySearchTree(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// 2���T���؂���邽�߂̃f�[�^��𐶐�
std::vector<std::vector<int>> GenerateDataSequences(int dataCount);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (����)
void CalculateBinarySearchTreeHeight(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (����) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeightMultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�X�ɍ���)
void CalculateBinarySearchTreeHeight2(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�X�ɍ���) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeight2MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�o�[�W����3) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeight3MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree);

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�o�[�W����4) (�}���`�X���b�h)
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
