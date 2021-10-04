// 4.2 - 二叉树 的代码
// 实际上是二叉树数据结构的模板
// 同时也是后面的二叉树的基类

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <functional>
using namespace std;

template <typename T>
struct BTNode {        // 二叉树的一个数据单元（节点）
    T value;           // 本节点存放的数据
    BTNode<T>* parent; // 指向本节点的亲节点
    BTNode<T>* lc;     // 指向本节点的左子代
    BTNode<T>* rc;     // 指向本节点的右子代

    // 判断是否是树根、左子代、右子代
    bool isRoot() const { return parent == nullptr; }
    bool isLeftChild() const { return !isRoot() && parent->lc == this; }
    bool isRightChild() const { return !isRoot() && parent->rc == this; }

    BTNode<T>(T e): value(e), parent(nullptr), lc(nullptr), rc(nullptr) {}
};

template <typename T>
class BinaryTree {
protected:
    int _size;         // 二叉树的规模
    BTNode<T>* _root;  // 指向二叉树的根节点
public:
    int size() const { return _size; }
    BTNode<T>* root() const { return _root; }  

    // -----------------------------------以下开始是示例代码的函数---------------------------
    BinaryTree(): _size(0), _root(nullptr) {} // 构造函数，生成空树

    void removeSubtree(BTNode<T>* node); // 删除子树
    void insertAsLeftChild(BTNode<T>* p, T e); // 插入节点（左子）
    void insertAsRightChild(BTNode<T>* p, T e); // 插入节点（右子）
    void insertAsRoot(T e); // 插入节点（根）
    void preorderTraverse(function<void(const BTNode<T>*&)> visit) const; // 先序遍历
    void inorderTraverse(function<void(const BTNode<T>*&)> visit) const; // 中序遍历
    void postorderTraverse(function<void(const BTNode<T>*&)> visit) const; // 后序遍历

    // -----------------------------------以下内容不在笔记正文中----------------------------
    BinaryTree(BTNode<T>* _root) : _root(_root) {} // 用于生成子树
    void updateSize();
    bool operator==(const BinaryTree<T>& other) const;
};

// 算法4.1 - 删除子树
template <typename T>
void BinaryTree<T>::removeSubtree(BTNode<T>* node) {
    function<void(BTNode<T>*)> remove;
    remove = [&_size](BTNode<T>* node) -> void {
        if (node == nullptr) { // 递归边界：空树
            return;
        }
        auto lc = node->lc, rc = node->rc;
        --_size;               // 每删除一个节点，更新一次二叉树规模
        delete node;           // 释放当前节点的空间
        remove(lc);            // 递归删除左右子树（尾递归）
        remove(rc);
    };
    if (node->isRoot()) {      // 首先把子树从整棵二叉树上分离出来
        _root = nullptr;
    } else {
        if (node->isLeftChild()) {
            node->parent->lc = nullptr;
        } else {
            node->parent->rc = nullptr;
        }
    }
    remove(node);              // 递归删除子树
}

// 算法4.2 - 插入节点（左子）
template <typename T>
void BinaryTree<T>::insertAsLeftChild(BTNode<T>* p, T e) {
    removeSubtree(p->lc);                    // 删除原位置的子树
    (p->lc = new BTNode<T>(e))->parent = p;  // 插入新的叶子
    ++_size;                                 // 更新规模
}

// 插入节点（右子）（正文中未给出）
template <typename T>
void BinaryTree<T>::insertAsRightChild(BTNode<T>* p, T e) {
    removeSubtree(p->rc);                    // 删除原位置的子树
    (p->rc = new BTNode<T>(e))->parent = p;  // 插入新的叶子
    ++_size;                                 // 更新规模
}

// 插入节点（根）（正文中未给出）
template <typename T>
void BinaryTree<T>::insertAsRoot(T e) {
    removeSubtree(_root);
    _root = new BTNode<T>(e);
    ++_size;
}

// 算法4.3A - 先序遍历
template <typename T>
void BinaryTree<T>::preorderTraverse(function<void(const BTNode<T>*&)> visit) const {
    function<void(const BTNode<T>*&)> traverse;
    traverse = [&visit](const BTNode<T>*& node) -> void {
        if (node == nullptr) {  // 递归边界：空树
            return;
        }
        visit(node);            // 访问树根
        traverse(node->lc);     // 递归遍历左子树
        traverse(node->rc);     // 递归遍历右子树
    };
    traverse(_root);            // 从整棵树的树根开始遍历
}

// 算法4.3B - 中序遍历
template <typename T>
void BinaryTree<T>::inorderTraverse(function<void(const BTNode<T>*&)> visit) const {
    function<void(const BTNode<T>*&)> traverse;
    traverse = [&visit](const BTNode<T>*& node) -> void {
        if (node == nullptr) {  // 递归边界：空树
            return;
        }
        traverse(node->lc);     // 递归遍历左子树
        visit(node);            // 访问树根
        traverse(node->rc);     // 递归遍历右子树
    };
    traverse(_root);            // 从整棵树的树根开始遍历
}

// 算法4.3C - 后序遍历
template <typename T>
void BinaryTree<T>::postorderTraverse(function<void(const BTNode<T>*&)> visit) const {
    function<void(const BTNode<T>*&)> traverse;
    traverse = [&visit](const BTNode<T>*& node) -> void {
        if (node == nullptr) {  // 递归边界：空树
            return;
        }
        traverse(node->lc);     // 递归遍历左子树
        traverse(node->rc);     // 递归遍历右子树
        visit(node);            // 访问树根
    };
    traverse(_root);            // 从整棵树的树根开始遍历
}

// -------------------------------以下内容不在笔记正文中------------------------------------------------
// 重新计算_size的值，用于将子树新建出来之后计算规模
template <typename T>
void BinaryTree<T>::updateSize() {
    _size = 0;
    preorderTraverse([&](const BTNode<T>*&) -> void {
        ++_size;                // 任意使用一种遍历方式即可
    });
}

template <typename T>
bool BinaryTree<T>::operator==(const BinaryTree<T>& other) const {
    function<bool(const BTNode<T>*&, const BTNode<T>*&)> compareSubtree;
    compareSubtree = [](const BTNode<T>*& n1, const BTNode<T>*& n2) -> bool {
        if (n1 != nullptr && n2 != nullptr) {
            return n1->value == n2->value && compareSubtree(n1->lc, n2->lc) && compareSubtree(n1->rc, n2->rc);
        }
        return n1 == nullptr && n2 == nullptr;
    };
    return compareSubtree(_root, other->_root);
}


#endif