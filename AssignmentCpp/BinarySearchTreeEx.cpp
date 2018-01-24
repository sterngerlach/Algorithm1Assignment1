
// Algorithm1Assignment1
// BinarySearchTreeEx.cpp

#include "BinarySearchTreeEx.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

// デストラクタ
CBinarySearchTreeEx::~CBinarySearchTreeEx()
{
    if (this->mAllocatedNodes) {
        delete[] this->mAllocatedNodes;
        this->mDataCount = 0;
    }
}

// 2分探索木のノード数を指定
void CBinarySearchTreeEx::SetDataCount(int dataCount)
{
    if (this->mAllocatedNodes) {
        delete[] this->mAllocatedNodes;
        this->mDataCount = 0;
    }

    // 指定された数のノードを先に作成
    // ノードの追加や削除の度にnewやdeleteを呼ぶ必要が無くなる
    this->mDataCount = dataCount;
    this->mAllocatedNodes = new(std::nothrow) Node<int, int>[dataCount];

    for (int i = 0; i < dataCount; ++i)
        this->mAllocatedNodes[i] = Node<int, int>(i + 1, 0);
}

// 指定されたキーとデータを持つノードを作成して2分探索木に挿入
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

// 指定されたノードを2分探索木から削除
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

// 与えられたノードをルートとする部分木から最大の要素を探し,
// その要素のキーとデータを保存してから, その要素自身を左側の子で置き換える
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

// 後順走査により2分探索木のノードを全て削除
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
