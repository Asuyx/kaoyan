// 排序

#ifndef SORT_H
#define SORT_H

#include <functional>
#include "utility.h"
using namespace std;

template <typename T>
class Sort {
protected:
    virtual void sort(T*, int, function<bool(const T&, const T&)>) {}
public:
    void operator()(T* A, int n, function<bool(const T&, const T&)> cmp) {
        sort(A, n, cmp);
    }
};

// 算法2.9
// 归并排序
template <typename T>
class MergeSort : public Sort<T> {
private:
    T* B; // 辅助数组
public:
    void mergeSort(T* A, int n, function<bool(const T&, const T&)> cmp) {
        if (n <= 1) { return; }        // 递归边界
        int m = n / 2;                 // 取中点
        mergeSort(A, m, cmp);          // 递归地排序前半部分
        mergeSort(A+m, n-m, cmp);      // 递归地排序后半部分
        arrayCopy(B, A, m);            // 开始归并，将前半部分复制到辅助数组，后半无需复制
        T* A1 = B, * A2 = A+m;         // 前半部分记为A1，后半部分记为A2
        int L1 = m, L2 = n - m;        // A1和A2的长度
        int i = 0, j = 0, k = 0;
        while (j < L1 && k < L2) {     // 在两个部分的元素都没有用尽时
            if (cmp(A1[j], A2[k])) {   // 比较它们还未加入A的最小的元素
                A[i++] = A1[j++];      // 将二者的较小者加入到A中
            } else {
                A[i++] = A2[k++];
            }
        }
        while (j < L1) {               // 如果A1还有多余元素没加入A
            A[i++] = A1[j++];          // 就将剩余元素加入A
        }
    }

    void sort(T* A, int n, function<bool(const T&, const T&)> cmp) {
        if (n <= 1) { return; }
        B = new T[n/2];
        mergeSort(A, n, cmp);
        delete[] B;
    }
};

#endif