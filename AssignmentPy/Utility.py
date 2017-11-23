
# -*- coding: utf-8 -*-
# Utility.py

import numpy as np
from BinarySearchTree import BinarySearchTree

def GenerateRandomPermutation(n):
    """ランダムな1からnまでの自然数の順列を生成"""
    if not isinstance(n, int):
        raise TypeError("int type is required")

    # 1からnまでが順に並んだ順列を生成
    randomPermutation = np.array(range(1, n + 1))

    # 生成した順列をシャッフル
    np.random.shuffle(randomPermutation)

    return randomPermutation

def GenerateDataSequences(dataCount):
    """2分探索木を作るためのデータ列を生成"""

    def GenerateDataSequencesHelper(start, end, dataSequences):
        """2分探索木を作るためのデータ列を生成
        GenerateDataSequences() のヘルパー関数"""
    
        if start > end:
            # 空のデータ列を追加
            dataSequences.append([])
            return

        for i in range(start, end + 1):
            leftTrees = []
            GenerateDataSequencesHelper(start, i - 1, leftTrees)

            rightTrees = []
            GenerateDataSequencesHelper(i + 1, end, rightTrees)

            for leftTree in leftTrees:
                for rightTree in rightTrees:
                    generatedTree = []
                    generatedTree.append(i)
                    generatedTree.extend(leftTree)
                    generatedTree.extend(rightTree)
                    dataSequences.append(generatedTree)

        return

    dataSequences = []
    GenerateDataSequencesHelper(1, dataCount, dataSequences)

    # dataCountが偶数の場合は後半のデータ列を捨てる
    if dataCount % 2 == 0:
        dataSequences = dataSequences[:int(len(dataSequences) / 2)]

    return dataSequences

def CalculateBinarySearchTreeHeight(dataCount):
    """指定されたノード数の相異なる2分探索木の高さの平均と分散の計算"""
    
    # 2分探索木の高さの平均と分散を計算するためのプログラム

    # ノード数がnの相異なる2分木の個数はカタラン数Cnに一致するので,
    # 生成する2分探索木の形状のパターン数もカタラン数Cnに一致する
    # 同じ形状(高さ)の2分探索木を複数回処理しないために, 個々の形状が1回ずつ現れるような入力データ列を生成する
    
    # 入力データ列の生成
    dataSequences = GenerateDataSequences(dataCount)

    # 2分探索木
    binarySearchTree = BinarySearchTree()

    # 高さと平均のデータ
    numOfBinarySearchTree = 0
    numOfAVLTree = 0

    # 個々の形状の2分探索木とAVL木の高さ
    heightOfBinarySearchTree = []
    heightOfAVLTree = []

    # 個々の形状の2分探索木を1つずつ調べる
    for dataSequence in dataSequences:
        # データを2分探索木に挿入
        binarySearchTree.Insert(dataSequence)

        treeHeight = binarySearchTree.Height()
        heightOfBinarySearchTree.append(treeHeight)
        numOfBinarySearchTree += 1

        if binarySearchTree.IsAVLTree():
            heightOfAVLTree.append(treeHeight)
            numOfAVLTree += 1

        # 2分探索木の持つ全てのノードを破棄
        binarySearchTree.Destroy()

    # 2分探索木の高さの平均と分散
    averageHeightOfBinarySearchTree = np.average(heightOfBinarySearchTree)
    varianceOfHeightOfBinarySearchTree = np.var(heightOfBinarySearchTree)

    # AVL木の高さの平均と分散
    averageHeightOfAVLTree = np.average(heightOfAVLTree)
    varianceOfHeightOfAVLTree = np.var(heightOfAVLTree)

    return (numOfBinarySearchTree,
            averageHeightOfBinarySearchTree,
            varianceOfHeightOfBinarySearchTree,
            numOfAVLTree,
            averageHeightOfAVLTree,
            varianceOfHeightOfAVLTree)

def CalculateBinarySearchTreeHeightFast(dataCount):
    """指定されたノード数の相異なる2分探索木の高さの平均と分散の計算"""

    # 入力データ列を返却するのではなく完成と同時に高さを計算する
    # 2分探索木
    binarySearchTree = BinarySearchTree()

    # 高さと平均のデータ
    numOfBinarySearchTree = 0
    numOfAVLTree = 0

    # 個々の形状の2分探索木とAVL木の高さ
    heightOfBinarySearchTree = []
    heightOfAVLTree = []

    def GenerateDataSequencesHelper(start, end, dataSequences):
        """2分探索木を作るためのデータ列を生成して同時に高さを計算"""
        nonlocal binarySearchTree
        nonlocal numOfBinarySearchTree
        nonlocal numOfAVLTree
        nonlocal heightOfBinarySearchTree
        nonlocal heightOfAVLTree

        if start > end:
            # 空のデータ列を追加
            dataSequences.append([])
            return

        for i in range(start, end + 1):
            leftTrees = []
            GenerateDataSequencesHelper(start, i - 1, leftTrees)

            rightTrees = []
            GenerateDataSequencesHelper(i + 1, end, rightTrees)

            for leftTree in leftTrees:
                for rightTree in rightTrees:
                    if (len(leftTree) + len(rightTree) + 1) == dataCount:
                        # 入力データ列が完成
                        # dataCountが偶数の場合は入力データの後半を無視できる
                        if (dataCount % 2 == 0) and (i > dataCount / 2):
                            return

                        # データを2分探索木に挿入
                        binarySearchTree.Insert(i)
                        binarySearchTree.Insert(leftTree)
                        binarySearchTree.Insert(rightTree)

                        treeHeight = binarySearchTree.Height()
                        heightOfBinarySearchTree.append(treeHeight)
                        numOfBinarySearchTree += 1

                        if binarySearchTree.IsAVLTree():
                            heightOfAVLTree.append(treeHeight)
                            numOfAVLTree += 1

                        # 2分探索木の持つ全てのノードを破棄
                        binarySearchTree.Destroy()
                    else:
                        generatedTree = []
                        generatedTree.append(i)
                        generatedTree.extend(leftTree)
                        generatedTree.extend(rightTree)
                        dataSequences.append(generatedTree)

        return

    GenerateDataSequencesHelper(1, dataCount, None)

    # 2分探索木の高さの平均と分散
    averageHeightOfBinarySearchTree = np.average(heightOfBinarySearchTree)
    varianceOfHeightOfBinarySearchTree = np.var(heightOfBinarySearchTree)

    # AVL木の高さの平均と分散
    averageHeightOfAVLTree = np.average(heightOfAVLTree)
    varianceOfHeightOfAVLTree = np.var(heightOfAVLTree)

    return (numOfBinarySearchTree,
            averageHeightOfBinarySearchTree,
            varianceOfHeightOfBinarySearchTree,
            numOfAVLTree,
            averageHeightOfAVLTree,
            varianceOfHeightOfAVLTree)
