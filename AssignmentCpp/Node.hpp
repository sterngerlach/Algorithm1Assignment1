
// Algorithm1Assignment1
// Node.hpp

#ifndef INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_NODE_HPP
#define INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_NODE_HPP

namespace Algorithm1Assignment1 {

namespace Cpp {

//
// Node構造体
// 木のノードの表現
//

template <typename TKey, typename TData>
struct Node {
    Node() = default;
    Node(const TKey& key, const TData& data, Node* pLeft = nullptr, Node* pRight = nullptr) :
        mKey(key), mData(data), mLeft(pLeft), mRight(pRight) { }
    ~Node() = default;

    TKey    mKey;
    TData   mData;
    Node*   mLeft;
    Node*   mRight;
};

} // namespace Cpp

} // namespace Algorithm1Assignment1

#endif // INCLUDE_ALGORITHM1_ASSIGNMENT1_CPP_NODE_HPP
