// 

#ifndef TREE_H
#define TREE_H

#include "vector.h"
#include "binary_tree.h"
using namespace std;

template <typename T>
struct TreeNode {        // 树的一个数据单元（节点）
    T value;             // 本节点存放的数据
    TreeNode<T>* parent; // 指向本节点的亲节点
    Vector<TreeNode<T>*> children; // 指向本节点的子节点序列（向量，也可以使用列表）

    TreeNode<T>(T e): value(e), parent(nullptr) {}
};

template <typename T>
class Tree {
protected:
    TreeNode<T>* _root;  // 指向树的根节点
public:
    TreeNode<T>*& root() const { return _root; }  

    // -----------------------------------以下开始是示例代码的函数---------------------------
    Tree(): _root(nullptr) {} // 构造函数，生成空树
    static BTNode<T>* toBinaryTree(const TreeNode<T>*& root); // 第一子代+下一姊妹，树转二叉树
    static TreeNode<T>* fromBinaryTree(const BTNode<T>*& root); // 第一子代+下一姊妹，二叉树转树
    static void preorderTraverse(function<void(TreeNode<T>*)> visit, TreeNode<T>* node); // 先根遍历
    static void postorderTraverse(function<void(TreeNode<T>*)> visit, TreeNode<T>* node); // 后根遍历
};

// 算法4.8A - 树->二叉树
template <typename T>
BTNode<T>* Tree<T>::toBinaryTree(const TreeNode<T>*& root) {
    if (root == nullptr) {       // 递归边界：空树对应空树
        return nullptr;
    }
    auto broot = new BTNode<T>(root->value); // 复制节点，创建树根
    BTNode<T>* p = broot;        // 下面开始逐一插入树上root的子节点
    BTNode<T>*& pos = broot->lc; // 即将被插入的节点位置
    root->children.traverse([&](Rank, const TreeNode<T>*& child) -> void {
        BinaryTree<T>::insertSubtree(p, pos, toBinaryTree(child)); // 递归插入子树
        pos = (p = pos)->rc;     // 下一姊妹会成为当前节点的右子代
    });
    return broot;
}

// 算法4.8B - 二叉树->树
template <typename T>
TreeNode<T>* Tree<T>::fromBinaryTree(const BTNode<T>*& root) {
    if (root == nullptr) { // 递归边界：空树对应空树
        return nullptr;
    }
    auto troot = new TreeNode<T>(root->value); // 复制节点，创建树根
    for (auto c = root->lc; c != nullptr; c = c->rc) { // 遍历root->lc的最右侧通路
        auto subtree = fromBinaryTree(c);      // 递归生成子树
        subtree->parent = troot;
        troot->children.push_back(subtree);
    }
    return troot;
}

// 算法4.9A - 先根遍历
template <typename T>
void Tree<T>::preorderTraverse(function<void(TreeNode<T>*)> visit, TreeNode<T>* node) {
    if (node == nullptr) { // 递归边界：空树
        return;
    }
    visit(node);
    node->children.traverse([&](Rank, const TreeNode<T>*& child) -> void {
        preorderTraverse(visit, child);
    });
}

// 算法4.9B - 后根遍历
template <typename T>
void Tree<T>::preorderTraverse(function<void(TreeNode<T>*)> visit, TreeNode<T>* node) {
    if (node == nullptr) { // 递归边界：空树
        return;
    }
    node->children.traverse([&](Rank, const TreeNode<T>*& child) -> void {
        preorderTraverse(visit, child);
    });
    visit(node);
}

#endif