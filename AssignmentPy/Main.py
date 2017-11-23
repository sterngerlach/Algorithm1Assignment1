
# -*- coding: UTF-8 -*-
# Main.py

import matplotlib.pyplot as plt
import numpy as np
import sys
import time

from Utility import GenerateRandomPermutation, CalculateBinarySearchTreeHeightFast

def main(argv=sys.argv[1:]):
    # 1から10までの自然数のランダムな列を100個発生させる
    print("Generating random permutation...")
    print("-" * 50)

    for i in range(100):
        print(GenerateRandomPermutation(10))

    # 2分探索木の高さの平均と分散の計算
    print("Calculating average height and variance of height of binary search trees...")

    # 計測開始
    startTime = time.time()

    # ノード数の最大値
    numOfNodeMax = 14

    # 2分探索木とAVL木の平均と分散
    history = {
        "AverageHeight": [],
        "VarianceOfHeight": [],
        "AverageHeightAVL": [],
        "VarianceOfHeightAVL": []
    }

    # データ数を変えて2分探索木の高さの平均と分散を計算
    for i in range(1, numOfNodeMax + 1):
        (numOfTrees, averageHeightOfTrees, varianceOfHeightOfTrees,
        numOfAVLTrees, averageHeightOfAVLTrees, varianceOfHeightOfAVLTrees) = CalculateBinarySearchTreeHeightFast(i)

        print("-" * 50)
        print("Elapsed Time: {0}".format(time.time() - startTime))
        print("n: {0}, Count: {1}, Average Height: {2}, Variance of Height: {3}"
              .format(i, numOfTrees, averageHeightOfTrees, varianceOfHeightOfTrees))
        print("n: {0}, AVL Tree Count: {1}, Average Height of AVL Tree: {2}, Variance of Height of AVL Tree: {3}"
              .format(i, numOfAVLTrees, averageHeightOfAVLTrees, varianceOfHeightOfAVLTrees))

        # 平均と分散の記録
        history["AverageHeight"].append(averageHeightOfTrees)
        history["VarianceOfHeight"].append(varianceOfHeightOfTrees)
        history["AverageHeightAVL"].append(averageHeightOfAVLTrees)
        history["VarianceOfHeightAVL"].append(varianceOfHeightOfAVLTrees)

    # グラフの作成
    fig = plt.figure()
    
    axisAverageHeight = fig.add_subplot(211)
    axisAverageHeight.set_title("Average Height", fontsize=18)
    axisAverageHeight.set_xlabel("Number of Nodes")
    axisAverageHeight.set_ylabel("Average Height")
    axisAverageHeight.plot(range(1, numOfNodeMax + 1), history["AverageHeight"],
                           label="Binary Search Trees", color="blue")
    axisAverageHeight.plot(range(1, numOfNodeMax + 1), history["AverageHeightAVL"],
                           label="AVL Trees", color="red")
    axisAverageHeight.legend()

    axisVariance = fig.add_subplot(212)
    axisVariance.set_title("Variance of Height", fontsize=18)
    axisVariance.set_xlabel("Number of Nodes")
    axisVariance.set_ylabel("Variance of Height")
    axisVariance.plot(range(1, numOfNodeMax + 1), history["VarianceOfHeight"],
                      label="Binary Search Trees", color="blue")
    axisVariance.plot(range(1, numOfNodeMax + 1), history["VarianceOfHeightAVL"],
                      label="AVL Trees", color="red")
    axisVariance.legend()

    fig.tight_layout()
    
    plt.show()

if __name__ == "__main__":
    main()
