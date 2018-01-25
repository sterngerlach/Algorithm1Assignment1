
// Algorithm1Assignment1
// Utility.cpp

#include <algorithm>
#include <cassert>
#include <map>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>

#include "Utility.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// �J�^�������̌v�Z
int CatalanNumber(int n)
{
    if (n <= 1) {
        return 1;
    } else {
        int result = 0;

        for (int k = 0; k < n; ++k)
            result += CatalanNumber(k) * CatalanNumber(n - k - 1);

        return result;
    }
}

// �t�B�{�i�b�`�؂Ɋ܂܂��m�[�h���̌v�Z
int FibonacciTreeNodeCount(int n)
{
    static std::vector<int> numOfNodes { 0, 1 };

    if (n < numOfNodes.size())
        return numOfNodes[n];

    for (size_t i = numOfNodes.size(); i <= n; ++i)
        numOfNodes.push_back(numOfNodes[i - 1] + numOfNodes[i - 2] + 1);

    return numOfNodes[n];
}

// AVL�؂̔��肪�K�v���ǂ���
bool ShouldCheckAVLTree(int numOfNodes, int numOfLeft, int numOfRight)
{
    // numOfNodes��2���؂Ɋ܂܂��m�[�h��
    // numOfLeft�͍������؂Ɋ܂܂��m�[�h��
    // numOfRight�͉E�����؂Ɋ܂܂��m�[�h��

    // �t�B�{�i�b�`�؂̃m�[�h����numOfNodes���r
    int n = 0;
    while (numOfNodes > FibonacciTreeNodeCount(++n));

    // n���s���߂���̂�1�߂�
    --n;

    // n - 2�Ԗڂ̃t�B�{�i�b�`�؂̃m�[�h����, numOfLeft��numOfRight�̂����ǂ��炩���������Ƃ��r
    return FibonacciTreeNodeCount(n - 2) <= std::min(numOfLeft, numOfRight);
}

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

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z
void TryAllPossibleBinarySearchTree(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2���T���؂̍����̕��ςƕ��U���v�Z���邽�߂̍ł��P���ȃv���O����

    // 1����n�܂ł����ɕ��񂾏���𐶐�
    std::vector<int> inputData;
    inputData.resize(dataCount);
    std::iota(std::begin(inputData), std::end(inputData), 1);

    // 2���T����
    CBinarySearchTree<int, int> binarySearchTree;

    // �����ƕ��σf�[�^
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // 2���T���؂̕�����\���ƍ������y�A�Ŋi�[���ꂽ�}�b�v
    std::map<std::string, int> mapTreeShapeAndHeight;
    std::map<std::string, int> mapAVLTreeShapeAndHeight;

    // std::next_permutation��false��Ԃ��܂ł͎��̏���𐶐���������
    // �S�Ă̏���𐶐���2���T���؂̓��̓f�[�^�Ƃ����, �\��2���T���؂�S�č쐬�ł���
    do {
        // �f�[�^��2���T���؂ɑ}��
        for (const auto n : inputData)
            binarySearchTree.Insert(n, 0);

        // 2���T���؂̕�����\���ƍ���
        std::string strBinarySearchTree = binarySearchTree.ToString();
        int treeHeight = binarySearchTree.Height();

        // ���ׂ�2���T���؂̌����X�V
        ++numOfBinarySearchTree;

        // ���Ɍ������Ă���2���T���؂̕�����\���Ɣ�r��, ����������ǉ�
        if (mapTreeShapeAndHeight.find(strBinarySearchTree) == mapTreeShapeAndHeight.end())
            mapTreeShapeAndHeight.insert(std::make_pair(strBinarySearchTree, treeHeight));

        // 2���T���؂�AVL�؂̏ꍇ
        if (binarySearchTree.IsAVLTree()) {
            // ���Ɍ������Ă���AVL�؂̕�����\���Ɣ�r��, ����������ǉ�
            if (mapAVLTreeShapeAndHeight.find(strBinarySearchTree) == mapAVLTreeShapeAndHeight.end())
                mapAVLTreeShapeAndHeight.insert(std::make_pair(strBinarySearchTree, treeHeight));

            // ���ׂ�AVL�؂̌����X�V
            ++numOfAVLTree;
        }

        // 2���T���؂̃f�[�^��j��
        binarySearchTree.Destroy();
    } while (std::next_permutation(std::begin(inputData), std::end(inputData)));

    auto accumulatePairSecond =
        [=](double dResult, const std::pair<std::string, int>& treeShapeAndHeight) -> double {
        return dResult + static_cast<double>(treeShapeAndHeight.second);
    };

    auto accumulateVarianceOfHeight =
        [=](double dResult, const std::pair<std::string, int>& treeShapeAndHeight, double dAverage) -> double {
        return dResult + std::pow(static_cast<double>(treeShapeAndHeight.second) - dAverage, 2.0);
    };

    // 2���T���؂̍����̕��ς̌v�Z
    averageHeight = std::accumulate(
        mapTreeShapeAndHeight.begin(),
        mapTreeShapeAndHeight.end(),
        0.0,
        accumulatePairSecond);
    averageHeight /= static_cast<double>(mapTreeShapeAndHeight.size());

    // 2���T���؂̍����̕��U�̌v�Z
    varianceOfHeight = std::accumulate(
        mapTreeShapeAndHeight.begin(),
        mapTreeShapeAndHeight.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeight));
    varianceOfHeight /= static_cast<double>(mapTreeShapeAndHeight.size());

    // AVL�؂̍����̕��ς̌v�Z
    averageHeightOfAVLTree = std::accumulate(
        mapAVLTreeShapeAndHeight.begin(),
        mapAVLTreeShapeAndHeight.end(),
        0.0,
        accumulatePairSecond);
    averageHeightOfAVLTree /= static_cast<double>(mapAVLTreeShapeAndHeight.size());

    // AVL�؂̍����̕��U�̌v�Z
    varianceOfHeightOfAVLTree = std::accumulate(
        mapAVLTreeShapeAndHeight.begin(),
        mapAVLTreeShapeAndHeight.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeightOfAVLTree));
    varianceOfHeightOfAVLTree /= static_cast<double>(mapAVLTreeShapeAndHeight.size());
}

