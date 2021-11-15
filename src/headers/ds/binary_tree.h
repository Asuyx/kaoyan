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
    BTNode<T>* _root;  // 指向二叉树的根节点
public:
    BTNode<T>*& root() const { return _root; }  

    // -----------------------------------以下开始是示例代码的函数---------------------------
    BinaryTree(): _root(nullptr) {} // 构造函数，生成空树

    static void removeSubtree(BTNode<T>*& node); // 删除子树
    static void insertSubtree(BTNode<T>* parent, BTNode<T>*& pos, BTNode<T>* node); // 插入子树 
    static void preorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node); // 先序遍历
    static void inorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node); // 中序遍历
    static void postorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node); // 后序遍历
    static void levelTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node); // 层次遍历

private:
    void preorderTraverseStack(function<void(BTNode<T>*)> visit); // 先序遍历（无递归）
    void preorderTraverseStackOpt(function<void(BTNode<T>*)> visit); // 先序遍历（无递归，优化）
    void inorderTraverseStackOpt(function<void(BTNode<T>*)> visit); // 中序遍历（无递归，优化）
    void postorderTraverseStack(function<void(BTNode<T>*)> visit); // 后序遍历（无递归，基于NRL）
    void postorderTraverseStackOpt(function<void(BTNode<T>*)> visit); // 后序遍历（无递归，优化）
    void preorderTraverseSucc(function<void(BTNode<T>*)> visit); // 使用直接后继做先序遍历

protected:
    static BTNode<T>* preorderSucc(BTNode<T>* node); // 先序序列的直接后继
    static BTNode<T>* preorderPred(BTNode<T>* node); // 先序序列的直接前驱
    static BTNode<T>* inorderSucc(BTNode<T>* node);  // 中序序列的直接后继
    static BTNode<T>* inorderPred(BTNode<T>* node);  // 中序序列的直接前驱
    static BTNode<T>* postorderSucc(BTNode<T>* node); // 后序序列的直接后继
    static BTNode<T>* postorderPred(BTNode<T>* node); // 后序序列的直接前驱

    // -----------------------------------以下内容不在笔记正文中----------------------------
public:
    BinaryTree(BTNode<T>* _root) : _root(_root) {} // 生成指定根的树
    BinaryTree(const BinaryTree<T>& T); // 复制构造函数
    
    static void insert(BTNode<T>* parent, BTNode<T>*& pos, T e); // 插入一个节点
    void preorderTraverse(function<void(BTNode<T>*)> visit) { preorderTraverse(visit, _root); }
    void inorderTraverse(function<void(BTNode<T>*)> visit) { inorderTraverse(visit, _root); }
    void postorderTraverse(function<void(BTNode<T>*)> visit) { postorderTraverse(visit, _root); }

    void updateSize();
    bool operator==(const BinaryTree<T>& other) const;
    template <typename T1> friend ostream& operator<< (ostream& out, const BinaryTree<T1>& T); // 使用cout方式打印
};

// 算法4.1 - 删除子树
template <typename T>
void BinaryTree<T>::removeSubtree(BTNode<T>*& node) {
    function<void(BTNode<T>*)> remove;
    remove = [&_size](BTNode<T>* node) -> void {
        if (node == nullptr) { // 递归边界：空树
            return;
        }
        auto lc = node->lc, rc = node->rc;
        delete node;           // 释放当前节点的空间
        remove(lc);            // 递归删除左右子树（尾递归）
        remove(rc);
    };
    remove(node);              // 递归删除子树
    node = nullptr;            // 这里node是上层的引用
}

// 算法4.2 - 插入子树
template <typename T>
void BinaryTree<T>::insertSubtree(BTNode<T>* parent, BTNode<T>*& pos, BTNode<T>* node) {
    removeSubtree(pos);        // 删除原位置的子树，注意这里pos是引用
    node->parent = parent;     // 更新被插入子树的亲代指针
    pos = node;                // 将新的子树挂载到pos的位置上
}

// 算法4.2 - 插入节点（正文中未给出）
template <typename T>
void BinaryTree<T>::insert(BTNode<T>* parent, BTNode<T>*& pos, T e) {
    insertSubtree(parent, pos, new BTNode<T>(e));
}

// 算法4.3A - 先序遍历
template <typename T>
void BinaryTree<T>::preorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node) {
    if (node == nullptr) {  // 递归边界：空树
        return;
    }
    visit(node);            // 访问树根
    traverse(node->lc);     // 递归遍历左子树
    traverse(node->rc);     // 递归遍历右子树
}

