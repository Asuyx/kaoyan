#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
using namespace std;

// 算法1.7（复杂度估计练习题）
int f1(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            for (int k = 1; k <= j; k++) {
                for (int l = 1; l <= j; l *= 2) {
                    s += k * l;
                }
            }
        }
    }
    return s;
}

int main() {
    clock_t start, end;
    double t, r;
    // 验证算法复杂度确实是O(n^3 * logn)
    // 用时间（纳秒）除以n^3 * logn计算比例
    // 在n充分大的时候，这个值是差不多的
    for (int n : {100, 250, 400, 700, 1000}) {
        start = clock();
        f1(n);
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC;
        r = t * 1e9 / (n*n*n*log2(n));
        cout << "time(" << setw(3) << n << ") = " << t << "\t";
        cout << "ratio = " << r << endl;
    }
    return 0;
}