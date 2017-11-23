
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

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算
void TryAllPossibleBinarySearchTree(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2分探索木の高さの平均と分散を計算するための最も単純なプログラム

    // 1からnまでが順に並んだ順列を生成
    std::vector<int> inputData;
    inputData.resize(dataCount);
    std::iota(std::begin(inputData), std::end(inputData), 1);

    // 2分探索木
    CBinarySearchTree<int, int> binarySearchTree;

    // 高さと平均データ
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // 2分探索木の文字列表現と高さがペアで格納されたマップ
    std::map<std::string, int> mapTreeShapeAndHeight;
    std::map<std::string, int> mapAVLTreeShapeAndHeight;

    // std::next_permutationがfalseを返すまでは次の順列を生成し続ける
    // 全ての順列を生成し2分探索木の入力データとすれば, 可能な2分探索木を全て作成できる
    do {
        // データを2分探索木に挿入
        for (const auto n : inputData)
            binarySearchTree.Insert(n, 0);

        // 2分探索木の文字列表現と高さ
        std::string strBinarySearchTree = binarySearchTree.ToString();
        int treeHeight = binarySearchTree.Height();
        
        // 調べた2分探索木の個数を更新
        ++numOfBinarySearchTree;

        // 既に見つかっている2分探索木の文字列表現と比較し, 無かったら追加
        if (mapTreeShapeAndHeight.find(strBinarySearchTree) == mapTreeShapeAndHeight.end())
            mapTreeShapeAndHeight.insert(std::make_pair(strBinarySearchTree, treeHeight));

        // 2分探索木がAVL木の場合
        if (binarySearchTree.IsAVLTree()) {
            // 既に見つかっているAVL木の文字列表現と比較し, 無かったら追加
            if (mapAVLTreeShapeAndHeight.find(strBinarySearchTree) == mapAVLTreeShapeAndHeight.end())
                mapAVLTreeShapeAndHeight.insert(std::make_pair(strBinarySearchTree, treeHeight));

            // 調べたAVL木の個数を更新
            ++numOfAVLTree;
        }

        // 2分探索木のデータを破棄
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

    // 2分探索木の高さの平均の計算
    averageHeight = std::accumulate(
        mapTreeShapeAndHeight.begin(),
        mapTreeShapeAndHeight.end(),
        0.0, 
        accumulatePairSecond);
    averageHeight /= static_cast<double>(mapTreeShapeAndHeight.size());

    // 2分探索木の高さの分散の計算
    varianceOfHeight = std::accumulate(
        mapTreeShapeAndHeight.begin(),
        mapTreeShapeAndHeight.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeight));
    varianceOfHeight /= static_cast<double>(mapTreeShapeAndHeight.size());

    // AVL木の高さの平均の計算
    averageHeightOfAVLTree = std::accumulate(
        mapAVLTreeShapeAndHeight.begin(),
        mapAVLTreeShapeAndHeight.end(),
        0.0,
        accumulatePairSecond);
    averageHeightOfAVLTree /= static_cast<double>(mapAVLTreeShapeAndHeight.size());

    // AVL木の高さの分散の計算
    varianceOfHeightOfAVLTree = std::accumulate(
        mapAVLTreeShapeAndHeight.begin(),
        mapAVLTreeShapeAndHeight.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeightOfAVLTree));
    varianceOfHeightOfAVLTree /= static_cast<double>(mapAVLTreeShapeAndHeight.size());
}

// 2分探索木を作るためのデータ列を生成
// GenerateDataSequences() のヘルパー関数
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
// カタラン数の計算
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

