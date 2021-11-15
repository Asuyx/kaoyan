// 例4.2 - 二叉树的各种遍历
// 以及从遍历序列重建二叉树的技术

#include "binary_search_tree.h"

const int n = 10; // 二叉树的规模

// 生成随机的二叉树
BinaryTree<char>* randomTree() {
    auto V = Vector<char>(n, n, [](Rank r) -> int { return r + 'A'; }); // 生成使用的字母列表
    BinaryTree<char>* BST = randomBST<char>(V, less_equal<char>()); // 利用字母列表生成BST
    auto RV = range(0, n); RV.shuffle();                            // 随机生成一个对应关系
    BST->preorderTraverse([&](BTNode<char>* node) -> void {
        node->value = V[RV[node->value - 'A']];                     // 利用对应关系打乱BST的中序序列
    });
    return BST;
}

