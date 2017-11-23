
# -*- coding: utf-8 -*-
# Node.py

class Node():
    """木のノードの表現"""

    def __init__(self, key, data=None, left=None, right=None):
        """コンストラクタ"""
        if left is not None:
            if not isinstance(left, Node):
                raise TypeError("Node type is required")

        if right is not None:
            if not isinstance(right, Node):
                raise TypeError("Node type is required")

        self.mKey = key
        self.mData = data
        self.mLeft = left
        self.mRight = right
