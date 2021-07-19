// 2.2 - 列表 中的 静态链表
// 双链线性静态链表

#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#include <iostream>
#include <functional>
#include "vector.h"
using namespace std;

typedef int Rank;

template <typename T>
struct StaticListNode { // 列表中的一个节点
    T value;            // 本节点存放的数据
    Rank pred;          // 直接前驱的秩
    Rank succ;          // 直接后继的秩
};

template <typename T>
class StaticList {
private:
    int _size;
    const Rank _head = 0;
    const Rank _tail = 1;
    Vector<StaticListNode<T>> V;
public:
    int size() const { return _size; }
    Rank head() const { return _head; }
    Rank tail() const { return _tail; }
    StaticListNode<T>* get(Rank r) const { return &V[r]; }

    // ----------------------------------以下内容不在笔记正文中----------------------------
    StaticList(T* A, int n); // 利用数组生成列表
    StaticList(const Vector<T>& V); // 利用向量生成列表
    StaticList(const StaticList<T>& L); // 复制构造函数

    Rank forward(Rank start, int step) const;
    Rank backward(Rank start, int step) const;
    Rank operator[](Rank index) const; // "循秩访问"

    void traverse(function<void(Rank)> visit) const; // 遍历

    template <typename T1> friend ostream& operator<< (ostream& out, const StaticList<T1>& L); // 使用cout方式打印

    void push_back(T e); // 在尾部插入元素
    T pop_back();  // 删除尾部的元素
    void push_front(T e); // 在头部插入元素
    T pop_front(); // 删除头部的元素

    // -----------------------------------以下开始是示例代码的函数---------------------------
    StaticList(); // 构造函数，生成空列表
    
    void insertAsPred(T e, Rank r); // 列表插入元素（前插，正文中未给出）
    void insertAsSucc(T e, Rank r); // 列表插入元素（后插，正文中未给出）
    void remove(Rank r); // 列表删除元素
    Rank find(T e) const; // 列表查找元素（正文中未给出）
};

template <typename T>
StaticList<T>::StaticList() {
    _size = 0;
    V.push_back(StaticListNode<T>());
    V.push_back(StaticListNode<T>());
    V[_head].succ = _tail; V[_head].pred = -1;
    V[_tail].pred = _head; V[_tail].succ = -1;
}

template <typename T>
void StaticList<T>::insertAsPred(T e, Rank r) {
    Rank x = V.size(), s = V[r].pred;
    V.push_back(StaticListNode<T>());
    V[x].value = e;                    // 创建新节点
    V[x].pred = s; V[x].succ = r;      // 将新节点的链子挂在p和q之间
    V[s].succ = x; V[r].pred = x;      // 断开p和q之间的两条链子
    ++_size;                           // 更新列表规模
}

template <typename T>
void StaticList<T>::insertAsSucc(T e, Rank r) {
    Rank x = V.size(), s = V[r].succ;
    V.push_back(StaticListNode<T>());
    V[x].value = e;                    // 创建新节点
    V[x].pred = r; V[x].succ = s;      // 将新节点的链子挂在p和q之间
    V[s].pred = x; V[r].succ = x;      // 断开p和q之间的两条链子
    ++_size;                           // 更新列表规模
}

#ifdef ALGORITHM_2_16_WITHOUT_DELETE
// 算法2.16A
template <typename T>
void StaticList<T>::remove(Rank r) {
    V[V[r].pred].succ = V[r].succ;
    V[V[r].succ].pred = V[r].pred; 
    --_size;
}
#endif

// 算法2.16B
template <typename T>
void StaticList<T>::remove(Rank r) {
    V[V[r].pred].succ = V[r].succ;     // 先执行列表中的删除
    V[V[r].succ].pred = V[r].pred; 
    --_size;
    Rank last = V.size() -1;           // 再执行向量中的删除
    V[r] = V[last];                    // 将V[last]移动到V[r]的位置上
    V[V[last].pred] = r;               // 连接V[last]的链子转为连到V[r]上
    V[V[last].succ] = r;
    V.pop_back();                      // 删除V的最后一个节点
}

template <typename T>
Rank StaticList<T>::find(T e) const {
    for (Rank r = V[_tail].pred; r != _head; r = V[r].pred) {
        if (V[r].value == e) { return r; }
    }
    return -1;
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
StaticList<T>::StaticList(const StaticList<T>& L) {
    _size = L._size;
    V = Vector<T>(L.V); // 利用向量的复制构造函数直接深复制
}

template <typename T>
Rank StaticList<T>::forward(Rank start, int step) const {
    for (; step > 0; --step) {
        start = V[start].succ;
    }
    return start;
}

template <typename T>
Rank StaticList<T>::backward(Rank start, int step) const {
    for (; step > 0; --step) {
        start = V[start].pred;
    }
    return start;
}

template <typename T>
Rank StaticList<T>::operator[](Rank index) const {
    if (index <= _size / 2) {
        return forward(_head, index+1);
    } else {
        return backward(_tail, _size-index);
    }
}

template <typename T>
void StaticList<T>::traverse(function<void(Rank)> visit) const {
    for (Rank r = V[_head].succ; r != _tail; r = V[r].succ) {
        visit(r);
    } 
}

template <typename T>
ostream& operator<< (ostream& out, const StaticList<T>& L)
{
    out << "L(";
    L.traverse([&](Rank r) -> void {
        if (r != L.V[L._head].succ) { out <<","; }
        out << L.V[r].value;
    });
    out << ")";
}

template <typename T>
void StaticList<T>::push_back(T e) {
    insertAsPred(_tail, e);
}

template <typename T>
void StaticList<T>::push_front(T e) {
    insertAsSucc(_head, e);
}

template <typename T>
T StaticList<T>::pop_back() {
    Rank r = V[_tail].pred;
    T temp = V[r].value;
    remove(r);
    return temp;
}

template <typename T>
T StaticList<T>::pop_front() {
    Rank r = V[_head].succ;
    T temp = V[r].value;
    remove(r);
    return temp;
}

#endif