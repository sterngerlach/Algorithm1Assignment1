
# -*- coding: utf-8 -*-
# TreeVisualization.py

import matplotlib.pyplot as plt
import networkx as nx
import sys
import traceback

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

from BinarySearchTree import BinarySearchTree

class MainWindow(QMainWindow):
    """メインウィンドウ"""
    
    def __init__(self):
        super(MainWindow, self).__init__()

        self.mCentralWidget = QWidget()
        self.setCentralWidget(self.mCentralWidget)
        
        self.mVerticalBoxLayout = QVBoxLayout()
        self.mCentralWidget.setLayout(self.mVerticalBoxLayout)

        self.mFont = QFont()
        self.mFont.setFamily("Meiryo UI")
        self.mFont.setPointSize(9)
        self.mFont.setStyle(QFont.StyleNormal)

        self.setFont(self.mFont)
        self.setWindowTitle("Binary Search Tree Visualization using PyQt5, Matplotlib, NetworkX")
        self.resize(QSize(960, 640))

        # 2分探索木の描画領域
        self.mFigure = plt.figure()
        self.mFigureCanvas = FigureCanvas(self.mFigure)
        self.mVerticalBoxLayout.addWidget(self.mFigureCanvas)

        self.mHorizontalLayout = QHBoxLayout()
        self.mVerticalBoxLayout.addLayout(self.mHorizontalLayout)

        # キーの入力用テキストボックス
        self.mInputBoxFont = QFont()
        self.mInputBoxFont.setFamily("Consolas")
        self.mInputBoxFont.setPointSize(9)
        self.mInputBoxFont.setStyle(QFont.StyleNormal)

        self.mInputBox = QLineEdit()
        self.mInputBox.setAlignment(Qt.AlignLeft)
        self.mInputBox.setFont(self.mInputBoxFont)
        self.mInputBox.returnPressed.connect(self.OnInputBoxReturnPressed)
        self.mHorizontalLayout.addWidget(self.mInputBox)

        # 2分探索木の操作用ボタン
        self.mInsertButton = QPushButton("Insert")
        self.mInsertButton.clicked.connect(self.OnInsertButtonClicked)
        self.mHorizontalLayout.addWidget(self.mInsertButton)

        self.mRemoveButton = QPushButton("Remove")
        self.mRemoveButton.clicked.connect(self.OnRemoveButtonClicked)
        self.mHorizontalLayout.addWidget(self.mRemoveButton)

        self.mDestroyButton = QPushButton("Destroy")
        self.mDestroyButton.clicked.connect(self.OnDestroyButtonClicked)
        self.mHorizontalLayout.addWidget(self.mDestroyButton)

        self.mInfoButton = QPushButton("Info")
        self.mInfoButton.clicked.connect(self.OnInfoButtonClicked)
        self.mHorizontalLayout.addWidget(self.mInfoButton)

        # メニューバー
        self.mMenuBar = self.menuBar()
        self.mOperationMenu = self.mMenuBar.addMenu("操作(&O)")

        # メニュー項目
        self.mInsertAction = QAction("ノードの挿入(&I)", self)
        self.mInsertAction.setStatusTip("指定されたキーを持つノードを作成して2分探索木に追加します.")
        self.mInsertAction.triggered.connect(self.OnInsertButtonClicked)

        self.mRemoveAction = QAction("ノードの削除(&R)", self)
        self.mRemoveAction.setStatusTip("指定されたキーを持つノードを2分探索木から削除します.")
        self.mRemoveAction.triggered.connect(self.OnRemoveButtonClicked)

        self.mDestroyAction = QAction("2分探索木の破棄(&D)", self)
        self.mDestroyAction.setStatusTip("2分探索木のノードを全て削除します.")
        self.mDestroyAction.triggered.connect(self.OnDestroyButtonClicked)

        self.mInfoAction = QAction("2分探索木の情報...(&I)", self)
        self.mInfoAction.setStatusTip("2分探索木に関する情報を表示します.")
        self.mInfoAction.triggered.connect(self.OnInfoButtonClicked)

        self.mOperationMenu.addAction(self.mInsertAction)
        self.mOperationMenu.addAction(self.mRemoveAction)
        self.mOperationMenu.addAction(self.mDestroyAction)
        self.mOperationMenu.addAction(self.mInfoAction)

        # ツールバー
        self.addToolBar(NavigationToolbar(self.mFigureCanvas, self))

        # ステータスバー
        self.statusBar()

        # 2分探索木
        self.mBinarySearchTree = BinarySearchTree()

    def OnInputBoxReturnPressed(self):
        self.OnInsertButtonClicked()
        self.mInputBox.clear()

    def OnInsertButtonClicked(self):
        """指定されたキーを持つノードを作成して2分探索木に追加"""
        if not self.mInputBox.text().isdigit():
            QMessageBox.warning(self, "Binary Search Tree Visualization", "キーの形式が不適切です.", QMessageBox.Ok)
            return
        else:
            try:
                self.mBinarySearchTree.Insert(int(self.mInputBox.text()))
            except ValueError as e:
                QMessageBox.warning(self, "Binary Search Tree Visualization", "既にキーが登録されています.", QMessageBox.Ok)
                return

        # グラフの描画
        self.DrawBinarySearchTree()

    def OnRemoveButtonClicked(self):
        """指定されたキーを持つノードを2分探索木から削除"""
        if not self.mInputBox.text().isdigit():
            QMessageBox.warning(self, "Binary Search Tree Visualization", "キー値の形式が不適切です.", QMessageBox.Ok)
            return
        else:
            try:
                self.mBinarySearchTree.Remove(int(self.mInputBox.text()))
            except ValueError as e:
                QMessageBox.warning(self, "Binary Search Tree Visualization", "指定されたキーを持つノードが見つかりません.", QMessageBox.Ok)
                return

        # グラフの描画
        self.DrawBinarySearchTree()

    def OnDestroyButtonClicked(self):
        """2分探索木のノードを全て削除"""
        self.mBinarySearchTree.Destroy()
        self.DrawBinarySearchTree()

    def OnInfoButtonClicked(self):
        """2分探索木に関する情報の表示"""

        if self.mBinarySearchTree.mRoot is None:
            QMessageBox.warning(self, "Binary Search Tree Visualization", "2分探索木が空です.", QMessageBox.Ok)
            return
        else:
            # メッセージ文字列の構築
            infoMsg = "Information about Current Binary Search Tree:\n"
            infoMsg += "String Representation: {0}\n".format(self.mBinarySearchTree.ToString())
            infoMsg += "Node Count: {0}\n".format(self.mBinarySearchTree.NodeCount())
            infoMsg += "Max Node Value: {0}\n".format(self.mBinarySearchTree.MaxNodeValue())
            infoMsg += "Min Node Value: {0}\n".format(self.mBinarySearchTree.MinNodeValue())
            infoMsg += "Height: {0}\n".format(self.mBinarySearchTree.Height())
            infoMsg += "IsAVLTree: {0}".format(self.mBinarySearchTree.IsAVLTree())

            # メッセージの表示
            QMessageBox.information(self, "Binary Search Tree Visualization", infoMsg, QMessageBox.Ok)

    def DrawBinarySearchTree(self):
        """グラフの描画"""

        # 描画領域のクリア
        self.mFigure.clf()

        # グラフの作成
        graph = self.CreateNetworkXGraph()

        # グラフのレイアウトの調整
        pos = self.CalculateGraphLayout()
        
        # グラフの描画
        nx.draw(graph, pos,
                with_labels=True, arrows=False,
                node_color="w", node_shape="s",
                edge_color="k", width=1,
                font_color="b", font_size=16,
                font_weight="normal", font_family="sans-serif")
        self.mFigureCanvas.draw()

    def CreateNetworkXGraph(self):
        """グラフの作成"""

        graph = nx.Graph()

        def CreateNetworkXGraphHelper(graph, parentNode, currentNode):
            if currentNode is None:
                return

            if parentNode is None:
                graph.add_node(currentNode.mKey)

            if currentNode.mLeft is not None:
                graph.add_node(currentNode.mLeft.mKey)
                graph.add_edge(currentNode.mKey, currentNode.mLeft.mKey)
                CreateNetworkXGraphHelper(graph, currentNode, currentNode.mLeft)

            if currentNode.mRight is not None:
                graph.add_node(currentNode.mRight.mKey)
                graph.add_edge(currentNode.mKey, currentNode.mRight.mKey)
                CreateNetworkXGraphHelper(graph, currentNode, currentNode.mRight)

            return

        CreateNetworkXGraphHelper(graph, None, self.mBinarySearchTree.mRoot)
        return graph

    def CalculateGraphLayout(self):
        """グラフのレイアウトの調整"""

        def CalculateGraphLayoutHelper(parentNode, currentNode, left, right, depth, height, pos):
            if currentNode is None:
                return

            if parentNode is None:
                pos[currentNode.mKey] = (0.5, 1.0)
            else:
                pos[currentNode.mKey] = ((left + right) / 2.0, 1.0 - 1.0 / (height + 1) * depth)

            if currentNode.mLeft is not None:
                CalculateGraphLayoutHelper(currentNode, currentNode.mLeft, left, (left + right) / 2.0, depth + 1, height, pos)

            if currentNode.mRight is not None:
                CalculateGraphLayoutHelper(currentNode, currentNode.mRight, (left + right) / 2.0, right, depth + 1, height, pos)

            return

        graphLayout = {}
        CalculateGraphLayoutHelper(None, self.mBinarySearchTree.mRoot, 0.0, 1.0, 0, self.mBinarySearchTree.Height(), graphLayout)
        return graphLayout

def main():
    # Qt5アプリケーションの初期化
    app = QApplication(sys.argv)

    # ウィンドウの作成
    mainWindow = MainWindow()
    mainWindow.show()

    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
