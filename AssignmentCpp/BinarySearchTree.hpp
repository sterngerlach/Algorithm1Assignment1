
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
// CBinarySearchTree�N���X
// 2���T���؂̕\��
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
// CBinarySearchTree�N���X�̎���
//

// �f�X�g���N�^
template <typename TKey, typename TData>
CBinarySearchTree<TKey, TData>::~CBinarySearchTree()
{
    if (this->mRoot)
        this->Destroy();
}

// �w�肳�ꂽ�L�[�����m�[�h��2���T���؂̒�����T��
template <typename TKey, typename TData>
Node<TKey, TData>* CBinarySearchTree<TKey, TData>::Search(
    const TKey& targetKey) const
{
    return this->SearchHelper(targetKey, this->mRoot);
}

// �w�肳�ꂽ�L�[�ƃf�[�^�����m�[�h���쐬����2���T���؂ɑ}��
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Insert(
    const TKey& key, const TData& data)
{
    this->InsertHelper(key, data, this->mRoot);
}

// �w�肳�ꂽ�L�[�����m�[�h��2���T���؂���폜
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Remove(
    const TKey& key)
{
    this->RemoveHelper(key, this->mRoot);
}

// �㏇�����ɂ��2���T���؂̃m�[�h��S�č폜
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::Destroy()
{
    this->DestroyHelper(this->mRoot);
}

// 2���T���؂̍����̌v�Z
template <typename TKey, typename TData>
int CBinarySearchTree<TKey, TData>::Height() const
{
    return this->HeightHelper(this->mRoot);
}

// 2���T���؂�AVL�؂��ǂ����̔���
template <typename TKey, typename TData>
bool CBinarySearchTree<TKey, TData>::IsAVLTree() const
{
    return this->IsAVLTreeHelper(this->mRoot);
}

// 2���T���؂𕶎���\���ɕϊ�
template <typename TKey, typename TData>
std::string CBinarySearchTree<TKey, TData>::ToString() const
{
    std::ostringstream strStream;
    this->ToStringHelper(strStream, this->mRoot);
    return strStream.str();
}

// �w�肳�ꂽ�L�[�����m�[�h��2���T���؂̒�����T��
// CBinarySearchTree::Search() �̃w���p�[�֐�
template <typename TKey, typename TData>
Node<TKey, TData>* CBinarySearchTree<TKey, TData>::SearchHelper(
    const TKey& targetKey,
    Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr) {
        // �^����ꂽ�L�[�����m�[�h��������Ȃ�����
        return nullptr;
    } else if (targetKey == pNode->mKey) {
        // �^����ꂽ�L�[��, ���݂̃m�[�h�̃L�[���������ꍇ�͌�������
        return pNode;
    } else {
        if (targetKey < pNode->mKey) {
            // �^����ꂽ�L�[��, ���݂̃m�[�h�̃L�[��菬�����ꍇ�͍��̎q�Ɉړ�
            return this->SearchHelper(targetKey, pNode->mLeft);
        } else {
            // �^����ꂽ�L�[��, ���݂̃m�[�h�̃L�[���傫���ꍇ�͉E�̎q�Ɉړ�
            return this->SearchHelper(targetKey, pNode->mRight);
        }
    }

    return nullptr;
}

// �w�肳�ꂽ�L�[�ƃf�[�^�����m�[�h���쐬����2���T���؂ɑ}��
// CBinarySearchTree::Insert() �̃w���p�[�֐�
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::InsertHelper(
    const TKey& key,
    const TData& data,
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // �^����ꂽ�L�[�ƃf�[�^�����m�[�h���쐬
        pNode = new(std::nothrow) Node<TKey, TData>(key, data);
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

// �w�肳�ꂽ�L�[�����m�[�h��2���T���؂���폜
// CBinarySearchTree::Remove() �̃w���p�[�֐�
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveHelper(
    const TKey& key, Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // �폜���ׂ��m�[�h��������Ȃ�����
        assert(false);
    } else if (key != pNode->mKey) {
        if (key < pNode->mKey) {
            this->RemoveHelper(key, pNode->mLeft);
        } else {
            this->RemoveHelper(key, pNode->mRight);
        }
    } else {
        // �폜���ׂ��m�[�h����������
        this->RemoveNode(pNode);
    }
}

// �w�肳�ꂽ�m�[�h��2���T���؂���폜
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveNode(
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr) {
        // �N���蓾�Ȃ�
        assert(false);
    } else if (pNode->mLeft == nullptr && pNode->mRight == nullptr) {
        // ���݂̃m�[�h���t�̏ꍇ�͒P�Ɏ�菜�������ł悢
        delete pNode;
        pNode = nullptr;
    } else if (pNode->mLeft == nullptr) {
        // ���݂̃m�[�h�ɍ����̎q�����Ȃ��ꍇ, �m�[�h���E���̎q�Œu��������
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mRight;
        delete pTmp;
        pTmp = nullptr;
    } else if (pNode->mRight == nullptr) {
        // ���݂̃m�[�h�ɉE���̎q�����Ȃ��ꍇ, �m�[�h�������̎q�Œu��������
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mLeft;
        delete pTmp;
        pTmp = nullptr;
    } else {
        // ���݂̃m�[�h�ɍ��E�����̎q������ꍇ, �m�[�h�������̕����؂̍ő�̗v�f�Œu��������
        // �����̕����؂̒�����ő�̗v�f��T����, �L�[�ƃf�[�^���擾
        TKey maxNodeKey;
        TData maxNodeData;
        this->RemoveMaxNode(pNode->mLeft, maxNodeKey, maxNodeData);

        // �ő�̗v�f�̃L�[�ƃf�[�^��, ���݂̃m�[�h�ɏ㏑��
        pNode->mKey = maxNodeKey;
        pNode->mData = maxNodeData;
    }
}

