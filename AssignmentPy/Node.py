
# -*- coding: utf-8 -*-
# Node.py

class Node():
    """木のノードの表現"""

    def __init__(self, key, data=None, left=None, right=None, parent=None):
        """コンストラクタ"""
        if left is not None:
            if not isinstance(left, Node):
                raise TypeError("Node type is required")

        if right is not None:
            if not isinstance(right, Node):
                raise TypeError("Node type is required")

        if parent is not None:
            if not isinstance(parent, Node):
                raise TypeError("Node type is required")

        self.mKey = key
        self.mData = data
        self.mLeft = left
        self.mRight = right
        self.mParent = parent

    def HasLeftChild(self):
        """左の子を持つかどうか"""
        return self.mLeft is not None

    def HasRightChild(self):
        """右の子を持つかどうか"""
        return self.mRight is not None

    def HasBothChildren(self):
        """左右の子を両方持つかどうか"""
        return self.mLeft is not None and self.mRight is not None

    def IsLeftChild(self):
        """自身が親の左の子かどうか"""
        return self.mParent is not None and self.mParent.mLeft is self

    def IsRightChild(self):
        """自身が親の右の子かどうか"""
        return self.mParent is not None and self.mParent.mRight is self

    def IsRootNode(self):
        """自身が根ノードかどうか"""
        return self.mParent is None

    def IsLeafNode(self):
        """自身が葉ノードかどうか"""
        return self.mLeft is None and self.mRight is None
