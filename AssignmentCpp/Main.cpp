
// Algorithm1Assignment1
// Main.cpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <numeric>
#include <sstream>
#include <thread>
#include <vector>

#include <crtdbg.h>

#include "BinarySearchTree.hpp"
#include "Utility.hpp"

using namespace Algorithm1Assignment1::Cpp;

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
                generatedTree.reserve(leftTree.size() + rightTree.size() + 1U);
                generatedTree.push_back(i);
                generatedTree.insert(std::end(generatedTree), std::begin(leftTree), std::end(leftTree));
                generatedTree.insert(std::end(generatedTree), std::begin(rightTree), std::end(rightTree));
                binarySearchTrees.push_back(std::move(generatedTree));
            }
        }
    }
}

#if 0
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
#endif

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

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z
void CalculateBinarySearchTreeHeightFast(
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

    auto accumulateVarianceOfHeight =
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

// �w�肳�ꂽ�m�[�h���̑��قȂ�2���T���؂̍����̕��ςƕ��U�̌v�Z (�}���`�X���b�h)
void CalculateBinarySearchTreeHeightFastMultiThreaded(
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
                for (const auto n : dataSequences.at(i))
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

    // �e��l�̌v�Z
    auto accumulateVarianceOfHeight =
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

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    // 1����n�܂ł̎��R���̃����_���ȗ��100����������
    std::cout << "Generating random permutations..." << '\n';

    std::vector<int> randomPermutation;

    for (int i = 0; i < 100; ++i) {
        randomPermutation = std::move(GenerateRandomPermutation(15));
        std::copy(std::begin(randomPermutation), std::end(randomPermutation), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << '\n';
    }

    // 2���T���؂̍����̕��ςƕ��U�̌v�Z
    std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

    std::intmax_t numOfBinarySearchTree;
    std::intmax_t numOfAVLTree;
    double averageHeight;
    double varianceOfHeight;
    double averageHeightOfAVLTree;
    double varianceOfHeightOfAVLTree;

    // �v���J�n
    auto startTime = std::chrono::system_clock::now();

    // �f�[�^����ς���2���T���؂̍����̕��ςƕ��U���v�Z
    for (int i = 1; i <= 15; ++i) {
#if 0
        TryAllPossibleBinarySearchTree(i,
            numOfBinarySearchTree, averageHeight, varianceOfHeight,
            numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);
#else
        if (i <= 9) {
            CalculateBinarySearchTreeHeightFast(i,
                numOfBinarySearchTree, averageHeight, varianceOfHeight,
                numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);
        } else {
            // i��10�ȏ�̏ꍇ�̓}���`�X���b�h�ł����s
            CalculateBinarySearchTreeHeightFastMultiThreaded(i,
                numOfBinarySearchTree, averageHeight, varianceOfHeight,
                numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);
        }
#endif
        auto endTime = std::chrono::system_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::ostringstream strStream;
        strStream
            << std::string(50, '-') << '\n'
            << "Elapsed Time: " << static_cast<double>(elapsedTime.count()) / 1000.0 << "s\n"
            << "n: " << i
            << ", Count: " << numOfBinarySearchTree
            << ", Average Height: " << averageHeight
            << ", Variance of Height: " << varianceOfHeight << '\n'
            << "n: " << i
            << ", AVL Tree Count: " << numOfAVLTree
            << ", Average Height of AVL Tree: " << averageHeightOfAVLTree
            << ", Variance of Height of AVL Tree: " << varianceOfHeightOfAVLTree << '\n';
        std::cout << strStream.str();

        /*
        std::vector<std::vector<int>> dataSequences = GenerateDataSequences(i);
        std::cout << "dataSequences.size(): " << dataSequences.size() << '\n';

        for (const auto& dataSequence : dataSequences) {
            std::copy(std::begin(dataSequence), std::end(dataSequence), std::ostream_iterator<int>(std::cout, ", "));
            std::cout << std::endl;
        }
        */
    }

    std::getchar();

    return 0;
}