// 2分探索木を作るためのデータ列を生成
std::vector<std::vector<int>> GenerateDataSequences(int dataCount)
{
    std::vector<std::vector<int>> dataSequences;
    GenerateDataSequencesHelper(1, dataCount, dataSequences);

    // dataCountが偶数の場合は後半のデータ列を捨てる
    if (dataCount % 2 == 0)
        dataSequences.erase(dataSequences.begin() + (dataSequences.size() / 2), dataSequences.end());

    return dataSequences;
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算
void CalculateBinarySearchTreeHeightFast(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2分探索木の高さの平均と分散を計算するためのプログラム

    // ノード数がnの相異なる2分木の個数はカタラン数Cnに一致するので,
    // 生成する2分探索木の形状のパターン数もカタラン数Cnに一致する
    // 同じ形状(高さ)の2分探索木を複数回処理しないために, 個々の形状が1回ずつ現れるような入力データ列を生成する
    
    // 入力データ列の生成
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // 2分探索木
    CBinarySearchTree<int, int> binarySearchTree;

    // 調べた2分探索木とAVL木の個数
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // 個々の形状の2分探索木とAVL木の高さ
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    for (const auto& dataSequence : dataSequences) {
        // データを2分探索木に挿入
        for (const auto n : dataSequence)
            binarySearchTree.Insert(n);

        // 2分探索木の高さ
        int treeHeight = binarySearchTree.Height();

        // 調べた2分探索木の個数を更新
        ++numOfBinarySearchTree;

        heightOfTree.push_back(treeHeight);

        // 2分探索木がAVL木の場合
        if (binarySearchTree.IsAVLTree()) {
            heightOfAVLTree.push_back(treeHeight);

            // 調べたAVL木の個数を更新
            ++numOfAVLTree;
        }

        // 2分探索木のデータを破棄
        binarySearchTree.Destroy();
    }

    auto accumulateVarianceOfHeight =
        [=](double dResult, int treeHeight, double dAverage) -> double {
        return dResult + std::pow(static_cast<double>(treeHeight) - dAverage, 2.0);
    };

    // 2分探索木の高さの平均の計算
    averageHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0);
    averageHeight /= static_cast<double>(heightOfTree.size());

    // 2分探索木の高さの分散の計算
    varianceOfHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeight));
    varianceOfHeight /= static_cast<double>(heightOfTree.size());

    // AVL木の高さの平均の計算
    averageHeightOfAVLTree = std::accumulate(
        heightOfAVLTree.begin(),
        heightOfAVLTree.end(),
        0.0);
    averageHeightOfAVLTree /= static_cast<double>(heightOfAVLTree.size());

    // AVL木の高さの分散の計算
    varianceOfHeightOfAVLTree = std::accumulate(
        heightOfAVLTree.begin(),
        heightOfAVLTree.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeightOfAVLTree));
    varianceOfHeightOfAVLTree /= static_cast<double>(heightOfAVLTree.size());
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (マルチスレッド)
void CalculateBinarySearchTreeHeightFastMultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 2分探索木の高さの平均と分散を計算するためのプログラム

    // 入力データ列の生成
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // 高さと平均データ
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // 個々の形状の2分探索木の高さを保持
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    // スレッド数
    const unsigned int numOfThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> calcThreads;
    calcThreads.reserve(numOfThreads);

    std::mutex mutex;
    unsigned int beginIndex;
    unsigned int endIndex;

    // スレッドの生成
    for (unsigned int i = 0; i < numOfThreads; ++i) {
        // 各スレッドにタスクを割り当て
        beginIndex = i * (dataSequences.size() / numOfThreads);
        endIndex = (i == numOfThreads - 1) ? (dataSequences.size()) : (i + 1) * (dataSequences.size() / numOfThreads);

        calcThreads.push_back(std::thread([&, beginIndex, endIndex]() {
            // 一時的なデータ
            CBinarySearchTree<int, int> tmpBinarySearchTree;
            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;

            for (unsigned int i = beginIndex; i < endIndex; ++i) {
                // 2分探索木にデータを追加
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

            // 値の更新
            std::lock_guard<std::mutex> lockGuard { mutex };

            numOfBinarySearchTree += tmpNumOfBinarySearchTree;
            numOfAVLTree += tmpNumOfAVLTree;

            heightOfTree.insert(std::end(heightOfTree), std::begin(tmpHeightOfTree), std::end(tmpHeightOfTree));
            heightOfAVLTree.insert(std::end(heightOfAVLTree), std::begin(tmpHeightOfAVLTree), std::end(tmpHeightOfAVLTree));
        }));
    }
    
    // スレッドの終了を待機
    std::for_each(std::begin(calcThreads), std::end(calcThreads), [](std::thread& calcThread) { calcThread.join(); });

    // 各種値の計算
    auto accumulateVarianceOfHeight =
        [=](double dResult, int treeHeight, double dAverage) -> double {
        return dResult + std::pow(static_cast<double>(treeHeight) - dAverage, 2.0);
    };

    // 2分探索木の高さの平均の計算
    averageHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0);
    averageHeight /= static_cast<double>(heightOfTree.size());

    // 2分探索木の高さの分散の計算
    varianceOfHeight = std::accumulate(
        heightOfTree.begin(),
        heightOfTree.end(),
        0.0,
        std::bind(accumulateVarianceOfHeight, std::placeholders::_1, std::placeholders::_2, averageHeight));
    varianceOfHeight /= static_cast<double>(heightOfTree.size());

    // AVL木の高さの平均の計算
    averageHeightOfAVLTree = std::accumulate(
        heightOfAVLTree.begin(),
        heightOfAVLTree.end(),
        0.0);
    averageHeightOfAVLTree /= static_cast<double>(heightOfAVLTree.size());

    // AVL木の高さの分散の計算
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

    // 1からnまでの自然数のランダムな列を100個発生させる
    std::cout << "Generating random permutations..." << '\n';

    std::vector<int> randomPermutation;

    for (int i = 0; i < 100; ++i) {
        randomPermutation = std::move(GenerateRandomPermutation(15));
        std::copy(std::begin(randomPermutation), std::end(randomPermutation), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << '\n';
    }

    // 2分探索木の高さの平均と分散の計算
    std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

    std::intmax_t numOfBinarySearchTree;
    std::intmax_t numOfAVLTree;
    double averageHeight;
    double varianceOfHeight;
    double averageHeightOfAVLTree;
    double varianceOfHeightOfAVLTree;

    // 計測開始
    auto startTime = std::chrono::system_clock::now();

    // データ数を変えて2分探索木の高さの平均と分散を計算
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
            // iが10以上の場合はマルチスレッド版を実行
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
