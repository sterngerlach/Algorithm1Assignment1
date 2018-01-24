
// Algorithm1Assignment1
// Main.cpp

#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include "Utility.hpp"

using namespace Algorithm1Assignment1::Cpp;

void ShowStatus(
    std::intmax_t numOfBinarySearchTree,
    std::intmax_t numOfAVLTree,
    double averageHeight,
    double varianceOfHeight,
    double averageHeightOfAVLTree,
    double varianceOfHeightOfAVLTree,
    int numOfNodes,
    const std::chrono::milliseconds& elapsedTime)
{
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
}

int main(int argc, char** argv)
{
#if defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

    --argc;
    ++argv;

    if (argc <= 0) {
        std::cerr << "Too few arguments\n";
        return EXIT_FAILURE;
    }

    if (!std::strcmp(argv[0], "generate-random-permutation")) {
        // �����_���Ȑ���̐���
        
        if (argc != 3) {
            std::cerr << "Invalid arguments\n";
            return EXIT_FAILURE;
        }

        int sequenceLength = std::atoi(argv[1]);
        int numOfSequences = std::atoi(argv[2]);

        std::vector<int> randomPermutation;

        std::cout << "Generating random permutations..." << '\n';

        for (int i = 0; i < numOfSequences; ++i) {
            randomPermutation = std::move(GenerateRandomPermutation(sequenceLength));
            std::copy(std::begin(randomPermutation), std::end(randomPermutation), std::ostream_iterator<int>(std::cout, ", "));
            std::cout << '\n';
        }
    } else if (!std::strcmp(argv[0], "calculate-binary-search-tree")) {
        // 2���T���؂̍����̕��ςƕ��U�̌v�Z

        if (argc != 2) {
            std::cerr << "Invalid arguments\n";
            return EXIT_FAILURE;
        }

        int maxNumOfNodes = std::atoi(argv[1]);

        std::intmax_t numOfBinarySearchTree;
        std::intmax_t numOfAVLTree;
        double averageHeight;
        double varianceOfHeight;
        double averageHeightOfAVLTree;
        double varianceOfHeightOfAVLTree;

        std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

        // �v���J�n
        auto startTime = std::chrono::system_clock::now();

        // �f�[�^����ς���2���T���؂̍����̕��ςƕ��U���v�Z
        for (int i = 1; i <= maxNumOfNodes; ++i) {
            // �S�Ẳ\��2���T���؂�Ђ��[���玎��
            TryAllPossibleBinarySearchTree(
                i,
                numOfBinarySearchTree,
                averageHeight,
                varianceOfHeight,
                numOfAVLTree,
                averageHeightOfAVLTree,
                varianceOfHeightOfAVLTree);

            auto endTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            ShowStatus(
                numOfBinarySearchTree,
                numOfAVLTree,
                averageHeight,
                varianceOfHeight,
                averageHeightOfAVLTree,
                varianceOfHeightOfAVLTree,
                i,
                elapsedTime);
        }
    } else if (!std::strcmp(argv[0], "calculate-binary-search-tree-fast")) {
        // 2���T���؂̍����̕��ςƕ��U�̌v�Z (����)

        if (argc != 2) {
            std::cerr << "Invalid arguments\n";
            return EXIT_FAILURE;
        }

        int maxNumOfNodes = std::atoi(argv[1]);

        std::intmax_t numOfBinarySearchTree;
        std::intmax_t numOfAVLTree;
        double averageHeight;
        double varianceOfHeight;
        double averageHeightOfAVLTree;
        double varianceOfHeightOfAVLTree;

        std::cout << "Calculating average height and variance of height of binary search trees..." << '\n';

        // �v���J�n
        auto startTime = std::chrono::system_clock::now();

        // �f�[�^����ς���2���T���؂̍����̕��ςƕ��U���v�Z
        for (int i = 1; i <= maxNumOfNodes; ++i) {
            if (i <= 9) {
                CalculateBinarySearchTreeHeight(
                    i,
                    numOfBinarySearchTree,
                    averageHeight,
                    varianceOfHeight,
                    numOfAVLTree,
                    averageHeightOfAVLTree,
                    varianceOfHeightOfAVLTree);
            } else {
                // i��10�ȏ�̏ꍇ�̓}���`�X���b�h�ł����s
                CalculateBinarySearchTreeHeight3MultiThreaded(
                    i,
                    numOfBinarySearchTree,
                    averageHeight,
                    varianceOfHeight,
                    numOfAVLTree,
                    averageHeightOfAVLTree,
                    varianceOfHeightOfAVLTree);
            }
            
            auto endTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            ShowStatus(
                numOfBinarySearchTree,
                numOfAVLTree,
                averageHeight,
                varianceOfHeight,
                averageHeightOfAVLTree,
                varianceOfHeightOfAVLTree,
                i,
                elapsedTime);
        }
    } else if (std::strcmp(argv[0], "binary-search-tree") == 0) {
        // 2���T���؂̃e�X�g

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
                // 2���T���؂ɐV�����m�[�h��}��
                strStream >> nodeValue;
                binarySearchTree.Insert(nodeValue);
                std::cout << binarySearchTree.ToString() << '\n';
            } else if (strOperation == "remove") {
                // 2���T���؂���m�[�h���폜
                strStream >> nodeValue;
                binarySearchTree.Remove(nodeValue);
                std::cout << binarySearchTree.ToString() << '\n';
            } else if (strOperation == "destroy") {
                // 2���T���؂̔j��
                binarySearchTree.Destroy();
                std::cout << binarySearchTree.ToString() << '\n';
            }
        }
    }

    std::getchar();

    return 0;
}
