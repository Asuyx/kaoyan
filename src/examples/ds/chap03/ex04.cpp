// 3.1 - 栈
// 尾递归消除的情况2
// 有返回值的函数

#include <iostream>
#include <vector>
#include "stack.h"
using namespace std;

// 输入：非负整数m，n，其中n <= m
// 输出：组合数C(m, n)

int combine1(int m, int n) {
    if (n == 0 || n == m) {                        // 递归边界
        return 1;
    }
    return combine1(m-1, n-1) + combine1(m-1, n);  // 递归公式
}

int combine2(int m, int n) {
    Stack<pair<int, int>> S;         // 只存储(m,n)
    int sum = 0;                     // 存储目前为止累加的结果
    S.push(make_pair(m, n));         // 将初始的(m,n)加入到栈中                    
    while (!S.empty()) {
        auto P = S.pop();
        m = P.first, n = P.second;
        if (n == 0 || n == m) {
            ++sum;                   // 只在递归边界上累加
        } else {                     // 不在递归边界上，入栈
            S.push(make_pair(m-1, n));
            S.push(make_pair(m-1, n-1));
        }
    }
    return sum;
}

// 因为模板只适用于无返回值，所以这里省略了套用模板的过程
// 请注意，虽然这里计算组合数是无后效的，C(m-1,n)和C(m-1,n-1)的入栈顺序可以任意
// 但如果有后效的话，入栈顺序必须和模板一样，和递归调用次序是反的

int main() {
    vector<function<int(int, int)>> functionVersions = {
        [](int m, int n) -> int { return combine1(m, n); },  // 直接的递归版本
        [](int m, int n) -> int { return combine2(m, n); }   // 直接的迭代版本
    };
    
    int m, n;
    cin >> m >> n;
    for (auto f : functionVersions) {
        cout << f(m, n) << endl;
    }
    return 0;
}
