// 2.3 - 分块表 - 列表套列表 的主要代码

#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <cmath>
#include <climits>
#include "list.h"
using namespace std;

template <typename T>
struct IndexNode {  // 索引指针节点
    ListNode<T>* p; // 指向的二级表节点
    Rank r;         // 指向的二级表节点在整个表中的秩
};

template <typename T>
class ListList {
private:
    const int k1 = 2;
    const int k2 = 4;

    List<IndexNode<T>> L1; // 一级表（索引表）
    List<T> L2;            // 二级表
public:
    int size() { return L2.size(); }
    ListNode<T>* head() { return L2.head(); }
    ListNode<T>* tail() { return L2.tail(); }

    // ----------------------------------以下内容不在笔记正文中----------------------------
    ListList() { }
    ListList(const ListList<T>& L); // 复制构造函数

    void traverse(function<void(ListNode<T>*)> visit) const; // 遍历

    template <typename T1> friend ostream& operator<< (ostream& out, const ListList<T1>& L); // 使用cout方式打印

    ListNode<T>* find(T e) const; // 列表查找元素

    // -----------------------------------以下开始是示例代码的函数---------------------------
    ListList(const List<T>& L); // 用列表创建LL表
    ListNode<T>* operator[](Rank index) const; // 算法2.18 - 循秩访问

    void insert(T e, Rank r); // 算法2.19 - 列表插入元素
    void remove(Rank r);      // 列表删除元素（正文中未给出）

private:
    void buildL1(); // 建立索引表
    
    void localReconstruct(ListNode<IndexNode<T>>* p1); // 算法2.20A - 索引表局部重构
    void globalReconstruct(); // 算法2.20B - 索引表整体重构
};

template <typename T>
ListList<T>::ListList(const List<T>& L) {
    L2 = L;
    buildL1();
}

template <typename T>
void ListList<T>::buildL1() {
    int len = (int)floor(sqrt(L2.size()));     // 两个索引指针之间的间隔，同时也是索引指针的数量
    if (len <= 0) { return; }                  // 空表，直接返回
    int r = -1;                                // 记录数据节点的秩
    L2.traverse([&, len](ListNode<T>* p) -> {
        if (++r % len == 0) {                  // 每隔len个数据节点，插入一个索引节点
            IndexNode<T> temp;
            temp.p = p; temp.r = r;
            L1.push_back(temp);
        }
    });
}

#ifdef ALGORITHM_2_18_SIMPLE
// 算法2.18A
template <typename T>
ListNode<T>* ListList<T>::operator[](Rank index) const {
    auto p1 = L1[0], p2 = p1->succ;
    while (p2 != L1.tail() && p2.r <= index) {
        p2 = (p1 = p2)->succ;
    }
    return p1->value.p->forward(index - p1->value.r);
}
#endif

// 算法2.18A2（正文中未给出）
template <typename T>
ListNode<T>* ListList<T>::operator[](Rank index) const {
    ListNode<T>* p1, * p2;                       // p2保持为p1->succ，将L2[r]定位到p1和p2之间
    if (index <= size() / 2) {                   // 如果r <= n/2，就从左边遍历索引表
        p1 = L1[0], p2 = p1->succ;
        while (p2 != L1.tail() && p2.r <= index) {
            p2 = (p1 = p2)->succ;
        }
    } else {                                     // 否则，从右边遍历索引表
        p2 = L1[L1.size()-1], p1 = p2->pred;
        while (p1 != L1.head() && p1.r > index) {
            p1 = (p2 = p1)->pred;
        }
    }
    Rank left_step = p1 == L1.head() ? INT_MAX : index - p1->value.r;  // 注意不能从一级表的哨兵出发
    Rank right_step = p2 == L1.tail() ? INT_MAX : p2->value.r - index; // 所以哨兵的情况设为INT_MAX
    if (left_step < right_step) {                 // 如果靠近p1，因为设成INT_MAX，不可能是哨兵
        return p1->value.p->forward(left_step);   // 就从p1指向的节点出发向后查找
    } else {
        return p2->value.p->backward(right_step); // 否则从p2出发向前查找
    }
}

