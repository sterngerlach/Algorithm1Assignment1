
// Algorithm1Assignment1
// BinarySearchTreeEx.cpp

#include "BinarySearchTreeEx.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// �f�X�g���N�^
CBinarySearchTreeEx::~CBinarySearchTreeEx()
{
    if (this->mAllocatedNodes) {
        delete[] this->mAllocatedNodes;
        this->mAllocatedNodes = nullptr;
        this->mDataCount = 0;
    }
}

// 2���T���؂̃m�[�h�����w��
void CBinarySearchTreeEx::SetDataCount(int dataCount)
{
    if (this->mAllocatedNodes) {
        delete[] this->mAllocatedNodes;
        this->mAllocatedNodes = nullptr;
        this->mDataCount = 0;
    }

    // �w�肳�ꂽ���̃m�[�h���ɍ쐬
    // �m�[�h�̒ǉ���폜�̓x��new��delete���ĂԕK�v�������Ȃ�
    this->mDataCount = dataCount;
    this->mAllocatedNodes = new(std::nothrow) Node<int, int>[dataCount];

    for (int i = 0; i < dataCount; ++i)
        this->mAllocatedNodes[i] = Node<int, int>(i + 1, 0);
}

// �w�肳�ꂽ�L�[�ƃf�[�^�����m�[�h���쐬����2���T���؂ɑ}��
void CBinarySearchTreeEx::InsertHelper(const int& key, const int& data, Node<int, int>*& pNode)
{
    if (pNode == nullptr) {
        pNode = &(this->mAllocatedNodes[key - 1]);
    } else if (key == pNode->mKey) {
        // ��d�o�^
        assert(false);
    } else {
        if (key < pNode->mKey) {
            this->InsertHelper(key, data, pNode->mLeft);
        } else {
            this->InsertHelper(key, data, pNode->mRight);
        }
    }
}

// �w�肳�ꂽ�m�[�h��2���T���؂���폜
void CBinarySearchTreeEx::RemoveNode(Node<int, int>*& pNode)
{
    if (pNode == nullptr) {
        assert(false);
    } else if (pNode->mLeft == nullptr && pNode->mRight == nullptr) {
        pNode = nullptr;
    } else if (pNode->mLeft == nullptr) {
        pNode = pNode->mRight;
    } else if (pNode->mRight == nullptr) {
        pNode = pNode->mLeft;
    } else {
        std::function<void(Node<int, int>*&, int&)> ProcessNodeWithMaxValue;

        ProcessNodeWithMaxValue =
            [&ProcessNodeWithMaxValue](Node<int, int>*& pMaxNode, int& maxNodeKey) {
            if (pMaxNode->mRight == nullptr) {
                maxNodeKey = pMaxNode->mKey;
                pMaxNode = pMaxNode->mLeft;
            } else {
                ProcessNodeWithMaxValue(pMaxNode->mRight, maxNodeKey);
            }
        };

        int maxNodeKey;
        ProcessNodeWithMaxValue(pNode->mLeft, maxNodeKey);
        pNode = &(this->mAllocatedNodes[maxNodeKey - 1]);
    }
}

// �㏇�����ɂ��2���T���؂̃m�[�h��S�č폜
void CBinarySearchTreeEx::DestroyHelper(Node<int, int>*& pNode)
{
    this->mRoot = nullptr;

    for (int i = 0; i < this->mDataCount; ++i) {
        this->mAllocatedNodes[i].mLeft = nullptr;
        this->mAllocatedNodes[i].mRight = nullptr;
    }
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
