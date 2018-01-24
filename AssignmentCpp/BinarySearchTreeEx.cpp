
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
        this->mDataCount = 0;
    }
}

// 2���T���؂̃m�[�h�����w��
void CBinarySearchTreeEx::SetDataCount(int dataCount)
{
    if (this->mAllocatedNodes) {
        delete[] this->mAllocatedNodes;
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
    } else if (key < pNode->mKey) {
        this->InsertHelper(key, data, pNode->mLeft);
    } else {
        this->InsertHelper(key, data, pNode->mRight);
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
        int maxNodeKey;
        int maxNodeData;
        this->RemoveMaxNode(pNode, maxNodeKey, maxNodeData);
        pNode = &(this->mAllocatedNodes[maxNodeKey - 1]);
    }
}

// �^����ꂽ�m�[�h�����[�g�Ƃ��镔���؂���ő�̗v�f��T��,
// ���̗v�f�̃L�[�ƃf�[�^��ۑ����Ă���, ���̗v�f���g�������̎q�Œu��������
void CBinarySearchTreeEx::RemoveMaxNode(Node<int, int>*& pNode, int& maxNodeKey, int& maxNodeData)
{
    if (pNode->mRight == nullptr) {
        maxNodeKey = pNode->mKey;
        maxNodeData = pNode->mData;
        pNode = pNode->mLeft;
    } else {
        this->RemoveMaxNode(pNode->mRight, maxNodeKey, maxNodeData);
    }
}

// �㏇�����ɂ��2���T���؂̃m�[�h��S�č폜
void CBinarySearchTreeEx::DestroyHelper(Node<int, int>*& /*pNode*/)
{
    this->mRoot = nullptr;

    for (int i = 0; i < this->mDataCount; ++i) {
        this->mAllocatedNodes[i].mLeft = nullptr;
        this->mAllocatedNodes[i].mRight = nullptr;
    }
}

} // namespace Cpp

} // namespace Algorithm1Assignment1
