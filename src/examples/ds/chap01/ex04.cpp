// 问题1.4 - 函数零点
// 输入：函数f(x)，区间(l,r)，误差限eps > 0
    /* 输入数据保证f(x)在数学上连续，且f(l)*f(r) < 0 */
// 输出：函数f(x)在区间(l,r)上的一个零点，绝对误差不超过eps

#include <iostream>
#include <functional>
#include <cmath>
using namespace std;

// 算法1.4A（循环版本）
double solve(function<double(double)> f, double l, double r, double eps) {
    while (r - l > eps) {             // 循环直到满足误差限
        double mid = l + (r - l) / 2; // 每次取(l,r)的中点
        if (f(l) * f(mid) <= 0) {     // 判断零点是否在(l,mid]中
            r = mid;
        } else {                      // 还是在(mid,r)中
            l = mid;
        }
    }
    return l;
}

// 算法1.4B（递归版本）
double solve_r(function<double(double)> f, double l, double r, double eps) {
    function<double(double, double)> recursion;
    recursion = [f, eps, &recursion](double l, double r) -> double {
        if (r - l < eps) { return l; } // 递归边界
        double mid = l + (r - l) / 2;  // 每次取(l,r)的中点
        if (f(l) * f(mid) <= 0) {      // 判断零点是否在(l,mid]中
            return recursion(l, mid);
        } else {                       // 还是在(mid,r)中
            return recursion(mid, r);
        }
    };
    return recursion(l, r);
}

int main() {
    const double eps = 1e-6;
    auto test = [=](function<double(double)> f, double l, double r) -> void {
        cout << "x0(loop) = " << solve(f,l,r,eps)   << "\t";
        cout << "x0(recu) = " << solve_r(f,l,r,eps) << "\n";
    };
    test([](double x) -> double { return exp(x) - 2; }, 0,   1);
    test([](double x) -> double { return x      - 1; }, 0,   2); // 结果可能并不准确，但满足误差限要求
    test([](double x) -> double { return x      - 1; }, 0.9, 2); // 此外，结果和初始给定的区间(l,r)相关
    return 0;
}