// 算法4.3B - 中序遍历
template <typename T>
void BinaryTree<T>::inorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node){
    if (node == nullptr) {  // 递归边界：空树
        return;
    }
    traverse(node->lc);     // 递归遍历左子树
    visit(node);            // 访问树根
    traverse(node->rc);     // 递归遍历右子树
}

// 算法4.3C - 后序遍历
template <typename T>
void BinaryTree<T>::postorderTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node) {
    if (node == nullptr) {  // 递归边界：空树
        return;
    }
    traverse(node->lc);     // 递归遍历左子树
    traverse(node->rc);     // 递归遍历右子树
    visit(node);            // 访问树根
}

// 算法4.3AS - 先序遍历（使用栈）
template <typename T>
void BinaryTree<T>::preorderTraverseStack(function<void(BTNode<T>*)> visit) {
    Stack<BTNode<T>*> S;
    S.push(_root);             // 从树根开始遍历
    while (!S.empty()) {
        auto t = S.pop();     // 取出栈顶元素
        if (t != nullptr) {   // 如果非空
            visit(t);         // 则对其进行访问
            S.push(t->rc);    // 并将其左右子代依次入栈
            S.push(t->lc);    // 注意顺序是反的
        }
    }
}

// 算法4.3AO - 先序遍历（使用栈，优化）
template <typename T>
void BinaryTree<T>::preorderTraverseStackOpt(function<void(BTNode<T>*)> visit) {
    Stack<BTNode<T>*> S;
    S.push(_root);                                         // 从树根开始遍历
    while (!S.empty()) {                                   
        for (auto t = S.pop(); t != nullptr; t = t->lc) {  // 对每个节点，沿着左子代到底
            visit(t);                                      // 访问沿途经过的节点
            S.push(t->rc);                                 // 并把右子代暂存到栈里
        }                                                  // 这样回溯时会从深到浅访问右姊妹
    }
}

// 算法4.3BO - 中序遍历（使用栈）
template <typename T>
void BinaryTree<T>::inorderTraverseStackOpt(function<void(BTNode<T>*)> visit) {
    Stack<BTNode<T>*> S;
    auto pushLeftChain = [&S](BTNode<T>* node) -> void {   // 从node开始一路向左入栈
        for (auto t = node; t != nullptr; t = t->lc) {     // 沿着左子树迭代到底
            S.push(t);
        }
    };
    pushLeftChain(_root);                                  // 先将整棵树的最左侧通路入栈
    while (!S.empty()) {                                   // 开始回溯
        auto t = S.pop();                                  // 回溯到路径上的节点t
        visit(t);                                          // 访问回溯到的节点
        pushLeftChain(t->rc);                              // 然后转向右子树
    }
}

// 算法4.3CS - 后序遍历（使用栈，基于NRL作为跳板）
template <typename T>
void BinaryTree<T>::postorderTraverseStack(function<void(BTNode<T>*)> visit) {
    Stack<BTNode<T>*> S;      // 用来做NRL遍历的辅助栈
    Stack<BTNode<T>*> St;     // NRL的访问次序
    S.push(_root);            // 从树根开始遍历
    while (!S.empty()) {
        auto t = S.pop();     // 取出栈顶元素
        if (t != nullptr) {   // 如果非空
            St.push(t);       // NRL的访问（记录下访问次序）
            S.push(t->lc);    // 并将其左右子代依次入栈
            S.push(t->rc);    // 顺序和NLR（先序）相反
        }
    }
    while (!St.empty()) {     // 沿着NRL的反序访问
        visit(St.pop());      // 就是按照LRN（后序）的反方向
    }
}

// 算法4.3CO - 后序遍历（使用栈，优化）
template <typename T>
void BinaryTree<T>::postorderTraverseStackOpt(function<void(BTNode<T>*)> visit) {
    Stack<BTNode<T>*> S;
    auto pushChildChain = [&S](BTNode<T>* node) -> void { // 从node开始一路向下
        for (auto t = node; t != nullptr; t = t->lc != nullptr ? t->lc : t->rc) {
            S.push(t);     // 优先向左下，如果左下为空则向右下，迭代到叶子为止
        }
    };
    pushChildChain(_root);                 // 向下到底，找到后序遍历的起点
    while (!S.empty()) {                   // 开始回溯
        auto t = S.pop();                  // 取出栈顶元素，取出后S.top应该是t的亲节点
        visit(t);                          // 先访问t
        if (t->isLeftChild()) {            // 如果t是左子代
            pushChildChain(t->parent->rc); // 则进入右姊妹
        }                                  // 右子代则继续向上回溯
    }
}