// 算法2.19A
template <typename T>
void ListList<T>::insert(T e, Rank r) {
    if (L2.size() == 0) {                       // 空表特判
        L1.push_back(IndexNode<T>());   
    }
    auto p1 = L1[0], p2 = p1->succ;             // 通过循秩访问先找到位置
    while (p2 != L1.tail() && p2.r <= r) {
        p2 = (p1 = p2)->succ;
    }
    auto p = p1->value.p->forward(r - p1->value.r); // 当前的L[r]（可能是tail）
    L2.insertAsPred(p, e);                      // 将e插入到L[r]的位置上
    if (L2.size() == 1) {                       // 空表特判
        p->value.r = 0;
        p->value.p = p->pred;
    }
    while (p2 != L1.tail()) {                   // 被插入位置之后的索引，需要将秩+1
        ++p2->value.r;
        p2 = p2->succ;
    }
    localReconstruct(p1);
}

template <typename T>
void ListList<T>::remove(Rank r) {
    auto p1 = L1[0], p2 = p1->succ;             // 通过循秩访问先找到位置
    while (p2 != L1.tail() && p2.r <= r) {
        p2 = (p1 = p2)->succ;
    }
    auto p = p1->value.p->forward(r - p1->value.r); // 当前的L[r]
    if (p1->value.r == r) {                     // 如果索引正好指向了这个被删除的节点
        if (p2 != L1.tail() && p2->value.r == r+1) { // 如果p1和p2之间只有p一个节点
            L1.remove(p1);                      // 就直接将p1删除
        } else {                                // 否则，选择将这个索引向后移动1个节点
            if ((p1->value.p = p->succ) == L2.tail()) { // 移动后如果到达tail，则也将p1删除
                L1.remove(p1);
            }
        }
    }
    L2.remove(p);
    while (p2 != L1.tail()) {                   // 被删除位置之后的索引，需要将秩-1
        --p2->value.r;
        p2 = p2->succ;
    }
    globalReconstruct();
}

// 算法2.20A
template <typename T>
void ListList<T>::localReconstruct(ListNode<IndexNode<T>>* p1) {
    int sqrt_n = (int)floor(sqrt(L2.size())); 
    if (p1->succ->value.r - p1->value.r > k1*sqrt_n) { // 判断是否需要重构
        auto p = p1->value.p;            // p1指向的节点
        for (int i = 1; i < k1; ++i) {   // 在中间插入k1-1个索引
            p = p->forward(sqrt_n);      // p移动到下一个被插入的索引指向的位置
            L1.insertAsSucc(p1, IndexNode<T>());      // 插入新的索引节点
            p1->succ->value.r = p1->value.r + sqrt_n;
            p1->succ->value.p = p;
            p1 = p1->succ;
        }
    }
}

// 算法2.20B
template <typename T>
void ListList<T>::globalReconstruct() {
    int sqrt_n = (int)floor(sqrt(L2.size()));
    if (L1.size() > k2*sqrt_n) {                  // 判断是否需要重构
        auto p = L1[0];
        while (p->succ->succ != L1.tail()) {      // 逐个判断，是否需要删除p->succ
            if (p->succ->succ->value.r - p1->value.r <= sqrt_n) {
                L1.remove(p->succ);               // 满足条件，删除p->succ
            } else {                              // 否则继续向后检索
                p = p->succ;
            }
        }
    }
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
ListList<T>::ListList(const ListList<T>& L) {
    L1 = L.L1; L2 = L.L2;
}

template <typename T>
void ListList<T>::traverse(function<void(ListNode<T>*)> visit) const {
    L2.traverse(visit);
}

template <typename T>
ListNode<T>* ListList<T>::find(T e) const {
    return L2.find(e);
}

template <typename T>
ostream& operator<< (ostream& out, const ListList<T>& L)
{
    out << "LL(";
    if (L2.size() > 0) {
        auto p1 = L1[0];     // 索引节点
        L2.traverse([&](ListNode<T>* p) -> {
            if (p != L2.head()->succ) { out <<","; }
            if (p1 != L1.tail() && p == p1->value.p) {
                out << "*";  // 有索引的节点打上*号
                p1 = p1->succ;
            }
            out << p->value;
        });
    }
    out << ")";
}

#endif