// �^����ꂽ�m�[�h�����[�g�Ƃ��镔���؂���ő�̗v�f��T��,
// ���̗v�f�̃L�[�ƃf�[�^��ۑ����Ă���, ���̗v�f���g�������̎q�Œu��������
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::RemoveMaxNode(
    Node<TKey, TData>*& pNode, TKey& maxNodeKey, TData& maxNodeData)
{
    if (pNode->mRight == nullptr) {
        // �^����ꂽ�m�[�h�𒸓_�Ƃ��镔���؂���ő�̗v�f����������
        // �ő�̗v�f�������Ă����L�[�ƃf�[�^��ێ�
        maxNodeKey = pNode->mKey;
        maxNodeData = pNode->mData;

        // �ő�̗v�f�����̍����̎q�Œu��������
        Node<TKey, TData>* pTmp = pNode;
        pNode = pNode->mLeft;
        delete pTmp;
        pTmp = nullptr;
    } else {
        // �����؂���ő�̗v�f��T��
        this->RemoveMaxNode(pNode->mRight, maxNodeKey, maxNodeData);
    }
}

// �㏇�����ɂ��2���T���؂̃m�[�h��S�č폜
// CBinarySearchTree::Destroy() �̃w���p�[�֐�
template <typename TKey, typename TData>
void CBinarySearchTree<TKey, TData>::DestroyHelper(
    Node<TKey, TData>*& pNode)
{
    if (pNode == nullptr)
        return;

    // �ŏ��Ɍ��݂̃m�[�h�̍����̎q���폜
    if (pNode->mLeft != nullptr)
        this->DestroyHelper(pNode->mLeft);

    // ���Ɍ��݂̃m�[�h�̉E���̎q���폜
    if (pNode->mRight != nullptr)
        this->DestroyHelper(pNode->mRight);

    // �Ō�Ɏ������폜
    delete pNode;
    pNode = nullptr;
}

// 2���T���؂̍����̌v�Z
// CBinarySearchTree::Height() �̃w���p�[�֐�
template <typename TKey, typename TData>
int CBinarySearchTree<TKey, TData>::HeightHelper(
    Node<TKey, TData>* pNode) const
{
    if (pNode == nullptr)
        return -1;
    else
        return std::max(this->HeightHelper(pNode->mLeft), this->HeightHelper(pNode->mRight)) + 1;
}

// 2���T���؂�AVL�؂��ǂ����̔���
// CBinarySearchTree::IsAVLTree() �̃w���p�[�֐�
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
// 2���T���؂�AVL�؂��ǂ����̔���
// CBinarySearchTree::IsAVLTree() �̃w���p�[�֐�
template <typename TKey, typename TData>
bool CBinarySearchTree<TKey, TData>::IsAVLTreeHelper(
    Node<TKey, TData>* pNode) const
{
    // �t�m�[�h�̏ꍇ�͏��AVL��
    if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
    return true;

    int nLeftHeight = 0;
    int nRightHeight = 0;

    if (pNode->mLeft != nullptr) {
        if (!this->IsAVLTreeHelper(pNode->mLeft)) {
            return false;
        } else {
            // �������؂��\������S�Ẵm�[�h�ɂ���, ���̃m�[�h�𒸓_�Ƃ��镔���؂�AVL�؂ł���ꍇ�ɂ̂�
            // ���݂̃m�[�h�̍������؂̍����𒲂ׂ�
            // CBinarySearchTree::HeightHelper() �͗^����ꂽ�m�[�h�𒸓_�Ƃ��镔���؂̍�����Ԃ��̂�,
            // �����1�𑫂����킹�Č��݂̃m�[�h�𒸓_�Ƃ��镔���؂̍��������߂�
            nLeftHeight = this->HeightHelper(pNode->mLeft) + 1;
        }
    }

    if (pNode->mRight != nullptr) {
        if (!this->IsAVLTreeHelper(pNode->mRight)) {
            return false;
        } else {
            // �E�����؂��\������S�Ẵm�[�h�ɂ���, ���̃m�[�h�𒸓_�Ƃ��镔���؂�AVL�؂ł���ꍇ�ɂ̂�
            // ���݂̃m�[�h�̉E�����؂̍����𒲂ׂ�
            // CBinarySearchTree::HeightHelper() �͗^����ꂽ�m�[�h�𒸓_�Ƃ��镔���؂̍�����Ԃ��̂�,
            // �����1�𑫂����킹�Č��݂̃m�[�h�𒸓_�Ƃ��镔���؂̍��������߂�
            nRightHeight = this->HeightHelper(pNode->mRight) + 1;
        }
    }

    �������؂ƉE�����؂̍�����1�ȉ��ł����AVL��
    return (std::abs(nLeftHeight - nRightHeight) <= 1);
}
*/

// 2���T���؂𕶎���\���ɕϊ�
// CBinarySearchTree::ToString() �̃w���p�[�֐�
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
