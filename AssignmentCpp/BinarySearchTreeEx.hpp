
// Algorithm1Assignment1
// BinarySearchTreeEx.hpp

#ifndef INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_EX_HPP
#define INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_EX_HPP

#include "BinarySearchTree.hpp"

namespace Algorithm1Assignment1 {

namespace Cpp {

//
// CBinarySearchTreeEx�N���X
// �����̕��ςƕ��U���v�Z���邽�߂ɓ�������2���T����
//

class CBinarySearchTreeEx final : public CBinarySearchTree<int, int> {
public:
    CBinarySearchTreeEx() : mAllocatedNodes(nullptr), mDataCount(0) { }
    ~CBinarySearchTreeEx();

    void SetDataCount(int dataCount);

protected:
    virtual void InsertHelper(const int& key, const int& data, Node<int, int>*& pNode) override;
    virtual void RemoveNode(Node<int, int>*& pNode) override;
    virtual void DestroyHelper(Node<int, int>*& pNode) override;

private:
    Node<int, int>* mAllocatedNodes;
    int mDataCount;
};

} // namespace Cpp

} // namespace Algorithm1Assignment1

#endif // INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_BINARY_SEARCH_TREE_EX_HPP
