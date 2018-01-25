
// Algorithm1Assignment1
// BinarySearchTree.hpp

#ifndef INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_HPP
#define INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_HPP

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <new>
#include <sstream>
#include <string>

#include "Node.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

//
// CBinarySearchTreeクラス
// 2分探索木の表現
//

template <typename TKey, typename TData>
class CBinarySearchTree {
public:
    CBinarySearchTree() : mRoot(nullptr) { }
    virtual ~CBinarySearchTree();

    Node<TKey, TData>* Search(const TKey& targetKey) const;
    void Insert(const TKey& key, const TData& data = TData());
    void Remove(const TKey& key);
    void Destroy();
    int Height() const;
    bool IsAVLTree() const;
    std::string ToString() const;

protected:
    virtual Node<TKey, TData>* SearchHelper(const TKey& targetKey, Node<TKey, TData>* pNode) const;
    virtual void InsertHelper(const TKey& key, const TData& data, Node<TKey, TData>*& pNode);
    virtual void RemoveHelper(const TKey& key, Node<TKey, TData>*& pNode);
    virtual void RemoveNode(Node<TKey, TData>*& pNode);
    virtual void RemoveMaxNode(Node<TKey, TData>*& pNode, TKey& maxNodeKey, TData& maxNodeData);
    virtual void DestroyHelper(Node<TKey, TData>*& pNode);
    virtual int HeightHelper(Node<TKey, TData>* pNode) const;
    virtual bool IsAVLTreeHelper(Node<TKey, TData>* pNode) const;
    virtual void ToStringHelper(std::ostringstream& strStream, Node<TKey, TData>* pNode) const;

protected:
    Node<TKey, TData>* mRoot;
};

//
// CBinarySearchTreeクラスの実装
//

// デストラクタ
template <typename TKey, typename TData>
CBinarySearchTree<TKey, TData>::~CBinarySearchTree()
{
    if (this->mRoot)
        this->Destroy();
}

// 指定されたキーを持つノードを2分探索木の中から探索
template <typename TKey, typename TData>
Node<TKey, TData>* CBinarySearchTree<TKey, TData>::Search(
    const TKey& targetKey) const
{
    return this->SearchHelper(targetKey, this->mRoot);
}

// 指定されたキーとデータを持つノードを作成して2分探索木に挿入
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Insert(
    const TKey& key, const TData& data)
{
    this->InsertHelper(key, data, this->mRoot);
}

// 指定されたキーを持つノードを2分探索木から削除
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Remove(
    const TKey& key)
{
    this->RemoveHelper(key, this->mRoot);
}

// 後順走査により2分探索木のノードを全て削除
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Destroy()
{
    this->DestroyHelper(this->mRoot);
}

// 2分探索木の高さの計算
template <typename TKey, typename TData>
int CBinarySearchTree<TKey, TData>::Height() const
{
    return this->HeightHelper(this->mRoot);
}

// 2分探索木がAVL木かどうかの判定
template <typename TKey, typename TData>
bool CBinarySearchTree<TKey, TData>::IsAVLTree() const
{
    return this->IsAVLTreeHelper(this->mRoot);
}

// 2分探索木を文字列表現に変換
template <typename TKey, typename TData>
std::string CBinarySearchTree<TKey, TData>::ToString() const
{
    std::ostringstream strStream;
    this->ToStringHelper(strStream, this->mRoot);
    return strStream.str();
}

// 指定されたキーを持つノードを2分探索木の中から探索
// CBinarySearchTree::Search() のヘルパー関数
template <typename TKey, typename TData>
Node<TKey, TData>* CBinarySearchTree<TKey, TData>::SearchHelper(
    const TKey& targetKey,
    Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr) {
        // 与えられたキーをもつノードが見つからなかった
        return nullptr;
    } else if (targetKey == pNode->mKey) {
        // 与えられたキーと, 現在のノードのキーが等しい場合は見つかった
        return pNode;
    } else {
        if (targetKey < pNode->mKey) {
            // 与えられたキーが, 現在のノードのキーより小さい場合は左の子に移動
            return this->SearchHelper(targetKey, pNode->mLeft);
        } else {
            // 与えられたキーが, 現在のノードのキーより大きい場合は右の子に移動
            return this->SearchHelper(targetKey, pNode->mRight);
        }
    }

    return nullptr;
}

// 指定されたキーとデータを持つノードを作成して2分探索木に挿入
// CBinarySearchTree::Insert() のヘルパー関数
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::InsertHelper(
    const TKey& key,
    const TData& data,
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // 与えられたキーとデータを持つノードを作成
        pNode = new(std::nothrow) Node<TKey, TData>(key, data);
    } else if (key == pNode->mKey) {
        // 二重登録
        assert(false);
    } else {
        if (key < pNode->mKey) {
            this->InsertHelper(key, data, pNode->mLeft);
        } else {
            this->InsertHelper(key, data, pNode->mRight);
        }
    }
}

// 指定されたキーを持つノードを2分探索木から削除
// CBinarySearchTree::Remove() のヘルパー関数
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveHelper(
    const TKey& key, Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // 削除すべきノードが見つからなかった
        assert(false);
    } else if (key != pNode->mKey) {
        if (key < pNode->mKey) {
            this->RemoveHelper(key, pNode->mLeft);
        } else {
            this->RemoveHelper(key, pNode->mRight);
        }
    } else {
        // 削除すべきノードが見つかった
        this->RemoveNode(pNode);
    }
}

