// 例4.1 - 中序序列和后序序列 重建 二叉树
// 问题4.4 - 重建二叉树（先序+中序）
// 输入：先序序列preSeq，中序序列inSeq
// 输出：重建的二叉树T

#include <iostream>
#include "vector.h"
#include "binary_tree.h"
using namespace std;

// 算法4.4A
template <typename T>
BinaryTree<T>* rebuildBinaryTree(const Vector<T>& preSeq, const Vector<T>& inSeq) {
    function<BTNode<T>*(int, int, int)> buildSubtree;
    buildSubtree = [&](int sP, int sI, int length) -> BTNode<T>* { 
        // 从 preSeq[sP:sP+len] 和 inSeq[sI:sI+len] 重建一棵子树
        for (int i = 0; i < length; ++i) {     // 找子树的根在中序序列上的位置
            if (inSeq[sI + i] == preSeq[sP]) { // 遍历中序序列直到找到先序序列首元素
                auto r = new BTNode<T>(preSeq[sP]);                // 重建树根节点
                (r->lc = buildSubtree(sP + 1, sI, i))->parent = r; // 重建左右子树
                (r->rc = buildSubtree(sP + i+1, sI + i+1, length-1-i))->parent = r;
                return r;                      // 重建子树完成
            }
        }
        return nullptr; // 递归边界：空树（length=0，不会进循环）
    };
    return new BinaryTree<T>(buildSubtree(0, 0, preSeq.size())); // 返回整个树的根节点
}

// 算法4.4B
template<typename T>
BinaryTree<T>* rebuildBinaryTree2(const Vector<T>& preSeq, const Vector<T>& inSeq) {
    int i = 0, j = 0, n = preSeq.size();               // 先序和中序序列中的索引指针
    auto comeToEnd = [&](int end) -> bool {            // 判断是否到达子树终点，标识符为preSeq[end]
        return end == n && j == n || preSeq[end] == inSeq[j]; // 即最左侧通路上的下一个节点A[i]
    };                                                 // 注意都溢出的情况也匹配
    function<BTNode<T>*(Rank)> buildSubtree;
    buildSubtree = [&](Rank end) -> BTNode<T>* {       // 构造子树的子程序，采用线性递归
        if (comeToEnd(end)) {                          // 递归边界是空树    
            return nullptr;                           
        }
        BTNode<T>* root = nullptr, * node = root;      // 子树的根节点作为root
        BTNode<T>*& pos = root;                        // 开始构造最左侧通路
        Rank l = i;
        while (i < n) {                                // 检索先序序列
            BinaryTree<T>::insert(node, pos, preSeq[i]); // 在最左侧通路上插入元素
            node = pos; pos = node->lc;                // 向左下继续构造
            if (preSeq[i++] == inSeq[j]) { break; }    // 检索到中序序列首元素为止
        }                                              // 最左侧通路构造结束
        Rank r = i;                                    // 此时最左侧通路为preSeq[l:r]
        for (++j; !comeToEnd(end); ++j) {              // 开始构造右姊妹序列（线性递归）
            if (--r < l) { r = end; }                  // 注意最后一棵右子树的终点在end处
            BinaryTree<T>::insertSubtree(node, node->rc, buildSubtree(r)); // 插入右子树
            node = node->parent;                       // 向右上方回溯
        }
        return root;                                   // 返回子树的根节点
    };
    return new BinaryTree<T>(buildSubtree(n));
}

// 算法4.4C
// 要求二叉树的节点上的元素即为标号0, 1, ..., n-1
BinaryTree<int>* rebuildBinaryTree3(const Vector<int>& preSeq, const Vector<int>& inSeq) {
    int n = preSeq.size();
    if (n == 0) { return new BinaryTree<int>(); }   // 空序列对应空树
    int* pos = new int[n];
    for (int i = 0; i < n; ++i) {
        pos[preSeq[i]] = i;                         // 记录在先序序列上的位置，作为比较器
    }
    auto u = new BTNode<int>(inSeq[0]);             // 中序序列上的当前节点
    for (Rank i = 1; i < n; ++i) {                  // 遍历中序序列
        auto v = new BTNode<int>(inSeq[i]);         // v = succ(u)
        if (pos[u->value] > pos[v->value]) {        // v在u前，即v是u的祖先
            auto p = u->parent;
            while (p != nullptr && pos[p->value] > pos[v->value]) {
                p = (u = p)->parent;                // 向上回溯，寻找插入的位置
            }
            if (p != nullptr) { p->rc = v; }        // 被插入为p的右子代
            v->parent = p;                          // 然后需要和回溯的这条链连接
            BinaryTree<int>::insertSubtree(v, v->lc, u);            
        } else {                                    // v在u后，即v是u的后代
            BinaryTree<int>::insertSubtree(u, u->rc, v);
        }
    }
    delete[] pos;
    while (!u->isRoot()) { u = u->parent; }          // 最后需要返回到树根
    return new BinaryTree<int>(u);
}

int main() {

    return 0;
}
