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
                r->lc = buildSubtree(preSeqStart + 1, inSeqStart, i);
                r->rc = buildSubtree(preSeqStart + i+1, inSeqStart+i+1, length-1 - i);
                return r;
            }
        }
        return nullptr; // 递归边界：空树（length=0，不会进循环）
    };
    buildSubtree(0, 0, preSeq.size());
}

int main() {

    return 0;
}
