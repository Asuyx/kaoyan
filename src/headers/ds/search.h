// 查找

#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "utility.h"
using namespace std;

template <typename T>
class Search {
protected:
    virtual int search(T*, int, T, function<bool(const T&, const T&)>);
public:
    int operator()(T* A, int n, T e, function<bool(const T&, const T&)> cmp) {
        return search(A, n, e, cmp);
    }
};

// 算法2.10
// 折半查找（最大秩，返回-1）
template <typename T>
class BinarySearch : public Search<T> {
public:
    // 算法2.10A - 递归版本
#ifdef BINARY_SEARCH_RECURSION
    int binarySearch(T* A, int n, T e, function<bool(const T&, const T&)> cmp) {
        int i = n / 2;                   // 折半
        if (n == 1) {                    // 递归边界
            return A[0] == e ? 0 : -1;
        }
        if (!cmp(A[i], e)) {
            return binarySearch(A, i, e);      // 递归进入左半部分
        } else {
            int r = binarySearch(A+i, n-i, e); // 递归进入右半部分
            return r < 0 ? r : r + i;    // 复原在整个数组中的下标
        }
    }
#endif
    // 算法2.10B - 迭代版本
    int binarySearch(T* A, int n, T e, function<bool(const T&, const T&)> cmp) {
        int i = 0;                 // 维护累计偏置量
        while (n > 1) {            // 判断是否到达递归边界
            int m = n / 2;         // 折半
            if (!cmp(A[m], e)) {
                n = m;             // 递归进入左半部分
            } else {
                i += m;
                A += m;            // 递归进入右半部分
                n -= m;
            }
        }
        return A[0] == e ? i : -1; // 递归边界
    }

    int search(T* A, int n, T e, function<bool(const T&, const T&)> cmp) {
        return binarySearch(A, n, e, cmp);   
    }
};

#endif