// 2���T���؂���邽�߂̃f�[�^��𐶐�
// GenerateDataSequences() �̃w���p�[�֐�
// n��16�𒴂���ƃ������g�p�ʂ�2GB��˔j����̂�, �R���p�C������LARGEADDRESSAWARE���[�h���w�肷��K�v������
void GenerateDataSequencesHelper(
    int start, int end, std::vector<std::vector<int>>& binarySearchTrees)
{
    if (start > end) {
        binarySearchTrees.push_back(std::vector<int>());
        return;
    }

    for (int i = start; i <= end; ++i) {
        std::vector<std::vector<int>> leftTrees;
        GenerateDataSequencesHelper(start, i - 1, leftTrees);

        std::vector<std::vector<int>> rightTrees;
        GenerateDataSequencesHelper(i + 1, end, rightTrees);

        for (auto& leftTree : leftTrees) {
            for (auto& rightTree : rightTrees) {
                std::vector<int> generatedTree;
                generatedTree.reserve(end - start + 1);
                // generatedTree.reserve(leftTree.size() + rightTree.size() + 1U);
                
                generatedTree.push_back(i);

                std::copy(std::begin(leftTree), std::end(leftTree), std::back_inserter(generatedTree));
                std::copy(std::begin(rightTree), std::end(rightTree), std::back_inserter(generatedTree));

                // generatedTree.insert(std::end(generatedTree), std::begin(leftTree), std::end(leftTree));
                // generatedTree.insert(std::end(generatedTree), std::begin(rightTree), std::end(rightTree));

                binarySearchTrees.push_back(std::move(generatedTree));
            }
        }
    }
}

// 2���T���؂���邽�߂̃f�[�^��𐶐�
std::vector<std::vector<int>> GenerateDataSequences(int dataCount)
{
    std::vector<std::vector<int>> dataSequences;
    GenerateDataSequencesHelper(1, dataCount, dataSequences);

    // dataCount�������̏ꍇ�͌㔼�̃f�[�^����̂Ă�
    if (dataCount % 2 == 0)
        dataSequences.erase(dataSequences.begin() + (dataSequences.size() / 2), dataSequences.end());

    return dataSequences;
}

