
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

// フィボナッチ木に含まれるノード数の計算
int FibonacciTreeNodeCount(int n)
{
    static std::vector<int> numOfNodes { 0, 1 };

    if (n < numOfNodes.size())
        return numOfNodes[n];

    for (size_t i = numOfNodes.size(); i <= n; ++i)
        numOfNodes.push_back(numOfNodes[i - 1] + numOfNodes[i - 2] + 1);

    return numOfNodes[n];
}

// AVL木の判定が必要かどうか
bool ShouldCheckAVLTree(int numOfNodes, int numOfLeft, int numOfRight)
{
    // numOfNodesは2分木に含まれるノード数
    // numOfLeftは左部分木に含まれるノード数
    // numOfRightは右部分木に含まれるノード数

    // フィボナッチ木のノード数とnumOfNodesを比較
    int n = 0;
    while (numOfNodes > FibonacciTreeNodeCount(++n));

    // nが行き過ぎるので1つ戻す
    --n;

    // n - 2番目のフィボナッチ木のノード数と, numOfLeftとnumOfRightのうちどちらか小さい方とを比較
    return FibonacciTreeNodeCount(n - 2) <= std::min(numOfLeft, numOfRight);
}

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
// nが16を超えるとメモリ使用量が2GBを突破するので, コンパイル時にLARGEADDRESSAWAREモードを指定する必要がある
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

// 2分探索木の高さの平均と分散の計算
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

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (高速)
void CalculateBinarySearchTreeHeight(
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

    // 2分探索木の高さの平均と分散の計算
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (高速) (マルチスレッド)
void CalculateBinarySearchTreeHeightMultiThreaded(
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

    // 2分探索木の高さの平均と分散の計算
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (更に高速)
void CalculateBinarySearchTreeHeight2(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // 入力データ列の生成
    std::vector<std::vector<int>> dataSequences = std::move(GenerateDataSequences(dataCount));

    // 2分探索木 (高さの平均と計算のために特化)
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

        // 2分探索木がAVL木の場合
        if (binarySearchTree.IsAVLTree()) {
            heightOfAVLTree.push_back(treeHeight);

            // 調べたAVL木の個数を更新
            ++numOfAVLTree;
        }

        // 2分探索木のデータを破棄
        binarySearchTree.Destroy();
    }

    // 2分探索木の高さの平均と分散の計算
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (更に高速) (マルチスレッド)
void CalculateBinarySearchTreeHeight2MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
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
            CBinarySearchTreeEx tmpBinarySearchTree;
            tmpBinarySearchTree.SetDataCount(dataCount);

            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;
            int treeHeight;

            for (unsigned int i = beginIndex; i < endIndex; ++i) {
                // 2分探索木にデータを追加
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

    // 2分探索木の高さの平均と分散の計算
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

// 指定されたノード数の相異なる2分探索木の高さの平均と分散の計算 (バージョン3) (マルチスレッド)
void CalculateBinarySearchTreeHeight3MultiThreaded(
    int dataCount,
    std::intmax_t& numOfBinarySearchTree,
    double& averageHeight,
    double& varianceOfHeight,
    std::intmax_t& numOfAVLTree,
    double& averageHeightOfAVLTree,
    double& varianceOfHeightOfAVLTree)
{
    // データの生成と同時に木の高さを調べることで, メモリ使用量を削減
    // 左右の部分木に含まれるノード数をみて, AVL木の判定が必要かどうかを調べる処理の追加

    // 高さと平均データ
    numOfBinarySearchTree = 0;
    numOfAVLTree = 0;

    // 個々の形状の2分探索木の高さを保持
    std::vector<int> heightOfTree;
    std::vector<int> heightOfAVLTree;

    // heightOfTree.reserve((dataCount % 2 == 0) ? CatalanNumber(dataCount) / 2 : CatalanNumber(dataCount));

    // スレッド数
    const unsigned int numOfThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> calcThreads;
    calcThreads.reserve(numOfThreads);

    std::mutex mutex;
    unsigned int minRootNodeValue = 0;
    unsigned int maxRootNodeValue = 0;
    unsigned int maxDataCount = (dataCount % 2 == 0) ? dataCount / 2 : dataCount;

    // スレッドの生成
    for (unsigned int i = 0; i < numOfThreads; ++i) {
        // 各スレッドにタスクを割り当て
        // 各スレッドが担当する根ノードの値の範囲
        minRootNodeValue = maxRootNodeValue + 1;
        maxRootNodeValue = (i == numOfThreads - 1) ? maxDataCount :
            (i + 1) * static_cast<int>(std::ceil((static_cast<double>(maxDataCount) / static_cast<double>(numOfThreads))));

        calcThreads.push_back(std::thread([&, minRootNodeValue, maxRootNodeValue]() {
            // 一時的なデータ
            CBinarySearchTreeEx tmpBinarySearchTree;
            tmpBinarySearchTree.SetDataCount(dataCount);

            std::vector<int> tmpHeightOfTree;
            std::vector<int> tmpHeightOfAVLTree;
            int tmpNumOfBinarySearchTree = 0;
            int tmpNumOfAVLTree = 0;
            int treeHeight;
            bool checkAVLTree;

            // 入力データ列の生成
            for (int i = minRootNodeValue; i <= maxRootNodeValue; ++i) {
                std::vector<std::vector<int>> leftTrees;
                leftTrees.reserve(CatalanNumber(i - 1));
                GenerateDataSequencesHelper(1, i - 1, leftTrees);

                std::vector<std::vector<int>> rightTrees;
                rightTrees.reserve(CatalanNumber(dataCount - i));
                GenerateDataSequencesHelper(i + 1, dataCount, rightTrees);

                // AVL木の判定が必要かどうか
                mutex.lock();
                checkAVLTree = ShouldCheckAVLTree(dataCount, i - 1, dataCount - i);
                mutex.unlock();

                for (const auto& leftTree : leftTrees) {
                    for (const auto& rightTree : rightTrees) {
                        // 2分探索木に根ノードを追加
                        tmpBinarySearchTree.Insert(i);

                        // 2分探索木に左部分木を追加
                        for (const auto n : leftTree)
                            tmpBinarySearchTree.Insert(n);

                        // 2分探索木に右部分木を追加
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

            // 値の更新
            mutex.lock();

            numOfBinarySearchTree += tmpNumOfBinarySearchTree;
            numOfAVLTree += tmpNumOfAVLTree;

            heightOfTree.insert(std::end(heightOfTree), std::begin(tmpHeightOfTree), std::end(tmpHeightOfTree));
            heightOfAVLTree.insert(std::end(heightOfAVLTree), std::begin(tmpHeightOfAVLTree), std::end(tmpHeightOfAVLTree));

            mutex.unlock();
        }));
    }

    // スレッドの終了を待機
    std::for_each(std::begin(calcThreads), std::end(calcThreads), [](std::thread& calcThread) { calcThread.join(); });

    // 2分探索木の高さの平均と分散の計算
    CalculateAverageHeightAndVarianceOfHeight(
        averageHeight, varianceOfHeight, heightOfTree,
        averageHeightOfAVLTree, varianceOfHeightOfAVLTree, heightOfAVLTree);
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
