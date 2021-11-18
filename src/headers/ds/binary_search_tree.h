// 

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "binary_tree.h"

template <typename T>
class BinarySearchTree : public BinaryTree<T> {
protected:
    function<bool(const T&, const T&)> _cmp; // 二叉搜索树使用的全序关系
public:
    BinarySearchTree(function<bool(const T&, const T&)> _cmp) : _cmp(_cmp) {}
    bool cmp(const T& a, const T& b) { return _cmp(a, b); }
    void insert(T e);
    BTNode<T>* find(T e) const;
};

template <typename T>
BTNode<T>* BinarySearchTree<T>::find(T e) const {
    BTNode<T>* pos = root();            // 从树根开始遍历
    while (pos != nullptr) { 
        if (pos->value == e) { break; } // 查找成功
        if (cmp(pos->value, e)) {       // 根据大小关系决定查找方向
            pos = pos->rc;
        } else {
            pos = pos->lc;
        }
    }
    return pos;
}

template <typename T>
void BinarySearchTree<T>::insert(T e) {
    BTNode<T>* parent = nullptr;
    BTNode<T>*& pos = root();
    while (pos != nullptr) {
        if (cmp(pos->value, e)) { // e >= (pos)，插入在右子树
            pos = (parent = pos)->rc;
        } else {                  // e < (pos)，插入在左子树
            pos = (parent = pos)->lc;
        }
    }
    insert(parent, pos, e); // 调用BT的插入
}

// 已知中序序列，随机生成BST
template <typename T>
BinarySearchTree<T>* randomBST(const Vector<T>& inSeq, function<bool(const T&, const T&)> cmp) {
    auto V = range(0, inSeq.size());
    V.shuffle();                 // 随机置乱秩，得到一个插入次序
    BinarySearchTree<T>* BST = new BinarySearchTree<T>(cmp);
    for (Rank r : V) {
        BST->insert(inSeq[r]);   // 按照V的次序进行插入
    }
    return BST;
}

#endif