
# -*- coding: utf-8 -*-
# BinarySearchTree.py

from Node import Node

class BinarySearchTree():
    """2分探索木の表現"""

    def __init__(self, root=None):
        """コンストラクタ"""
        if root is not None and not isinstance(root, Node):
            raise TypeError("Node type is required")

        self.mRoot = root

    def __del__(self):
        """デストラクタ"""
        self.Destroy()

    def __str__(self):
        """2分探索木を文字列表現に変換"""
        return self.ToString()

    def __contains__(self, item):
        """2分探索木が指定されたキーを持つノードを含んでいるかどうか"""
        return self.Search(item) is not None

    def Search(self, key):
        """指定されたキーを持つノードを2分探索木の中から探索"""
        return self.SearchHelper(key, self.mRoot)

    def Insert(self, key, data=None):
        """指定されたキーとデータを持つノードを作成して2分探索木に挿入"""
        if isinstance(key, (list, tuple)):
            for item in key:
                self.Insert(item, None)
        else:
            if self.mRoot is None:
                # ルートがない場合はここで作成
                self.mRoot = Node(key, data, None, None, None)
            else:
                # 頂点を持つ場合は適切な場所を探して追加
                self.InsertHelper(key, data, self.mRoot)

    def Remove(self, key):
        """指定されたキーを持つノードを2分探索木から削除"""
        self.RemoveHelper(key, self.mRoot)
    
    def Destroy(self):
        """2分探索木のノードを全て削除"""
        self.DestroyHelper(self.mRoot)

    def Height(self):
        """2分探索木の高さの計算"""
        return self.HeightHelper(self.mRoot)

    def IsAVLTree(self):
        """2分探索木がAVL木かどうかの判定"""
        return self.IsAVLTreeHelper(self.mRoot)

    def ToString(self):
        """2分探索木を文字列表現に変換"""
        return self.ToStringHelper(self.mRoot)

    def NodeCount(self):
        """2分探索木の持つノードの数を取得"""
        return self.NodeCountHelper(self.mRoot)

    def MaxNodeValue(self):
        """2分探索木の最大のキー値を取得"""
        return self.MaxNodeValueHelper(self.mRoot)

    def MinNodeValue(self):
        """2分探索木の最小のキー値を取得"""
        return self.MinNodeValueHelper(self.mRoot)

    def SearchHelper(self, key, node):
        """指定されたキーを持つノードを2分探索木の中から探索
        BinarySearchTree::Search() のヘルパーメソッド"""
        if node is None:
            # 与えられたキーをもつノードが見つからなかった
            return None
        elif key == node.mKey:
            # 与えられたキーと, 現在のノードのキーが等しい場合は見つかった
            return node
        else:
            if key < node.mKey:
                # 与えられたキーが, 現在のノードのキーより小さい場合は左の子に移動
                return self.SearchHelper(key, node.mLeft)
            else:
                # 与えられたキーが, 現在のノードのキーより大きい場合は右の子に移動
                return self.SearchHelper(key, node.mRight)

    def InsertHelper(self, key, data, node):
        """指定されたキーとデータを持つノードを作成して2分探索木に挿入
        BinarySearchTree::Insert() のヘルパーメソッド"""
        if node is None:
            # 起こり得ない
            assert False
        elif key == node.mKey:
            # 二重登録
            raise ValueError("Key '{0}' already registered".format(key))
        else:
            if key < node.mKey:
                if node.HasLeftChild():
                    self.InsertHelper(key, data, node.mLeft)
                else:
                    # 与えられたキーとデータを持つノードを作成
                    node.mLeft = Node(key, data, None, None, node)
            else:
                if node.HasRightChild():
                    self.InsertHelper(key, data, node.mRight)
                else:
                    # 与えられたキーとデータを持つノードを作成
                    node.mRight = Node(key, data, None, None, node)

    def RemoveHelper(self, key, node):
        """指定されたキーを持つノードを2分探索木から削除
        BinarySearchTree::Remove() のヘルパーメソッド"""
        if node is None:
            # 削除すべきノードが見つからなかった
            raise ValueError("Key '{0}' not found".format(key))
        elif key != node.mKey:
            if key < node.mKey:
                # 左の子を探す
                self.RemoveHelper(key, node.mLeft)
            else:
                # 右の子を探す
                self.RemoveHelper(key, node.mRight)
        else:
            # 削除すべきノードが見つかった
            self.RemoveNode(node)

    def RemoveNode(self, node):
        """指定されたノードを2分探索木から削除"""
        if node is None:
            # 起こり得ない
            assert False
        elif node.IsLeafNode():
            # 現在のノードが葉の場合は単に取り除くだけでよい
            if node.IsRootNode():
                # 2分探索木がルートしか持たない場合
                self.mRoot = None
            elif node.IsLeftChild():
                # 自身を親から参照する必要があることに注意
                node.mParent.mLeft = None
            else:
                node.mParent.mRight = None
        elif not node.HasLeftChild():
            # 現在のノードに左側の子がいない場合, ノードを右側の子で置き換える
            if node.IsRootNode():
                self.mRoot.mRight.mParent = None
                self.mRoot = self.mRoot.mRight
            elif node.IsLeftChild():
                node.mRight.mParent = node.mParent
                node.mParent.mLeft = node.mRight
            else:
                node.mRight.mParent = node.mParent
                node.mParent.mRight = node.mRight
        elif not node.HasRightChild():
            # 現在のノードに右側の子がいない場合, ノードを左側の子で置き換える
            if node.IsRootNode():
                self.mRoot.mLeft.mParent = None
                self.mRoot = self.mRoot.mLeft
            elif node.IsLeftChild():
                node.mLeft.mParent = node.mParent
                node.mParent.mLeft = node.mLeft
            else:
                node.mLeft.mParent = node.mParent
                node.mParent.mRight = node.mLeft
        else:
            # 現在のノードに左右両方の子がいる場合, ノードを左側の部分木の最大の要素で置き換える
            # 左側の部分木の最大の要素を探す
            maxNode = node.mLeft

            while maxNode.HasRightChild():
                maxNode = maxNode.mRight

            # 左側の部分木の最大の要素が持っていたキーとデータを, 現在のノードに上書き
            node.mKey = maxNode.mKey
            node.mData = maxNode.mData

            # 左側の部分木の最大の要素をその左側の子で置き換える
            if maxNode.HasLeftChild():
                maxNode.mLeft.mParent = maxNode.mParent

            if maxNode.IsLeftChild():
                maxNode.mParent.mLeft = maxNode.mLeft
            else:
                maxNode.mParent.mRight = maxNode.mLeft

    def DestroyHelper(self, node):
        """2分探索木のノードを全て削除
        BinarySearchTree::Destroy() のヘルパーメソッド"""
        if node is None:
            return

        # 最初に現在のノードの左側の子を削除
        if node.HasLeftChild():
            self.DestroyHelper(node.mLeft)

        # 次に現在のノードの右側の子を削除
        if node.HasRightChild():
            self.DestroyHelper(node.mRight)

        # 最後に自分を削除
        if node.IsRootNode():
            self.mRoot = None
        elif node.IsLeftChild():
            node.mParent.mLeft = None
        else:
            node.mParent.mRight = None

    def HeightHelper(self, node):
        """2分探索木の高さの計算
        BinarySearchTree::Height() のヘルパーメソッド"""
        if node is None:
            return -1
        else:
            return max(self.HeightHelper(node.mLeft), self.HeightHelper(node.mRight)) + 1

    def IsAVLTreeHelper(self, node):
        """2分探索木がAVL木かどうかの判定
        BinarySearchTree::IsAVLTree() のヘルパーメソッド"""

        # 葉ノードの場合は常にAVL木
        if node.IsLeafNode():
            return True

        # 左部分木と右部分木の高さ
        leftTreeHeight = 0
        rightTreeHeight = 0

        if node.HasLeftChild():
            if not self.IsAVLTreeHelper(node.mLeft):
                return False
            else:
                # 左部分木を構成する全てのノードについて, そのノードを頂点とする部分木がAVL木である場合にのみ
                # 現在のノードの左部分木の高さを調べる
                # BinarySearchTree::HeightHelper() は与えられたノードを頂点とする部分木の高さを返すので,
                # これに1を足し合わせて現在のノードを頂点とする部分木の高さを求める
                leftTreeHeight = self.HeightHelper(node.mLeft) + 1

        if node.HasRightChild():
            if not self.IsAVLTreeHelper(node.mRight):
                return False
            else:
                # 右部分木を構成する全てのノードについて, そのノードを頂点とする部分木がAVL木である場合にのみ
                # 現在のノードの右部分木の高さを調べる
                # BinarySearchTree::HeightHelper() は与えられたノードを頂点とする部分木の高さを返すので,
                # これに1を足し合わせて現在のノードを頂点とする部分木の高さを求める
                rightTreeHeight = self.HeightHelper(node.mRight) + 1

        return abs(leftTreeHeight - rightTreeHeight) <= 1

    def ToStringHelper(self, node):
        """2分探索木を文字列表現に変換
        BinarySearchTree::ToString() のヘルパーメソッド"""
        
        if node is None:
            return ""

        # 2分探索木の文字列表現
        strExpression = str(node.mKey)

        if node.IsLeafNode():
            return strExpression

        strExpression += "("

        if node.HasLeftChild():
            strExpression += self.ToStringHelper(node.mLeft)

        strExpression += ","

        if node.HasRightChild():
            strExpression += self.ToStringHelper(node.mRight)

        strExpression += ")"

        return strExpression

    def NodeCountHelper(self, node):
        """2分探索木の持つノードの数を取得
        BinarySearchTree::NodeCount() のヘルパーメソッド"""

        if node is None:
            return 0
        elif node.HasBothChildren():
            return 1 + self.NodeCountHelper(node.mLeft) + self.NodeCountHelper(node.mRight)
        elif node.HasLeftChild():
            return 1 + self.NodeCountHelper(node.mLeft)
        elif node.HasRightChild():
            return 1 + self.NodeCountHelper(node.mRight)
        else:
            return 1

    def MaxNodeValueHelper(self, node):
        """2分探索木の最大のキー値を取得
        BinarySearchTree::MaxNodeValue() のヘルパーメソッド"""

        if node.HasRightChild():
            return self.MaxNodeValueHelper(node.mRight)
        else:
            return node.mKey

    def MinNodeValueHelper(self, node):
        """2分探索木の最小のキー値を取得
        BinarySearchTree::MinNodeValue() のヘルパーメソッド"""

        if node.HasLeftChild():
            return self.MinNodeValueHelper(node.mLeft)
        else:
            return node.mKey