// 指定されたノードを2分探索木から削除
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveNode(
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // 起こり得ない
        assert(false);
    } else if (pNode->mLeft == nullptr && pNode->mRight == nullptr) {
        // 現在のノードが葉の場合は単に取り除くだけでよい
        delete pNode;
        pNode = nullptr;
    } else if (pNode->mLeft == nullptr) {
        // 現在のノードに左側の子がいない場合, ノードを右側の子で置き換える
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mRight;
        delete pTmp;
        pTmp = nullptr;
    } else if (pNode->mRight == nullptr) {
        // 現在のノードに右側の子がいない場合, ノードを左側の子で置き換える
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mLeft;
        delete pTmp;
        pTmp = nullptr;
    } else {
        // 現在のノードに左右両方の子がいる場合, ノードを左側の部分木の最大の要素で置き換える
        // 左側の部分木の中から最大の要素を探して, キーとデータを取得
        TKey maxNodeKey;
        TData maxNodeData;
        this->RemoveMaxNode(pNode->mLeft, maxNodeKey, maxNodeData);

        // 最大の要素のキーとデータを, 現在のノードに上書き
        pNode->mKey = maxNodeKey;
        pNode->mData = maxNodeData;
    }
}

// 与えられたノードをルートとする部分木から最大の要素を探し,
// その要素のキーとデータを保存してから, その要素自身を左側の子で置き換える
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveMaxNode(
    Node<TKey, TData>*& pNode, TKey& maxNodeKey, TData& maxNodeData)
{
    if (pNode->mRight == nullptr) {
        // 与えられたノードを頂点とする部分木から最大の要素が見つかった
        // 最大の要素が持っていたキーとデータを保持
        maxNodeKey = pNode->mKey;
        maxNodeData = pNode->mData;

        // 最大の要素をその左側の子で置き換える
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mLeft;
        delete pTmp;
        pTmp = nullptr;
    } else {
        // 部分木から最大の要素を探索
        this->RemoveMaxNode(pNode->mRight, maxNodeKey, maxNodeData);
    }
}

// 後順走査により2分探索木のノードを全て削除
// CBinarySearchTree::Destroy() のヘルパー関数
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::DestroyHelper(
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr)
        return;

    // 最初に現在のノードの左側の子を削除
    if (pNode->mLeft != nullptr)
        this->DestroyHelper(pNode->mLeft);

    // 次に現在のノードの右側の子を削除
    if (pNode->mRight != nullptr)
        this->DestroyHelper(pNode->mRight);

    // 最後に自分を削除
    delete pNode;
    pNode = nullptr;
}

// 2分探索木の高さの計算
// CBinarySearchTree::Height() のヘルパー関数
template <typename TKey, typename TData>
int CBinarySearchTree<TKey, TData>::HeightHelper(
    Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr)
        return -1;
    else
        return std::max(this->HeightHelper(pNode->mLeft), this->HeightHelper(pNode->mRight)) + 1;
}

// 2分探索木がAVL木かどうかの判定
// CBinarySearchTree::IsAVLTree() のヘルパー関数
template <typename TKey, typename TData>
bool CBinarySearchTree<TKey, TData>::IsAVLTreeHelper(
    Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr)
        return true;

    int nLeftHeight = (pNode->mLeft) ? (this->HeightHelper(pNode->mLeft) + 1) : 0;
    int nRightHeight = (pNode->mRight) ? (this->HeightHelper(pNode->mRight) + 1) : 0;

    if (std::abs(nLeftHeight - nRightHeight) > 1)
        return false;
    else
        return this->IsAVLTreeHelper(pNode->mLeft) && this->IsAVLTreeHelper(pNode->mRight);
}

/*
// 2分探索木がAVL木かどうかの判定
// CBinarySearchTree::IsAVLTree() のヘルパー関数
template <typename TKey, typename TData>
bool CBinarySearchTree<TKey, TData>::IsAVLTreeHelper(
    Node<TKey, TData>* pNode) const
{
    // 葉ノードの場合は常にAVL木
    if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
    return true;

    int nLeftHeight = 0;
    int nRightHeight = 0;

    if (pNode->mLeft != nullptr) {
        if (!this->IsAVLTreeHelper(pNode->mLeft)) {
            return false;
        } else {
            // 左部分木を構成する全てのノードについて, そのノードを頂点とする部分木がAVL木である場合にのみ
            // 現在のノードの左部分木の高さを調べる
            // CBinarySearchTree::HeightHelper() は与えられたノードを頂点とする部分木の高さを返すので,
            // これに1を足し合わせて現在のノードを頂点とする部分木の高さを求める
            nLeftHeight = this->HeightHelper(pNode->mLeft) + 1;
        }
    }

    if (pNode->mRight != nullptr) {
        if (!this->IsAVLTreeHelper(pNode->mRight)) {
            return false;
        } else {
            // 右部分木を構成する全てのノードについて, そのノードを頂点とする部分木がAVL木である場合にのみ
            // 現在のノードの右部分木の高さを調べる
            // CBinarySearchTree::HeightHelper() は与えられたノードを頂点とする部分木の高さを返すので,
            // これに1を足し合わせて現在のノードを頂点とする部分木の高さを求める
            nRightHeight = this->HeightHelper(pNode->mRight) + 1;
        }
    }

    左部分木と右部分木の高さが1以下であればAVL木
    return (std::abs(nLeftHeight - nRightHeight) <= 1);
}
*/

// 2分探索木を文字列表現に変換
// CBinarySearchTree::ToString() のヘルパー関数
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::ToStringHelper(
    std::ostringstream& strStream, Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr)
        return;

    strStream << pNode->mKey;

    if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
        return;

    strStream << "(";

    if (pNode->mLeft != nullptr)
        this->ToStringHelper(strStream, pNode->mLeft);

    strStream << ",";

    if (pNode->mRight != nullptr)
        this->ToStringHelper(strStream, pNode->mRight);
    
    strStream << ")";
}

} // namespace Cpp

} // namespace Algorithm1Assignment1

#endif // INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_HPP