// 算法4.7A - 先序序列直接后继
template<typename T>
BTNode<T>* BinaryTree<T>::preorderSucc(BTNode<T>* node) {
    if (node->lc != nullptr) { return node->lc; } // 有左子取左子
    if (node->rc != nullptr) { return node->rc; } // 无左有右取右子
    while (!node->isRoot()) {
        if (node->isLeftChild() && node->parent->rc != nullptr) {
            return node->parent->rc;              // 有右姊妹则取右姊妹
        } else {
            node = node->parent;                  // 无右姊妹继续向上回溯
        }
    }
    return nullptr;                               // 回溯到树根仍然没有右姊妹，结束
}

// 算法4.7B - 中序序列直接后继
template <typename T>
BTNode<T>* BinaryTree<T>::inorderSucc(BTNode<T>* node) {
    if (node->rc != nullptr) { // 有右子，则向右下1步，再左下到底，和算法4.3BO一致
        for (node = node->rc; node->lc != nullptr; node = node->lc);
    } else {                   // 无右子，则向左上到底，再右上1步
        for (; node->isRightChild(); node = node->parent);
        node = node->parent;
    }
    return node;
}

// 算法4.7C - 后序序列直接后继
template <typename T>
BTNode<T>* BinaryTree<T>::postorderSucc(BTNode<T>* node) {
    if (node->isRoot()) { return nullptr; } // 根节点没有后继
    if (node->isRightChild()) { return node->parent; } // 右子的直接后继必是亲代
    if (node->parent->rc != nullptr) { // 左子，如果有右姊妹则直接后继是右姊妹的最左侧
        node = node->parent->rc;       // 一路向下，有左则左，无左则右，和算法4.3CO一致
        while (node->lc != nullptr || node->rc != nullptr) {
            node = node->lc != nullptr ? node->lc : node->rc;
        }
    } else {
        return node->parent;           // 如果没有右姊妹，直接后继也是亲代
    }
}

// 算法4.7A2 - 先序序列直接前驱（算法4.7C的对称形式）
template <typename T>
BTNode<T>* BinaryTree<T>::preorderPred(BTNode<T>* node) {
    if (node->isRoot()) { return nullptr; }       // 根节点没有前驱
    if (node->isLeftChild()) { return node->parent; } // 左子的直接前驱必是亲代
    if (node->parent->lc != nullptr) { // 右子，如果有左姊妹则直接前驱是左姊妹的最右侧
        node = node->parent->lc;       // 这里是在左姊妹一路向下（优先右子），和后序遍历算法4.3CO是相反的
        while (node->lc != nullptr || node->rc != nullptr) {
            node = node->rc != nullptr ? node->rc : node->lc;
        }
    } else {
        return node->parent;           // 如果没有左姊妹，直接前驱也是亲代
    }
}

// 算法4.7B2 - 中序序列直接前驱（算法4.7B的对称形式）
template <typename T>
BTNode<T>* BinaryTree<T>::inorderPred(BTNode<T>* node) {
    if (node->lc != nullptr) { // 有左子，则向左下1步，再右下到底
        for (node = node->lc; node->rc != nullptr; node = node->rc);
    } else {                   // 无左子，则向右上到底，再左上1步
        for (; node->isLeftChild(); node = node->parent);
        node = node->parent;
    }
    return node;
}

// 算法4.7C2 - 后序序列直接前驱（算法4.7A的对称形式）
template<typename T>
BTNode<T>* BinaryTree<T>::preorderSucc(BTNode<T>* node) {
    if (node->rc != nullptr) { return node->rc; } // 有右子取右子
    if (node->lc != nullptr) { return node->lc; } // 无右有左取左子
    while (!node->isRoot()) {
        if (node->isRightChild() && node->parent->lc != nullptr) {
            return node->parent->lc;              // 有左姊妹则取左姊妹
        } else {
            node = node->parent;                  // 无左姊妹继续向上回溯
        }
    }
    return nullptr;                               // 回溯到树根仍然没有左姊妹，结束
}

// 算法4.3AL - 先序遍历（使用后继方法）
template <typename T>
void BinaryTree<T>::preorderTraverseSucc(function<void(BTNode<T>*)> visit) {
    for (auto p = _root; p != nullptr; p = preorderSucc(p)) {
        visit(p);
    }
}

// 算法 - 层次遍历
template <typename T>
void BinaryTree<T>::levelTraverse(function<void(BTNode<T>*)> visit, BTNode<T>* node) {
    Queue<BTNode<T>*> Q;
    Q.enqueue(node);         // 从树根开始遍历
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