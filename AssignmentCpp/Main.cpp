
// Algorithm1Assignment1
// Main.cpp

#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <crtdbg.h>

#include "Utility.hpp"

using namespace Algorithm1Assignment1::Cpp;

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

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    --argc;
    ++argv;

    if (argc <= 0) {
        std::cerr << "Too few arguments" << '\n';
        return EXIT_SUCCESS;
    }

    auto showStatus = [](std::intmax_t numOfBinarySearchTree, std::intmax_t numOfAVLTree,
        double averageHeight, double varianceOfHeight,
        double averageHeightOfAVLTree, double varianceOfHeightOfAVLTree,
        int numOfNodes, const std::chrono::milliseconds& elapsedTime) {
        std::ostringstream strStream;
        strStream
            << std::string(50, '-') << '\n'
            << "Elapsed Time: " << static_cast<double>(elapsedTime.count()) / 1000.0 << "s\n"
            << "n: " << numOfNodes
            << ", Count: " << numOfBinarySearchTree
            << ", Average Height: " << averageHeight
            << ", Variance of Height: " << varianceOfHeight << '\n'
            << "n: " << numOfNodes
            << ", AVL Tree Count: " << numOfAVLTree
            << ", Average Height of AVL Tree: " << averageHeightOfAVLTree
            << ", Variance of Height of AVL Tree: " << varianceOfHeightOfAVLTree << '\n';
        std::cout << strStream.str();
    };

    if (std::strcmp(argv[0], "generate-random-permutation") == 0) {
        // 1からsequenceLengthまでの自然数のランダムな列をnumOfSequences個発生させる

        int sequenceLength = std::atoi(argv[1]);
        int numOfSequences = std::atoi(argv[2]);

        std::vector<int> randomPermutation;

        std::cout << "Generating random permutations..." << '\n';

        for (int i = 0; i < numOfSequences; ++i) {
            randomPermutation = std::move(GenerateRandomPermutation(sequenceLength));
            std::copy(std::begin(randomPermutation), std::end(randomPermutation), std::ostream_iterator<int>(std::cout, ", "));
            std::cout << '\n';
        }
    } else if (std::strcmp(argv[0], "calculate-binary-search-tree") == 0) {
        // 2分探索木の高さの平均と分散の計算

        int maxNumOfNodes = std::atoi(argv[1]);

        std::intmax_t numOfBinarySearchTree;
        std::intmax_t numOfAVLTree;
        double averageHeight;
        double varianceOfHeight;
        double averageHeightOfAVLTree;
        double varianceOfHeightOfAVLTree;

        std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

        // 計測開始
        auto startTime = std::chrono::system_clock::now();

        // データ数を変えて2分探索木の高さの平均と分散を計算
        for (int i = 1; i <= maxNumOfNodes; ++i) {
            // 全ての可能な2分探索木を片っ端から試す
            TryAllPossibleBinarySearchTree(i,
                numOfBinarySearchTree, averageHeight, varianceOfHeight,
                numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);

            auto endTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            showStatus(
                numOfBinarySearchTree, numOfAVLTree,
                averageHeight, varianceOfHeight,
                averageHeightOfAVLTree, varianceOfHeightOfAVLTree,
                i, elapsedTime);
        }
    } else if (std::strcmp(argv[0], "calculate-binary-search-tree-fast") == 0) {
        // 2分探索木の高さの平均と分散の計算 (高速)

        int maxNumOfNodes = std::atoi(argv[1]);

        std::intmax_t numOfBinarySearchTree;
        std::intmax_t numOfAVLTree;
        double averageHeight;
        double varianceOfHeight;
        double averageHeightOfAVLTree;
        double varianceOfHeightOfAVLTree;

        std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

        // 計測開始
        auto startTime = std::chrono::system_clock::now();

        // データ数を変えて2分探索木の高さの平均と分散を計算
        for (int i = 1; i <= maxNumOfNodes; ++i) {
            if (i <= 9) {
                CalculateBinarySearchTreeHeight(i,
                    numOfBinarySearchTree, averageHeight, varianceOfHeight,
                    numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);
            } else {
                // iが10以上の場合はマルチスレッド版を実行
                CalculateBinarySearchTreeHeight3MultiThreaded(i,
                    numOfBinarySearchTree, averageHeight, varianceOfHeight,
                    numOfAVLTree, averageHeightOfAVLTree, varianceOfHeightOfAVLTree);
            }
            
            auto endTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            showStatus(
                numOfBinarySearchTree, numOfAVLTree,
                averageHeight, varianceOfHeight,
                averageHeightOfAVLTree, varianceOfHeightOfAVLTree,
                i, elapsedTime);
        }
    } else if (std::strcmp(argv[0], "binary-search-tree") == 0) {
        // 2分探索木の要素の追加や削除

        CBinarySearchTree<int, int> binarySearchTree;
        std::string strCommand;

        while (true) {
            std::cout << "> ";

            if (!std::getline(std::cin, strCommand))
                break;

            if (strCommand.empty())
                break;

            std::istringstream strStream { strCommand };
            std::string strOperation;
            int nodeValue;

            strStream >> strOperation;

            if (strOperation == "insert") {
                // 2分探索木に新しいノードを挿入
                strStream >> nodeValue;
                binarySearchTree.Insert(nodeValue);
                std::cout << binarySearchTree.ToString() << '\n';
            } else if (strOperation == "remove") {
                // 2分探索木からノードを削除
                strStream >> nodeValue;
                binarySearchTree.Remove(nodeValue);
                std::cout << binarySearchTree.ToString() << '\n';
            } else if (strOperation == "destroy") {
                // 2分探索木の破棄
                binarySearchTree.Destroy();
                std::cout << binarySearchTree.ToString() << '\n';
            }
        }
    }

    std::getchar();

    return 0;
}
