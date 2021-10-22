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
BinaryTree<T> rebuildBinaryTree1(const Vector<T>& preSeq, const Vector<T>& inSeq) {
    function<BTNode<T>*(int, int, int)> buildSubtree;
    buildSubtree = [&](int preSeqStart, int inSeqStart, int length) -> BTNode<T>* {
        // 从 preSeq[pSS:pSS+len] 和 inSeq[iSS:iSS+len] 重建二叉树
        for (int i = 0; i < length; ++i) {   // 找子树的根在中序序列上的位置
            if (inSeq[inSeqStart + i] == preSeq[preSeqStart]) { // 先重建树根
                auto r = new BTNode<T>(preSeq[preSeqStart]);    // 在重建左右子树
                (r->lc = buildSubtree(preSeqStart + 1, inSeqStart, i))->parent = r;
                (r->rc = buildSubtree(preSeqStart + i+1, inSeqStart+i+1, length-1 - i))->parent = r;
                return r;
            }
        }
        return nullptr; // 递归边界：空树（length=0，不会进循环）
    };
    int n = preSeq.size();
    return new BinaryTree<T>(n, buildSubtree(0, 0, n));
}

// 算法4.4B
template<typename T>
BinaryTree<T> rebuildBinaryTree2(const Vector<T>& preSeq, const Vector<T>& inSeq) {
    int i = 0, j = 0, n = preSeq.size();               // 先序和中序序列中的索引指针
    auto buildTreeFramework = [&]() -> BTNode<T>* {    // 构造最左侧通路的子程序
        BTNode<T>* last = nullptr;                     // 上一个被构造出来的节点
        for (; i < n && preSeq[i] != inSeq[j]; ++i) {  // 检索先序序列到中序序列首元素为止
            auto node = new BTNode<T>(preSeq[i]);      // 新建一个节点
            if (last != nullptr) {
                (node->parent = last)->lc = node;      // 挂载在上一个节点的左子代位置
            }
            last = node;                               // 更新上一个节点
        }
        return last;                                   // 返回最左侧通路的末端
    };
    function<BTNode<T>*(bool, const T&)> buildSubtree; // 构造子树的子程序，采用线性递归
    buildSubtree = [&](bool end, const T& endPoint) -> BTNode<T>* {
        if (j == n || (end && endPoint == inSeq[j])) { // 参数表示（是否采用）中序序列终止符
            return nullptr;                            // 递归边界是空树
        }
        BTNode<T>* last = buildTreeFramework();        // 构造最左侧通路
        for (++i, ++j; j < n && (end || inSeq[j] != endPoint); last = last->parent, ++j) {
            (last->rc = buildSubtree(preSeq[i]))->parent = last; // 利用已知边界构造右子树
        }
        while (last->parent != nullptr) {              // 即使达到了边界 ---- 是否可以删去？
            last = last->parent;                       // 也要回溯到根节点为止
        }
        return last;                                   // 返回所生成子树的根节点
    };
    return new BinaryTree<T>(n, buildSubtree(false, inSeq[0]));
}

int main() {

    return 0;
}