// 2���T���؂̍����̕��ςƕ��U�̌v�Z
void CalculateAverageHeightAndVarianceOfHeight(
    double& averageHeight,
    double& varianceOfHeight,
    const std::vector<int>& heightOfTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree,
    const std::vector<int>& heightOfAVLTree)
{
    static auto accumulateVarianceOfHeight =
        [=](double dResult, int treeHeight, double dAverage) -> double {
        return dResult + std::pow(static_cast<double>(treeHeight) - dAverage, 2.0);
    };

    // 2���T���؂̍����̕��ς̌v�Z
    averageHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0);
    averageHeight /= static_cast<double>(heightOfTree.size());

    // 2���T���؂̍����̕��U�̌v�Z
    varianceOfHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeight));
    varianceOfHeight /= static_cast<double>(heightOfTree.size());

    // AVL�؂̍����̕��ς̌v�Z
    averageHeightOfAVLTree = std::accumulate(
        heightOfAVLTree.begin(),
        heightOfAVLTree.end(),
        0.0);
    averageHeightOfAVLTree /= static_cast<double>(heightOfAVLTree.size());

    // AVL�؂̍����̕��U�̌v�Z
    varianceOfHeightOfAVLTree = std::accumulate(
        heightOfAVLTree.begin(),
        heightOfAVLTree.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeightOfAVLTree));
    varianceOfHeightOfAVLTree /= static_cast<double>(heightOfAVLTree.size());
}

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (����)
void CalculateBinarySearchTreeHeight(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2���T���؂̍����̕��ςƕ��U���v�Z���邽�߂̃v���O����

    // �m�[�h����n�̑��قȂ�2���؂̌��̓J�^������Cn�Ɉ�v����̂�,
    // ��������2���T���؂̌`��̃p�^�[�������J�^������Cn�Ɉ�v����
    // �����`��(����)��2���T���؂𕡐��񏈗����Ȃ����߂�, �X�̌`��1�񂸂����悤�ȓ��̓f�[�^��𐶐�����

    // ���̓f�[�^��̐���
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // 2���T����
    CBinarySearchTree<int, int> binarySearchTree;

    // ���ׂ�2���T���؂�AVL�؂̌�
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // �X�̌`���2���T���؂�AVL�؂̍���
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    for (const auto& dataSequence : dataSequences) {
        // �f�[�^��2���T���؂ɑ}��
        for (const auto n : dataSequence)
            binarySearchTree.Insert(n);

        // 2���T���؂̍���
        int treeHeight = binarySearchTree.Height();

        // ���ׂ�2���T���؂̌����X�V
        ++numOfBinarySearchTree;

        heightOfTree.push_back(treeHeight);

        // 2���T���؂�AVL�؂̏ꍇ
        if (binarySearchTree.IsAVLTree()) {
            heightOfAVLTree.push_back(treeHeight);

            // ���ׂ�AVL�؂̌����X�V
            ++numOfAVLTree;
        }

        // 2���T���؂̃f�[�^��j��
        binarySearchTree.Destroy();
    }

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (����) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeightMultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2���T���؂̍����̕��ςƕ��U���v�Z���邽�߂̃v���O����

    // ���̓f�[�^��̐���
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // �����ƕ��σf�[�^
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // �X�̌`���2���T���؂̍�����ێ�
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    // �X���b�h��
    const unsigned int numOfThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> calcThreads;
    calcThreads.reserve(numOfThreads);

    std::mutex mutex;
    unsigned int beginIndex;
    unsigned int endIndex;

    // �X���b�h�̐���
    for (unsigned int i = 0; i < numOfThreads; ++i) {
        // �e�X���b�h�Ƀ^�X�N�����蓖��
        beginIndex = i * (dataSequences.size() / numOfThreads);
        endIndex = (i == numOfThreads - 1) ? (dataSequences.size()) : (i + 1) * (dataSequences.size() / numOfThreads);

        calcThreads.push_back(std::thread([&, beginIndex, endIndex]() {
            // �ꎞ�I�ȃf�[�^
            CBinarySearchTree<int, int> tmpBinarySearchTree;
            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;

            for (unsigned int i = beginIndex; i < endIndex; ++i) {
                // 2���T���؂Ƀf�[�^��ǉ�
                for (const auto n : dataSequences[i])
                    tmpBinarySearchTree.Insert(n);

                int treeHeight = tmpBinarySearchTree.Height();
                tmpHeightOfTree.push_back(treeHeight);
                ++tmpNumOfBinarySearchTree;

                if (tmpBinarySearchTree.IsAVLTree()) {
                    tmpHeightOfAVLTree.push_back(treeHeight);
                    ++tmpNumOfAVLTree;
                }

                tmpBinarySearchTree.Destroy();
            }

            // �l�̍X�V
            std::lock_guard<std::mutex> lockGuard { mutex };

            numOfBinarySearchTree += tmpNumOfBinarySearchTree;
            numOfAVLTree += tmpNumOfAVLTree;

            heightOfTree.insert(std::end(heightOfTree), std::begin(tmpHeightOfTree), std::end(tmpHeightOfTree));
            heightOfAVLTree.insert(std::end(heightOfAVLTree), std::begin(tmpHeightOfAVLTree), std::end(tmpHeightOfAVLTree));
        }));
    }

    // �X���b�h�̏I����ҋ@
    std::for_each(std::begin(calcThreads), std::end(calcThreads), [](std::thread& calcThread) { calcThread.join(); });

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�X�ɍ���)
void CalculateBinarySearchTreeHeight2(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // ���̓f�[�^��̐���
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // 2���T���� (�����̕��ςƌv�Z�̂��߂ɓ���)
    CBinarySearchTreeEx binarySearchTree;
    binarySearchTree.SetDataCount(dataCount);

    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    for (const auto& dataSequence : dataSequences) {
        for (const auto n : dataSequence)
            binarySearchTree.Insert(n);

        int treeHeight = binarySearchTree.Height();

        ++numOfBinarySearchTree;

        heightOfTree.push_back(treeHeight);

        // 2���T���؂�AVL�؂̏ꍇ
        if (binarySearchTree.IsAVLTree()) {
            heightOfAVLTree.push_back(treeHeight);

            // ���ׂ�AVL�؂̌����X�V
            ++numOfAVLTree;
        }

        // 2���T���؂̃f�[�^��j��
        binarySearchTree.Destroy();
    }

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�X�ɍ���) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeight2MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // ���̓f�[�^��̐���
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // �����ƕ��σf�[�^
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // �X�̌`���2���T���؂̍�����ێ�
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    // �X���b�h��
    const unsigned int numOfThreads = std::thread::hardware_concurrency();
    
    std::vector<std::thread> calcThreads;
    calcThreads.reserve(numOfThreads);

    std::mutex mutex;
    unsigned int beginIndex;
    unsigned int endIndex;

    // �X���b�h�̐���
    for (unsigned int i = 0; i < numOfThreads; ++i) {
        // �e�X���b�h�Ƀ^�X�N�����蓖��
        beginIndex = i * (dataSequences.size() / numOfThreads);
        endIndex = (i == numOfThreads - 1) ? (dataSequences.size()) : (i + 1) * (dataSequences.size() / numOfThreads);

        calcThreads.push_back(std::thread([&, beginIndex, endIndex]() {
            // �ꎞ�I�ȃf�[�^
            CBinarySearchTreeEx tmpBinarySearchTree;
            tmpBinarySearchTree.SetDataCount(dataCount);

            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;
            int treeHeight;

            for (unsigned int i = beginIndex; i < endIndex; ++i) {
                // 2���T���؂Ƀf�[�^��ǉ�
                for (const auto n : dataSequences[i])
                    tmpBinarySearchTree.Insert(n);

                treeHeight = tmpBinarySearchTree.Height();
                tmpHeightOfTree.push_back(treeHeight);
                ++tmpNumOfBinarySearchTree;

                if (tmpBinarySearchTree.IsAVLTree()) {
                    tmpHeightOfAVLTree.push_back(treeHeight);
                    ++tmpNumOfAVLTree;
                }

                tmpBinarySearchTree.Destroy();
            }

            // �l�̍X�V
            std::lock_guard<std::mutex> lockGuard { mutex };

            numOfBinarySearchTree += tmpNumOfBinarySearchTree;
            numOfAVLTree += tmpNumOfAVLTree;

            heightOfTree.insert(std::end(heightOfTree), std::begin(tmpHeightOfTree), std::end(tmpHeightOfTree));
            heightOfAVLTree.insert(std::end(heightOfAVLTree), std::begin(tmpHeightOfAVLTree), std::end(tmpHeightOfAVLTree));
        }));
    }

    // �X���b�h�̏I����ҋ@
    std::for_each(std::begin(calcThreads), std::end(calcThreads), [](std::thread& calcThread) { calcThread.join(); });

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�o�[�W����3) (�}���`�X���b�h)
void CalculateBinarySearchTreeHeight3MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // �f�[�^�̐����Ɠ����ɖ؂̍����𒲂ׂ邱�Ƃ�, �������g�p�ʂ��팸
    // ���E�̕����؂Ɋ܂܂��m�[�h�����݂�, AVL�؂̔��肪�K�v���ǂ����𒲂ׂ鏈���̒ǉ�

    // �����ƕ��σf�[�^
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // �X�̌`���2���T���؂̍�����ێ�
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    // heightOfTree.reserve((dataCount % 2 == 0) ? CatalanNumber(dataCount) / 2 : CatalanNumber(dataCount));

    // �X���b�h��
    const unsigned int numOfThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> calcThreads;
    calcThreads.reserve(numOfThreads);

    std::mutex mutex;
    unsigned int minRootNodeValue = 0;
    unsigned int maxRootNodeValue = 0;
    unsigned int maxDataCount = (dataCount % 2 == 0) ? dataCount / 2 : dataCount;

    // �X���b�h�̐���
    for (unsigned int i = 0; i < numOfThreads; ++i) {
        // �e�X���b�h�Ƀ^�X�N�����蓖��
        // �e�X���b�h���S�����鍪�m�[�h�̒l�͈̔�
        minRootNodeValue = maxRootNodeValue + 1;
        maxRootNodeValue = (i == numOfThreads - 1) ? maxDataCount :
            (i + 1) * static_cast<int>(std::ceil((static_cast<double>(maxDataCount) / static_cast<double>(numOfThreads))));

        calcThreads.push_back(std::thread([&, minRootNodeValue, maxRootNodeValue]() {
            // �ꎞ�I�ȃf�[�^
            CBinarySearchTreeEx tmpBinarySearchTree;
            tmpBinarySearchTree.SetDataCount(dataCount);

            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;
            int treeHeight;
            bool checkAVLTree;

            // ���̓f�[�^��̐���
            for (int i = minRootNodeValue; i <= maxRootNodeValue; ++i) {
                std::vector<std::vector<int>> leftTrees;
                leftTrees.reserve(CatalanNumber(i - 1));
                GenerateDataSequencesHelper(1, i - 1, leftTrees);

                std::vector<std::vector<int>> rightTrees;
                rightTrees.reserve(CatalanNumber(dataCount - i));
                GenerateDataSequencesHelper(i + 1, dataCount, rightTrees);

                // AVL�؂̔��肪�K�v���ǂ���
                mutex.lock();
                checkAVLTree = ShouldCheckAVLTree(dataCount, i - 1, dataCount - i);
                mutex.unlock();

                for (const auto& leftTree : leftTrees) {
                    for (const auto& rightTree : rightTrees) {
                        // 2���T���؂ɍ��m�[�h��ǉ�
                        tmpBinarySearchTree.Insert(i);

                        // 2���T���؂ɍ������؂�ǉ�
                        for (const auto n : leftTree)
                            tmpBinarySearchTree.Insert(n);

                        // 2���T���؂ɉE�����؂�ǉ�
                        for (const auto n : rightTree)
                            tmpBinarySearchTree.Insert(n);

                        treeHeight = tmpBinarySearchTree.Height();
                        tmpHeightOfTree.push_back(treeHeight);
                        ++tmpNumOfBinarySearchTree;

                        if (checkAVLTree && tmpBinarySearchTree.IsAVLTree()) {
                            tmpHeightOfAVLTree.push_back(treeHeight);
                            ++tmpNumOfAVLTree;
                        }

                        tmpBinarySearchTree.Destroy();
                    }
                }
            }

            // �l�̍X�V
            mutex.lock();

            numOfBinarySearchTree += tmpNumOfBinarySearchTree;
            numOfAVLTree += tmpNumOfAVLTree;

            heightOfTree.insert(std::end(heightOfTree), std::begin(tmpHeightOfTree), std::end(tmpHeightOfTree));
            heightOfAVLTree.insert(std::end(heightOfAVLTree), std::begin(tmpHeightOfAVLTree), std::end(tmpHeightOfAVLTree));

            mutex.unlock();
        }));
    }

    // �X���b�h�̏I����ҋ@
    std::for_each(std::begin(calcThreads), std::end(calcThreads), [](std::thread& calcThread) { calcThread.join(); });

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
