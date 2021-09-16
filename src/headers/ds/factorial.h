// 计算一个数的阶乘

#ifndef FACTORIAL_H
#define FACTORIAL_H

int factorial(int n) {
    int prod = 1;
    for (int i = 2; i <= n; ++i) {
        prod *= i;
    }
    return prod;
}

#endif