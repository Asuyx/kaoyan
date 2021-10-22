// 4.2 - 二叉树 的代码
// 实际上是二叉树数据结构的模板
// 同时也是后面的二叉树的基类

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <functional>
#include "stack.h"
#include "queue.h"
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
    void levelTraverse(function<void(const BTNode<T>*&)> visit) const; // 层次遍历

protected:
    void preorderTraverseWithStack(function<void(const BTNode<T>*&)> visit) const; // 先序遍历（无递归）
    void preorderTraverseWithStackAndStructure(function<void(const BTNode<T>*&)> visit) const; // 先序遍历（无递归，优化）
    void inorderTraverseWithStackAndStructure(function<void(const BTNode<T>*&)> visit) const; // 中序遍历（无递归，优化）
    BTNode<T>* preorderSucc(BTNode<T>* p) const; // 先序序列的直接后继
    void preorderTraverseWithSucc(function<void(const BTNode<T>*&)> visit) const; // 使用直接后继做先序遍历

    // -----------------------------------以下内容不在笔记正文中----------------------------
public:
    BinaryTree(BTNode<T>* _root) : _root(_root) {} // 生成指定根的树
    BinaryTree(int _size, BTNode<T>* _root) : _size(_size), _root(_root) {} // 指定规模和根的树
    BinaryTree(const BinaryTree<T>& T); // 复制构造函数
    void updateSize();
    bool operator==(const BinaryTree<T>& other) const;
    template <typename T1> friend ostream& operator<< (ostream& out, const BinaryTree<T1>& T); // 使用cout方式打印
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

// 算法4.3AS - 先序遍历（使用栈）
template <typename T>
void BinaryTree<T>::preorderTraverseWithStack(function<void(const BTNode<T>*&)> visit) const {
    Stack<BTNode<T>*&> S;
    S.push(_root);            // 从树根开始遍历
    while (!S.empty()) {
        auto t = S.pop();     // 取出栈顶元素
        if (t != nullptr) {   // 如果非空
            visit(t);         // 则对其进行访问
            S.push(t->rc);    // 并将其左右子代依次入栈
            S.push(t->lc);    // 注意顺序是反的
        }
    }
}

// 算法4.3AS2 - 先序遍历（使用栈，优化）
template <typename T>
void BinaryTree<T>::preorderTraverseWithStackAndStructure(function<void(const BTNode<T>*&)> visit) const {
    Stack<BTNode<T>*&> S;
    S.push(_root);                                         // 从树根开始遍历
    while (!S.empty()) {                                   
        for (auto t = S.pop(); t != nullptr; t = t->lc) {  // 对每个节点，沿着左子代到底
            visit(t);                                      // 访问沿途经过的节点
            S.push(t->rc);                                 // 并把右子代暂存到栈里
        }                                                  // 这样回溯时会从深到浅访问右姊妹
    }
}

// 算法4.3BS - 中序遍历（使用栈）
template <typename T>
void BinaryTree<T>::inorderTraverseWithStackAndStructure(function<void(const BTNode<T>*&)> visit) const {
    Stack<BTNode<T>*&> S;
    for (auto t = _root; t != nullptr; t = t->lc) {
        S.push(t);                                         // 树根开始沿着左子代到底先入栈
    }
    while (!S.empty()) {                                   // 开始回溯
        auto t = S.pop();                                  // 回溯到路径上的节点t
        visit(t);                                          // 访问回溯到的节点
        for (t = t->rc; t != nullptr; t = t->lc) {         // 然后转向右子代
            S.push(t);                                     // 并再次沿左子代到底
        }
    }
}

// 算法 - 先序遍历后继
template<typename T>
BTNode<T>* BinaryTree<T>::preorderSucc(BTNode<T>* p) const {
    if (p->lc != nullptr) { return p->lc; } // 有左子取左子
    if (p->rc != nullptr) { return p->rc; } // 无左有右取右子
    while (p->parent != nullptr) {
        if (p->isLeftChild() && p->parent->rc != nullptr) {
            return p->parent->rc;           // 有右姊妹则取右姊妹
        } else {
            p = p->parent;                  // 无右姊妹继续向上回溯
        }
    }
    return nullptr;                         // 回溯到树根仍然没有右姊妹，结束
}

// 算法4.3A2 - 先序遍历（使用后继方法）
template <typename T>
void BinaryTree<T>::preorderTraverseWithSucc(function<void(const BTNode<T>*&)> visit) const {
    for (auto p = _root; p != nullptr; p = preorderSucc(p)) {
        visit(p);
    }
}

// 算法 - 层次遍历
template <typename T>
void BinaryTree<T>::levelTraverse(function<void(const BTNode<T>*&)> visit) const {
    Queue<BTNode<T>*&> Q;
    Q.enqueue(_root);         // 从树根开始遍历
    while (!Q.empty()) {
        auto f = Q.dequeue(); // 取出队列中的队首元素
        if (f != nullptr) {   // 如果非空
            visit(f);         // 则对其进行访问
            Q.enqueue(f->lc); // 并将其左右子代依次入队
            Q.enqueue(f->rc);
        }
    }
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

// 复制构造函数（深复制）
template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& T) {
    _size = T.size();
    function<BTNode<T>*(const BTNode<T>*)> copySubTree;  // 递归复制每一个子树
    copySubTree = [](const BTNode<T>* r) -> BTNode<T>* {
        if (r == nullptr) {
            return nullptr;
        }
        auto node = new BTNode<T>(r->value);
        node->lc = copySubTree(r->lc);
        node->rc = copySubTree(r->rc);
        if (node->lc != nullptr) {
            node->lc->parent = node; 
        }
        if (node->rc != nullptr) {
            node->rc->parent = node;
        }
        return node;
    };
    _root = copySubTree(T.root());
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

// 在示例代码中，输出使用层次遍历
// 为了可以复原，补全了外部节点
template <typename T>
ostream& operator<< (ostream& out, const BinaryTree<T>& T)
{
    out << "T(";
    if (T.size() > 0) {
        out << T.root()->value;
        T.levelTraverse([&](const BTNode<T>*& node) -> void {
            out << "," << (node->lc != nullptr ? node->lc->value : '^');
            out << "," << (node->rc != nullptr ? node->rc->value : '^');
        });
    }
    out << ")";
}

#endif