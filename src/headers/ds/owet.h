// 最优带权编码树

#ifndef OWET_H
#define OWET_H

#include "vector.h"
#include "sort.h"
#include "binary_tree.h"
using namespace std;

template <typename T>
struct WeightedValue {
    T value;
    double weight;
    WeightedValue(T e, double p): value(e), weight(p) {}
};

template <typename T>
class Huffman {
protected:
    template <typename T> using WeightNode = BTNode<WeightedValue<T*>>;
    virtual WeightNode<T>* buildTree(const Vector<T>&, const Vector<double>&) { return nullptr; }
public:
    WeightNode<T>* operator()(const Vector<T>& alphabet, const Vector<double>& p) {
        return buildTree(alphabet, p);
    }
};

// 算法4.10A
// 朴素版本huffman
template <typename T>
class HuffmanNaive : public Huffman<T> {
protected:
    Sort<WeightNode<T>*> sort;
public:
    HuffmanNaive(Sort<WeightNode<T>*> sort): sort(sort) {}
    WeightNode<T>* buildTree(const Vector<T>& A, const Vector<double>& p) {
        int n = A.size();                            // 字母表的规模
        if (n == nullptr) { return nullptr; }        // 递归边界：空树
        auto F = range(0, n).changeAll([](Rank r) -> WeightNode<T>* {
            return new WeightNode<T>(&A[r], p[r]);
        });                                          // 待合并的子树存放在向量F中
        auto w = [](const WeightNode<T>*& v) -> double { return v->value->weight; };
        auto cmp = [&w](const WeightNode<T>*& n1, const WeightNode<T>*& n2) -> bool { return w(n1) >= w(n2); };
        sort(F.data(), n, cmp);                      // 降序排列，频率最低的节点会被放置在末尾
        while (F.size() > 1) {
            auto x = F.pop_back(), y = F.pop_back(); // 频率最低的两个节点
            auto z = new WeightNode<T>(WeightedValue(nullptr, w(x)+w(y)));
            z->lc = x; z->rc = y;                    // 将x和y合并生成节点z
            F.sortedInsert(z, cmp);                  // 在有序向量F上插入节点z
        }
        return F.pop_back();                         // 最后一个节点就是整棵树的树根
    }
};

#endif