// 2.2 - 列表 的主要代码

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <functional>
#include "vector.h"
using namespace std;

typedef int Rank;

template <typename T>
struct ListNode {       // 列表中的一个数据单元（节点）
    T value;            // 本节点存放的数据
    ListNode<T>* pred;  // 指向列表中的直接前驱
    ListNode<T>* succ;  // 指向列表中的直接后继

    ListNode<T>* forward(int step);
    ListNode<T>* backward(int step);
};

template<typename T>
ListNode<T>* ListNode<T>::forward(int step) {
    auto p = this;
    for (; step > 0; --step) {
        p = p->succ;
    }
    return p;
}

template<typename T>
ListNode<T>* ListNode<T>::backward(int step) {
    auto p = this;
    for (; step > 0; --step) {
        p = p->pred;
    }
    return p;
}

template <typename T>
class List {
private:
    int _size;          // 列表的规模
    ListNode<T>* _head; // 列表的头哨兵节点
    ListNode<T>* _tail; // 列表的尾哨兵节点
public:
    int size() const { return _size; }
    ListNode<T>* head() const { return _head; }
    ListNode<T>* tail() const { return _tail; }

    // ----------------------------------以下内容不在笔记正文中----------------------------
    List(T* A, int n); // 利用数组生成列表
    List(const Vector<T>& V); // 利用向量生成列表
    List(const List<T>& L); // 复制构造函数

    void traverse(function<void(ListNode<T>*)> visit) const; // 遍历

    template <typename T1> friend ostream& operator<< (ostream& out, const List<T1>& L); // 使用cout方式打印

    void push_back(T e); // 在尾部插入元素
    T pop_back();  // 删除尾部的元素
    void push_front(T e); // 在头部插入元素
    T pop_front(); // 删除头部的元素

    // -----------------------------------以下开始是示例代码的函数---------------------------
    List(); // 构造函数，生成空列表
    ListNode<T>* operator[](Rank index) const; // "循秩访问"
    
    void insertAsPred(T e, ListNode<T>* p); // 算法2.13 - 列表插入元素（前插）
    void insertAsSucc(T e, ListNode<T>* p); // 算法2.13 - 列表插入元素（后插，正文中未给出）
    void remove(ListNode<T>* p); // 算法2.14 - 列表删除元素
    ListNode<T>* find(T e) const; // 算法2.15 - 列表查找元素
    
    void mergeSort(function<bool(const T&, const T&)> cmp); // 算法2.16 - 归并排序
    void mergeSort();
private:
    void mergeSort(ListNode<T>*& s, ListNode<T>* t, int n, function<bool(const T&, const T&)> cmp); // 归并排序的原型函数
};

template <typename T>
List<T>::List() {
    _size = 0;
    _head = new ListNode<T>();
    _tail = new ListNode<T>();
    _head->succ = _tail; _head->pred = nullptr;
    _tail->pred = _head; _tail->succ = nullptr; 
}

template <typename T>
ListNode<T>* List<T>::operator[](Rank index) const {
    if (index <= _size / 2) {
        return _head->forward(index+1);
    } else {
        return _tail->backward(_size-index);
    }
}

// 算法2.13A
template <typename T>
void List<T>::insertAsPred(T e, ListNode<T>* p) {
    auto q = p->pred, x = new ListNode<T>();
    x->value = e;                   // 创建新节点
    x->pred = q; x->succ = p;       // 将新节点的链子挂在p和q之间
    q->succ = x; p->pred = x;       // 断开p和q之间的两条链子
    ++_size;                        // 更新列表规模
}

// 算法2.13A2（后插，正文中未给出）
template <typename T>
void List<T>::insertAsSucc(T e, ListNode<T>* p) {
    auto q = p->succ, x = new ListNode<T>();
    x->value = e;                   // 创建新节点
    x->pred = p; x->succ = q;       // 将新节点的链子挂在p和q之间
    q->pred = x; p->succ = x;       // 断开p和q之间的两条链子
    ++_size;                        // 更新列表规模
}

#ifdef ALGORITHM_2_13_SINGLE
// 算法2.13B（单链表前插）
template <typename T>
void List<T>::insertAsPred(T e, ListNode<T>* p) {
    auto x = new ListNode<T>();
    x->value = p->value;            // 复制节点p
    x->succ = p->succ; p->succ = x; // 后插，复制得到的x当做被插节点
    p->value = e;                   // 而原来的p当做插入的节点
    ++_size;                        // 更新列表规模
}
#endif

// 算法2.14A
template <typename T>
void List<T>::remove(ListNode<T>* p) {
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    --_size;
    delete p;  // 释放内存
}

// 算法2.15A
template <typename T>
ListNode<T>* List<T>::find(T e) const {
    for (auto p = _tail->pred; p != _head; p = p->pred) {
        if (p->value == e) { return p; } // 从后向前检索
    }
    return nullptr;
}

// 算法2.16A
template <typename T>              // 从s开始到t（不包括t）的n个节点进行排序，保证排序后s仍然指向起点
void List<T>::mergeSort(ListNode<T>*& s, ListNode<T>* t, int n, function<bool(const T&, const T&)> cmp) {
    if (n <= 1) { return; }        // 递归边界
    int m = n / 2;                 // 取中点
    auto mid = s->forward(m);      // 取m次succ，找到列表的中点
    mergeSort(s, mid, m, cmp);     // 递归地排序列表的前半部分
    mergeSort(mid, t, n-m, cmp);   // 递归地排序列表的后半部分
    auto sp = s->pred;             // 暂存s的直接前驱，用来在归并结束后将s复位到起点
    auto p = s, q = mid;           // p和q作为两部分的指针
    while (p != q && q != t) {     // 注意，前半部分用完的标志是p追上q而不是mid（mid可能会被置换到p前面去）
        if (cmp(p->value, q->value)) {
            p = p->succ;           // 前半小，不动
        } else {
            auto t = q;
            q = q->succ;           // 后半小，移到前面去，将t从q->pred移动到p->pred
            (q->pred = t->pred)->succ = q;
            (t->pred = p->pred)->succ = t;
            t->succ = p; p->pred = t;
        }
    }
    s = sp->succ;                  // 保证s仍然指向这一段列表的起点，否则递归返回之后mid会乱掉
}

template <typename T>
void List<T>::mergeSort(function<bool(const T&, const T&)> cmp) {
    ListNode<T>* start = _head->succ;
    mergeSort(start, _tail, _size, cmp);
}

template <typename T>
void List<T>::mergeSort() {
    mergeSort(less_equal<T>());
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
static pair<ListNode<T>*, ListNode<T>*> make_list(T* A, int n) {
    auto head = new ListNode<T>();
    auto p = head;
    for (int i = 0; i < n; ++i) {
        auto q = new ListNode<T>();
        q->value = A[i];
        q->pred = p; p->succ = q;
        p = q;
    }
    auto tail = new ListNode<T>();
    tail->pred = p; p->succ = tail;
    return make_pair(head, tail);
}

template <typename T>
List<T>::List(T* A, int n) {
    _size = n;
    auto pair = make_list(A, n);
    _head = pair.first;
    _tail = pair.second;
}

template <typename T>
List<T>::List(const Vector<T>& V) {
    _size = V.size();
    auto pair = make_list(V.data(), V.size());
    _head = pair.first;
    _tail = pair.second;
}

template <typename T>
List<T>::List(const List<T>& L) {
    _size = L._size;
    _head = new ListNode<T>();
    auto q = _head;                  // q表示当前已生成列表的最后一个节点
    L.traverse([&q](ListNode<T>* p) -> void { 
        auto r = new ListNode<T>();
        r->value = p->value;         // 复制节点
        r->pred = q; q->succ = r;    // 插入到q的后面
        q = r;                       // 更新q
    });
    _tail = new ListNode<T>();       // 最后连上尾节点
    _tail->pred = q; q->succ = _tail;
}

template <typename T>
void List<T>::traverse(function<void(ListNode<T>*)> visit) const {
    for (auto p = _head->succ; p != _tail; p = p->succ) {
        visit(p);
    }
}

template <typename T>
ostream& operator<< (ostream& out, const List<T>& L)
{
    out << "L(";
    L.traverse([&](ListNode<T>* p) -> void {
        if (p != L._head->succ) { out <<","; }
        out << p->value;
    });
    out << ")";
}

template <typename T>
void List<T>::push_back(T e) {
    insertAsPred(e, _tail);
}

template <typename T>
void List<T>::push_front(T e) {
    insertAsSucc(e, _head);
}

template <typename T>
T List<T>::pop_back() {
    auto p = _tail->pred;
    T temp = p->value;
    remove(p);
    return temp;
}

template <typename T>
T List<T>::pop_front() {
    auto p = _head->succ;
    T temp = p->value;
    remove(p);
    return temp;
}

#endif
